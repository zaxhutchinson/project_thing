/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ResourceLoader.cpp
 * Author: zax
 *
 * Created on January 1, 2018, 3:13 PM
 */

#include "ResourceLoader.hpp"
#include "Log.hpp"

uptr<ResourceLoader> ResourceLoader::instance = nullptr;

ResourceLoader * ResourceLoader::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<ResourceLoader>();
    }
    return instance.get();
}

ResourceLoader::ResourceLoader() {
}

ResourceLoader::ResourceLoader(const ResourceLoader& orig) {
}

ResourceLoader::~ResourceLoader() {
}
