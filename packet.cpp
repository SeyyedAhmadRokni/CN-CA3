#include "packet.h"
#include <iostream>
#include <bits/shared_ptr.h>

Packet::Packet(std::string _dest_addr, std::string _source_addr,std::string _mask,std::string _Type = "packet", IPVersion _ipv) {
    dest_addr = _dest_addr;
    source_addr = _source_addr;
    Type = _Type;
    ipv = _ipv;
    mask = _mask;
}


void Packet::addPath(std::string _ip){
    path.push_back(_ip);
}


void Packet::setFinalDest(std::string dest){
    finalDest = dest;
}

std::string Packet::getFinalDest(){
    return finalDest;
}


int Packet::getInitialASNumber(){
    if (ASNumbers.empty()){
        std::cout << "ooooooooooooooooooooooooooh no set AS for you packets"<< std::endl;
    }
    return ASNumbers[0];
}


void Packet::setType(std::string _type){
    Type = _type;
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
    ASNumbers.insert(ASNumbers.begin(), _AS);
}

bool Packet::changeToIPV6(){
    if (ipv == IPV_6){
        return false;
    }
    encapsulated = this;
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
    if (this == nullptr){
        return;
    }
    bufferWaitingCycles++;
}

void Packet::setPacketDestination(std::string _dest){
    dest_addr = _dest;
}

int Packet::getBufferCycles(){
    return bufferWaitingCycles;
}

int Packet::getDeliveryCycles(){
    return bufferWaitingCycles;
}

std::vector<std::string> Packet::getPath(){
    return path;
}

PacketType Packet::getPacketType(Packet* packet){
    if (packet->getType().compare(RIP_PACKET) == 0){
        return RIP;
    }
    else if (packet->getType().compare(OSPF_PACKET) == 0){
        return OSPF;
    }
    else if (packet->getType().compare(BPG_PACKET) == 0){
        return BGP;
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


std::shared_ptr<Packet> Packet::getEncapsulatedPacket(){
    return encapsulatedPacket;
}

bool Packet::isEncapsulated(){
    return encapsulated;
}

void Packet::seEncapsulatedPacket(std::shared_ptr<Packet> _packet){
    encapsulatedPacket = _packet;
    encapsulated = true;
}

