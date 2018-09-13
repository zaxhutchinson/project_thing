/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"Comms.hpp"

Comms::Comms() {
    quit_sim = false;
    quit_display = false;
    pause_sim = true;
    test_done = false;
}
Comms::Comms(const Comms& orig) {
}
Comms::~Comms() {   
}

