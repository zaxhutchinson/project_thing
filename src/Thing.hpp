#ifndef THING_HPP
#define THING_HPP

#include<limits>
#include<iostream>

#include"zaxlib.hpp"
#include"Config.hpp"

class Thing {
public:
    Thing();
    Thing(std::string name);
    Thing(std::string name, int num_details);
    std::string GetName();
    int NumDetails();
    double GetDetail(int index);
    void SetDetail(int index, double amount);
    void AddDetail(int index, double amount);
    void CreateDetail(double amount);
    sptr<Thing> NormalizeDetail();
    static sptr<Thing> AbsDiff(sptr<Thing> t1, sptr<Thing> t2);
    std::string ToString();
private:
    std::string name;
    int num_details;
    vec<double> details;
};

#endif
