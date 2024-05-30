#include "fetch_neo_data.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

std::vector<NEO> fetchNEOData(const QString& apiKey) {
    QNetworkAccessManager manager;
    QEventLoop loop;

    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QString url = "https://api.nasa.gov/neo/rest/v1/neo/browse?api_key=" + apiKey;
    QNetworkRequest request((QUrl(url)));  // Fixed the parentheses here

    QNetworkReply *reply = manager.get(request);

    loop.exec();  // Run the event loop until the network request is finished

    std::vector<NEO> neos;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
        QJsonObject json_obj = json_doc.object();

        // Debugging: print the whole JSON object
        qDebug() << "Full JSON Response:" << json_doc.toJson(QJsonDocument::Indented);

        QJsonArray neo_array = json_obj["near_earth_objects"].toArray();

        for (const QJsonValue& value : neo_array) {
            QJsonObject neo_obj = value.toObject();
            NEO neo;

            QJsonObject estimated_diameter = neo_obj["estimated_diameter"].toObject();
            QJsonObject kilometers = estimated_diameter["kilometers"].toObject();
            neo.diameter = (kilometers["estimated_diameter_min"].toDouble() + kilometers["estimated_diameter_max"].toDouble()) / 2;

            QJsonArray close_approach_data = neo_obj["close_approach_data"].toArray();
            if (!close_approach_data.isEmpty()) {
                QJsonObject first_approach = close_approach_data.first().toObject();

                // Debug: print the close approach data object
                qDebug() << "Close Approach Data:" << first_approach;

                if (first_approach.contains("miss_distance") && first_approach["miss_distance"].isObject()) {
                    QJsonObject miss_distance = first_approach["miss_distance"].toObject();
                    if (miss_distance.contains("kilometers")) {
                        bool ok;
                        neo.distance = miss_distance["kilometers"].toString().toDouble(&ok);
                        if (!ok) {
                            qDebug() << "Conversion failed for distance:" << miss_distance["kilometers"].toString();
                            neo.distance = 0;
                        }
                    } else {
                        qDebug() << "Missing 'kilometers' in 'miss_distance'";
                    }
                } else {
                    qDebug() << "Missing 'miss_distance' or not an object";
                }

                if (first_approach.contains("relative_velocity") && first_approach["relative_velocity"].isObject()) {
                    QJsonObject relative_velocity = first_approach["relative_velocity"].toObject();
                    if (relative_velocity.contains("kilometers_per_hour")) {
                        bool ok;
                        neo.speed = relative_velocity["kilometers_per_hour"].toString().toDouble(&ok);
                        if (!ok) {
                            qDebug() << "Conversion failed for speed:" << relative_velocity["kilometers_per_hour"].toString();
                            neo.speed = 0;
                        }
                    } else {
                        qDebug() << "Missing 'kilometers_per_hour' in 'relative_velocity'";
                    }
                } else {
                    qDebug() << "Missing 'relative_velocity' or not an object";
                }

                // Debugging: print the extracted values
                qDebug() << "Parsed Distance:" << neo.distance;
                qDebug() << "Parsed Speed:" << neo.speed;
            }

            neos.push_back(neo);
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    delete reply;
    return neos;
}
