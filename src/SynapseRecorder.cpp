#include"SynapseRecorder.hpp"

SynapseRecorder::SynapseRecorder() {

}

SynapseRecorder::SynapseRecorder(std::string filename) {
    this->filename = config::RECDIR + "/" + filename;
}

void SynapseRecorder::AddConnection(sptr<Connection> conn){
    connections.push_back(conn);
}

void SynapseRecorder::Update(int64_t time) {
    double pot = 0.0;
    for(vec_sptr<Connection>::iterator it = connections.begin();
            it != connections.end(); it++) {
        pot += (*it)->TotalPotentiation();
    }

    data_to_write.push_back(pot);
}

void SynapseRecorder::WriteSize() {
    std::ofstream of(filename,std::ios::binary|std::ios::out | std::ios::app);
    int size = Size();
    of.write(reinterpret_cast<char*>(&size),sizeof(double));
    of.close();
}

void SynapseRecorder::Write() {
    std::ofstream of(filename,std::ios::binary|std::ios::out | std::ios::app);
    int index = 0;
    while(index<data_to_write.size()) {
        of.write(reinterpret_cast<char*>(&data_to_write[index]),sizeof(double));
        index++;
    }
    of.close();
}

void SynapseRecorder::SetFilename(std::string name) {
    this->filename = config::RECDIR + "/" + name;
}

int SynapseRecorder::Size() {
    return connections.size();
}
