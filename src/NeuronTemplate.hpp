/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   NeuronTemplate.hpp
 * Author: zax
 *
 * Created on May 28, 2017, 2:51 PM
 */

#ifndef NEURONTEMPLATE_HPP
#define NEURONTEMPLATE_HPP

#include<memory>
#include<map>

enum NeuronType {
    RegularSpikingPPC,
    RegularSpiking,
    MediumSpiny,
    Relay,
    FastSpiking,
    FastSpikingTAN,
    ReticularThalamicNucleus
};

struct NT {
    double CAP;
    double VR;
    double VT;
    double K;
    double VPEAK;
    double A;
    double B;
    double C;
    double D;
    double NOISE;
    double BETA;
};


class NeuronTemplate {
public:
    static NeuronTemplate * Instance() {
        if(instance == nullptr) {
            instance = std::make_unique<NeuronTemplate>();
        }
        return instance.get();
    }

    NeuronTemplate();

    NT * GetNT(NeuronType type);

    std::map<NeuronType, std::unique_ptr<NT>> neuron_templates;

private:
    static std::unique_ptr<NeuronTemplate> instance;
};

#endif /* NEURONTEMPLATE_HPP */
