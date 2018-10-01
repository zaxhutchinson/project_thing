#include"Thing.hpp"

Thing::Thing() {

}

Thing::Thing(std::string name) {
    this->name = name;
}

Thing::Thing(std::string name, int num_details) {
    this->name = name;
    this->num_details = num_details;
    for(int i = 0; i < num_details; i++) {
        details.push_back(0.0);
    }
}
std::string Thing::GetName() {
    return name;
}
int Thing::NumDetails() {
    return details.size();
}
double Thing::GetDetail(int index) {
    return details[index];
}
void Thing::SetDetail(int index, double amount) {
    details[index]=amount;
}
void Thing::AddDetail(int index, double amount) {
    details[index]+=amount;
}
void Thing::CreateDetail(double amount) {
    details.push_back(amount);
    num_details++;
}
sptr<Thing> Thing::NormalizeDetail() {
    sptr<Thing> t = std::make_shared<Thing>(name+"_norm",num_details);
    double max_val=0.0;

    for(int i = 0; i < details.size(); i++) {
        if(details[i] > max_val) {
            max_val=details[i];
        }
    }

    for(int i = 0; i < details.size(); i++) {
        t->SetDetail(i, details[i] / max_val);
    }
    return t;
}
sptr<Thing> Thing::AbsDiff(sptr<Thing> t1, sptr<Thing> t2) {
    sptr<Thing> diff = std::make_shared<Thing>("diff",t2->NumDetails());
    for(int i = 0; i < diff->NumDetails(); i++) {
        double d = 100.0 * t2->GetDetail(i);
        double offby = t1->GetDetail(i) - d;
        diff->SetDetail(i, offby / 100.0);
    }
    return diff;


    // sptr<Thing> diff = std::make_shared<Thing>("off_by",t2->NumDetails());
    // for(int i = 0; i < t2->NumDetails(); i++) {
    //     diff->SetDetail(i, t1->GetDetail(i) - t2->GetDetail(i));
    // }
    // return diff;
}
std::string Thing::ToString() {
    std::string str("");
    for(int i = 0; i < num_details; i++) {
        str += "\n\t" + std::to_string(details[i]);
    }
    return str;
}