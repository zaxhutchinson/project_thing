#ifndef SYNAPSERECORDER_HPP
#define SYNAPSERECORDER_HPP

#include<string>
#include<string>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdint>
#include<thread>
#include<functional>
#include<future>
#include<mutex>
#include<queue>

#include"zaxlib.hpp"
#include"Connection.hpp"

class SynapseRecorder {

public:
    SynapseRecorder();
    SynapseRecorder(std::string filename);
    void AddConnection(sptr<Connection> conn);
    void Update(int64_t time);
    void Write();
    void WriteSize();
    void SetFilename(std::string name);
    int Size();

private:
    std::string filename;
    vec_sptr<Connection> connections;
    vec<double> data_to_write;

};




#endif
