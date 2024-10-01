#include <iostream>

#include "experiment.hpp"

int main(){
    vector<double> a = {1, 2, 3};
    vector<double> b = {3, -2, 1};
    double c = 3-4+3;
    std::cout<<scalar_product_simple(a, b)<<" "<<c<<"\n";
    return 0;
}
