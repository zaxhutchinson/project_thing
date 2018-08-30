#include"Dopamine.hpp"

Dopamine::Dopamine() {
    strength=0.0;
}

void Dopamine::SetStrength(double strength) {
    this->strength = strength;
}
double Dopamine::GetStrength() {
    return strength;
}