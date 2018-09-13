#ifndef THINGMANAGER_HPP
#define THINGMANAGER_HPP

#include<memory>
#include<map>
#include<fstream>
#include<string>
#include<sstream>

#include"tinyxml2.h"

#include"zaxlib.hpp"
#include"Thing.hpp"
#include"Log.hpp"
#include"Config.hpp"

class ThingManager {
public:
    static ThingManager * Instance();
    ThingManager();
    void LoadThings(std::string filename);
    sptr<Thing> GetThing(std::string name);
private:
    static uptr<ThingManager> instance;
    vec_sptr<Thing> things;
};

#endif