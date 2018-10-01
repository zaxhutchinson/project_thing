/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Neuron.cpp
 * Author: zax
 *
 * Created on May 28, 2017, 1:28 PM
 */

#include "Neuron.hpp"
#include "Log.hpp"

int Neuron::id_counter = 0;

int Neuron::NextID() {
    return id_counter++;
}

double Neuron::Distance(sptr<Neuron> a, sptr<Neuron> b) {
    return std::sqrt(std::pow(b->X()-a->X(),2.0) +
            std::pow(b->Y()-a->Y(), 2.0) +
            std::pow(b->Z()-a->Z(), 2.0));
}

Neuron::Neuron() {
}

Neuron::Neuron(const Neuron& orig) {
}

Neuron::Neuron(const NT * nt, long seed, unsigned int region_id, int datasize) {
    id = NextID();
    this->region_id = region_id;

    cap = nt->CAP;
    vr = nt->VR;
    vt = nt->VT;
    k = nt->K;
    vpeak = nt->VPEAK;
    a = nt->A;
    b = nt->B;
    c = nt->C;
    d = nt->D;
    noise = nt->NOISE;
    beta = nt->BETA;

    this->data_size = datasize;

    alpha_index=0;
    alpha_index_mod=datasize;
    for(int i = 0; i < alpha_index_mod; i++) {
        alpha_output.push_back(0.0);
    }

    //cur_time = 0;
    this->seed = seed;
    rng = std::mt19937_64( seed );

    std::uniform_real_distribution<double> xDist(0.0,config::CORE_X);
    std::uniform_real_distribution<double> yDist(0.0,config::CORE_Y);
    std::uniform_real_distribution<double> zDist(0.0,config::CORE_Z);

    this->x = xDist(rng);
    this->y = yDist(rng);
    this->z = zDist(rng);

    for(int i = 0; i < datasize; i++) {
        v.push_back(c);
        u.push_back(d);
    }

    spiked = false;

    pre_index = data_size-1;
    cur_index = 0;
    //cur_time = 0;
    v[pre_index] = c;
    u[pre_index] = d;

    dopamine = std::make_shared<Dopamine>();
}

Neuron::Neuron(const NT * nt, long seed, unsigned int region_id,
        int datasize, std::string name) {
    id = NextID();
    this->region_id = region_id;

    cap = nt->CAP;
    vr = nt->VR;
    vt = nt->VT;
    k = nt->K;
    vpeak = nt->VPEAK;
    a = nt->A;
    b = nt->B;
    c = nt->C;
    d = nt->D;
    noise = nt->NOISE;
    beta = nt->BETA;

    this->data_size = datasize;

    this->name = name;

    alpha_index=0;
    alpha_index_mod=datasize;
    for(int i = 0; i < alpha_index_mod; i++) {
        alpha_output.push_back(0.0);
    }

    this->seed = seed;
    rng = std::mt19937_64( seed );

    std::uniform_real_distribution<double> xDist(0.0,config::CORE_X);
    std::uniform_real_distribution<double> yDist(0.0,config::CORE_Y);
    std::uniform_real_distribution<double> zDist(0.0,config::CORE_Z);

    this->x = xDist(rng);
    this->y = yDist(rng);
    this->z = zDist(rng);

    for(int i = 0; i < datasize; i++) {
        v.push_back(c);
        u.push_back(d);
    }

    spiked = false;

    pre_index = data_size-1;
    cur_index = 0;
    //cur_time = 0;
    v[pre_index] = c;
    u[pre_index] = d;

    dopamine = std::make_shared<Dopamine>();
}

Neuron::~Neuron() {
}


void Neuron::SaveRNGToSeed() {
    seed = rng();
}

int Neuron::ID() { return id; }
std::string Neuron::IDAsString() { return std::to_string(id); }
std::string Neuron::Name() { return name; }
void Neuron::Name(std::string name) { this->name = name; }

double Neuron::CAP() { return cap; }
void Neuron::CAP(double val) { cap = val; }
double Neuron::VR() { return vr; }
void Neuron::VR(double val) { vr = val; }
double Neuron::VT() { return vt; }
void Neuron::VT(double val) { vt = val; }
double Neuron::K() { return k; }
void Neuron::K(double val) { k = val; }
double Neuron::VPEAK() { return vpeak; }
void Neuron::VPEAK(double val) { vpeak = val; }
double Neuron::A() { return a; }
void Neuron::A(double val) { a = val; }
double Neuron::B() { return b; }
void Neuron::B(double val) { b = val; }
double Neuron::C() { return c; }
void Neuron::C(double val) { c = val; }
double Neuron::D() { return d; }
void Neuron::D(double val) { d = val; }
double Neuron::NOISE() { return noise; }
void Neuron::NOISE(double val) { noise = val; }
double Neuron::BETA() { return beta; }
void Neuron::BETA(double val) { beta = val; }
int Neuron::RegionID() { return region_id; }
void Neuron::RegionID(int id) { region_id = id; }

