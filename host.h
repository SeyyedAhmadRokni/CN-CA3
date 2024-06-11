#ifndef HOST_H
#define HOST_H

#include "buffer.h"
#include <QObject>
#include <random>

const std::string INVALID_IP = "invalid IP";

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(double parteo_alpha, double parteo_mx, int _as, std::string _mask, QObject *parent = nullptr);
    explicit Host(std::string _ip, double parteo_alpha, double parteo_mx, int _as, std::string _mask, QObject *parent = nullptr);
    void setPartners(const std::vector<std::string>& _partners);
    void createAndSendPacket();
    double paretoDistribution();
    void setRouter(std::string _ip);
    Buffer* port;
    std::string getIp();
    bool sent =0;

private:
    double alpha, xm;
    std::string ip = INVALID_IP;
    std::string mask;
    std::default_random_engine* generator;
    std::vector<std::string> partners;
    void setIp(std::shared_ptr<Packet> packet);
    int AS;
    std::string routerIp;
    int sendIpPacketPeriod = -1;
signals:
    void sendPacket(std::shared_ptr<Packet> _packet);


public slots:
    void parteoSendPacket();
    void handlePackets();

};

#endif // HOST_H
