/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Config.cpp
 * Author: zax
 *
 * Created on August 16, 2017, 8:01 AM
 */

#include "Config.hpp"

namespace config {

    std::random_device rd;
    std::mt19937_64 rng;

    int DATASIZE;
    bool DEBUG;
    std::string LOG_FNAME_MAIN;
    std::string LOG_FNAME_DEBUG;
    std::string LOG_FNAME_RUN;
    std::string RECDIR;
    double ALPHA_BASE;

    int NUMBER_CORE_NEURONS;
    int CORE_X;
    int CORE_Y;
    int CORE_Z;

    // SYNAPSE
    double MIN_STRENGTH;
    double STDP_DECAY;
    int MAX_DELAY;
    double FAST_POTENTIATION_DECAY;
    double MEDIUM_POTENTIATION_DECAY;
    double SLOW_POTENTIATION_DECAY;
    double FAST_POTENTIATION_MOD;
    double MEDIUM_POTENTIATION_MOD;
    double SLOW_POTENTIATION_MOD;
    // map
    int NUM_THING_DETAILS;

}

bool config::LoadConfig(std::string filename) {

    INIReader reader(filename);

    if(reader.ParseError() < 0) {
        std::cerr << "Cannot load " << filename << std::endl;
    }

    rng.seed(rd());

    DATASIZE = reader.GetInteger("general","datasize",1000);
    DEBUG = reader.GetBoolean("general","debug",false);
    LOG_FNAME_MAIN = reader.Get("general","log_fname_main","out.log");
    LOG_FNAME_DEBUG = reader.Get("general","log_fname_debug","debug.log");
    LOG_FNAME_RUN = reader.Get("general","log_fname_run","run.log");
    RECDIR = reader.Get("general","recdir","recordings");
    ALPHA_BASE = reader.GetReal("general","alpha_base",20.0);

    NUMBER_CORE_NEURONS = reader.GetInteger("brain","number_core_neurons",1);
    CORE_X = reader.GetInteger("brain", "core_x",1000);
    CORE_Y = reader.GetInteger("brain", "core_y", 1000);
    CORE_Z = reader.GetInteger("brain", "core_z", 1000);

    MIN_STRENGTH = reader.GetReal("synapse","min_strength",0.1);
    STDP_DECAY = reader.GetReal("synapse","stdp_decay",0.9);
    MAX_DELAY = reader.GetInteger("synapse","max_delay",20);
    FAST_POTENTIATION_DECAY = reader.GetReal("synapse","fast_potentiation_decay",0.001);
    MEDIUM_POTENTIATION_DECAY = reader.GetReal("synapse","medium_potentiation_decay",0.001);
    SLOW_POTENTIATION_DECAY = reader.GetReal("synapse","slow_potentiation_decay",0.001);
    FAST_POTENTIATION_MOD = reader.GetReal("synapse","fast_potentiation_mod",2.0);
    MEDIUM_POTENTIATION_MOD = reader.GetReal("synapse","medium_potentiation_mod",2.0);
    SLOW_POTENTIATION_MOD = reader.GetReal("synapse","slow_potentiation_mod",2.0);

    NUM_THING_DETAILS = reader.GetReal("map","num_thing_details",16);
}

void config::PrintConfig() {
    std::cout << "======================================================\n";
    std::cout << "                 CONFIG DATA\n";
    std::cout << "======================================================\n";

    std::cout << "\n*** GENERAL ***\n"
            << "DATASIZE: " << DATASIZE << std::endl;

    std::cout << "\n*** BRAIN ***\n"
            << "CORE_X: " << CORE_X << std::endl
            << "CORE_Y: " << CORE_Y << std::endl
            << "CORE_Z: " << CORE_Z << std::endl
            << "NUMBER_CORE_NEURONS: " << NUMBER_CORE_NEURONS << std::endl;

    std::cout << "\n*** SYNAPSE ***\n"
            << "MIN_STRENGTH: " << MIN_STRENGTH << std::endl
            << "STDP_DECAY: " << STDP_DECAY << std::endl
            << "MAX_DELAY: " << MAX_DELAY << std::endl;


    std::cout << "======================================================\n\n";

}
