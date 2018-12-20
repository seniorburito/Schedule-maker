#include "Course.h"

Course::Course(std::string cn, std::string  sec, std::string  ge, std::string  mt){
    course_num = cn;
    meeting_time = mt.substr(0, mt.size()-1);
    section = sec;
    GE = ge;
}
Course::Course(){
    course_num = "";
    meeting_time = "";
    section = "";
    GE = "";
}

void Course::print(){
    std::cout << "course is " << course_num << " section is " << section << " meeting time is " << meeting_time << " GE is " << GE << "*" << std::endl;
}