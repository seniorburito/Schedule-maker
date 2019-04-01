#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
vector<string> desired_GEs = {"WRI", "ALS-A", "EIN", "WRI", "WRI", "HWC", "ORC", "SED"};
vector<string> desired_classes = {"ENGL 185", "CSCI 241", "ART 102", "THEAT 250", "SWRK 221", "MATH 220", "BIO 123", "CHEM 121", "GERM 217", "LATIN 231"};
#define POPULATION_SIZE 1000
#define POINTS_FOR_VALID_SCHEDULE 20
#define POINTS_FOR_DESIRED_CLASSES 3
#define POINTS_FOR_DESIRED_GES 3

int credit_limit = 4;
//course object
class course {
public: 
    string course_num;
    string meeting_time;
    string section;
    string GE;
    course(string cn, string sec, string ge, string mt){
        course_num = cn;
        meeting_time = mt.substr(0, mt.size()-1);
        section = sec;
        GE = ge;
    }
    course(){
        course_num = "";
        meeting_time = "";
        section = "";
        GE = "";
    }
    void print(){
        cout << "course is " << course_num << " section is " << section << " meeting time is " << meeting_time << " GE is " << GE << "*" << endl;
    }
};

// check if two courses happen at different times
bool course_conflict(course &a, course &b){
    if(a.meeting_time == b.meeting_time){
        return true;
    };
    return false;
}

//return a random integer
int random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
} 

vector<course> create_random_schedule(vector<course> &catalog) { 
    vector<course> random_schedule(credit_limit);
    for(int i = 0;i<credit_limit;i++) 
        random_schedule[i] = catalog[random_num(0, catalog.size()-1)];
    return random_schedule; 
} 

class Schedule {
    int compute_class_fitness();
    int compute_GE_fitness();
    bool is_valid();
public:
    vector<course> chromosome;
    int fitness;

    Schedule(vector<course> chromosome);
    Schedule mate(Schedule parent2, vector<course> &catalog);
    int cal_fitness();
};

Schedule::Schedule(vector<course> chromosome) { 
    this->chromosome = chromosome; 
    fitness = cal_fitness(); 
}; 
  
// Perform mating and produce a new schedule based on two old schedules 
Schedule Schedule::mate(Schedule par2, vector<course> &catalog) { 
    // chromosome for offspring 
    vector<course> child_chromosome(credit_limit);
  
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
  
    // create new Schedule(offspring) using  
    // generated chromosome for offspring 
    return Schedule(child_chromosome); 
}; 

//see if the class schedule is valid (no conflict between any two classes)
bool Schedule::is_valid(){
    for(int i=0; i < chromosome.size(); i++){
        for(int j=0; j < chromosome.size(); j++) if (i!=j){
            if(course_conflict(chromosome[i], chromosome[j])){
                return false;
            }
        }
    }
    return true;
}

int Schedule::compute_class_fitness(){
    int fitness = 0;
    vector<bool> used(desired_classes.size(), false);
    for(int i = 0; i<chromosome.size(); i++){
        for(int j = 0; j < desired_classes.size(); j++){
            if(chromosome[i].course_num == desired_classes[j] && !used[j]){
                fitness -= POINTS_FOR_DESIRED_CLASSES;
                used[j] = true;
            }
        }
    }
    return fitness;
}

int Schedule::compute_GE_fitness(){
    int fitness = 0;
    vector<bool> used(desired_GEs.size(), false);
    for(int i = 0; i<chromosome.size(); i++){
        for(int j = 0; j < desired_GEs.size(); j++){
            if(chromosome[i].GE == desired_GEs[j] && !used[j]){
                fitness -= POINTS_FOR_DESIRED_GES;
                used[j] = true;
            }
        }
    }
    return fitness;
}  

