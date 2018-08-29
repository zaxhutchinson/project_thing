/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Connection.hpp
 * Author: zax
 *
 * Created on June 6, 2017, 10:29 AM
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include<cmath>
#include<memory>
#include<vector>
#include<utility>
#include<deque>
#include<queue>
#include<mutex>
#include<functional>

#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/list.hpp"

#include"zaxlib.hpp"
#include"NeuronTemplate.hpp"
#include"Config.hpp"

class Neuron;

enum PlasticityType {
    STANDARD_E,
    //STANDARD_I,
    ZAX_2018_E,
    ZAX_2018_I,
};



class Connection {
public:

    static double E;
    static double POT_MOD;
    static double ZETA_MAX;
    static double ZETA_DECAY;

    Connection();
    Connection(double weight, double strength, bool learn,
        sptr<Neuron> pre, sptr<Neuron> post, PlasticityType p_type,
        int delay);
    Connection(const Connection& orig);
    virtual ~Connection();

    sptr<Neuron> Pre();
    sptr<Neuron> Post();
    void AddPre(sptr<Neuron> pre);
    void AddPost(sptr<Neuron> post);

    void SetPlasticity();

    void PreSTANDARD_E(long time);
    void PostSTANDARD_E(long time);
    void PreZAX_2018_I(long time);
    void PostZAX_2018_I(long time);

    void RunPrePlasticity(long time);
    void RunPostPlasticity(long time);

    double Output(long time);

    void SetLearn(bool learn);

    double Strength();
    double FastPotentiation();
    double MediumPotentiation();
    double SlowPotentiation();
    double TotalPotentiation();

    void AddToPostActivityHistory(int amt);
    double AvgPostActivity();

    template<class Archive>
    void save(Archive & ar) const {
        ar(pre,post,strength,weight,fast_potentiation,
                medium_potentiation, slow_potentiation,
                learn,post_activity_history,
                p_type,delay,last_pre_spike,last_post_spike);
    }
    template<class Archive>
    void load(Archive & ar) {
        ar(pre,post,strength,weight,fast_potentiation,
                medium_potentiation, slow_potentiation,
                learn,post_activity_history,
                p_type,delay,last_pre_spike,last_post_spike);

    }


private:
    wptr<Neuron> pre;
    wptr<Neuron> post;

    long last_post_spike;
    long last_pre_spike;

    PlasticityType p_type;

    std::function<void(long)> pre_plasticity;
    std::function<void(long)> post_plasticity;

    double min_strength;
    double strength;
    double weight;
    double fast_potentiation;
    double medium_potentiation;
    double slow_potentiation;
    int64_t last_potentiation_time;

    int delay;

    std::list<double> post_activity_history;

    bool learn;
};

#endif /* CONNECTION_HPP */
