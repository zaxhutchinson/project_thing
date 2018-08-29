/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExInput.hpp
 * Author: zax
 *
 * Created on July 31, 2017, 10:42 AM
 */

#ifndef EXINPUT_HPP
#define EXINPUT_HPP

#include<string>
#include<vector>

#include"cereal/archives/binary.hpp"

#include"zaxlib.hpp"

class ExInput {
public:
    ExInput();
    ExInput(double base_output);
    ExInput(const ExInput& orig);
    virtual ~ExInput();
    double Output();
    void Strength(double val);
    double Strength();
    
    template<class Archive>
    void save(Archive & ar) const {
        ar(output,strength);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(output,strength);
    }
    
private:
    double output;
    double strength;
};

#endif /* EXINPUT_HPP */

