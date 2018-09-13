/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Serializer.cpp
 * Author: zax
 * 
 * Created on January 1, 2018, 11:36 AM
 */

#include "Serializer.hpp"
#include "Sim.hpp"

uptr<Serializer> Serializer::instance = nullptr;
Serializer * Serializer::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<Serializer>();
    }
    return instance.get();
}

Serializer::Serializer() {
    
}

Serializer::Serializer(const Serializer& orig) {
}

Serializer::~Serializer() {
}

void Serializer::SaveComms(sptr<Sim> & sim, std::string name) {
    sim->SaveRNGToSeed();
    sim->GetAgent()->GetMind()->SaveNeuronRNGToSeed(); // Save individual RNGs
    std::ofstream out(name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive oarchive(out);
    oarchive(sim);
}

void Serializer::LoadComms(sptr<Sim> & sim, std::string name) {
    std::ifstream in(name, std::ios::binary | std::ios::in);
    cereal::BinaryInputArchive iarchive(in);
    iarchive(sim);
}