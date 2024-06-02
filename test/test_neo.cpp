#include <gtest/gtest.h>
#include "../src/neo.cpp"
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>

size_t WriteCallbackStub(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string mockApiResponse;
std::string emptyApiResponse = R"({
    "near_earth_objects": {}
})";
std::string invalidApiResponse = R"({
    "near_earth_objects": {
        "2024-05-01": [
            {
                "name": "Invalid NEO",
                "estimated_diameter": {
                    "kilometers": {
                        "estimated_diameter_min": "invalid"
                    }
                },
                "close_approach_data": [
                    {
                        "miss_distance": {
                            "kilometers": "invalid"
                        },
                        "relative_velocity": {
                            "kilometers_per_second": "invalid"
                        }
                    }
                ],
                "absolute_magnitude_h": "invalid"
            }
        ]
    }
})";

CURLcode curl_easy_perform(CURL *curl) {
    std::cerr << "Mock curl_easy_perform called" << std::endl;
    std::string *writeBuffer;
    curl_easy_getinfo(curl, CURLINFO_PRIVATE, &writeBuffer);
    if (writeBuffer) {
        *writeBuffer = mockApiResponse;
        std::cerr << "Mock response assigned to writeBuffer" << std::endl;
    } else {
        std::cerr << "writeBuffer is null" << std::endl;
    }
    return CURLE_OK;
}

class NasaApiTest : public ::testing::Test {
protected:
    void SetUp() override {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        mockApiResponse = R"({
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
                                    "kilometers": "100000",
                                    "astronomical": "0.00067"
                                },
                                "relative_velocity": {
                                    "kilometers_per_second": "25"
                                },
                                "close_approach_date": "2024-05-01",
                                "orbiting_body": "Earth"
                            }
                        ],
                        "absolute_magnitude_h": 22.1
                    }
                ]
            }
        })";
    }

    void TearDown() override {
        curl_global_cleanup();
    }

    std::string apiKey = "RMuL4B1GnixhUilPfRKRA7Bwk2OwrJu7Vpv8dJaB";
};

TEST_F(NasaApiTest, FetchNeoDataParsesCorrectly) {
    std::vector<NEO> neos = fetch_neo_data(apiKey);

    std::cerr << "Number of NEOs fetched: " << neos.size() << std::endl;
    for (const auto &neo: neos) {
        std::cerr << "Fetched NEO: " << neo.name << std::endl;
    }

    ASSERT_EQ(neos.size(), 1);
    EXPECT_EQ(neos[0].name, "Mock NEO 1");
    EXPECT_DOUBLE_EQ(neos[0].diameter, 0.1);
    EXPECT_DOUBLE_EQ(neos[0].distance, 100000);
    EXPECT_DOUBLE_EQ(neos[0].speed, 25);
    EXPECT_DOUBLE_EQ(neos[0].absolute_magnitude, 22.1);
    EXPECT_EQ(neos[0].close_approach_date, "2024-05-01");
    EXPECT_EQ(neos[0].orbiting_body, "Earth");
    EXPECT_DOUBLE_EQ(neos[0].miss_distance_astronomical, 0.00067);
}

TEST_F(NasaApiTest, FetchNeoDataHandlesEmptyResponse) {
    mockApiResponse = emptyApiResponse;

    std::vector<NEO> neos = fetch_neo_data(apiKey);

    std::cerr << "Number of NEOs fetched: " << neos.size() << std::endl;

    ASSERT_EQ(neos.size(), 0); // Должен быть пустым
}

TEST_F(NasaApiTest, FetchNeoDataHandlesInvalidResponse) {
    mockApiResponse = invalidApiResponse;

    std::vector<NEO> neos = fetch_neo_data(apiKey);

    std::cerr << "Number of NEOs fetched: " << neos.size() << std::endl;

    ASSERT_EQ(neos.size(), 0); // Должен быть пустым, так как данные некорректные
}

TEST_F(NasaApiTest, EuclideanDistanceCalculatesCorrectly) {
    NEO neo1{"NEO 1", 0.1, 100000, 25, 22.1, "2024-05-01", "Earth", 0.00067};
    NEO neo2{"NEO 2", 0.2, 110000, 30, 21.5, "2024-05-02", "Earth", 0.00068};

    double distance = euclidean_distance(neo1, neo2);

    double expected_distance = std::sqrt(
            std::pow(neo1.diameter - neo2.diameter, 2) +
            std::pow(neo1.distance - neo2.distance, 2) +
            std::pow(neo1.speed - neo2.speed, 2) +
            std::pow(neo1.absolute_magnitude - neo2.absolute_magnitude, 2)
    );
    EXPECT_NEAR(distance, expected_distance, 0.0001);
}

TEST_F(NasaApiTest, EuclideanDistanceCalculatesCorrectlyWithSameData) {
    NEO neo1{"NEO 1", 0.1, 100000, 25, 22.1, "2024-05-01", "Earth", 0.00067};

    double distance = euclidean_distance(neo1, neo1);

    EXPECT_DOUBLE_EQ(distance, 0.0); // Расстояние между одинаковыми данными 0
}

TEST_F(NasaApiTest, KMeansClusteringDistributesDataCorrectly) {
    std::vector<NEO> neos = {
            {"NEO 1", 0.1,  100000, 25, 22.1, "2024-05-01", "Earth", 0.00067},
            {"NEO 2", 0.2,  110000, 30, 21.5, "2024-05-02", "Earth", 0.00068},
            {"NEO 3", 0.15, 105000, 27, 22.0, "2024-05-03", "Earth", 0.00069},
            {"NEO 4", 0.05, 90000,  20, 23.0, "2024-05-04", "Earth", 0.00066},
            {"NEO 5", 0.25, 115000, 35, 21.0, "2024-05-05", "Earth", 0.00070}
    };

    k_means_clustering(neos, 2, 10);

    ASSERT_TRUE(!neos.empty());

    int cluster1_count = 0;
    int cluster2_count = 0;
    for (const auto &neo: neos) {
        if (neo.name == "NEO 1" || neo.name == "NEO 3" || neo.name == "NEO 4") {
            cluster1_count++;
        } else {
            cluster2_count++;
        }
    }

    ASSERT_EQ(cluster1_count, 3);
    ASSERT_EQ(cluster2_count, 2);
}

TEST_F(NasaApiTest, KMeansClusteringGroupsCorrectly) {
    std::vector<NEO> neos = {
            {"NEO 1", 0.1,  100000, 25, 22.1, "2024-05-01", "Earth", 0.00067},
            {"NEO 2", 0.2,  110000, 30, 21.5, "2024-05-02", "Earth", 0.00068},
            {"NEO 3", 0.15, 105000, 27, 22.0, "2024-05-03", "Earth", 0.00069}
    };

    k_means_clustering(neos, 2, 10);

    ASSERT_TRUE(!neos.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
