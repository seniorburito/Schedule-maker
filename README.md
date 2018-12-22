# Schedule Maker

This C++ code generates best course schedules using heuristic search based on:
* User's desired general education requirements
* User's desired classes 

I made this to make my life easier because I hate making schedules and this program can make it easier

The course catalogs were datamined from St. Olaf's student registration system and can also be found on the school's public catalog at stolaf.edu

The genetic algorithm is based on the tutorial provided by Atul Kumar found [here](https://www.geeksforgeeks.org/genetic-algorithms/)

Here, each course represents a gene and each full schedule represents a genome 

To run the code:
* Download and cd into the folder containing the genetic.cpp file
* Run g++ -std=c++11 -o genetic genetic.cpp
* Run ./genetic

It will look like this: 

![demo](../assets/Demo.gif)

Work to be done:
* Add desired professor features
* Reformat the code to be more readable 
* Add other heuristic search methods such as simulated annealing
* Improve the class_conflict method (currently its a dummy that only sees if their meeting times are exactly the same)
* Add more data to deal with 
* Consider class requirements and prerequisites 