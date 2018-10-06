#include"Agent.hpp"


Agent::Agent() {

}
void Agent::Reset() {
    mind->Reset();
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Agent::Build() {
    prepare_response=false;
    response_end_time=0;
    response=nullptr;
}
void Agent::AddMind(sptr<Mind> mind) {
    this->mind = mind;
}
void Agent::AddEye(sptr<ExInput> ex_input) {
    eyes.push_back(ex_input);
}
void Agent::AddEar(sptr<ExInput> ex_input) {
    ears.push_back(ex_input);
}
void Agent::AddMouth(sptr<Electrode> electrode) {
    mouths.push_back(electrode);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
int Agent::GetX() { return x; }
int Agent::GetY() { return y; }
void Agent::SetX(int x) { this->x = x; }
void Agent::SetY(int y) { this->y = y; }
sptr<Mind> Agent::GetMind() { return mind; }
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Agent::StartLearning() {
    mind->StartLearning();
}
void Agent::EndLearning() {
    mind->EndLearning();
}
void Agent::GiveFeedback(sptr<Thing> feedback) {
    
    

    for(int i = 0; i < mouths.size() && i < feedback->NumDetails(); i++) {
        DAStrength das;
        if(feedback->GetDetail(i)>0) das.high=feedback->GetDetail(i);
        else das.low = (-feedback->GetDetail(i));
        mouths[i]->GiveFeedback(das);
    }

    mind->RunFeedback();
}
// void Agent::EndFeedback(int input_channel) {
//     mind->PurgeDopamine(input_channel);
// }
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Agent::GiveVisualInput(sptr<Thing> thing) {
    for(int i = 0; i < eyes.size(); i++) {
        eyes[i]->Strength(thing->GetDetail(i));
    }
}
void Agent::GiveAuralInput(sptr<Thing> thing) {
    for(int i = 0; i < ears.size(); i++) {
        ears[i]->Strength(thing->GetDetail(i));
    }
}
void Agent::PurgeVisualInput() {
    for(int i = 0; i < eyes.size(); i++) {
        eyes[i]->Strength(0.0);
    }
}
void Agent::PurgeAuralInput() {
    for(int i = 0; i < ears.size(); i++) {
        ears[i]->Strength(0.0);
    }
}
void Agent::PurgeAllInput() {
    PurgeVisualInput();
    PurgeAuralInput();
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Agent::PrepareResponse(int64_t time, int64_t time_to_think) {
    prepare_response=true;
    response = std::make_shared<Thing>("response",mouths.size());
    response_end_time = time+time_to_think;
}
void Agent::ConsiderResponse() {
    if(response!=nullptr) {
        for(int i = 0; i < mouths.size(); i++) {
            if(mouths[i]->Spike()) {
                response->AddDetail(i,1.0);
            }
        }
    }
}
sptr<Thing> Agent::GetRawResponse() {
    return response;
}
sptr<Thing> Agent::GetNormalizedResponse() {
    return response->NormalizeDetail();
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Agent::Update(int64_t time, std::mt19937_64 & rng) {

    mind->Update(time, rng);

    if(prepare_response) {
        if(time < response_end_time) {
            ConsiderResponse();
        } else {
            prepare_response=false;
            response_end_time=0;
        }
    }
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////