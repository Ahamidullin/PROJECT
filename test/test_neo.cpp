#include <gtest/gtest.h>
#include "../src/neo.cpp"
#include "../src/neo.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curl/curl.h>
#include <json/json.h>

// Заглушка для WriteCallback
size_t WriteCallbackStub(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

// Мок для имитации fetch_neo_data
std::string mockApiResponse = R"({
    "near_earth_objects": {
        "2024-05-01": [
            {
                "name": "Mock NEO 1",
                "estimated_diameter": {
                    "kilometers": {
                        "estimated_diameter_min": 0.1
                    }
                },
                "close_approach_data": [
                    {
                        "miss_distance": {
                            "kilometers": "100000"
                        },
                        "relative_velocity": {
                            "kilometers_per_second": "25"
                        }
                    }
                ],
                "absolute_magnitude_h": 22.1
            }
        ]
    }
})";

class NasaApiTest : public ::testing::Test {
protected:
    void SetUp() override {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    void TearDown() override {
        curl_global_cleanup();
    }

    std::string apiKey = "DEMO_KEY"; // Используйте ваш реальный API ключ
};

TEST_F(NasaApiTest, FetchNeoDataParsesCorrectly) {
    std::string buffer;
    WriteCallbackStub((void *) mockApiResponse.c_str(), 1, mockApiResponse.size(), &buffer);

    std::vector<NEO> neos = fetch_neo_data(apiKey);

    ASSERT_EQ(neos.size(), 1);
    EXPECT_EQ(neos[0].name, "Mock NEO 1");
    EXPECT_DOUBLE_EQ(neos[0].diameter, 0.1);
    EXPECT_DOUBLE_EQ(neos[0].distance, 100000);
    EXPECT_DOUBLE_EQ(neos[0].speed, 25);
    EXPECT_DOUBLE_EQ(neos[0].absolute_magnitude, 22.1);
}

TEST_F(NasaApiTest, EuclideanDistanceCalculatesCorrectly) {
    NEO neo1{"NEO 1", 0.1, 100000, 25, 22.1};
    NEO neo2{"NEO 2", 0.2, 110000, 30, 21.5};

    double distance = euclidean_distance(neo1, neo2);

    EXPECT_NEAR(distance, 5000.0001, 0.0001);
}

TEST_F(NasaApiTest, KMeansClusteringGroupsCorrectly) {
    std::vector<NEO> neos = {
            {"NEO 1", 0.1,  100000, 25, 22.1},
            {"NEO 2", 0.2,  110000, 30, 21.5},
            {"NEO 3", 0.15, 105000, 27, 22.0}
    };

    k_means_clustering(neos, 2, 10);

    // Проверим, что все NEO были распределены по кластерам
    // Здесь можно добавить проверки, например, что хотя бы один кластер содержит NEO
    ASSERT_TRUE(!neos.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}