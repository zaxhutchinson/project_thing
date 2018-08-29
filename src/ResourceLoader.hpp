/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ResourceLoader.hpp
 * Author: zax
 *
 * Created on January 1, 2018, 3:13 PM
 */

#ifndef RESOURCELOADER_HPP
#define RESOURCELOADER_HPP

#include<memory>

#include"tinyxml2.h"

#include"zaxlib.hpp"

class ResourceLoader {
public:
    static ResourceLoader * Instance();
    ResourceLoader();
    ResourceLoader(const ResourceLoader& orig);
    virtual ~ResourceLoader();
private:
    static uptr<ResourceLoader> instance;
};

#endif /* RESOURCELOADER_HPP */
