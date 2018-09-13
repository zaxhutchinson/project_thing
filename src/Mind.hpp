/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Mind.hpp
 * Author: zax
 *
 * Created on December 22, 2017, 10:39 AM
 */

#ifndef MIND_HPP
#define MIND_HPP

#include<string>

#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/vector.hpp"

#include"zaxlib.hpp"
#include"Neuron.hpp"
#include"Connection.hpp"
#include"ExInput.hpp"
#include"Electrode.hpp"
#include"SynapseRecorder.hpp"
#include"Dopamine.hpp"

class Mind {
public:
    Mind();
    Mind(const Mind& orig);
    virtual ~Mind();
    void AddRegion(vec_sptr<Neuron> region);
    void AddConnection(sptr<Connection> conn);
    void AddExInput(sptr<ExInput> exin);
    void AddElectrodeGroup(sptr<ElectrodeGroup> group);
    void AddElectrode(sptr<Electrode> electrode, int group);
    void AddSynapseRecorder(sptr<SynapseRecorder> rec);
    void AddDopamineChannel(sptr<Dopamine> dopamine);

    const vec<vec_sptr<Neuron>> & GetRegions();
    vec_sptr<Neuron> & GetRegion(int region);
    int GetNumberOfRegions();
    sptr<ElectrodeGroup> GetElectrodeGroup(int index);
    vec_sptr<ElectrodeGroup> & GetElectrodeGroups();
    int GetNumberOfElectrodeGroups();
    sptr<ExInput> GetInput(int index);
    //sptr<ExInput> GetOutput(int index);
    int GetNumberOfInputs();
    int GetNumberOfConnections();
    //int GetNumberOfOutputs();
    vec_sptr<ExInput> & GetInputs();
    //vec_wptr<ExInput> & GetOutputs();
    sptr<Connection> GetConnection(int index);
    vec_sptr<Dopamine> & GetDopamineChannels();
    sptr<Dopamine> GetDopamineChannel(int index);

    void Update(int64_t time);
    void StartLearning();
    void EndLearning();
    void ReleaseDopamine(int dopamine_channel, double strength);
    void PurgeDopamine(int dopamine_channel);

    void CleanUp();

    std::string GetTopology();

    void SaveNeuronRNGToSeed();
    void TurnOffAllInput();

    template<class Archive>
    void save(Archive & ar) const {
        ar(regions,conns,input,electrode_groups);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(regions,conns,input,electrode_groups);
        for(vec<vec_sptr<Neuron>>::iterator rit = regions.begin();
                rit != regions.end(); rit++) {
            for(vec_sptr<Neuron>::iterator nit = rit->begin();
                    nit != rit->end(); nit++) {
                all_neurons.push_back(*nit);
            }
        }
    }

private:
    vec<vec_sptr<Neuron>> regions;
    vec_sptr<Neuron> all_neurons;
    vec_sptr<Connection> conns;
    vec_sptr<ExInput> input;
    vec_sptr<ElectrodeGroup> electrode_groups;
    vec_sptr<SynapseRecorder> syn_recorders;
    vec_sptr<Dopamine> dopamine_channels;
};

#endif /* MIND_HPP */
