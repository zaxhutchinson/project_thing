#ifndef TESTMANAGER_HPP
#define TESTMANAGER_HPP

#include<queue>
#include<string>
#include<map>
#include<list>

#include"tinyxml2.h"

#include"zaxlib.hpp"
#include"Thing.hpp"
#include"ThingManager.hpp"
#include"Config.hpp"

struct TestDef {
    
    TestDef(int id, std::string qname,
                    std::string aname,
                    int64_t sstart,
                    int64_t sduration,
                    int64_t rstart,
                    int64_t rduration,
                    int64_t fstart,
                    int64_t fduration,
                    int64_t etime);
    int id;
    std::string qname;      // Question (thing) name
    std::string aname;     // Answer (thing) name
    int64_t sstart;         // Stimulus start
    int64_t sduration;      // Stimulus duration
    int64_t rstart;         // Response start
    int64_t rduration;      // Response duration
    int64_t fstart;         // Feedback time
    int64_t fduration;      // Feedback duration
    int64_t etime;          // End time
};

struct Test {
    Test(const TestDef & tp);
    sptr<Thing> question;
    sptr<Thing> answer;
    int id;
    int64_t sstart;         // Stimulus start
    int64_t sduration;      // Stimulus duration
    int64_t rstart;         // Response start
    int64_t rduration;      // Response duration
    int64_t fstart;         // Feedback start
    int64_t fduration;      // Feedback duration
    int64_t etime;          // End time
};

struct SessionDef {
    SessionDef(int id, std::string sname);
    int id;
    std::string sname;                          // test name
    vec<int> test_def_ids;
};

struct Session {
    Session(const SessionDef & sd);
    bool SessionDone();
    bool StartStimulus(int64_t time);
    bool EndStimulus(int64_t time);
    bool StartResponse(int64_t time);
    bool EndResponse(int64_t time);
    int64_t ResponseDuration();
    bool StartFeedback(int64_t time);
    bool EndFeedback(int64_t time);
    bool EndTest(int64_t time);
    sptr<Thing> GetCurrentStimulus();
    sptr<Thing> GetCurrentFeedback();
    void RemoveCurrentTest();
    sptr<Test> GetNextTest();

    std::string name;
    list_sptr<Test> tests;
};

class TestManager {
public:
    static TestManager * Instance();
    TestManager();
    void LoadFiles();
    void LoadTestDefs();
    void LoadSessionDefs();
    sptr<Session> CreateSession(int id);
    sptr<TestDef> GetTestDef(int id);
    sptr<SessionDef> GetSessionDef(int id);
private:
    static uptr<TestManager> instance;
    std::map<int,sptr<TestDef>> test_defs;
    std::map<int,sptr<SessionDef>> session_defs;
};

#endif