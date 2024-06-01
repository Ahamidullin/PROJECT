#include <iostream>
#include <string>
#include "neo.h"

int main() {
  std::string api_key = "RMuL4B1GnixhUilPfRKRA7Bwk2OwrJu7Vpv8dJaB";
  std::vector<NEO> neos = fetch_neo_data(api_key);

  if (!neos.empty()) {
    int k = 5;
    int iterations = 200;
    k_means_clustering(neos, k, iterations);
  } else {
    std::cerr << "No NEO data available" << std::endl;
  }
  return 0;
}