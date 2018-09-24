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
#include"ThingManager.hpp"

#include"Comms.hpp"
#include"ResourceLoader.hpp"

using namespace std;

void PrintHelp();
void CheckStackSize();
sptr<Sim> LoadSim(std::string name);
sptr<Sim> BuildSim(int model);
sptr<Session> LoadSession(int session_id);
int BatchRun(sptr<Sim> sim, std::string session_dir);

int main(int argc, char** argv) {
    CheckStackSize();
    config::LoadConfig("config/config.ini");

    Log::Instance()->Write("PROJECT THING: Started");

    ThingManager::Instance()->LoadThings(config::THING_FILE);
    Log::Instance()->Write("THING MANAGER: Things loaded");
    TestManager::Instance()->LoadFiles();
    Log::Instance()->Write("TEST MANAGER: Tests loaded");
    
    bool load_model=false;
    std::string save_name;
    std::string load_name;
    int build = 0;
    int session_id=-1;

    Log::Instance()->Write("CMD LINE ARGS:");
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i],"-n")==0) {
            save_name = std::string(argv[i+1]);
            build = std::stoi(argv[i+2]);
            Log::Instance()->Write("  New: "+std::string(argv[i+1])+" "+std::string(argv[i+2]));
        }
        if(strcmp(argv[i],"-l")==0) {
            load_model=true;
            load_name = std::string(argv[i+1]);
            if(save_name.empty()) {
                save_name = load_name;
            }
            Log::Instance()->Write("  Load: "+std::string(argv[i+1]));
        }
        if(strcmp(argv[i],"-s")==0) {
            save_name = std::string(argv[i+1]);
            Log::Instance()->Write("  Save: "+std::string(argv[i+1]));
        }
        if(strcmp(argv[i],"-t")==0) {
            session_id = std::stoi(argv[i+1]);
            Log::Instance()->Write("  SessionID: "+std::string(argv[i+1]));
        }
        if(strcmp(argv[i],"-h")==0) {
            PrintHelp();
        }
    }
    
    sptr<Sim> sim = nullptr;
    if(load_model) {
        sim = LoadSim(config::MODELDIR+load_name);
    } else {
        sim = BuildSim(build);
    }

    sptr<Session> session = nullptr;
    if(session_id!=-1) {
        session = LoadSession(session_id);
    }

    // Make sure we're good
    if(sim==nullptr) {
        Log::Instance()->Write("MAIN: Sim null");
        return 1;
    }
    if(session==nullptr) {
        Log::Instance()->Write("MAIN: Session null.");
        return 1;
    }

    //Log::Instance()->Write(sim->GetAgent()->GetMind()->GetTopology());
    
    // Set the session.
    sim->SetSession(session);
    Log::Instance()->Write("MAIN: Session set: " + session->name);

    // Create session directory
    std::string dir = config::RECDIR + "/" +session->name;
    int err=mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(err==-1) {
        std::cout << "Error creating dir: " << dir << std::endl;
        return 1;
    }
    Log::Instance()->Write("MAIN: Directory created: " + dir);
    
    int rtn = BatchRun(sim, dir);

    // Save and done
    Serializer::Instance()->SaveComms(sim,config::MODELDIR+save_name);

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

sptr<Sim> LoadSim(std::string name) {
    sptr<Sim> sim = nullptr;
    Serializer::Instance()->LoadComms(sim,name);
    return sim;
}
sptr<Sim> BuildSim(int model) {
    sptr<Sim> sim = std::make_shared<Sim>();
    sim->BuildAgent(model);

    // Add the electrode groups to the sim itself.
    for(int i = 0; i < sim->GetAgent()->GetMind()->GetNumberOfElectrodeGroups(); i++) {
        sptr<ElectrodeGroup> eg = sim->GetAgent()->GetMind()->GetElectrodeGroup(i);
        sim->AddRecordingElectrodeGroup(eg);
    }

    return sim;
}
sptr<Session> LoadSession(int session_id) {
    return TestManager::Instance()->CreateSession(session_id);
}

int BatchRun(sptr<Sim> sim, std::string session_dir) {

    Log::Instance()->Write("MAIN: Run started");

    sptr<Session> session = sim->GetSession();

    Log::Instance()->Write("MAIN: Session got");

    int test_counter = 0;

    while(!session->SessionDone()) {
        std::cout << "Run # " << std::to_string(test_counter) << std::endl;
        Log::Instance()->Write("MAIN: test run #" + std::to_string(test_counter));

        sptr<Comms> comms = std::make_shared<Comms>();

        std::string test_id = std::to_string(session->GetNextTest()->id);

        std::string test_dir = session_dir + 
                "/" + test_id + "_" + std::to_string(test_counter);


        const int err=mkdir(test_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if(err==-1) {
            std::cout << "Error creating dir: " << test_dir << std::endl;
            return 1;
        } else {
            comms->recdir=test_dir;
        }

        std::thread t_sim;

        Log::Instance()->Write("MAIN: starting eletrode recorders");

        sim->StartAllRecordingElectrodes(test_dir);

        Log::Instance()->Write("MAIN: electrode recorders started");

        Log::Instance()->Write("MAIN: recording started");

        comms->pause_sim=false;

        // Begin logging the time.
        struct timespec start, finish;
        double elapsed;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        t_sim = std::thread(&Sim::RunSimulation, sim, comms);

        Log::Instance()->Write("MAIN: sim thread started.");
        
        while(!comms->quit_sim) {
            if(comms->test_done) {
                comms->pause_sim=true;
                sim->StopAllRecordingElectrodes();
                sim->GetAgent()->GetMind()->CleanUp();
                comms->quit_sim=true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
        
        Log::Instance()->Write("MAIN: joining sim thread");
        if(t_sim.joinable()) t_sim.join();
        Log::Instance()->Write("MAIN: Sim thread joined");

        clock_gettime(CLOCK_MONOTONIC, &finish);

        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        Log::Instance()->Write("  Time: " + std::to_string(elapsed));

        test_counter++;
    }

        Log::Instance()->Write("MAIN: run done");
}

void PrintHelp() {
    std::cout << "HELP MENU FOR SFROST" << std::endl;
    std::cout << "\t-n: NEW" << std::endl
                << "\t\t-n <save_name> <build_number>" << std::endl;
    std::cout << "\t-l: LOAD" << std::endl 
                << "\t\t-l <load_name>" << std::endl
                << "\t\t  If -s is absent, save_name is set to load_name" << std::endl;
    std::cout << "\t-s: SAVE" << std::endl
                << "\t\t-s <save_name>" << std::endl;
    std::cout << "\t-t: SESSION ID" << std::endl
                << "\t\t-t <session_id>" << std::endl;
    std::cout << "\t-h: HELP" << std::endl;
}
