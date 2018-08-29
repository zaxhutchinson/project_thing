/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"NeuronTemplate.hpp"

std::unique_ptr<NeuronTemplate> NeuronTemplate::instance =nullptr;

NeuronTemplate::NeuronTemplate() {

    std::unique_ptr<NT> rs_ppc = std::make_unique<NT>();
    rs_ppc->CAP = 100.0;
    rs_ppc->VR = -60.0;
    rs_ppc->VT = -40.0;
    rs_ppc->K = 0.7;
    rs_ppc->VPEAK = 35.0;
    rs_ppc->A = 0.03;
    rs_ppc->B = -2.0;
    rs_ppc->C = -50.0;
    rs_ppc->D = 100.0;
    rs_ppc->NOISE = 350.0;//300.0;
    rs_ppc->BETA = 0.0;
    neuron_templates[NeuronType::RegularSpikingPPC] = std::move(rs_ppc);

    std::unique_ptr<NT> rs = std::make_unique<NT>();
    rs->CAP = 100.0;
    rs->VR = -60.0;
    rs->VT = -40.0;
    rs->K = 0.7;
    rs->VPEAK = 35.0;
    rs->A = 0.03;
    rs->B = -2.0;
    rs->C = -50.0;
    rs->D = 100.0;
    rs->NOISE = 350.0;//300.0;
    rs->BETA = 0.0;
    neuron_templates[NeuronType::RegularSpiking] = std::move(rs);

    // Used for the Caudate in FROST
    std::unique_ptr<NT> msn = std::make_unique<NT>();
    msn->CAP = 50.0;
    msn->VR = -80.0;
    msn->VT = -25.0;
    msn->K = 1.0;
    msn->VPEAK = 40.0;
    msn->A = 0.01;
    msn->B = -20.0;
    msn->C = -55.0;
    msn->D = 150.0;
    msn->NOISE = 800.0;//100.0;//400.0;
    msn->BETA = 0.0;
    neuron_templates[NeuronType::MediumSpiny] = std::move(msn);

    // Used for the MDN in FROST
    std::unique_ptr<NT> relay = std::make_unique<NT>();
    relay->CAP = 200.0;
    relay->VR = -60.0;
    relay->VT = -50.0;
    relay->K = 1.6;
    relay->VPEAK = 35.0;
    relay->A = 0.01;
    relay->B = 0;
    relay->C = -50.0;
    relay->D = 100.0;
    relay->NOISE = 800.0;//400.0;
    relay->BETA = 0.0;
    neuron_templates[NeuronType::Relay] = std::move(relay);

    // Fast Spiking Neurons: used for GPi and e in FROST
    // Used for PFC I in Sfrost
    std::unique_ptr<NT> fsn = std::make_unique<NT>();
    fsn->CAP = 20.0;
    fsn->VR = -55.0;
    fsn->VT = -40.0;
    fsn->K = 1.0;
    fsn->VPEAK = 25.0;
    fsn->A = 0.15;
    fsn->B = 8.0;
    fsn->C = -55.0;
    fsn->D = 200.0;
    fsn->NOISE = 0.0;//100.0;
    fsn->BETA = 0.0;
    neuron_templates[NeuronType::FastSpiking] = std::move(fsn);

    // Fast Spiking Neurons: used for GPi and e in FROST
    std::unique_ptr<NT> fsn_tan = std::make_unique<NT>();
    fsn_tan->CAP = 20.0;
    fsn_tan->VR = -55.0;
    fsn_tan->VT = -40.0;
    fsn_tan->K = 1.0;
    fsn_tan->VPEAK = 25.0;
    fsn_tan->A = 0.15;
    fsn_tan->B = 8.0;
    fsn_tan->C = -55.0;
    fsn_tan->D = 200.0;
    fsn_tan->NOISE = 100.0;
    fsn_tan->BETA = 160.0;
    neuron_templates[NeuronType::FastSpikingTAN] = std::move(fsn_tan);


    std::unique_ptr<NT> rtn = std::make_unique<NT>();
    rtn->CAP = 40.0;
    rtn->VR = -65.0;
    rtn->VT = -45.0;
    rtn->K = 0.25;
    rtn->VPEAK = 0.0;
    rtn->A = 0.015;
    rtn->B = 2.0;
    rtn->C = -55.0;
    rtn->D = 50.0;
    rtn->NOISE = 0.0;//100.0;//500.0;
    rtn->BETA = 0.0;
    neuron_templates[NeuronType::ReticularThalamicNucleus] = std::move(rtn);
}

NT * NeuronTemplate::GetNT(NeuronType type) {
    return neuron_templates.at(type).get();
}
