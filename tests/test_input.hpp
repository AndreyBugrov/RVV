#pragma once

#include <vector>

template<class T>
class TestInput{
protected:
    size_t length_;
    std::vector<T> data_;
public:
   TestInput(const T& data, size_t len): data_(data), length_(len){}
   size_t length() const{
    return length_;
   }
   std::vector<T>& data() const{
    return data_;
   }
};