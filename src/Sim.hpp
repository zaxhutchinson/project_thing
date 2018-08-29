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
#include"zaxlib.hpp"

class Sim {
public:
    Sim();
    Sim(const Sim& orig);
    virtual ~Sim();

    void BuildMind(int sim_id);

    void RunSimulation(sptr<Comms> comms);

    int Time();
    sptr<Mind> GetMind();

    void SaveRNGToSeed();
    void TurnOffAllInput();

    void IncrementWaitTime();
    void DecrementWaitTime();
    int GetWaitTime();

    void RecordData();
    void AddRecordingElectrodeGroup(sptr<ElectrodeGroup> eg);
    void StopAllRecordingElectrodes();
    void StartAllRecordingElectrodes();
    void ClearAllRecordingElectrodes();

    template<class Archive>
    void save(Archive & ar) const {
        ar(time,mind,seed);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(time,mind,seed);
        rng = std::mt19937_64(seed);
        Log::Instance()->Write("SERIALIZE: Loading Sim Complete");
    }
private:
    vec_sptr<ElectrodeGroup> recording;
    sptr<Mind> mind;
    int64_t time;
    int wait_time; // in millisecs
    long seed;
    std::mt19937_64 rng;
};

#endif /* SIM_HPP */
