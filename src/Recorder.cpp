/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Recorder.cpp
 * Author: zax
 * 
 * Created on February 15, 2018, 6:44 PM
 */

#include "Recorder.hpp"
#include "Config.hpp"
#include "AlphaBuffer.hpp"
#include "Log.hpp"

Recorder::Recorder() {
    this->path = config::RECDIR + "/";
    this->run = false;   
}
Recorder::Recorder(std::string path) {
    this->path = path;
    this->run = false;
}

Recorder::Recorder(const Recorder& orig) {
    this->path = orig.path;
    this->run = orig.run;
}

Recorder::~Recorder() {
    if(t_write.joinable()) {
        t_write.join();
    }
}

void Recorder::AddNeuronData(uptr<NeuronData> data) {
    qlock.lock();
    data_to_write.push(std::move(data));
    qlock.unlock();
}

void Recorder::StartRecording() {
    this->run=true;
    while(t_write.joinable()) ;
    this->t_write = std::thread(&Recorder::WriteThread, this);
}

void Recorder::StopRecording() {
    run=false;
    if(t_write.joinable()) {
        t_write.join();
    }
}

void Recorder::ClearUpRecording() {
    // Clean up all data left in buffers.
    while(!data_to_write.empty()) {
        
        uptr<NeuronData> data = std::move(data_to_write.front());
        data_to_write.pop();
        
        std::string fname(this->path +"/"+ data->filename);
        std::ofstream of(fname,std::ios::binary|std::ios::out|std::ios::app);

        
        for(unsigned int i = 0; i < data->spikes.size(); i++) {
            of.write(reinterpret_cast<char*>(&data->spikes[i]), sizeof(int64_t));
        }
        
        of.close();
    }
}

void Recorder::WriteThread() {
    while(run) {
        
        if(!data_to_write.empty()) {
            
            qlock.lock();
            uptr<NeuronData> data = std::move(data_to_write.front());
            data_to_write.pop();
            qlock.unlock();
            
            std::string fname(this->path +"/"+ data->filename);
            std::ofstream of(fname,std::ios::binary|std::ios::out|std::ios::app);
            for(unsigned int i = 0; i < data->spikes.size(); i++) {
                of.write(reinterpret_cast<char*>(&data->spikes[i]), sizeof(int64_t));
            }
            
            of.close();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}