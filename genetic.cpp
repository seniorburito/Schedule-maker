#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
vector<string> desired_GEs = {"WRI", "ALS-A", "EIN", "WRI", "WRI", "HWC", "ORC", "SED"};
vector<string> desired_classes = {"ENGL 185", "CSCI 241", "ART 102", "THEAT 250", "SWRK 221", "MATH 220", "BIO 123", "CHEM 121", "GERM 217", "LATIN 231"};
#define POPULATION_SIZE 1000

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



//function to see if two courses happen at different times
bool course_conflict(course &a, course &b){
    if(a.meeting_time == b.meeting_time){
        return true;
    };
    return false;
}

//see if the class schedule has any conflicts
bool valid_schedule(vector<course> &list){
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
int random_num(int start, int end) 
{ 
    int range = (end-start)+1; 
    int random_int = start+(rand()%range); 
    return random_int; 
} 

vector<course> create_gnome(vector<course> &catalog) 
{ 
    int len = credit_limit; 
    vector<course> gnome(len);
    for(int i = 0;i<len;i++) 
        gnome[i] = catalog[random_num(0, catalog.size()-1)];
    return gnome; 
} 

int class_fitness(vector<course> &chromosome){
    int fitness = 0;
    vector<bool> used(desired_classes.size(), false);
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

int GE_fitness(vector<course> &chromosome){
    int fitness = 0;
    vector<bool> used(desired_GEs.size(), false);
    for(int i = 0; i<chromosome.size(); i++){
        for(int j = 0; j < desired_GEs.size(); j++){
            if(chromosome[i].GE == desired_GEs[j] && !used[j]){
                fitness -= 3;
                used[j] = true;
            }
        }
    }
    return fitness;
}

class Individual {
    public:
        vector<course> chromosome;
        int fitness;
        Individual(vector<course> chromosome);
        Individual mate(Individual parent2, vector<course> &catalog);
        int cal_fitness();
};

Individual::Individual(vector<course> chromosome) 
{ 
    this->chromosome = chromosome; 
    fitness = cal_fitness(); 
}; 
  
// Perform mating and produce new offspring 
Individual Individual::mate(Individual par2, vector<course> &catalog) 
{ 
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



int main(){
    srand((unsigned)(time(0))); 
    ifstream ip("2018s1d.csv");
    
    //check if the file is opened
    if(!ip.is_open()) cout << "ERROR: FILE NOT OPEN" << '\n';
    
    string ln;
    vector<course> course_catalog;


    while(ip.good()){
        getline(ip, ln, '\n');
        stringstream ss(ln);
        string temp;
        vector<string> t(6);
        for(int i = 0; getline(ss,temp, ','); i++){
            t[i] = temp;
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

    vector<course> test2 = create_gnome(course_catalog);
    cout << "here" << endl;
    vector<course> test = {course_catalog[2], course_catalog[3], course_catalog[4], course_catalog[5]};
    cout << course_catalog.size() << endl;

    vector<vector<course>> top10;
    for(int i = 0; i<10; i++){
        int generation = 0; 
    
        vector<Individual> population; 
        bool found = false; 
    
        // create initial population 
        for(int i = 0;i<POPULATION_SIZE-1;i++) 
        { 
            vector<course> gnome = create_gnome(course_catalog); 
            population.push_back(Individual(gnome)); 
        } 

        //good start 

        vector<course> start1 = {course_catalog[1], course_catalog[3], course_catalog[5], course_catalog[6]};
        population.push_back(Individual(start1));

        while(! found) 
        { 
            // sort the population in increasing order of fitness score 
            sort(population.begin(), population.end()); 
    
            //if it reaches a reasonable number of generations
            // terminate the loop 
            if(generation == 50) 
            { 
                found = true; 
                break; 
            } 
    
            // Otherwise generate new offsprings for new generation 
            vector<Individual> new_generation; 
    
            // Perform Elitism, that mean 10% of fittest population 
            // goes to the next generation 
            int s = (10*POPULATION_SIZE)/100; 
            for(int i = 0;i<s;i++) 
                new_generation.push_back(population[i]); 
    
            // From 50% of fittest population, Individuals 
            // will mate to produce offspring 
            s = (90*POPULATION_SIZE)/100; 
            for(int i = 0;i<s;i++) 
            { 
                int len = population.size(); 
                int r = random_num(0, 50); 
                Individual parent1 = population[r]; 
                r = random_num(0, 50); 
                Individual parent2 = population[r]; 
                Individual offspring = parent1.mate(parent2, course_catalog); 
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
        cout<< "Generation: " << generation << "\n"; 
        for(int i = 0; i < population[0].chromosome.size(); i++){
            population[0].chromosome[i].print();
        }
        cout<< "Fitness: "<< population[0].fitness << "\n";
        top10.push_back(population[0].chromosome);
    } 
    for(int i = 0; i<10; i++){
        for(int j = 0; j < top10[i].size(); j++){
            top10[i][j].print();
        }
        cout << "******************************************************" << endl;
    }
    return 0;
}