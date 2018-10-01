#include"ThingManager.hpp"

uptr<ThingManager> ThingManager::instance = nullptr;
ThingManager * ThingManager::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<ThingManager>();
    }
    return instance.get();
}

ThingManager::ThingManager() {

}

void ThingManager::LoadThings(std::string filename) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(config::THING_FILE.c_str());

    tinyxml2::XMLElement * root = doc.RootElement();
    tinyxml2::XMLElement * ething = root->FirstChildElement("thing");

    while(ething!=nullptr) {

        tinyxml2::XMLElement * ename = ething->FirstChildElement("name");
        
        sptr<Thing> t = std::make_shared<Thing>(ename->GetText(),0);

        tinyxml2::XMLElement * edetail = ething->FirstChildElement("detail");

        while(edetail != nullptr) {
            try {
                double d = std::stod(edetail->GetText());
                t->CreateDetail(d);
            } catch(std::invalid_argument e) {
                Log::Instance()->Write("THINGMANAGER: Invalid detail " + std::string(edetail->GetText()));
            }
            edetail = edetail->NextSiblingElement("detail");
        }

        things.push_back(t);

        ething = ething->NextSiblingElement("thing");
    }
}

sptr<Thing> ThingManager::GetThing(std::string name) {
    for(vec_sptr<Thing>::iterator it = things.begin();
            it != things.end(); it++) {
        if((*it)->GetName()==name) {
            return *it;
        }
    }
    return nullptr;
}