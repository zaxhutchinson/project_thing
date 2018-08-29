/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zaxlib_struct.hpp
 * Author: zax
 *
 * Created on November 30, 2017, 10:22 AM
 */

#ifndef ZAXLIB_STRUCT_HPP
#define ZAXLIB_STRUCT_HPP

#include<memory>
#include<string>

#include"zaxlib_templates.hpp"
#include "zaxlib_enum.hpp"

struct SpikeField {
    int curtime;
    int elements;
    vec_uptr<bool> spikes;
    vec_uptr<std::string> names;
};

struct InputSwitch {
    InputSwitch() {
        output = OutputType::None;
    }
    vec<InputType> types;
    OutputType output;
};

struct NeuronData {
    std::string filename;
    unsigned int buffer_size;
    vec<int64_t> spikes;
};

#endif /* ZAXLIB_STRUCT_HPP */

