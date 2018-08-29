/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Builder.hpp
 * Author: zax
 *
 * Created on December 25, 2017, 8:50 AM
 */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include<random>
#include<algorithm>
#include<iostream>

#include"Log.hpp"
#include"Mind.hpp"
#include"zaxlib.hpp"
#include"NeuronTemplate.hpp"
#include"SynapseRecorder.hpp"

class Builder {
public:
    static Builder * Instance();
    Builder();
    Builder(const Builder& orig);
    virtual ~Builder();

//    sptr<Mind> Build001(long seed);



private:
    static uptr<Builder> instance;
    std::mt19937_64 rng;
};

#endif /* BUILDER_HPP */
