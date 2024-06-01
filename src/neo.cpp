#include "neo.h"
#include <curl/curl.h>
#include <cmath>
#include <cstdlib>
#include <json/json.h>
#include <sstream>
#include <iostream>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *) userp)->append((char *) contents, size * nmemb);
  return size * nmemb;
}

std::vector<NEO> fetch_neo_data(const std::string &api_key) {
  std::string url =
      "https://api.nasa.gov/neo/rest/v1/feed?start_date=2024-05-01&end_date=2024-05-07&api_key=" + api_key;
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::vector<NEO> neos;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
      return neos;
    }

    Json::CharReaderBuilder readerBuilder;
    Json::Value root;
    std::string errs;
    std::istringstream s(readBuffer);
    if (!Json::parseFromStream(readerBuilder, s, &root, &errs)) {
      std::cerr << "Failed to parse JSON: " << errs << std::endl;
      return neos;
    }

    for (const auto &date: root["near_earth_objects"].getMemberNames()) {
      for (const auto &neo: root["near_earth_objects"][date]) {
        try {
          if (neo["name"].isString() &&
              neo["estimated_diameter"]["kilometers"]["estimated_diameter_min"].isDouble() &&
              neo["close_approach_data"][0]["miss_distance"]["kilometers"].isString() &&
              neo["close_approach_data"][0]["relative_velocity"]["kilometers_per_second"].isString() &&
              neo["absolute_magnitude_h"].isDouble() &&
              neo["close_approach_data"][0]["close_approach_date"].isString() &&
              neo["close_approach_data"][0]["orbiting_body"].isString() &&
              neo["close_approach_data"][0]["miss_distance"]["astronomical"].isString()) {

            std::string name = neo["name"].asString();
            double diameter = neo["estimated_diameter"]["kilometers"]["estimated_diameter_min"].asDouble();
            double distance = std::stod(
                neo["close_approach_data"][0]["miss_distance"]["kilometers"].asString());
            double speed = std::stod(
                neo["close_approach_data"][0]["relative_velocity"]["kilometers_per_second"].asString());
            double absolute_magnitude = neo["absolute_magnitude_h"].asDouble();
            std::string close_approach_date = neo["close_approach_data"][0]["close_approach_date"].asString();
            std::string orbiting_body = neo["close_approach_data"][0]["orbiting_body"].asString();
            double miss_distance_astronomical = std::stod(
                neo["close_approach_data"][0]["miss_distance"]["astronomical"].asString());

            neos.push_back({name, diameter, distance, speed, absolute_magnitude, close_approach_date, orbiting_body, miss_distance_astronomical});
          } else {
            std::cerr << "Invalid data types in NEO entry." << std::endl;
          }
        } catch (const std::exception &e) {
          std::cerr << "Error processing NEO data: " << e.what() << std::endl;
        }
      }
    }
  }
  return neos;
}

double euclidean_distance(const NEO &a, const NEO &b) {
  return std::sqrt(std::pow(a.diameter - b.diameter, 2) +
      std::pow(a.distance - b.distance, 2) +
      std::pow(a.speed - b.speed, 2) +
      std::pow(a.absolute_magnitude - b.absolute_magnitude, 2));
}

void k_means_clustering(std::vector<NEO> &neos, int k, int iterations) {
  int n = neos.size();
  std::vector<NEO> centroids(k);
  std::vector<int> labels(n);

  for (int i = 0; i < k; ++i) {
    centroids[i] = neos[rand() % n];
  }

  for (int iter = 0; iter < iterations; ++iter) {
    for (int i = 0; i < n; ++i) {
      double min_dist = euclidean_distance(neos[i], centroids[0]);
      int label = 0;
      for (int j = 1; j < k; ++j) {
        double dist = euclidean_distance(neos[i], centroids[j]);
        if (dist < min_dist) {
          min_dist = dist;
          label = j;
        }
      }
      labels[i] = label;
    }

    std::vector<NEO> new_centroids(k, {"", 0, 0, 0, 0, "", "", 0});
    std::vector<int> counts(k, 0);
    for (int i = 0; i < n; ++i) {
      int label = labels[i];
      new_centroids[label].diameter += neos[i].diameter;
      new_centroids[label].distance += neos[i].distance;
      new_centroids[label].speed += neos[i].speed;
      new_centroids[label].absolute_magnitude += neos[i].absolute_magnitude;
      counts[label] += 1;
    }
    for (int j = 0; j < k; ++j) {
      if (counts[j] != 0) {
        new_centroids[j].diameter /= counts[j];
        new_centroids[j].distance /= counts[j];
        new_centroids[j].speed /= counts[j];
        new_centroids[j].absolute_magnitude /= counts[j];
      }
    }
    centroids = new_centroids;
  }

  for (int i = 0; i < k; ++i) {
    std::cout << "Cluster " << i + 1 << ":" << std::endl;
    for (int j = 0; j < n; ++j) {
      if (labels[j] == i) {
        std::cout << "  NEO(name: " << neos[j].name
                  << ", diameter: " << neos[j].diameter
                  << ", distance: " << neos[j].distance
                  << ", speed: " << neos[j].speed
                  << ", absolute magnitude: " << neos[j].absolute_magnitude
                  << ", close approach date: " << neos[j].close_approach_date
                  << ", orbiting body: " << neos[j].orbiting_body
                  << ", miss distance (AU): " << neos[j].miss_distance_astronomical << ")" << std::endl;
      }
    }
  }
}