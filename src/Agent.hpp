#ifndef AGENT_HPP
#define AGENT_HPP

#include"cereal/archives/binary.hpp"
#include"cereal/types/memory.hpp"
#include"cereal/types/vector.hpp"

#include"zaxlib.hpp"
#include"Mind.hpp"
#include"ExInput.hpp"
#include"Thing.hpp"
#include"Sound.hpp"

class Agent {
public:
    Agent();
    /////////////////////////////////////////////////////////////////////////
    void Build();
    void AddMind(sptr<Mind> mind);
    void AddEye(sptr<ExInput> ex_input);
    void AddEar(sptr<ExInput> ex_input);
    /////////////////////////////////////////////////////////////////////////
    int GetX();
    int GetY();
    void SetX(int x);
    void SetY(int y);
    /////////////////////////////////////////////////////////////////////////
    void VisualInput(sptr<Thing> thing);
    void AuralInput(sptr<Sound> sound);
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
};

#endif
