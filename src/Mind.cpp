/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Mind.cpp
 * Author: zax
 *
 * Created on December 22, 2017, 10:39 AM
 */

#include "Mind.hpp"

Mind::Mind() {
}

Mind::Mind(const Mind& orig) {
}

Mind::~Mind() {
}

void Mind::AddRegion(vec_sptr<Neuron> region) {
    regions.push_back(region);
}
void Mind::AddConnection(sptr<Connection> conn) {
    conns.push_back(conn);
}
void Mind::AddExInput(sptr<ExInput> exin) {
    input.push_back(exin);
}
void Mind::AddElectrodeGroup(sptr<ElectrodeGroup> group) {
    electrode_groups.push_back(group);
}
void Mind::AddElectrode(sptr<Electrode> electrode, int group) {
    if(group < electrode_groups.size()) {
        electrode_groups[group]->electrodes.push_back(electrode);
    }
}
void Mind::AddSynapseRecorder(sptr<SynapseRecorder> rec) {
    syn_recorders.push_back(rec);
}

int Mind::GetNumberOfRegions() {
    return regions.size();
}
const vec<vec_sptr<Neuron>> & Mind::GetRegions() {
    return regions;
}

vec_sptr<Neuron> & Mind::GetRegion(int region) {
    return regions[region];
}

sptr<ElectrodeGroup> Mind::GetElectrodeGroup(int index) {
    if(index < electrode_groups.size()) return electrode_groups[index];
    else return nullptr;
}
vec_sptr<ElectrodeGroup> & Mind::GetElectrodeGroups() {
    return electrode_groups;
}
int Mind::GetNumberOfElectrodeGroups() {
    return electrode_groups.size();
}
sptr<ExInput> Mind::GetInput(int index) {
    if(index < input.size()) return input[index];
    else return nullptr;
}
//sptr<ExInput> Mind::GetOutput(int index) {
//    if(index < output.size()) return output[index].lock();
//    else return nullptr;
//}
int Mind::GetNumberOfInputs() { return input.size(); }
int Mind::GetNumberOfConnections() { return conns.size(); }
//int Mind::GetNumberOfOutputs() { return output.size(); }
vec_sptr<ExInput> & Mind::GetInputs() {
    return input;
}
//vec_wptr<ExInput> & Mind::GetOutputs() {
//    return output;
//}
sptr<Connection> Mind::GetConnection(int index) {
    return conns[index];
}
void Mind::Update(long time) {
    for(int i = 0; i < regions.size(); i++) {
        #pragma omp parallel for
        for(int k = 0; k < regions[i].size(); k++) {
            regions[i][k]->Update(time);
        }
    }
    for(int i = 0; i < syn_recorders.size(); i++) {
        syn_recorders[i]->Update(time);
    }
}
void Mind::Learn() {
    for(int i = 0; i < conns.size(); i++) {
        //conns[i]->RunPlasticity();
    }
}

void Mind::CleanUp() {
    for(int i = 0; i < syn_recorders.size(); i++) {
        syn_recorders[i]->Write();
    }
}

std::string Mind::GetTopology() {
    std::string str("");
    str += "  REGIONS: " + std::to_string(regions.size()) + "\n";

    for(int i = 0; i < regions.size(); i++) {
        str += "    [" + std::to_string(i) + "] " + std::to_string(regions[i].size()) +
                " neurons\n";
    }


    return str;
}

void Mind::SaveNeuronRNGToSeed() {
    for(vec<vec_sptr<Neuron>>::iterator it = regions.begin();
            it != regions.end(); it++) {
        for(vec_sptr<Neuron>::iterator it2 = it->begin();
                it2 != it->end(); it2++) {
            (*it2)->SaveRNGToSeed();
        }
    }
}

void Mind::TurnOffAllInput() {
    for(vec_sptr<ExInput>::iterator it = input.begin();
            it != input.end(); it++) {
        (*it)->Strength(0.0);
    }
}
