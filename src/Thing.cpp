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
    return num_details;
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
    double sum=0.0;

    for(int i = 0; i < details.size(); i++) {
        sum=details[i];
    }

    for(int i = 0; i < details.size(); i++) {
        t->SetDetail(i, details[i] / sum);
    }
    return t;
}
double Thing::AbsDiff(sptr<Thing> t1, sptr<Thing> t2) {
    double diff=0.0;
    int min_details = (t1->NumDetails()<t2->NumDetails() ? 
                        t1->NumDetails() :
                        t2->NumDetails()); 
    for(int i = 0; i < min_details; i++) {
        diff = t1->GetDetail(i) - t2->GetDetail(i);
    }
    return std::abs(diff) / min_details;
}
std::string Thing::ToString() {
    std::string str(name);
    for(int i = 0; i < num_details; i++) {
        str += "\n" + std::to_string(details[i]);
    }
    return str+"\n";
}