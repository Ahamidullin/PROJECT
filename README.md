### Overview

This project aims to develop a program that utilizes the NASA Asteroids - NeoWs API to explore and categorize Near Earth Objects (NEOs) based on their characteristics. The program focuses on fetching information about NEOs from the NASA Asteroids - NeoWs API and visualizing this data for analysis.

### Project Relevance

In today's rapidly evolving world, space exploration holds immense significance for humanity's future. Safeguarding against potential threats such as asteroid impacts is crucial for our survival.

According to NASA, the probability of a significant asteroid collision with Earth occurs roughly once every 10,000 years, emphasizing the importance of space exploration and vigilance.

Exploring space and categorizing NEOs provide valuable insights into our universe's composition and help us understand and potentially mitigate the threat of asteroid impacts.

By analyzing data from the NASA NeoWs API, we can better understand the cosmos and improve our preparedness against potential celestial threats.

### Implementation

#### Visualization

- Utilize Qt widgets to create an intuitive interface for users to interact with the program.

#### Data Fetching from NASA NeoWs API

- Develop a C++ program using Qt's networking classes to fetch data from the NASA NeoWs API.
- Set up HTTP requests to retrieve JSON data containing information about NEOs.
- Parse JSON responses to extract relevant details like NEO names, distances, diameters, etc.

#### Clustering Algorithm Implementation

- Implement a clustering algorithm, such as k-means, in C++ to categorize NEO data into clusters based on their attributes.
- Define appropriate distance metrics for clustering, considering factors like distance, diameter, and other parameters.
- Optimize the algorithm for efficiency and scalability to handle potentially large datasets of NEOs.

#### Project Presentation

- Showcase the functionalities of the Qt interface, demonstrating how users can interact with and visualize NEO data.
- Explain the implementation details of data fetching from the NASA NeoWs API and the clustering algorithm used.
- Discuss any challenges encountered during development and the strategies employed to overcome them.
- Present insights or findings from the clustered NEO data, discussing potential real-world applications or avenues for further enhancement.

### Requirements

- C++ compiler
- Qt for developing the application
- CURL library for making API requests
- JSON library for parsing API response

### References

- [NASA NeoWs API documentation](https://api.nasa.gov/api.html#neows-lookup)
- [NASA Near Earth Object Program](https://cneos.jpl.nasa.gov/)
