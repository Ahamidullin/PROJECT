#ifndef NEO_H
#define NEO_H

#include <string>
#include <vector>

struct NEO {
    std::string name;
    double diameter;
    double distance;
    double speed;
    double absolute_magnitude;
};

std::vector<NEO> fetch_neo_data(const std::string &api_key);

double euclidean_distance(const NEO &a, const NEO &b);

void k_means_clustering(std::vector<NEO> &neos, int k, int iterations);

#endif // NEO_H