double Neuron::X() { return x; }
double Neuron::Y() { return y; }
double Neuron::Z() { return z; }

void Neuron::Update(int64_t time) {

    v[cur_index] = (v[pre_index] +
            (k * (v[pre_index]-vr) * (v[pre_index]-vt) -
            u[pre_index] + beta +
            Input(time) + GetExInput() + GetInternalNoise(time)) /
            cap);

    u[cur_index] = (u[pre_index] + a * (b * (v[pre_index]-vr) - u[pre_index]));

    if(v[cur_index] >= vpeak) {
        v[pre_index] = vpeak;
        v[cur_index] = c;
        u[cur_index] += d;

        spiked = true;
        spikes.AddSpike();
        STDP(time);

    }
    else {
        spiked = false;
    }

    CalculateNextOutput();
    spikes.Update();

    //cur_time++;
    pre_index = cur_index;
    cur_index = (cur_index+1)%data_size;

    BackProp();
}

double Neuron::GetInternalNoise(int64_t time) {
    std::uniform_real_distribution<double> dist(-noise,noise);
    return dist(rng);
}

void Neuron::CalculateNextOutput() {
    double s = spikes.Sum();
    alpha_output[alpha_index]= s / (1 + std::abs(s));
    alpha_index = (alpha_index+1)%alpha_index_mod;
}

double Neuron::Output(int delay) {

    return alpha_output[((alpha_index-delay)+alpha_index_mod)%alpha_index_mod];
}

double Neuron::Input(int64_t time) {
    double in = 0.0;
    for(int i = 0; i < input.size(); i++) {
        in += (input[i]->Output(time));
    }
    return in;
}

double Neuron::GetExInput() {
    double exi = 0.0;
    for(vec_sptr<ExInput>::iterator it = ex_input.begin();
            it != ex_input.end(); it++) {
        exi += (*it)->Output();
    }
    return exi;
}

int Neuron::NumberOfCurrentSpikes() {
    return spikes.NumSpikes();
}

int Neuron::MostRecentSpike() {
    return spikes.MostRecent();
}

double Neuron::TotalSpikeAge() {
    return spikes.TotalAge();
}

bool Neuron::Spiked() { return spiked; }

void Neuron::AddDopamineStrength(DAStrength strength) {
    dopamine->AddStrength(strength);
}
void Neuron::SetDopamineStrength(DAStrength strength) {
    dopamine->SetStrength(strength);
}

void Neuron::BackProp() {
    DAStrength da_str = dopamine->GetStrength();

    if(da_str.high >= config::DOPAMINE_MIN || da_str.low >= config::DOPAMINE_MIN) {

        double total_input_strength = 0.0;
        for(vec_sptr<Connection>::iterator it = input.begin();
                it != input.end(); it++) {
            total_input_strength += (*it)->Strength();
        }

        // Divide da and send to connections
        for(vec_sptr<Connection>::iterator it = input.begin();
                it != input.end(); it++) {
            
            DAStrength das;
            das.high= ((*it)->Strength() / total_input_strength) * da_str.high;
            das.low = ((*it)->Strength() / total_input_strength) * da_str.low;

            (*it)->SetDopamineStrength(das);
        }

        
    }
    // Purge dopamine from neuron.
    dopamine->SetStrength(0.0,0.0);
}

void Neuron::STDP(int64_t time) {
    for(vec_sptr<Connection>::iterator it = input.begin();
            it != input.end(); it++) {
        (*it)->RunPostPlasticity(time);
    }
    for(vec_sptr<Connection>::iterator it = output.begin();
            it != output.end(); it++) {
        (*it)->RunPrePlasticity(time);
    }
}

void Neuron::PurgeExternalInputs() {
    ex_input.clear();
}

void Neuron::AddOutputConnection(sptr<Connection> output_conn) {
    output.push_back(output_conn);
}

void Neuron::AddInputConnection(sptr<Connection> input_conn) {
    input.push_back(input_conn);
}

void Neuron::AddExInputConnection(sptr<ExInput> ex_input_conn) {
    ex_input.push_back(ex_input_conn);
}
