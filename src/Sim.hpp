/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Sim.hpp
 * Author: zax
 *
 * Created on January 1, 2018, 12:25 PM
 */

#ifndef SIM_HPP
#define SIM_HPP

#include<chrono>
#include<thread>
#include<memory>
#include<random>

#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"

#include"Builder.hpp"
#include"Log.hpp"
#include"Comms.hpp"
#include"Agent.hpp"
#include"zaxlib.hpp"
#include"TestManager.hpp"

class Sim {
public:
    Sim();
    Sim(const Sim& orig);
    virtual ~Sim();

    bool BuildAgent(int sim_id);

    void RunSimulation(sptr<Comms> comms);

    int Time();
    sptr<Agent> GetAgent();

    void SetSession(sptr<Session> session);
    sptr<Session> GetSession();

    void SaveRNGToSeed();

    void IncrementWaitTime();
    void DecrementWaitTime();
    int GetWaitTime();

    void RecordData();
    void AddRecordingElectrodeGroup(sptr<ElectrodeGroup> eg);
    void StopAllRecordingElectrodes();
    void StartAllRecordingElectrodes(std::string path);
    void ClearAllRecordingElectrodes();

    template<class Archive>
    void save(Archive & ar) const {
        ar(agent,seed,recording);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(agent,seed,recording);
        rng = std::mt19937_64(seed);
        Log::Instance()->Write("SERIALIZE: Loading Sim Complete");
    }
private:
    vec_sptr<ElectrodeGroup> recording;
    sptr<Agent> agent;
    sptr<Session> session;
    int64_t time;
    int wait_time; // in millisecs
    long seed;
    std::mt19937_64 rng;
};

#endif /* SIM_HPP */
