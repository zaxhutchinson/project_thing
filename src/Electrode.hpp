/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Electrode.hpp
 * Author: zax
 *
 * Created on November 21, 2017, 12:40 PM
 */

#ifndef ELECTRODE_HPP
#define ELECTRODE_HPP

#include<functional>
#include<string>

#include"cereal/archives/binary.hpp"
#include"cereal/types/string.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/vector.hpp"

#include"zaxlib.hpp"
#include"Neuron.hpp"
#include"Recorder.hpp"



class Electrode {
public:
    Electrode();
    Electrode(std::string name);
    Electrode(const Electrode& orig);
    virtual ~Electrode();
    
    void SetRecorder(sptr<Recorder> recorder);
    void InitNeuronRecordingData();
    
    void Name(std::string val);
    std::string Name();
    
    //int NumNeurons();
    void SetElectrode(sptr<Neuron> neuron);
    double Measure();
    bool Spike();
    int TimeLastSpike();
    
    void RecordNeuronData(int64_t time);
    void WriteNeuronData();
    
    template<class Archive>
    void save(Archive & ar) const {
        ar(name,neuron);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(name,neuron);
        InitNeuronRecordingData();
    }
    
private:
    std::string name;
    wptr<Neuron> neuron;
    sptr<Recorder> recorder;
    uptr<NeuronData> neuron_data;
};

struct ElectrodeGroup {
    std::string name;
    vec_sptr<Electrode> electrodes;
    sptr<Recorder> recorder;
    
    ElectrodeGroup();
    ElectrodeGroup(std::string name);
    unsigned int GetNumberOfElectrodes();
    sptr<Electrode> GetElectrode(int index);
    
    void MakeRecorder();
    void StartRecording();
    void StopRecording();
    void RecordData(int64_t time);

    template<class Archive>
    void save(Archive & ar) const {
        ar(name,electrodes);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(name,electrodes);
        MakeRecorder();
    }
};

#endif /* ELECTRODE_HPP */

