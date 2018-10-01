#include"Dopamine.hpp"

Dopamine::Dopamine() {
    strength.high=0.0;
    strength.low=0.0;
}

void Dopamine::SetStrength(DAStrength strength) {
    this->strength = strength;
}
void Dopamine::SetStrength(double high, double low) {
    this->strength.high = high;
    this->strength.low = low;
}
DAStrength Dopamine::GetStrength() {
    return strength;
}
void Dopamine::AddStrength(DAStrength strength) {
    this->strength.high += strength.high;
    this->strength.low += strength.low;
}