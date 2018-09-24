#ifndef AGENT_HPP
#define AGENT_HPP

#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/vector.hpp"

#include"zaxlib.hpp"
#include"Mind.hpp"
#include"ExInput.hpp"
#include"Thing.hpp"
#include"Electrode.hpp"

class Agent {
public:
    Agent();

    void Reset();
    /////////////////////////////////////////////////////////////////////////
    void Build();
    void AddMind(sptr<Mind> mind);
    void AddEye(sptr<ExInput> ex_input);
    void AddEar(sptr<ExInput> ex_input);
    void AddMouth(sptr<Electrode> electrode);
    /////////////////////////////////////////////////////////////////////////
    int GetX();
    int GetY();
    void SetX(int x);
    void SetY(int y);
    sptr<Mind> GetMind();
    /////////////////////////////////////////////////////////////////////////
    void StartLearning();
    void EndLearning();
    void GiveFeedback(sptr<Thing> feedback);
    //void EndFeedback(int input_channel);
    /////////////////////////////////////////////////////////////////////////
    void GiveVisualInput(sptr<Thing> thing);
    void GiveAuralInput(sptr<Thing> thing);
    void PurgeVisualInput();
    void PurgeAuralInput();
    void PurgeAllInput();
    /////////////////////////////////////////////////////////////////////////
    void PrepareResponse(int64_t time, int64_t time_to_think);
    void ConsiderResponse();
    sptr<Thing> GetRawResponse();
    sptr<Thing> GetNormalizedResponse();
    /////////////////////////////////////////////////////////////////////////
    void Update(int64_t time, std::mt19937_64 & rng);
    /////////////////////////////////////////////////////////////////////////
    template<class Archive>
    void save(Archive & ar) const {
        ar(x,y,mind,eyes,ears);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(x,y,mind,eyes,ears);

    }
private:
    int x;
    int y;
    sptr<Mind> mind;
    vec_sptr<ExInput> eyes;
    vec_sptr<ExInput> ears;
    vec_sptr<Electrode> mouths;

    int64_t response_end_time;
    bool prepare_response;
    sptr<Thing> response;
};

#endif
