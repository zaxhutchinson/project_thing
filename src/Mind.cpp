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

void Mind::Reset() {

    for(vec_sptr<Connection>::iterator it = conns.begin();
            it != conns.end(); it++) {
        (*it)->Reset();
    }

}

void Mind::AddRegion(vec_sptr<Neuron> region) {
    all_neurons.insert(all_neurons.begin(), region.begin(), region.end());
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
void Mind::AddDopamineChannel(sptr<Dopamine> dopamine) {
    dopamine_channels.push_back(dopamine);
}
/* int Mind::GetNumberOfRegions() {
    return regions.size();
}
const vec<vec_sptr<Neuron>> & Mind::GetRegions() {
    return regions;
}

vec_sptr<Neuron> & Mind::GetRegion(int region) {
    return regions[region];
} */

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
vec_sptr<Dopamine> & Mind::GetDopamineChannels() {
    return dopamine_channels;
}
sptr<Dopamine> Mind::GetDopamineChannel(int index) {
    return dopamine_channels[index];
}
void Mind::Update(int64_t time, std::mt19937_64 & rng) {

    std::shuffle(all_neurons.begin(), all_neurons.end(), rng);

    #pragma omp parallel for
    for(int k = 0; k < all_neurons.size(); k++) {
        all_neurons[k]->Update(time);
    }
    for(int i = 0; i < syn_recorders.size(); i++) {
        syn_recorders[i]->Update(time);
    }
    //#pragma omp parallel for
    // double change=0.0;
    for(vec_sptr<Connection>::iterator it = conns.begin();
             it != conns.end(); it++) {
        (*it)->DecayDeltaTrace();
    //     change+=(*it)->Learn();
    }
    // if(change != 0.0)
    //     std::cout << "CHANGE: " << change << std::endl;

}
void Mind::StartLearning() {
    for(int i = 0; i < conns.size(); i++) {
        conns[i]->SetLearning(true);
    }
}
void Mind::EndLearning() {
    for(int i = 0; i < conns.size(); i++) {
        conns[i]->SetLearning(false);
    }
}
void Mind::RunFeedback() {
    
    double change=0.0;

    do {
        change=0.0;

        for(int k = 0; k < all_neurons.size(); k++) {
            all_neurons[k]->BackProp();
        }

        
        for(vec_sptr<Connection>::iterator it = conns.begin();
                it != conns.end(); it++) {

            change+=(*it)->Learn();
        }
        if(change != 0.0)
            std::cout << "CHANGE: " << change << std::endl;
    } while(std::abs(change) >= config::DOPAMINE_MIN);
}
void Mind::ReleaseDopamine(int dopamine_channel, DAStrength strength) {
    sptr<Dopamine> da = GetDopamineChannel(dopamine_channel);
    da->SetStrength(strength);
}
void Mind::PurgeDopamine(int dopamine_channel) {
    sptr<Dopamine> da = GetDopamineChannel(dopamine_channel);
    da->SetStrength(0.0,0.0);
}

void Mind::CleanUp() {
    for(int i = 0; i < syn_recorders.size(); i++) {
        syn_recorders[i]->Write();
    }
}

std::string Mind::GetTopology() {

}

void Mind::SaveNeuronRNGToSeed() {
    for(vec_sptr<Neuron>::iterator it = all_neurons.begin();
            it != all_neurons.end(); it++) {
            (*it)->SaveRNGToSeed();
    }
}

void Mind::TurnOffAllInput() {
    for(vec_sptr<ExInput>::iterator it = input.begin();
            it != input.end(); it++) {
        (*it)->Strength(0.0);
    }
}
