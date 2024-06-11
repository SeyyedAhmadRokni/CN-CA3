#ifndef PACKET_H
#define PACKET_H


#include<string>
#include <vector>

const std::string REQUEST_IP_PACKET = "REQUEST_IP";
const std::string REGULAR_PACKET = "REGULAR";
const std::string RIP_PACKET = "RIP";
const std::string OSPF_PACKET = "OSPF";
const std::string EBPG_PACKET = "EBPG";
const std::string IBGP_PACKET = "IBGP";



typedef enum PacketType{
    RIP,
    OSPF,
    IBGP,
    EBGP,
    REGULAR,
    REQUEST_IP,
    OTHERS
} PacketType;

typedef enum IPVersion{
    IPV_4,
    IPV_6
} IPVersion;

class Packet
{
public:
    Packet(std::string _dest_addr, std::string _source_addr, std::string Type, IPVersion _ipv = IPV_4);
    void setBody(std::string _body);
    void addASNumber(int _AS);
    bool changeToIPV6();
    void increaseDeliveryCycles();
    void increaseBufferWaitingCycles();    int getInitialASNumber();
    std::string getType();
    std::string getSource();
    std::string getBody();
    std::string getDest();
    IPVersion getIpVersion();
    virtual ~Packet(){}
    void setPacketDestination(std::string _dest);
    static PacketType getPacketType(Packet* packet);

protected:
    std::string source_addr;
    std::vector<int> ASNumbers;

private:
    std::string Type;
    std::string dest_addr;
    std::string body;
    int bufferWaitingCycles = 0;
    int deliveryCycles = 0;
    std::vector<std::string> route;
    IPVersion ipv;
    Packet* ipv6Packet = nullptr;
    bool encapsulated = false;


};

#endif // PACKET_H
