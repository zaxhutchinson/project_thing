/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: zax
 *
 * Created on December 22, 2017, 10:10 AM
 */

//#define DISPLAY_ACTIVE

#include<time.h>
#include<iostream>
#include<random>
#include<thread>
#include<fstream>
#include<sys/resource.h>
#include<sys/stat.h>

#include"Builder.hpp"

#include"Sim.hpp"
#include"Comms.hpp"
#include"Electrode.hpp"
#include"Log.hpp"
#include"Serializer.hpp"
#include"ResourceLoader.hpp"

#include"Comms.hpp"
#include"ResourceLoader.hpp"

using namespace std;

void CheckStackSize();
int BatchRun(int build);

int main(int argc, char** argv) {

    CheckStackSize();

    config::LoadConfig("config/config.ini");

    if(argc>1) {
        for(int i = 0; i < argc; i++) {
            if(strcmp(argv[i],"-b")==0) {
                int build = std::stoi(argv[i+1]);
                return BatchRun(build);
            }
        }
    }

    Log::Instance()->Write("MAIN: All done");

    return 0;
}

void CheckStackSize() {
    const rlim_t stack_size = 64 * 1024 * 1024;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if( result == 0 ) {
        if(rl.rlim_cur < stack_size) {
            rl.rlim_cur = stack_size;
            result = setrlimit(RLIMIT_STACK, &rl);
            if(result != 0) {
                std::cerr << "setrlimit returned result: " << result << std::endl;
            }
            else {
                std::cout << "Stack size set to " << stack_size << std::endl;
            }
        }
    }
}

int BatchRun(int build) {

    std::string dir = config::RECDIR;

    // Begin logging the time.
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    config::RECDIR=dir;
    const int err=mkdir(config::RECDIR.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(err==-1) {
        std::cout << "Error creating dir: " << config::RECDIR << std::endl;
        return 1;
    }

    sptr<Comms> comms = std::make_shared<Comms>();

    std::thread t_sim;

    sptr<Sim> sim = std::make_shared<Sim>();
    sim->BuildMind(build);

    for(int i = 0; i < sim->GetMind()->GetNumberOfElectrodeGroups(); i++) {
        sptr<ElectrodeGroup> eg = sim->GetMind()->GetElectrodeGroup(i);
        sim->AddRecordingElectrodeGroup(eg);
    }

    sim->StartAllRecordingElectrodes();

    comms->pause_sim=false;

    t_sim = std::thread(&Sim::RunSimulation, sim, comms);

    while(!comms->quit_sim) {
        if(true) {
            comms->pause_sim=true;
            sim->StopAllRecordingElectrodes();
            sim->GetMind()->CleanUp();
            comms->quit_sim=true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
    if(t_sim.joinable()) t_sim.join();

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    Log::Instance()->Write("  Time: " + std::to_string(elapsed));

}
