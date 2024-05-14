#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

std::string getHazardDescription(int hazardLevel) {
  switch (hazardLevel) {
    case 0:
      return "Белая зона (нет риска): Небольшие объекты, не представляющие угрозы.";
    case 1:
      return "Зелёная зона (нормальная): Рядовое открытие, не представляющее опасности.";
    case 2:
    case 3:
    case 4:
      return "Жёлтая зона (пристальное внимание): Тесное сближение, заслуживающее внимания астрономов.";
    case 5:
    case 6:
    case 7:
      return "Оранжевая зона (угроза): Тесное сближение, представляющее серьёзную угрозу.";
    case 8:
    case 9:
    case 10:
      return "Красная зона: Неизбежные столкновения, способные вызвать глобальные катастрофы.";
    default:
      return "Неизвестный уровень угрозы.";
  }
}



int main() {
  try {
    std::string const host = "api.nasa.gov";
    std::string const port = "443";
    std::string api_key = "DEMO_KEY"; // Используйте ваш реальный API ключ
    std::string target = "/neo/rest/v1/feed?start_date=2015-09-07&end_date=2015-09-08&api_key=" + api_key;

    net::io_context ioc;
    ssl::context ctx{ssl::context::sslv23_client};

    // Настройка контекста SSL для проверки сертификата сервера
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(ssl::verify_peer);

    tcp::resolver resolver(ioc);
    ssl::stream<beast::tcp_stream> stream(ioc, ctx);

    // Разрешение DNS и подключение
    auto const results = resolver.resolve(host, port);
    beast::get_lowest_layer(stream).connect(results);

    // Рукопожатие SSL
    stream.handshake(ssl::stream_base::client);

    // Формируем и отправляем HTTP запрос
    http::request<http::empty_body> req{http::verb::get, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream, req);

    // Получение и обработка ответа
    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    std::string body = beast::buffers_to_string(res.body().data());
    auto json = boost::json::parse(body);

    // Обработка JSON и вывод информации об астероидах
    if (json.as_object().find("near_earth_objects") != json.as_object().end()) {
      auto nearEarthObjects = json.as_object().at("near_earth_objects").as_object();
      for (auto const& date : nearEarthObjects) {
        std::cout << "Дата: " << date.key() << std::endl;
        auto asteroids = date.value().as_array();
        for (const auto& asteroid : asteroids) {
          std::cout << "Имя астероида: " << asteroid.as_object().at("name").as_string() << std::endl;
          bool isHazardous = asteroid.as_object().at("is_potentially_hazardous_asteroid").as_bool();
          std::string description = getHazardDescription(isHazardous);
          std::cout << "Оценка угрозы: " << description << std::endl;
          std::cout << std::endl;
        }
      }
    } else {
      std::cout << "Информация об астероидах не найдена." << std::endl;
    }

    beast::error_code ec;
    stream.shutdown(ec);
    if (ec && ec != beast::errc::not_connected && ec != ssl::error::stream_truncated) {
      throw beast::system_error{ec};
    }

  } catch (std::exception const& e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
