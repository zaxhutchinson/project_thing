#ifndef DOPAMINE_HPP
#define DOPAMINE_HPP

#include"cereal/archives/binary.hpp"

#include"zaxlib.hpp"

class Dopamine {
public:
    Dopamine();
    void SetStrength(double strength);
    double GetStrength();

    template<class Archive>
    void save(Archive & ar) const {
        ar(strength);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(strength);
    }
private:
    double strength;
};

#endif