#include "Individual.h"
#include "Course.h"

bool Individual::course_conflict(Course &a, Course &b){
    if(a.meeting_time == b.meeting_time){
        return true;
    };
    return false;
}

//see if the class schedule has any conflicts
bool Individual::valid_schedule(std::vector<Course> &list){
    for(int i=0; i < list.size(); i++){
        int j = 0;
        while(j < i){
            if(course_conflict(list[i], list[j])){
                return false;
            }
            j++;
        }
        j++;
        while(j < list.size()){
            if(course_conflict(list[i], list[j])){
                return false;
            }
            j++;
        }
    }
    return true;
}

//return a random integer
int Individual::random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
} 

int Individual::class_fitness(std::vector<Course> &chromosome){
    int fitness = 0;
    std::vector<bool> used(desired_classes.size(), false);
    for(int i = 0; i<chromosome.size(); i++){
        for(int j = 0; j < desired_classes.size(); j++){
            if(chromosome[i].course_num == desired_classes[j] && !used[j]){
                fitness -= 3;
                used[j] = true;
            }
        }
    }
    return fitness;
}


Individual::Individual(std::vector<Course> chromosome) 
{ 
    this->chromosome = chromosome; 
    fitness = cal_fitness(); 
}; 
  
// Perform mating and produce new offspring 
Individual Individual::mate(Individual par2, std::vector<Course> &catalog) 
{ 
    // chromosome for offspring 
    std::vector<Course> child_chromosome(credit_limit);
  
    int len = chromosome.size(); 
    for(int i = 0;i<len;i++) 
    { 
        // random probability  
        float p = random_num(0, 100)/100; 
  
        // if prob is less than 0.45, insert gene 
        // from parent 1  
        if(p < 0.45) 
            child_chromosome[i] = chromosome[i]; 
  
        // if prob is between 0.45 and 0.90, insert 
        // gene from parent 2 
        else if(p < 0.90) 
            child_chromosome[i] = par2.chromosome[i]; 
  
        // otherwise insert random gene(mutate),  
        // for maintaining diversity 
        else
            child_chromosome[i] = catalog[random_num(0, catalog.size()-1)]; 
    } 
  
    // create new Individual(offspring) using  
    // generated chromosome for offspring 
    return Individual(child_chromosome); 
}; 
  

// Calculate fittness score, it is the number of 
// characters in string which differ from target 
// string. 
int Individual::cal_fitness() 
{ 
    int len = credit_limit; 
    int fitness = 0; 
    
    if(valid_schedule(chromosome)){
        fitness -= 20;
    }
    else{
        fitness += 20;
    }

    fitness += class_fitness(chromosome);

    return fitness;     
}; 
  
// Overloading < operator 
bool operator<(const Individual &ind1, const Individual &ind2) 
{ 
    return ind1.fitness < ind2.fitness; 
} 


