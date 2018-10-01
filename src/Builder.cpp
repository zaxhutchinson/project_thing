/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | NeuronTemplate
 * and open the template in the editor.
 */

/*
 * File:   Builder.cpp
 * Author: zax
 *
 * Created on December 25, 2017, 8:50 AM
 */

#include "Builder.hpp"
#include "Electrode.hpp"

uptr<Builder> Builder::instance = nullptr;
Builder * Builder::Instance() {
    if(instance==nullptr) {
        instance = std::make_unique<Builder>();
    }
    return instance.get();
}

Builder::Builder() {
}

Builder::Builder(const Builder& orig) {
}

Builder::~Builder() {
}

sptr<Mind> Builder::BuildMind000(long seed) {
    Log::Instance()->Write("BUILDER: Build Mind 000");

    // Mind obj
    sptr<Mind> mind = std::make_shared<Mind>();

    // Set up rng and dice.
    rng.seed(seed);
    std::uniform_real_distribution<double> dice(0.0,1.0);
    std::uniform_int_distribution<int> delay(1,5);
    int datasize = config::DATASIZE;
    //////////////////////////////////////////////////////////////
    // Configuration
    int size_input              = 4;
    int size_output             = 4;
    int neurons_per_input       = 10;
    //int neurons_per_output      = 10;

    double input_w              = 200.0;

    int size_pfc_pc             = 200;
    int size_pfc_in             = 35;
    int size_pfc_ii             = 5;

    double pfc_pc_pc_cp         = 0.2;
    double pfc_pc_pc_w          = 50.0;
    double pfc_pc_in_cp         = 0.5;
    double pfc_pc_in_w          = 20.0;
    double pfc_in_pc_cp         = 0.5;
    double pfc_in_pc_w          = -20.0;
    double pfc_pc_ii_cp         = 0.5;
    double pfc_pc_ii_w          = 20.0;
    double pfc_ii_in_cp         = 0.5;
    double pfc_ii_in_w          = -20.0;

    double max_conn_dist        = 50.0;

    vec_sptr<Neuron> pfc_pc;
    vec_sptr<Neuron> pfc_in;
    vec_sptr<Neuron> pfc_ii;

    PlasticityType e_type = PlasticityType::STANDARD_E;
    PlasticityType i_type = PlasticityType::ZAX_2018_I;

    //////////////////////////////////////////////////////////////
    // Neurons
    for(int i = 0; i < size_pfc_pc; i++) {
        pfc_pc.push_back(std::make_shared<Neuron>(
            NeuronTemplate::Instance()->GetNT(NeuronType::RegularSpiking),
            rng(), 0, datasize,
            "PFC_PC("+std::to_string(i)+")"
        ));
    }
    for(int i = 0; i < size_pfc_in; i++) {
        pfc_in.push_back(std::make_shared<Neuron>(
            NeuronTemplate::Instance()->GetNT(NeuronType::FastSpiking),
            rng(), 1, datasize,
            "PFC_IN("+std::to_string(i)+")"
        ));
    }
    for(int i = 0; i < size_pfc_ii; i++) {
        pfc_ii.push_back(std::make_shared<Neuron>(
            NeuronTemplate::Instance()->GetNT(NeuronType::FastSpiking),
            rng(), 2, datasize,
            "PFC_II("+std::to_string(i)+")"
        ));
    }
    //////////////////////////////////////////////////////////////
    // Connections

    sptr<Dopamine> da = std::make_shared<Dopamine>();
    mind->AddDopamineChannel(da);

    for(int i = 0; i < size_pfc_pc; i++) {
        for(int k = 0; k < size_pfc_pc; k++) {
            if(i==k) continue; // no self connections
            sptr<Neuron> pre = pfc_pc[i];
            sptr<Neuron> post = pfc_pc[k];
            if(Neuron::Distance(pre,post)<max_conn_dist) {
                if(pfc_pc_pc_cp<dice(rng)) {
                    
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_pc_w, std::abs(pfc_pc_pc_w), -std::abs(pfc_pc_pc_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                } else {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_pc_w, 0.0, -std::abs(pfc_pc_pc_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }
    for(int i = 0; i < size_pfc_pc; i++) {
        for(int k = 0; k < size_pfc_in; k++) {
            sptr<Neuron> pre = pfc_pc[i];
            sptr<Neuron> post = pfc_in[k];
            if(Neuron::Distance(pre,post)<max_conn_dist) {
                if(pfc_pc_in_cp<dice(rng)) {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_in_w, std::abs(pfc_pc_in_w), -std::abs(pfc_pc_in_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                } else {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_in_w, 0.0, -std::abs(pfc_pc_in_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }
    for(int i = 0; i < size_pfc_in; i++) {
        for(int k = 0; k < size_pfc_pc; k++) {
            sptr<Neuron> pre = pfc_in[i];
            sptr<Neuron> post = pfc_pc[k];
            if(Neuron::Distance(pre,post)<max_conn_dist) {
                if(pfc_in_pc_cp<dice(rng)) {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_in_pc_w, std::abs(pfc_in_pc_w), -std::abs(pfc_in_pc_w),
                        true, false,
                        pre, post, i_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                } else {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_in_pc_w, 0.0, -std::abs(pfc_in_pc_w),
                        true, false,
                        pre, post, i_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }
    for(int i = 0; i < size_pfc_pc; i++) {
        for(int k = 0; k < size_pfc_ii; k++) {
            sptr<Neuron> pre = pfc_pc[i];
            sptr<Neuron> post = pfc_ii[k];
            if(Neuron::Distance(pre,post)<max_conn_dist) {
                if(pfc_pc_ii_cp<dice(rng)) {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_ii_w, std::abs(pfc_pc_ii_w), -std::abs(pfc_pc_ii_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                } else {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_pc_ii_w, 0.0, -std::abs(pfc_pc_ii_w),
                        true, false,
                        pre, post, e_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }
    for(int i = 0; i < size_pfc_ii; i++) {
        for(int k = 0; k < size_pfc_in; k++) {
            sptr<Neuron> pre = pfc_ii[i];
            sptr<Neuron> post = pfc_in[k];
            if(Neuron::Distance(pre,post)<max_conn_dist) {
                if(pfc_ii_in_cp<dice(rng)) {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_ii_in_w, std::abs(pfc_ii_in_w), -std::abs(pfc_ii_in_w),
                        true, false,
                        pre, post, i_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                } else {
                    sptr<Connection> conn = std::make_shared<Connection>(
                        pfc_ii_in_w, 0.0, -std::abs(pfc_ii_in_w),
                        true, false,
                        pre, post, i_type, Neuron::Distance(pre,post)/10.0
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////
    // External Input
    int k = 0;
    for(int i = 0; i < size_input; i++) {
        sptr<ExInput> exin = std::make_shared<ExInput>(input_w);
        for(int j = 0; j < neurons_per_input; j++, k++) {
            sptr<Neuron> neuron = pfc_pc[k];
            neuron->AddExInputConnection(exin);
        }
        mind->AddExInput(exin);
    }

    // IF more EGs are added, make sure that the Mouth intended output
    // is the first one added to the mind. These electrodes will be
    // referenced as parts of the mouth.
    sptr<ElectrodeGroup> egout = std::make_shared<ElectrodeGroup>("OUTPUT");
    for(int i = 0; i < size_output; i++, k++) {
        sptr<Neuron> neuron = pfc_pc[k];
        sptr<Electrode> electrode = std::make_shared<Electrode>("OUT(" + std::to_string(i)+")");
        electrode->SetElectrode(neuron);
        egout->electrodes.push_back(electrode);
    }
    mind->AddElectrodeGroup(egout);

    mind->AddRegion(pfc_pc);
    mind->AddRegion(pfc_in);
    mind->AddRegion(pfc_ii);

    //config::PrintConfig();
    //////////////////////////////////////////////////////////////
    // END return mind
    return mind;
}
/*
sptr<Mind> Builder::Build011(long seed) {
    Log::Instance()->Write("BUILDER: Started - build 011");

    sptr<Mind> mind = std::make_shared<Mind>();

    rng.seed(seed);
    std::uniform_real_distribution<double> dice(0.0,1.0);

    // Min delay must be > 0. Else some plasticity calculations could divide by 0.
    std::uniform_int_distribution<int> exDelayDist(1,config::MAX_DELAY);
    std::uniform_int_distribution<int> inDelayDist(1,config::MAX_DELAY/2);

    int datasize = config::DATASIZE;

    int size_input = 16;
    int in_per_input = 8;
    int pfc_per_input = 128;
    int pfciF_per_input = 12;
    int pfciS_per_input = 12;
    int thal_per_input = 8;
    int cd_per_input = 8;
    int gpi_per_input = 8;

    double exin_in1_cp=         0.5;
    double exin_in2_cp=         0.0;
    double exin_in_w=           200.0;

    double in_in_w=             10.0;
    double in_in_cp=            0.0;
    double in_pfc_w=            200.0;
    double in_pfc_cp=           0.2;
    double pfc_in_w=            20.0;
    double pfc_in_cp=           0.2;
    double pfc_pfc_w=           18.0;
    double pfc_pfc_SAME_cp=     0.7;//0.7;
    double pfc_pfc_DIFF_cp=     0.1;
    double pfc_pfc_long_dist=   0.1; // scalar of cp for long distance conns

    // Universal Inhibition
    double pfc_pfciF_w=         45.0;
    double pfc_pfciF_cp=        0.6;
    double pfciF_pfc_w=         -80.0;
    double pfciF_pfc_cp=        0.6;
    // Lateral Inhibition
    double pfc_pfciS_w=         45.0;
    double pfc_pfciS_cp=        0.4;
    double pfciS_pfc_w=         -45.0;
    double pfciS_pfc_cp=        0.4;

    double pfc_thal_w=          80.0;
    double pfc_thal_cp=         0.5;
    double thal_pfc_w=          80.0;
    double thal_pfc_cp=         0.5;
    double pfc_cd_w=            180.0;
    double pfc_cd_cp=           0.5;
    double cd_gpi_w=            -200.0;
    double cd_gpi_cp=           0.5;
    double gpi_thal_w=          -160.0;
    double gpi_thal_cp=         0.5;

    vec_sptr<Neuron> in;
    vec<vec_sptr<Neuron>> pfc;
    vec_sptr<Neuron> pfciF;
    vec_sptr<Neuron> pfciS;
    vec<vec_sptr<Neuron>> thal;
    vec<vec_sptr<Neuron>> cd;
    vec<vec_sptr<Neuron>> gpi;

    PlasticityType e_type = PlasticityType::STANDARD_E;
    PlasticityType i_type = PlasticityType::ZAX_2018_I;

    ///////////////////////////////////////////////////////////////////////////


    for(int i = 0; i < size_input; i++) {

        for(int j = 0; j < in_per_input; j++) {
            in.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::RegularSpikingPPC),
                    rng(),
                    i,
                    datasize,
                    "IN("+std::to_string(i) + ","+std::to_string(j)+")"));
        }


        vec_sptr<Neuron> pfc_region;

        for(int j = 0; j < pfc_per_input; j++) {
            pfc_region.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::RegularSpiking),
                    rng(),
                    i,
                    datasize,
                    "PFC("+std::to_string(i)+","+std::to_string(j)+")"
            ));
        }
        pfc.push_back(pfc_region);


        for(int j = 0; j < pfciF_per_input; j++) {
            pfciF.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::FastSpiking),
                    rng(),
                    i,
                    datasize,
                    "PFCI_F("+std::to_string(i)+","+std::to_string(j)+")"
            ));
        }

        for(int j = 0; j < pfciS_per_input; j++) {
            pfciS.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::FastSpiking),
                    rng(),
                    i,
                    datasize,
                    "PFCI_S("+std::to_string(i)+","+std::to_string(j)+")"
            ));
        }


        vec_sptr<Neuron> thal_region;
        for(int j = 0; j < thal_per_input; j++) {
            thal_region.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::Relay),
                    rng(),
                    i,
                    datasize,
                    "THAL("+std::to_string(i)+","+std::to_string(j)+")"
            ));
        }
        thal.push_back(thal_region);

        vec_sptr<Neuron> cd_region;
        for(int j = 0; j < cd_per_input; j++) {
            cd_region.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::MediumSpiny),
                    rng(),
                    i,
                    datasize,
                    "CD("+std::to_string(i)+","+std::to_string(j)+")"
                    ));
        }
        cd.push_back(cd_region);

        vec_sptr<Neuron> gpi_region;
        for(int j = 0; j < gpi_per_input; j++) {
            gpi_region.push_back(std::make_shared<Neuron>(
                    NeuronTemplate::Instance()->GetNT(NeuronType::FastSpikingTAN),
                    rng(),
                    i,
                    datasize,
                    "GPI("+std::to_string(i)+","+std::to_string(j)+")"
                    ));
        }
        gpi.push_back(gpi_region);
    }

    ///////////////////////////////////////////////////////////////////////////

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < in_per_input; j++) {
            for(int k = 0; k < in_per_input; k++) {
                if(j==k) continue;
                else if(dice(rng)<in_in_cp) {
                    sptr<Neuron> pre = in[i*in_per_input+j];
                    sptr<Neuron> post = in[i*in_per_input+k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                        in_in_w, std::abs(in_in_w), false,
                        pre, post, e_type, inDelayDist(rng)
                    );
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }


    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < pfc_per_input; j++) {
            for(int k = 0; k < in_per_input; k++) {
                if(dice(rng)<in_pfc_cp) { // IN A to PFC
                    sptr<Neuron> pre = in[in_per_input*i+k];
                    sptr<Neuron> post = pfc[i][j];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            in_pfc_w, std::abs(in_pfc_w), false,
                            pre, post, e_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);


                }
                if(dice(rng)<pfc_in_cp) { // PFC to IN A
                    sptr<Neuron> pre = pfc[i][j];
                    sptr<Neuron> post = in[in_per_input*i+k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            pfc_in_w, std::abs(pfc_in_w), false,
                            pre, post, e_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);

                }
            }
        }
    }
    vec_sptr<SynapseRecorder> vec_SR;
    for(int i = 0; i < size_input; i++) {
        vec_SR.push_back(std::make_shared<SynapseRecorder>("CONN_"+std::to_string(i)));
    }

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < size_input; j++) {
            for(int k = 0; k < pfc_per_input; k++) {
                for(int l = 0; l < pfc_per_input; l++) {

                    if(i==j && k==l) continue;

                    sptr<Neuron> pre = pfc[i][k];
                    sptr<Neuron> post = pfc[j][l];

                    double cp = (pre->RegionID()==post->RegionID())
                        ? pfc_pfc_SAME_cp
                        : pfc_pfc_DIFF_cp;
                    if(Neuron::Distance(pre,post)<250.0) {
                        if(dice(rng)<cp) {
                            sptr<Connection> conn = std::make_shared<Connection>(
                                    pfc_pfc_w, std::abs(pfc_pfc_w), true,
                                    pre, post, e_type, inDelayDist(rng));
                            pre->AddOutputConnection(conn);
                            post->AddInputConnection(conn);
                            mind->AddConnection(conn);
                            if(i==j) {
                                vec_SR[i]->AddConnection(conn);
                            }
                        }
                    }
                    else {
                        if(dice(rng)<(cp*pfc_pfc_long_dist)) {
                            sptr<Connection> conn = std::make_shared<Connection>(
                                    pfc_pfc_w, std::abs(pfc_pfc_w), true,
                                    pre, post, e_type, inDelayDist(rng));
                            pre->AddOutputConnection(conn);
                            post->AddInputConnection(conn);
                            mind->AddConnection(conn);
                            if(i==j) {
                                vec_SR[i]->AddConnection(conn);
                            }
                        }
                    }
                }
            }
        }
    }

    for(vec_sptr<SynapseRecorder>::iterator it = vec_SR.begin();
            it != vec_SR.end(); it++) {
        (*it)->WriteSize();
        mind->AddSynapseRecorder(*it);
    }

    int pfcS_incoming[size_input] = {0};
    int pfcS_outgoing[size_input] = {0};

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < pfc_per_input; j++) {
            for(int k = 0; k < size_input*pfciS_per_input; k++) {

                if(static_cast<int>(std::floor(k/pfciS_per_input))==i) {

                    sptr<Neuron> pre = pfc[i][j];
                    sptr<Neuron> post = pfciS[k];

                    if(Neuron::Distance(pre,post)<250.0) {

                        if(dice(rng)<pfc_pfciS_cp) {

                            sptr<Connection> conn = std::make_shared<Connection>(
                                    pfc_pfciS_w, std::abs(pfc_pfciS_w), false,
                                    pre, post, e_type, inDelayDist(rng));
                            pre->AddOutputConnection(conn);
                            post->AddInputConnection(conn);
                            mind->AddConnection(conn);

                            pfcS_incoming[i]++;
                        }
                    } else {
                        if(dice(rng)<(pfc_pfciS_cp*pfc_pfc_long_dist)) {
                            sptr<Connection> conn = std::make_shared<Connection>(
                                    pfc_pfciS_w, std::abs(pfc_pfciS_w), false,
                                    pre, post, e_type, inDelayDist(rng));
                            pre->AddOutputConnection(conn);
                            post->AddInputConnection(conn);
                            mind->AddConnection(conn);

                            pfcS_incoming[i]++;
                        }
                    }
                }
                else {
                    sptr<Neuron> pre = pfciS[k];
                    sptr<Neuron> post = pfc[i][j];

                    if(Neuron::Distance(pre,post)<250.0) {

                        if(dice(rng)<pfciS_pfc_cp) {
                            sptr<Connection> conn = std::make_shared<Connection>(
                                    pfciS_pfc_w, std::abs(pfciS_pfc_w), false,
                                    pre, post, i_type, inDelayDist(rng));
                            pre->AddOutputConnection(conn);
                            post->AddInputConnection(conn);
                            mind->AddConnection(conn);

                            pfcS_outgoing[i]++;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < size_input; i++) {
        Log::Instance()->Write("PFCS "+std::to_string(i)+" IN " + std::to_string(pfcS_incoming[i]) +
                " OUT " + std::to_string(pfcS_outgoing[i]));
    }

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < pfc_per_input; j++) {
            for(int k = 0; k < size_input*pfciF_per_input; k++) {

                sptr<Neuron> pre = pfc[i][j];
                sptr<Neuron> post = pfciF[k];
                if(Neuron::Distance(pre,post)<250.0) {
                    if(dice(rng)<pfc_pfciF_cp) {
                        sptr<Connection> conn = std::make_shared<Connection>(
                                pfc_pfciF_w, std::abs(pfc_pfciF_w), false,
                                pre, post, e_type, inDelayDist(rng));
                        pre->AddOutputConnection(conn);
                        post->AddInputConnection(conn);
                        mind->AddConnection(conn);
                    }
                }
                else {
                    if(dice(rng)<(pfc_pfciF_cp*pfc_pfc_long_dist)) {
                        sptr<Connection> conn = std::make_shared<Connection>(
                                pfc_pfciF_w, std::abs(pfc_pfciF_w), false,
                                pre, post, e_type, inDelayDist(rng));
                        pre->AddOutputConnection(conn);
                        post->AddInputConnection(conn);
                        mind->AddConnection(conn);
                    }
                }

                pre = pfciF[k];
                post = pfc[i][j];
                if(Neuron::Distance(pre,post)<250.0) {
                    if(dice(rng)<pfciF_pfc_cp) {
                        sptr<Connection> conn = std::make_shared<Connection>(
                                pfciF_pfc_w, std::abs(pfciF_pfc_w), false,
                                pre, post, e_type, inDelayDist(rng));
                        pre->AddOutputConnection(conn);
                        post->AddInputConnection(conn);
                        mind->AddConnection(conn);
                    }
                }
            }
        }
    }

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < pfc_per_input; j++) {
            for(int k = 0; k < thal_per_input; k++) {
                if(dice(rng)<pfc_thal_cp) {
                    sptr<Neuron> pre = pfc[i][j];
                    sptr<Neuron> post = thal[i][k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            pfc_thal_w, std::abs(pfc_thal_w), false,
                            pre, post, e_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }

                if(dice(rng)<thal_pfc_cp) {
                    sptr<Neuron> pre = thal[i][k];
                    sptr<Neuron> post = pfc[i][j];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            thal_pfc_w, std::abs(thal_pfc_w), false,
                            pre, post, e_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < pfc_per_input; j++) {
            for(int k = 0; k < cd_per_input; k++) {
                if(dice(rng)<pfc_cd_cp) {
                    sptr<Neuron> pre = pfc[i][j];
                    sptr<Neuron> post = cd[i][k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            pfc_cd_w, std::abs(pfc_cd_w), false,
                            pre, post, e_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }

    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < cd_per_input; j++) {
            for(int k = 0; k < gpi_per_input; k++) {
                if(dice(rng)<cd_gpi_cp) {
                    sptr<Neuron> pre = cd[i][j];
                    sptr<Neuron> post = gpi[i][k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            cd_gpi_w, std::abs(cd_gpi_w), false,
                            pre, post, i_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                }
            }
        }
    }

    int num_conns = 0;
    for(int i = 0; i < size_input; i++) {
        for(int j = 0; j < gpi_per_input; j++) {
            for(int k = 0; k < thal_per_input; k++) {
                if(dice(rng)<gpi_thal_cp) {
                    sptr<Neuron> pre = gpi[i][j];
                    sptr<Neuron> post = thal[i][k];
                    sptr<Connection> conn = std::make_shared<Connection>(
                            gpi_thal_w, std::abs(gpi_thal_w), false,
                            pre, post, i_type, exDelayDist(rng));
                    pre->AddOutputConnection(conn);
                    post->AddInputConnection(conn);
                    mind->AddConnection(conn);
                    num_conns++;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // EX INPUTS
    Log::Instance()->Write("BUILDER: Building external inputs");

    for(int i = 0; i < size_input; i++) {

        sptr<ExInput> exin = std::make_shared<ExInput>(exin_in_w);
        for(int k = 0; k < in_per_input; k++) {
            if(dice(rng)<exin_in1_cp) {
                sptr<Neuron> neuron = in[in_per_input*i+k];
                neuron->AddExInputConnection(exin);
            }
        }
        mind->AddExInput(exin);

        exin = std::make_shared<ExInput>(exin_in_w);
        for(int k = 0; k < in_per_input; k++) {
            if(dice(rng)<exin_in2_cp) {
                sptr<Neuron> neuron = in[in_per_input*i+k];
                neuron->AddExInputConnection(exin);
            }
        }
        mind->AddExInput(exin);

    }


    //////////////////////////////////////////////////////////////////////////
    // ELECTRODES
    Log::Instance()->Write("BUILDER: Building electrodes");

    sptr<ElectrodeGroup> eg_in = std::make_shared<ElectrodeGroup>("IN");
    vec_sptr<ElectrodeGroup> eg_pfc;
    sptr<ElectrodeGroup> eg_pfciF = std::make_shared<ElectrodeGroup>("PFCI_F");
    sptr<ElectrodeGroup> eg_pfciS = std::make_shared<ElectrodeGroup>("PFCI_S");
    vec_sptr<ElectrodeGroup> eg_thal;
    vec_sptr<ElectrodeGroup> eg_cd;
    vec_sptr<ElectrodeGroup> eg_gpi;

    for(int i = 0; i < size_input*in_per_input; i++) {
        sptr<Neuron> neuron = in[i];
        sptr<Electrode> electrode = std::make_shared<Electrode>("IN(" + std::to_string(i)+")");
        electrode->SetElectrode(neuron);
        eg_in->electrodes.push_back(electrode);
    }

    for(int i = 0; i < size_input; i++) {



        sptr<ElectrodeGroup> eg = std::make_shared<ElectrodeGroup>("PFC("+std::to_string(i)+")");
        for(int j = 0; j < pfc_per_input; j++) {
            sptr<Neuron> neuron = pfc[i][j];
            sptr<Electrode> electrode = std::make_shared<Electrode>("PFC("+std::to_string(i)+","+std::to_string(j)+")");
            electrode->SetElectrode(neuron);
            eg->electrodes.push_back(electrode);
        }
        eg_pfc.push_back(eg);



        eg = std::make_shared<ElectrodeGroup>("THAL("+std::to_string(i)+")");
        for(int j = 0; j < thal_per_input; j++) {
            sptr<Neuron> neuron = thal[i][j];
            sptr<Electrode> electrode = std::make_shared<Electrode>("THAL("+std::to_string(i)+","+std::to_string(j)+")");
            electrode->SetElectrode(neuron);
            eg->electrodes.push_back(electrode);
        }
        eg_thal.push_back(eg);

        eg = std::make_shared<ElectrodeGroup>("CD("+std::to_string(i)+")");
        for(int j = 0; j < cd_per_input; j++) {
            sptr<Neuron> neuron = cd[i][j];
            sptr<Electrode> electrode = std::make_shared<Electrode>("CD("+std::to_string(i)+","+std::to_string(j)+")");
            electrode->SetElectrode(neuron);
            eg->electrodes.push_back(electrode);
        }
        eg_cd.push_back(eg);

        eg = std::make_shared<ElectrodeGroup>("GPI("+std::to_string(i)+")");
        for(int j = 0; j < gpi_per_input; j++) {
            sptr<Neuron> neuron = gpi[i][j];
            sptr<Electrode> electrode = std::make_shared<Electrode>("GPI("+std::to_string(i)+","+std::to_string(j)+")");
            electrode->SetElectrode(neuron);
            eg->electrodes.push_back(electrode);
        }
        eg_gpi.push_back(eg);
    }

    for(int i = 0; i < pfciF_per_input*size_input; i++) {
        sptr<Neuron> neuron = pfciF[i];
        sptr<Electrode> electrode = std::make_shared<Electrode>("PFCI_F("+std::to_string(i)+")");
        electrode->SetElectrode(neuron);
        eg_pfciF->electrodes.push_back(electrode);
    }

    for(int i = 0; i < pfciS_per_input*size_input; i++) {
        sptr<Neuron> neuron = pfciS[i];
        sptr<Electrode> electrode = std::make_shared<Electrode>("PFCI_S("+std::to_string(i)+")");
        electrode->SetElectrode(neuron);
        eg_pfciS->electrodes.push_back(electrode);
    }

    mind->AddElectrodeGroup(eg_in);
    for(unsigned int i = 0; i < eg_pfc.size(); i++) {
        mind->AddElectrodeGroup(eg_pfc[i]);
    }
    mind->AddElectrodeGroup(eg_pfciF);
    mind->AddElectrodeGroup(eg_pfciS);
    for(unsigned int i = 0; i < eg_thal.size(); i++) {
        mind->AddElectrodeGroup(eg_thal[i]);
    }
    for(unsigned int i = 0; i < eg_cd.size(); i++) {
        mind->AddElectrodeGroup(eg_cd[i]);
    }
    for(unsigned int i = 0; i < eg_gpi.size(); i++) {
        mind->AddElectrodeGroup(eg_gpi[i]);
    }

    /////////////////////////////////////////////////////////////////////

    mind->AddRegion(in);
    mind->AddRegion(pfciS);
    mind->AddRegion(pfciF);
    for(int i = 0; i < size_input; i++) {
        mind->AddRegion(thal[i]);
        mind->AddRegion(pfc[i]);
        mind->AddRegion(cd[i]);
        mind->AddRegion(gpi[i]);
    }



    Log::Instance()->Write("BUILDER: Mind topology\n" + mind->GetTopology());

    Log::Instance()->Write("BUILDER: Done");

    return mind;
}
*/
