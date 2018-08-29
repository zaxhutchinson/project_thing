/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Electrode.cpp
 * Author: zax
 * 
 * Created on November 21, 2017, 12:40 PM
 */

#include "Electrode.hpp"
#include "Log.hpp"


///////////////////////////////////////////////////////////////////////////////
Electrode::Electrode() {}
Electrode::Electrode(std::string name) {
    this->name = name;
    InitNeuronRecordingData();
}

Electrode::Electrode(const Electrode& orig) {
}

Electrode::~Electrode() {
}

void Electrode::SetRecorder(sptr<Recorder> recorder) {
    this->recorder = recorder;
}

void Electrode::InitNeuronRecordingData() {
    this->neuron_data = std::make_unique<NeuronData>();
    this->neuron_data->filename = this->name;
    this->neuron_data->buffer_size = config::DATASIZE;
}

void Electrode::Name(std::string val) {
    name = val;
}

std::string Electrode::Name() {
    return name;
}

//int Electrode::NumNeurons() { return neurons.size(); }

void Electrode::SetElectrode(sptr<Neuron> neuron) {
    this->neuron=neuron;
}

double Electrode::Measure() {
    return neuron.lock()->Output(0);
}

bool Electrode::Spike() {
    return neuron.lock()->Spiked();
}

void Electrode::RecordNeuronData(int64_t time) {
    if(Spike()) {
        this->neuron_data->spikes.push_back(time);
        
        if(this->neuron_data->spikes.size()==this->neuron_data->buffer_size) {
            WriteNeuronData();
        
            InitNeuronRecordingData();
        }
    }
}

void Electrode::WriteNeuronData() {
    recorder->AddNeuronData(std::move(neuron_data));
    InitNeuronRecordingData();
}

////////////////////////////////////////////////////////////////////////////

ElectrodeGroup::ElectrodeGroup() {}
ElectrodeGroup::ElectrodeGroup(std::string name) {
    this->name = name;
}
unsigned int ElectrodeGroup::GetNumberOfElectrodes() {
    return electrodes.size();
}
sptr<Electrode> ElectrodeGroup::GetElectrode(int index) {
    if(index < electrodes.size()) {
        return electrodes[index];
    }
    else return nullptr;
}

void ElectrodeGroup::MakeRecorder() {
    recorder = std::make_shared<Recorder>();
}

void ElectrodeGroup::StartRecording() {
    MakeRecorder();
    recorder->StartRecording();
    for(vec_sptr<Electrode>::iterator it = electrodes.begin();
            it != electrodes.end(); it++) {
        (*it)->SetRecorder(recorder);
    }
}

void ElectrodeGroup::StopRecording() {
    if(recorder != nullptr) {
        recorder->StopRecording();
        for(vec_sptr<Electrode>::iterator it = electrodes.begin();
                it != electrodes.end(); it++) {
            (*it)->WriteNeuronData();
        }
        recorder->ClearUpRecording();
        recorder = nullptr;
    }
}

void ElectrodeGroup::RecordData(int64_t time) {
    for(vec_sptr<Electrode>::iterator it = electrodes.begin();
            it != electrodes.end(); it++) {
        (*it)->RecordNeuronData(time);
    }
}