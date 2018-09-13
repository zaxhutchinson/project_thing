/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Comms.hpp
 * Author: zax
 *
 * Created on December 25, 2017, 11:12 AM
 */

#ifndef COMMS_HPP
#define COMMS_HPP



#include"zaxlib.hpp"
#include"Mind.hpp"

struct Comms {

    bool quit_sim;
    bool quit_display;
    bool pause_sim;
    bool test_done;

    Comms();
    Comms(const Comms & orig);
    ~Comms();

};






#endif /* COMMS_HPP */

