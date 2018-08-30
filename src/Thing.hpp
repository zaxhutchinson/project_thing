#ifndef THING_HPP
#define THING_HPP

#include<limits>

#include"zaxlib.hpp"

class Thing {
public:
    Thing();
    Thing(int num_details);
    double GetDetail(int index);
    void SetDetail(int index, double amount);
    void AddDetail(int index, double amount);
    void NormalizeDetail();
private:
    vec<double> details;
};

#endif
