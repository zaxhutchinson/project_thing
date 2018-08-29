/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntVec3.hpp
 * Author: zax
 *
 * Created on July 14, 2017, 8:03 AM
 */

#ifndef INTVEC3_HPP
#define INTVEC3_HPP

#include<cmath>
#include<iostream>

class IntVec3 {
public:
    IntVec3();
    IntVec3(int _x, int _y, int _z);
    IntVec3(const IntVec3& orig);
    virtual ~IntVec3();
    
    friend std::ostream& operator<<(std::ostream &stream, const IntVec3 & obj);
    bool operator==(const IntVec3 & other);
    
    int X() const;
    int Y() const;
    int Z() const;
    void X(int _x);
    void Y(int _y);
    void Z(int _z);
    
    int Distance(IntVec3 * other);

    
private:
    int x;
    int y;
    int z;
};

#endif /* INTVEC3_HPP */

