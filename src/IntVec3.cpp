/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntVec3.cpp
 * Author: zax
 * 
 * Created on July 14, 2017, 8:03 AM
 */

#include "IntVec3.hpp"
#include "Connection.hpp"

IntVec3::IntVec3() {
}

IntVec3::IntVec3(int _x, int _y, int _z) :
x(_x), y(_y), z(_z) {
    
}

IntVec3::IntVec3(const IntVec3 & orig) {
    x = orig.X();
    y = orig.Y();
    z = orig.Z();
}

IntVec3::~IntVec3() {
}

std::ostream& operator<<(std::ostream &stream, const IntVec3 & obj) {
    return stream << "(" << obj.X() << "," << obj.Y() << "," << obj.Z() << ")";
}

bool IntVec3::operator ==(const IntVec3& other) {
    if(x == other.X() &&
            y == other.Y() &&
            z == other.Z()) {
        return true;
    }
    else {
        return false;
    }
}

int IntVec3::X() const { return x; }
int IntVec3::Y() const { return y; }
int IntVec3::Z() const { return z; }
void IntVec3::X(int _x) { x = _x; }
void IntVec3::Y(int _y) { y = _y; }
void IntVec3::Z(int _z) { z = _z; }

int IntVec3::Distance(IntVec3 * other) {
    return static_cast<int>(
            std::sqrt(
                std::pow(other->X() - x, 2.0) +
                std::pow(other->Y() - y, 2.0) +
                std::pow(other->Z() - z, 2.0)
            ));
}