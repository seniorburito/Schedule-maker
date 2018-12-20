#include <iostream> 
#include <string>

class Course {
    public: 
        std::string course_num;
        std::string meeting_time;
        std::string section;
        std::string GE;
        Course(std::string cn, std::string sec, std::string ge, std::string mt);
        Course();
        void print();
};

