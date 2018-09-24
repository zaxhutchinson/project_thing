#include"TestManager.hpp"


TestDef::TestDef(int id, std::string qname,
                std::string aname,
                int64_t sstart, int64_t sduration,
                int64_t rstart, int64_t rduration,
                int64_t fstart, int64_t fduration,
                int64_t etime) {

    this->id = id;
    this->qname = qname;
    this->aname = aname;
    this->sstart = sstart;
    this->sduration = sduration;
    this->rstart = rstart;
    this->rduration = rduration;
    this->fstart = fstart;
    this->fduration = fduration;
    this->etime = etime;
}

Test::Test(const TestDef & tp) {
    question = ThingManager::Instance()->GetThing(tp.qname);
    answer = ThingManager::Instance()->GetThing(tp.aname);
    this->id = tp.id;
    sstart = tp.sstart;
    sduration = tp.sduration;
    rstart = tp.rstart;
    rduration = tp.rduration;
    fstart = tp.fstart;
    fduration = tp.fduration;
    etime = tp.etime;
}

SessionDef::SessionDef(int id, std::string sname) {
    this->id = id;
    this->sname = sname;
}

Session::Session(const SessionDef & sd) {
    this->name = sd.sname;
    for(vec<int>::const_iterator it = sd.test_def_ids.begin();
            it != sd.test_def_ids.end(); it++) {
        sptr<Test> test = std::make_shared<Test>(*(TestManager::Instance()->GetTestDef(*it)));
        tests.push_back(test);
    }
}
bool Session::SessionDone() {
    if(tests.empty()) return true;
    else return false;
}

bool Session::StartStimulus(int64_t time) {
    if(time == tests.front()->sstart) {
        return true;
    } return false;
}
bool Session::EndStimulus(int64_t time) {
    if(time == tests.front()->sstart+tests.front()->sduration) {
        return true;
    } else return false;
}
bool Session::StartResponse(int64_t time) {
    if(time == tests.front()->rstart) {
        return true;
    } else return false;
}
bool Session::EndResponse(int64_t time) {
    if(time == tests.front()->rstart+tests.front()->rduration) {
        return true;
    } else return false;
}
int64_t Session::ResponseDuration() {
    return tests.front()->rduration;
}
bool Session::GiveFeedback(int64_t time) {
    if(time >= tests.front()->fstart && time < tests.front()->fstart+tests.front()->fduration) {
        return true;
    } else return false;
}
// bool Session::EndFeedback(int64_t time) {
//     if(time == tests.front()->fstart+tests.front()->fduration) {
//         return true;
//     } else return false;
// }
bool Session::EndTest(int64_t time) {
    if(time == tests.front()->etime) {
        return true;
    } else return false;
}
sptr<Thing> Session::GetCurrentStimulus() {
    return tests.front()->question;
}
sptr<Thing> Session::GetCurrentFeedback() {
    return tests.front()->answer;
}
void Session::RemoveCurrentTest() {
    tests.pop_front();
}
sptr<Test> Session::GetNextTest() {
    if(!tests.empty()) {
        return tests.front();
    } else return nullptr;
}
uptr<TestManager> TestManager::instance = nullptr;
TestManager * TestManager::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<TestManager>();
    }
    return instance.get();
}
TestManager::TestManager() {
    
}

void TestManager::LoadFiles() {
    LoadTestDefs();
    LoadSessionDefs();
}

void TestManager::LoadTestDefs() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(config::TEST_FILE.c_str());
    tinyxml2::XMLElement * root = doc.RootElement();
    tinyxml2::XMLElement * testdef = root->FirstChildElement("testdef");

    while(testdef != nullptr) {

        tinyxml2::XMLElement * eid = testdef->FirstChildElement("id");
        tinyxml2::XMLElement * eqname = testdef->FirstChildElement("qname");
        tinyxml2::XMLElement * eaname = testdef->FirstChildElement("aname");
        tinyxml2::XMLElement * esstart = testdef->FirstChildElement("sstart");
        tinyxml2::XMLElement * esduration = testdef->FirstChildElement("sduration");
        tinyxml2::XMLElement * erstart = testdef->FirstChildElement("rstart");
        tinyxml2::XMLElement * erduration = testdef->FirstChildElement("rduration");
        tinyxml2::XMLElement * efstart = testdef->FirstChildElement("fstart");
        tinyxml2::XMLElement * efduration = testdef->FirstChildElement("fduration");
        tinyxml2::XMLElement * eetime = testdef->FirstChildElement("etime");

        int id = std::stoi(eid->GetText());
        std::string qname = eqname->GetText();
        std::string aname = eaname->GetText();
        int64_t sstart = std::stol(esstart->GetText());
        int64_t sduration = std::stol(esduration->GetText());
        int64_t rstart = std::stol(erstart->GetText());
        int64_t rduration = std::stol(erduration->GetText());
        int64_t fstart = std::stol(efstart->GetText());
        int64_t fduration = std::stol(efduration->GetText());
        int64_t etime = std::stol(eetime->GetText());

        sptr<TestDef> td = std::make_shared<TestDef>(
            id,qname,aname,sstart,
            sduration,rstart,rduration,
            fstart,fduration,etime
        );

        test_defs[id]=td;
        
        testdef = testdef->NextSiblingElement("testdef");
    }
}

void TestManager::LoadSessionDefs() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(config::SESSION_FILE.c_str());
    tinyxml2::XMLElement * root = doc.RootElement();
    tinyxml2::XMLElement * sdef = root->FirstChildElement("sessiondef");

    while(sdef != nullptr) {

        tinyxml2::XMLElement * eid = sdef->FirstChildElement("id");
        tinyxml2::XMLElement * esname = sdef->FirstChildElement("sname");

        int id = std::stoi(eid->GetText());
        std::string sname = esname->GetText();
        sptr<SessionDef> sd = std::make_shared<SessionDef>(id,sname);

        for(tinyxml2::XMLElement * etd = sdef->FirstChildElement("testdefid");
                etd != nullptr;
                etd = etd->NextSiblingElement("testdefid")) {

            int testdefid = std::stoi(etd->GetText());
            int iters=0;
            if(etd->QueryIntAttribute("iterations",&iters)==tinyxml2::XML_SUCCESS) {
                for(int i=0; i < iters; i++) {
                    sd->test_def_ids.push_back(testdefid);
                }
            }

        }

        session_defs[sd->id]=sd;

        sdef = sdef->NextSiblingElement("sessiondef");
    }
}

sptr<Session> TestManager::CreateSession(int id) {
    return std::make_shared<Session>(
        *(GetSessionDef(id))
    );
}

sptr<TestDef> TestManager::GetTestDef(int id) {
    try {
        return test_defs.at(id);
    } catch(std::out_of_range e) {
        return nullptr;
    }
}
sptr<SessionDef> TestManager::GetSessionDef(int id) {
    try {
        return session_defs.at(id);
    } catch(std::out_of_range e) {
        return nullptr;
    }
}