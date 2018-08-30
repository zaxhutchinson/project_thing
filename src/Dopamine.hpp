#ifndef DOPAMINE_HPP
#define DOPAMINE_HPP

#include"zaxlib.hpp"

class Dopamine {
public:
    Dopamine();
    void SetStrength(double strength);
    double GetStrength();
private:
    double strength;
};

#endif