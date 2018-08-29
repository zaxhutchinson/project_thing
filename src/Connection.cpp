/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Connection.cpp
 * Author: zax
 *
 * Created on June 6, 2017, 10:29 AM
 */

#include <complex>

#include "Connection.hpp"
#include "Neuron.hpp"
#include "Config.hpp"

double Connection::E = std::exp(1.0);

Connection::Connection() {

}

Connection::Connection(double weight, double strength, bool learn,
        sptr<Neuron> pre, sptr<Neuron> post, PlasticityType p_type,
        int delay) {
    this->weight = weight;
    this->strength = strength;
    this->min_strength = -strength;
    this->fast_potentiation = 0.0;
    this->medium_potentiation = 0.0;
    this->slow_potentiation = 0.0;
    this->learn = learn;
    this->pre = pre;
    this->post = post;
    this->p_type = p_type;
    this->last_pre_spike = -1;
    this->last_post_spike = -1;
    this->last_potentiation_time = -1;
    this->delay = delay;

    SetPlasticity();
}

Connection::Connection(const Connection& orig) {

}

Connection::~Connection() {
}

sptr<Neuron> Connection::Pre() { return pre.lock(); }
sptr<Neuron> Connection::Post() { return post.lock(); }
void Connection::AddPre(sptr<Neuron> pre) { this->pre = pre; }
void Connection::AddPost(sptr<Neuron> post) { this->post = post; }

void Connection::SetPlasticity() {
    switch(p_type) {
        case PlasticityType::STANDARD_E:
            pre_plasticity = std::bind(&Connection::PreSTANDARD_E,this,std::placeholders::_1);
            post_plasticity = std::bind(&Connection::PostSTANDARD_E,this,std::placeholders::_1);
            break;
        case PlasticityType::ZAX_2018_I:
            pre_plasticity = std::bind(&Connection::PreZAX_2018_I,this,std::placeholders::_1);
            post_plasticity = std::bind(&Connection::PostZAX_2018_I,this,std::placeholders::_1);
            break;
    }
}

void Connection::PreSTANDARD_E(long time) {
    last_pre_spike = time;
    if(learn && last_post_spike>-1) {
        double diff = static_cast<double>(last_pre_spike-last_post_spike);
        double delta = 0.0;

        if(diff<30.0) {
            delta = -0.1 * std::exp(-diff/30.0);
            if(last_potentiation_time>0) {
                fast_potentiation = std::exp(-config::FAST_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                medium_potentiation = std::exp(-config::MEDIUM_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                slow_potentiation = std::exp(-config::SLOW_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
            }
        }
        strength+=delta;
        if(strength<min_strength) strength=min_strength;
    }
}

void Connection::PostSTANDARD_E(long time) {
    last_post_spike = time;
    if(learn && last_pre_spike>-1) {
        double diff = static_cast<double>(last_post_spike-last_pre_spike);
        double delta = 0.0;
        if(diff<30.0) {
            delta = 0.1 * std::exp(-diff/30.0);
            last_potentiation_time = time;
            fast_potentiation=medium_potentiation=slow_potentiation=1.0;
        }
        else {
            if(last_potentiation_time>0) {
                fast_potentiation = std::exp(-config::FAST_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                medium_potentiation = std::exp(-config::MEDIUM_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                slow_potentiation = std::exp(-config::SLOW_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
            }
        }
        strength+=delta;
        if(strength<min_strength) strength=min_strength;
    }
}


void Connection::PreZAX_2018_I(long time) {
    last_pre_spike = time;
    if(learn && last_post_spike>-1) {
        double diff = static_cast<double>(last_pre_spike-last_post_spike);
        double delta = 0.0;
        if(diff<20.0) {
            delta = 0.1 * std::exp(-diff/20.0);
        }
        else if(diff<100.0) {
            delta = -0.025 * std::exp(-diff/100.0);
            if(last_potentiation_time>0) {
                fast_potentiation = std::exp(-config::FAST_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                medium_potentiation = std::exp(-config::MEDIUM_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                slow_potentiation = std::exp(-config::SLOW_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
            }
        }
        strength+=delta;
        if(strength<min_strength) strength=min_strength;
    }
}

void Connection::PostZAX_2018_I(long time) {
    last_post_spike=time;
    if(learn && last_pre_spike>-1) {
        double diff = static_cast<double>(last_post_spike-last_pre_spike);
        double delta=0.0;
        if(diff<20.0) {
            delta = 0.1 * std::exp(-diff/20.0);
            last_potentiation_time = time;
            fast_potentiation=medium_potentiation=slow_potentiation=1.0;
        }
        else if(diff<100.0) {
            delta = -0.025 * std::exp(-diff/100.0);
            if(last_potentiation_time>0) {
                fast_potentiation = std::exp(-config::FAST_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                medium_potentiation = std::exp(-config::MEDIUM_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
                slow_potentiation = std::exp(-config::SLOW_POTENTIATION_DECAY*
                        (time-last_potentiation_time));
            }
        }
        strength+=delta;
        if(strength<min_strength) strength=min_strength;
    }
}

void Connection::RunPrePlasticity(long time) {
    pre_plasticity(time);
}
void Connection::RunPostPlasticity(long time) {
    post_plasticity(time);
}


double Connection::Output(long time) {

    double preout = pre.lock()->Output(delay);

    if(strength<0.0) {
        return 0.0;
    }
    else {
        return (1.0+
                config::FAST_POTENTIATION_MOD*fast_potentiation +
                config::MEDIUM_POTENTIATION_MOD*medium_potentiation +
                config::SLOW_POTENTIATION_MOD*slow_potentiation) *
                preout *
                ((weight*strength) / (std::abs(weight)+std::abs(strength)));
    }
}

void Connection::SetLearn(bool learn) {
    this->learn = learn;
}

double Connection::Strength() { return strength; }
double Connection::FastPotentiation() { return fast_potentiation; }
double Connection::MediumPotentiation() { return medium_potentiation; }
double Connection::SlowPotentiation() { return slow_potentiation; }
double Connection::TotalPotentiation() {
    return FastPotentiation()+MediumPotentiation()+SlowPotentiation();
}

void Connection::AddToPostActivityHistory(int amt) {
    while(post_activity_history.size() > 9) {
        post_activity_history.pop_front();
    }
    post_activity_history.push_back(static_cast<double>(amt));
}

double Connection::AvgPostActivity() {
    if(post_activity_history.size()==0) {
        return 0.001;
    }
    else {
        double total = 0.0;
        for(std::list<double>::iterator it = post_activity_history.begin();
                it != post_activity_history.end(); it++) {
            total += *it;
        }
        return total / post_activity_history.size();

    }
}
