/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Config.hpp
 * Author: zax
 *
 * Created on August 16, 2017, 8:01 AM
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include<string>
#include<iostream>
#include<memory>
#include<random>

#include"INIReader.h"

namespace config {

    extern std::random_device rd;
    extern std::mt19937_64 rng;

    extern int DATASIZE;
    extern bool DEBUG;
    extern std::string LOG_FNAME_MAIN;
    extern std::string LOG_FNAME_DEBUG;
    extern std::string LOG_FNAME_RUN;
    extern std::string RECDIR;
    extern double ALPHA_BASE;

    // BRAIN
    extern int CORE_X;
    extern int CORE_Y;
    extern int CORE_Z;
    extern int NUMBER_CORE_NEURONS;

    // SYNAPSE
    extern double MIN_STRENGTH;
    extern double STDP_DECAY;
    extern int MAX_DELAY;
    extern double FAST_POTENTIATION_DECAY;
    extern double MEDIUM_POTENTIATION_DECAY;
    extern double SLOW_POTENTIATION_DECAY;
    extern double FAST_POTENTIATION_MOD;
    extern double MEDIUM_POTENTIATION_MOD;
    extern double SLOW_POTENTIATION_MOD;
    extern double DELTA_TRACE_DECAY;

    // MAP
    extern int NUM_THING_DETAILS;

    bool LoadConfig(std::string filename);

    void PrintConfig();
};

#endif /* CONFIG_HPP */
