#include "packet.h"
#include <iostream>

Packet::Packet(std::string _dest_addr, std::string _source_addr,std::string _Type = "packet", IPVersion _ipv) {
    dest_addr = _dest_addr;
    source_addr = _source_addr;
    Type = _Type;
    ipv = _ipv;
}

int Packet::getInitialASNumber(){
    if (ASNumbers.empty()){
        std::cout << "ooooooooooooooooooooooooooh no set AS for you packets"<< std::endl;
    }
    return ASNumbers[0];
}

std::string Packet::getDest(){
    return dest_addr;
}


std::string Packet::getBody(){
    return body;
}

void Packet::setBody(std::string _body){
    body = _body;
}


std::string Packet::getType(){
    return  Type;
}


std::string Packet::getSource(){
    return source_addr;
}


void Packet::addASNumber(int _AS){
    ASNumbers.push_back(_AS);
}

bool Packet::changeToIPV6(){
    if (ipv == IPV_6){
        return false;
    }
    ipv6Packet = this;
    encapsulated = true;
    return true;
}

IPVersion Packet::getIpVersion(){
    return ipv;
}

void Packet::increaseDeliveryCycles(){
    deliveryCycles++;
}

void Packet::increaseBufferWaitingCycles(){
    bufferWaitingCycles++;
}

void Packet::setPacketDestination(std::string _dest){
    dest_addr = _dest;
}

PacketType Packet::getPacketType(Packet* packet){
    if (packet->getType().compare(RIP_PACKET) == 0){
        return RIP;
    }
    else if (packet->getType().compare(OSPF_PACKET) == 0){
        return OSPF;
    }
    else if (packet->getType().compare(EBPG_PACKET) == 0){
        return EBGP;
    }
    else if(packet->getType().compare(IBGP_PACKET) == 0){
        return IBGP;
    }
    else if(packet->getType().compare(REGULAR_PACKET) == 0){
        return REGULAR;
    }
    else if(packet->getType().compare(REQUEST_IP_PACKET) == 0){
        return REQUEST_IP;
    }
    else{
        return OTHERS;
    }

}
