#include"Thing.hpp"

Thing::Thing() {

}

Thing::Thing(int num_details) {
    for(int i = 0; i < num_details; i++) {
        details.push_back(0.0);
    }
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
void Thing::NormalizeDetail() {
    double max_detail=std::numeric_limits<double>::min();
    int max_detail_index=0;

    for(int i = 0; i < details.size(); i++) {
        if(details[i] > max_detail) {
            max_detail = details[i];
            max_detail_index = i;
        }
    }

    for(int i = 0; i < details.size(); i++) {
        details[i] /= max_detail;
    }
}