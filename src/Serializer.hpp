/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Serializer.hpp
 * Author: zax
 *
 * Created on January 1, 2018, 11:36 AM
 */

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include<string>
#include<memory>
#include<fstream>

#include"cereal/cereal.hpp"
#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"

#include"zaxlib.hpp"
#include"Sim.hpp"
#include"Comms.hpp"

class Serializer {
public:
    static Serializer * Instance();
    Serializer();
    Serializer(const Serializer& orig);
    virtual ~Serializer();
    
    void SaveComms(sptr<Sim> & sim, std::string name);
    void LoadComms(sptr<Sim> & sim, std::string name);
    
private:
    static uptr<Serializer> instance;
};

#endif /* SERIALIZER_HPP */

