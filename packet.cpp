#include "packet.h"
#include <iostream>

Packet::Packet(std::string _dest_addr, std::string _source_addr,std::string _Type = "packet") {
    dest_addr = _dest_addr;
    source_addr = _source_addr;
    Type = _Type;
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


void Packet::setNextHopPort(int _port){
    nextHopPort = _port;
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


