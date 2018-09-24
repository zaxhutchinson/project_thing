/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Log.cpp
 * Author: zax
 * 
 * Created on December 25, 2017, 1:01 PM
 */

#include "Log.hpp"
#include "Config.hpp"

std::unique_ptr<Log> Log::instance = nullptr;

Log* Log::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<Log>();
    }
    return instance.get();
}

Log::Log() {
    main_out = std::ofstream(config::LOG_FNAME_MAIN, std::ofstream::out);
    debug_out = std::ofstream(config::LOG_FNAME_DEBUG, std::ofstream::out);
    run_out = std::ofstream(config::LOG_FNAME_RUN, std::ofstream::out);
    debug=config::DEBUG;
}

Log::Log(const Log& orig) {
}

Log::~Log() {
    main_out.close();
    debug_out.close();
    run_out.close();
}

void Log::Write(std::string str) {
    main_out << str << std::endl << std::flush;
}

bool Log::GetDebug() { return debug; }
void Log::SetDebug(bool val) { this->debug = val; }
void Log::Debug(std::string msg) {
    if(debug) {
        debug_out << msg << std::endl;
    }
}

void Log::Run(std::string msg) {
    run_out << msg << std::endl << std::flush;
}