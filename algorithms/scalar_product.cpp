#include "scalar_product.hpp"

double scalar_product_simple(const vector<double>& a, const vector<double>& b){
    if(a.size()!=b.size()){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a.size(), " and ", b.size()));
    }
    size_t len = a.size();
    double prod = 0.0; 
    for(int i =0; i < len; ++i){
        prod+=a[i]*b[i];
    }
    return prod;
}

double scalar_product_std(const vector<double>& a, const vector<double>& b){
    if(a.size()!=b.size()){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a.size(), " and ", b.size()));
    }
    return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

double scalar_product_boost(const vector<double>& a, const vector<double>& b){
    if(a.size()!=b.size()){
        throw Exception(ErrorType::kUnequalLengthError, generate_string("Not equal lengths: ", a.size(), " and ", b.size()));
    }
    return 0.;
}