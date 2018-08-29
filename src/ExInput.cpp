/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ExInput.cpp
 * Author: zax
 * 
 * Created on July 31, 2017, 10:42 AM
 */

#include "ExInput.hpp"
ExInput::ExInput() {}
ExInput::ExInput(double base_output) {
    this->output = base_output;
    this->strength = 0.0;
}

ExInput::ExInput(const ExInput& orig) {
    this->output = orig.output;
    this->strength = 0.0;
}

ExInput::~ExInput() {
}

double ExInput::Output() {
    return output*strength;
}

void ExInput::Strength(double val) {
    strength = val;
}

double ExInput::Strength() {
    return strength;
}