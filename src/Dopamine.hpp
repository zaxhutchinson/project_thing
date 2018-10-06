#ifndef DOPAMINE_HPP
#define DOPAMINE_HPP

#include"cereal/archives/binary.hpp"

#include"zaxlib.hpp"

struct DAStrength {
    double high=0.0;
    double low=0.0;

    template<class Archive>
    void save(Archive & ar) const {
        ar(high,low);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(high,low);
    }
};

class Dopamine {
public:
    Dopamine();
    void SetStrength(DAStrength strength);
    void SetStrength(double high, double low);
    DAStrength GetStrength();
    void AddStrength(DAStrength strength);
    void AddStrengthHigh(double h);
    void AddStrengthLow(double l);

    template<class Archive>
    void save(Archive & ar) const {
        ar(strength);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(strength);
    }
private:
    DAStrength strength;
};

#endif