// Calculate fittness score (lass_fitness + GE_fitness + is_valid)
int Schedule::cal_fitness() { 
    int fitness = 0; 
    
    if(is_valid()) fitness -= POINTS_FOR_VALID_SCHEDULE;
    else fitness += POINTS_FOR_VALID_SCHEDULE;

    fitness += compute_class_fitness() + compute_GE_fitness();

    return fitness;     
}; 
  
// Overloading < operator 
bool operator<(const Schedule &ind1, const Schedule &ind2) { 
    return ind1.fitness < ind2.fitness; 
} 

vector<course> course_catalog;
void input_courses_info_from_csv(){
	ifstream ip("2018s1d.csv");
    
    //check if the file is opened
    if(!ip.is_open()) cout << "ERROR: FILE NOT OPEN" << '\n';
    
    string ln;

    // input info of courses 
    while(ip.good()){
        getline(ip, ln, '\n');
        stringstream ss(ln);
        string temp;
        vector<string> t(6);
        for(int i = 0; getline(ss,temp, ','); i++){
            t[i] = temp;
            cout << t[i] << endl;
        }

        //clean up that shit
        if(t[0] == "" || t[5] == "" || t[1] == "Course #"){
            continue;
        }
        int g = t[5][0];
        if(g == 13){
            continue;
        }

        course_catalog.push_back(course(t[1], t[2], t[4], t[5]));
    };

    ip.close();
    for(int i = 0; i < course_catalog.size(); i++){
        course_catalog[i].print();
    }
}

// Apply Genetic Algorithm with each Schedule as an Individiual
// trying to maximize the Fitness (= class_fitness + GE_fitness + is_valid) 
vector<course> genetic_algorithm(){
    int generation = 0; 

    vector<Schedule> population; 

    // create initial population 
    for(int i = 0;i<POPULATION_SIZE-1;i++) { 
        vector<course> gnome = create_random_schedule(course_catalog); 
        population.push_back(Schedule(gnome)); 
    } 

    // Initial generation
    vector<course> first_generation = {course_catalog[1], course_catalog[3], course_catalog[5], course_catalog[6]};
    population.push_back(Schedule(first_generation));

    // if it reaches a reasonable number of generations
    // terminate the loop 
    while(generation < 50) { 
        // sort the population in increasing order of fitness score 
        sort(population.begin(), population.end()); 

        // Otherwise generate new offsprings for new generation 
        vector<Schedule> new_generation; 

        // Perform Elitism, that means 10% of fittest population 
        // goes to the next generation 
        int s = (10*POPULATION_SIZE)/100; 
        for(int i = 0;i<s;i++) 
            new_generation.push_back(population[i]); 

        // From 50% of fittest population, Schedules 
        // will mate to produce offspring 
        s = (90*POPULATION_SIZE)/100; 
        for(int i = 0;i<s;i++) { 
            int len = population.size(); 
            int r = random_num(0, 50); 
            Schedule parent1 = population[r]; 
            r = random_num(0, 50); 
            Schedule parent2 = population[r]; 
            Schedule offspring = parent1.mate(parent2, course_catalog); 
            new_generation.push_back(offspring);  
        } 
        population = new_generation; 
        cout<< "Generation: " << generation << "\n"; 
        for(int i = 0; i < population[0].chromosome.size(); i++){
            population[0].chromosome[i].print();
        }
        cout<< "Fitness: "<< population[0].fitness << "\n"; 

        generation++; 
    }
    return population[0].chromosome; 
}
int main(){
    srand((unsigned)(time(0))); 
    input_courses_info_from_csv();

    vector<vector<course>> top10;
    for(int i = 0; i<10; i++){
    	cout << "Start\n";
        top10.push_back(genetic_algorithm());
    }

    cout << "Print top 10 schedule: \n";
    for(int i = 0; i<10; i++){
        for(int j = 0; j < top10[i].size(); j++){
            top10[i][j].print();
        }
        cout << "******************************************************" << endl;
    }
    return 0;
}