/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Recorder.hpp
 * Author: zax
 *
 * Created on February 15, 2018, 6:44 PM
 */

#ifndef RECORDER_HPP
#define RECORDER_HPP

#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdint>
#include<thread>
#include<functional>
#include<future>
#include<list>
#include<mutex>
#include<queue>

//#include<H5Cpp.h>

#include"zaxlib.hpp"

enum class RecordType {
    ALPHAOUT,
    SPIKES,
};



class Recorder {
public:
    Recorder();
    Recorder(const Recorder& orig);
    virtual ~Recorder();
    
    void AddNeuronData(uptr<NeuronData> data);

    void WriteThread();
    
    void StartRecording();
    void StopRecording();
    void ClearUpRecording();
private:
    bool run;
    std::string path;
    RecordType type;
    std::mutex qlock;
    std::queue<uptr<NeuronData>> data_to_write;
    std::thread t_write;
};

#endif /* RECORDER_HPP */

