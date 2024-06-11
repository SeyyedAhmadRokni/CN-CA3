#include "host.h"

#include "packet.h"
#include <iostream>
#include <random>
#include <cmath>
#include <time.h>
#include <sstream>

Host::Host(double pareto_alpha, double pareto_xm, int _as, QObject *parent)
    : QObject{parent}{

    alpha = pareto_alpha;
    xm = pareto_xm;
    port = new Buffer(0);
    srand(time(NULL));
    generator = new std::default_random_engine(time(NULL));
    AS = _as;
}

void Host::setPartners(const std::vector<std::string>& _partners){
    for (std::string p: _partners){
        partners.push_back(p);
    }
}


void Host::createAndSendPacket(){
    std::cout << "regular packet sent." << std::endl;
    int random = rand()%partners.size();
    std::string choosed_partner = partners[random];
    std::shared_ptr<Packet> packet = std::make_shared<Packet>(choosed_partner, ip, REGULAR_PACKET);
    packet.get()->setBody("Besme Allah Alrahman Alrahim");
    packet.get()->addASNumber(AS);
    std::cout << "Packet sent"<<std::endl;
    port->addToOutBuffer(packet);
}

double Host::paretoDistribution() {
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    double u = uniform_dist(*generator);
    return xm / std::pow(1 - u, 1.0 / alpha);
}


void Host::parteoSendPacket(){
    if (ip.compare(INVALID_IP) == 0){
        std::cout << "IP packet sent." << std::endl;
        std::shared_ptr<Packet> packet = std::make_shared<Packet>(routerIp, ip, REQUEST_IP_PACKET);
        packet->addASNumber(AS);
        port->addToOutBuffer(packet);
        port->sendPacket();
        return ;
    }
    double pareto = paretoDistribution();
    double random = rand();
    if (random < pareto && sent == 0){
        createAndSendPacket();
    }
}

void Host::handlePackets(){
    std::shared_ptr<Packet> packet = port->getFirstPacket();
    if (packet.get() != nullptr){
        switch (Packet::getPacketType(packet.get())) {
        case REGULAR:
            std::cout <<"WOW " << packet.get()->getSource() << std::endl;
            break;
        case REQUEST_IP:
            if (ip.compare(INVALID_IP) == 0){
                ip = packet.get()->getBody();
                std::cout <<"IP setted. " << packet.get()->getBody() << std::endl;

            }
        default:
            break;
        }
    }
    port->sendPacket();
}

std::string Host::getIp(){
    return ip;
}

void Host::setRouter(std::string _ip){
    routerIp = _ip;
}
