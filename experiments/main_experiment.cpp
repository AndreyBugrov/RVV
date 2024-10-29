#include <iostream>
#include <string>

#include "experiment_task.hpp"

double abobus(double a, double b, bool yes, int number){
    return yes? pow(a, b)*sin(a)/cos(b)+number: (1/pow(a, 1/b))*pow(b, tan(a))/number;
}

int main(){
    // recognize function type
    // get arguments
    //map<string name, enum class function_index>
    //switch by function_index
    //run task
    BaseTaskOutput output = run_experiment(abobus, 7.5, -3.2, false, 5);
    std::cout<<"ended: ";
    if(output.ended()){
        std::cout<<"true";
    }else{
        std::cout<<"false";
    }
    std::cout<<"\n";
    std::cout<<"seconds: "<<output.time()<<"\n";
    std::cout<<"what: "<<output.what()<<"\n";
    std::cout<<"error message: "<<output.error_message()<<"\n";
    return 0;
}
