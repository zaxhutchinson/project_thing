/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Neuron.hpp
 * Author: zax
 *
 * Created on May 28, 2017, 1:28 PM
 */

#ifndef NEURON_HPP
#define NEURON_HPP

#include<deque>
#include<vector>
#include<cmath>
#include<random>
#include<memory>
#include<limits>
#include<iterator>
#include<mutex>
#include<omp.h>
#include<string>
#include<cstdint>

#include"cereal/archives/binary.hpp"
#include"cereal/types/vector.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/string.hpp"

#include"NeuronTemplate.hpp"
#include"ExInput.hpp"
#include"Connection.hpp"
#include"IntVec3.hpp"
#include"AlphaBuffer.hpp"
#include"Log.hpp"

class Neuron {
public:

    static int id_counter;
    static int NextID();
    static double Distance(sptr<Neuron> a, sptr<Neuron> b);

    Neuron();
    Neuron(const Neuron& orig);
    Neuron(const NT * nt, long seed, unsigned int region_id, int datasize);
    Neuron(const NT * nt, long seed, unsigned int region_id,
            int datasize, std::string name);
    virtual ~Neuron();

    int ID();
    std::string IDAsString();

    std::string Name();
    void Name(std::string name);

    double CAP();
    void CAP(double val);
    double VR();
    void VR(double val);
    double VT();
    void VT(double val);
    double K();
    void K(double val);
    double VPEAK();
    void VPEAK(double val);
    double A();
    void A(double val);
    double B();
    void B(double val);
    double C();
    void C(double val);
    double D();
    void D(double val);
    double NOISE();
    void NOISE(double val);
    double BETA();
    void BETA(double val);
    int RegionID();
    void RegionID(int id);

    void Update(int64_t time);
    double GetInternalNoise(int64_t time);
    void CalculateNextOutput();
    double Output(int delay);
    double Input(int64_t time);
    double GetExInput();
    int NumberOfCurrentSpikes();
    int MostRecentSpike();
    double TotalSpikeAge();
    bool Spiked();

    double X();
    double Y();
    double Z();

    void STDP(int64_t time);

    void AddOutputConnection(sptr<Connection> output_conn);
    void AddInputConnection(sptr<Connection> input_conn);
    void AddExInputConnection(sptr<ExInput> ex_input_conn);

    void SaveRNGToSeed();

    void PurgeExternalInputs();

    template<class Archive>
    void save(Archive & ar) const {
        ar(id,name,cap,vr,vt,k,vpeak,a,b,c,d,noise,beta,
                alpha_output,alpha_index,alpha_index_mod,
                cur_index,pre_index,
                data_size,v,u,spikes,input,output,ex_input,
                seed,spiked,region_id);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(id,name,cap,vr,vt,k,vpeak,a,b,c,d,noise,beta,
                alpha_output,alpha_index,alpha_index_mod,
                cur_index,pre_index,
                data_size,v,u,spikes,input,output,ex_input,
                seed,spiked,region_id);
        rng = std::mt19937_64(seed);
    }

private:
    int id;

    std::string name;

    double cap;
    double vr;
    double vt;
    double k;
    double vpeak;
    double a;
    double b;
    double c;
    double d;
    double noise;
    double beta;

    int alpha_index;
    int alpha_index_mod;
    std::vector<double> alpha_output;

    //int cur_time;
    int cur_index;
    int pre_index;
    int data_size;
    vec<double> v;
    vec<double> u;
    AlphaBuffer spikes;
    bool spiked;

    double x;
    double y;
    double z;

    vec_sptr<Connection> input;
    vec_sptr<Connection> output;
    vec_sptr<ExInput> ex_input;

    std::mt19937_64 rng;
    long seed;

    int region_id;
};

#endif /* NEURON_HPP */
