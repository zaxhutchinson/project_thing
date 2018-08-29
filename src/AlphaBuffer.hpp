/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AlphaBuffer.hpp
 * Author: zax
 *
 * Created on December 24, 2017, 11:02 AM
 */

#ifndef ALPHABUFFER_HPP
#define ALPHABUFFER_HPP

#include<cmath>
#include<memory>
#include<list>
#include<iostream>

#include"cereal/archives/binary.hpp"
#include"cereal/types/vector.hpp"
#include"cereal/types/list.hpp"

#include"zaxlib.hpp"

class AlphaBuffer {
public:
    AlphaBuffer();
    AlphaBuffer(const AlphaBuffer& orig);
    virtual ~AlphaBuffer();
    void AddSpike();
    double Sum();
    int NumSpikes();
    void Update();
    double TotalAge();
    int MostRecent();
    
    template<class Archive>
    void save(Archive & ar) const {
        ar(alpha_length,alpha_func,spikes);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(alpha_length,alpha_func,spikes);
    }
    
private:
    int alpha_length;
    vec<double> alpha_func;
    std::list<int> spikes;
};

#endif /* ALPHABUFFER_HPP */

