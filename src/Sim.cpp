/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Sim.cpp
 * Author: zax
 *
 * Created on January 1, 2018, 12:25 PM
 */

#include "Sim.hpp"
#include "ResourceLoader.hpp"

Sim::Sim() {

    std::random_device rd;
    rng = std::mt19937_64(rd());

    time = 0;
    wait_time = 0;
}

Sim::Sim(const Sim& orig) {
}

Sim::~Sim() {
}

void Sim::BuildMind(int sim_id) {
    switch(sim_id) {
//        case 1:
//            mind = Builder::Instance()->Build001(rng()); break;

    }
}

void Sim::RunSimulation(sptr<Comms> comms) {

    Log::Instance()->Write("MAIN: Starting main loop");

    while(!comms->quit_sim) {
        while(comms->pause_sim && !comms->quit_sim) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        mind->Update(time);

        RecordData();

        time++;
        if(wait_time>0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        }
        if(time%1000==0) Log::Instance()->Write("Time: "+std::to_string(time));
    }
    Log::Instance()->Write("MAIN: Main loop done");
}

int Sim::Time() { return time; }
sptr<Mind> Sim::GetMind() { return mind; }

void Sim::SaveRNGToSeed() {
    seed = rng();
}

void Sim::TurnOffAllInput() {
    mind->TurnOffAllInput();
}

void Sim::IncrementWaitTime() {
    wait_time++;
}

void Sim::DecrementWaitTime() {
    if(wait_time>0) wait_time--;
}

int Sim::GetWaitTime() {
    return wait_time;
}

void Sim::RecordData() {
    for(vec_sptr<ElectrodeGroup>::iterator it = recording.begin();
            it != recording.end(); it++) {
        (*it)->RecordData(time);
    }
}

void Sim::AddRecordingElectrodeGroup(sptr<ElectrodeGroup> eg) {
    recording.push_back(eg);
}

void Sim::StartAllRecordingElectrodes() {
    for(vec_sptr<ElectrodeGroup>::iterator it = recording.begin();
            it != recording.end(); it++) {
        (*it)->StartRecording();
    }
}

void Sim::StopAllRecordingElectrodes() {
    for(vec_sptr<ElectrodeGroup>::iterator it = recording.begin();
            it != recording.end(); it++) {
        (*it)->StopRecording();
    }
}

void Sim::ClearAllRecordingElectrodes() {
    StopAllRecordingElectrodes();
    recording.clear();
}
