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
    session=nullptr;
}

Sim::Sim(const Sim& orig) {
}

Sim::~Sim() {
}

void Sim::BuildAgent(int sim_id) {
    agent = std::make_shared<Agent>();
    
    // Build and Add Mind
    sptr<Mind> mind = nullptr;
    switch(sim_id) {
        case 0:
            mind = Builder::Instance()->BuildMind000(rng()); break;

    }
    agent->AddMind(mind);

    // Electrode Group in slot 0 should always be the mouth.
    // Add the mind's electrodes in eg to agent as mouth.
    // Hacky code.
    sptr<ElectrodeGroup> eg = mind->GetElectrodeGroup(0);
    for(vec_sptr<Electrode>::iterator it = eg->electrodes.begin();
            it != eg->electrodes.end(); it++) {
        agent->AddMouth(*it);
    }

    vec_sptr<ExInput> & exins = mind->GetInputs();
    for(vec_sptr<ExInput>::iterator it = exins.begin();
            it != exins.end(); it++) {
        agent->AddEye(*it);
    }

}

void Sim::RunSimulation(sptr<Comms> comms) {

    time=0;

    Log::Instance()->Write("SIM: Starting main loop");

    sptr<Thing> response = nullptr;
    sptr<Thing> off_by = nullptr;
    
    while(!comms->test_done) {
        
        while(comms->pause_sim && !comms->quit_sim) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        agent->Update(time, rng);

        RecordData();

        if(session->StartStimulus(time)) {
            agent->GiveVisualInput(session->GetCurrentStimulus());
        } else if(session->EndStimulus(time)) {
            agent->PurgeAllInput();
        } 
        
        if(session->StartResponse(time)) {
            agent->PrepareResponse(time,session->ResponseDuration());
        } else if(session->EndResponse(time)) {
            response = agent->GetNormalizedResponse();
            std::cout << response->ToString() << std::endl;
            off_by = Thing::AbsDiff(response,session->GetCurrentFeedback());
        } 
        
        if(session->StartFeedback(time)) {
            agent->StartFeedback(off_by);
        } else if(session->EndFeedback(time)) {
            agent->EndFeedback(0);
        } 
        
        if(session->EndTest(time)) {
            comms->test_done=true;
        }

        time++;
        if(wait_time>0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        }
        if(time%1000==0) Log::Instance()->Write("Time: "+std::to_string(time));
    }
    
    std::cout << "SIM offby: " << off_by << std::endl;
    
    session->RemoveCurrentTest();
    Log::Instance()->Write("SIM: Main loop done");
}

int Sim::Time() { return time; }
sptr<Agent> Sim::GetAgent() { return agent; }

void Sim::SetSession(sptr<Session> session) {
    this->session = session;
}
sptr<Session> Sim::GetSession() {
    return this->session;
}
void Sim::SaveRNGToSeed() {
    seed = rng();
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

void Sim::StartAllRecordingElectrodes(std::string path) {
    Log::Instance()->Write("SIM: StartAllRecordingElectrodes - Enter");
    for(vec_sptr<ElectrodeGroup>::iterator it = recording.begin();
            it != recording.end(); it++) {
        (*it)->StartRecording(path);
    }
    Log::Instance()->Write("SIM: StartAllRecordingElectrodes - Exit");
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
