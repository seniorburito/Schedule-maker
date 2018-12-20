#include <vector> 
#include "Course.h"
#define credit_limit 4

class Individual {
    public:
        std::vector<Course> chromosome;
        int fitness;
        Individual(std::vector<Course> chromosome);
        Individual mate(Individual parent2, std::vector<Course> &catalog);
        int cal_fitness();
        bool course_conflict(Course &a, Course &b);
        bool valid_schedule(std::vector<Course> &list);
        int random_num(int start, int end);
        int class_fitness(std::vector<Course> &chromosome);
};