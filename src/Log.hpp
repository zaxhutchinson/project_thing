/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.hpp
 * Author: zax
 *
 * Created on December 25, 2017, 1:01 PM
 */

#ifndef LOG_HPP
#define LOG_HPP

#include<iostream>
#include<fstream>
#include<memory>

class Log {
public:
    static Log * Instance();
    Log();
    Log(const Log& orig);
    virtual ~Log();
    void Write(std::string str);
    bool GetDebug();
    void SetDebug(bool val);
    void Debug(std::string msg);
    void Run(std::string msg);
private:
    static std::unique_ptr<Log> instance;
    std::ofstream main_out;
    std::ofstream debug_out;
    std::ofstream run_out;
    bool debug;
};

#endif /* LOG_HPP */

