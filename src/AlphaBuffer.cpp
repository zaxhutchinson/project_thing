/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AlphaBuffer.cpp
 * Author: zax
 * 
 * Created on December 24, 2017, 11:02 AM
 */

#include "AlphaBuffer.hpp"
#include "Log.hpp"
#include "Config.hpp"

AlphaBuffer::AlphaBuffer() {
    alpha_length = static_cast<int>(7.7*config::ALPHA_BASE);
    for(int i = 0; i < alpha_length; i++) {
        alpha_func.push_back((i / config::ALPHA_BASE) * 
        std::exp( -(i - config::ALPHA_BASE) / config::ALPHA_BASE));
    }
}

AlphaBuffer::AlphaBuffer(const AlphaBuffer& orig) {
}

AlphaBuffer::~AlphaBuffer() {
}

void AlphaBuffer::AddSpike() {
    spikes.push_back(0);
}

double AlphaBuffer::Sum() {
    double sum = 0.0;
    for(std::list<int>::iterator it = spikes.begin(); it != spikes.end(); ) {

        sum += alpha_func[(*it)];
        it++;

    }
    return sum;
}

void AlphaBuffer::Update() {
    for(std::list<int>::iterator it = spikes.begin(); it != spikes.end(); ) {
        if(++(*it) >= alpha_length) it = spikes.erase(it);
        else {
            it++;
        }
    }
}

int AlphaBuffer::NumSpikes() {
    return spikes.size();
}

double AlphaBuffer::TotalAge() {
    double stdp = 0.0;
    for(std::list<int>::iterator it = spikes.begin(); it != spikes.end(); it++) {
        stdp += (1.0 / *it);
    }
    return stdp;
}

int AlphaBuffer::MostRecent() {
    if(spikes.size()>0) return spikes.back();
    else return -1;
}