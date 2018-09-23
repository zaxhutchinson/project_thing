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
    std::string MODELDIR;
    double ALPHA_BASE;
    std::string THING_FILE;
    std::string TEST_FILE;
    std::string SESSION_FILE;

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
    double DELTA_TRACE_DECAY;
    
    int NUM_THING_DETAILS;


    // DOPAMINE
    double DOPAMINE_DECAY;
    double DOPAMINE_MIN;
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
    MODELDIR = reader.Get("general","modeldir","models");
    ALPHA_BASE = reader.GetReal("general","alpha_base",20.0);
    THING_FILE = reader.Get("general","thing_file","thing_defs.xml");
    TEST_FILE = reader.Get("general","test_file","test_defs.xml");
    SESSION_FILE = reader.Get("general","session_file","session_defs.xml");

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
    DELTA_TRACE_DECAY = reader.GetReal("synapse","delta_trace_decay",0.001);

    DOPAMINE_DECAY = reader.GetReal("dopamine","dopamine_decay",0.1);
    DOPAMINE_MIN = reader.GetReal("dopamine","dopamine_min",0.01);
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
