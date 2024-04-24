_Overview_

This project aims to develop a program that utilizes two NASA APIs, namely NASA Asteroids - NeoWs and NASA Exoplanet, to explore and categorize celestial objects based on their characteristics. The program will focus on classifying exoplanets using data from the NASA Exoplanet API and fetching information about Near Earth Objects (NEOs) from the NASA Asteroids - NeoWs API.
Project relevance

In today's rapidly evolving world, space exploration holds immense significance for humanity's future. Safeguarding against potential threats such as asteroid impacts is crucial for our survival. 

According to NASA, the probability of a significant asteroid collision with Earth occurs roughly once every 10,000 years, emphasizing the importance of space exploration and vigilance.

Exploring space, categorizing NEOs, and studying exoplanets based on their characteristics provide valuable insights into our universe's composition, distribution of planetary systems, and potential habitability of other worlds.

By analyzing data from NASA APIs, we can better understand the cosmos and potentially discover planets with conditions conducive to supporting life.


_Implementation:_
1)	Visualization:
•	Utilize Qt widgets to create an intuitive interface for users to interact with the program.

•	Implement visualizations such as scatter plots or bar charts to illustrate clusters of exoplanets based on their attributes.
3)	Data Fetching from NASA Exoplanet API:
•	Develop a C++ program using Qt's networking classes to fetch data from the NASA Exoplanet API.

•	Set up HTTP requests to retrieve JSON data containing information about exoplanets.

•	Parse JSON responses to extract relevant details like exoplanet names, distances, diameters, etc.
4)	Clustering Algorithm Implementation:
•	Implement a clustering algorithm, such as k-means, in C++ to categorize exoplanet data into clusters based on their attributes.

•	Define appropriate distance metrics for clustering, considering factors like distance, diameter, and other parameters.

•	Optimize the algorithm for efficiency and scalability to handle potentially large datasets of exoplanets.
5)	Project Presentation:
•	Showcase the functionalities of the Qt interface, demonstrating how users can interact with and visualize exoplanet data.

•	Explain the implementation details of data fetching from the NASA Exoplanet API and the clustering algorithm used.

•	Discuss any challenges encountered during development and the strategies employed to overcome them.

•	Present insights or findings from the clustered exoplanet data, discussing potential real-world applications or avenues for further enhancement.

_Requirements_
1)	C++ compiler
2)	QT for developing an application
3)	СURL library for making API requests
4)	JSON library for parsing API response

_References_
NASA Exoplanet Archive API
NASA NeoWs API documentation: https://api.nasa.gov/api.html#neows-lookup
NASA Near Earth Object Program: https://cneos.jpl.nasa.gov/


