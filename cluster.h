#ifndef CLUSTER_H
#define CLUSTER_H

#include "commandreader.h"
#include "clockgenerator.h"
#include "router.h"
#include "host.h"
#include <QObject>

class Cluster : public QObject
{
    Q_OBJECT
public:
    explicit Cluster(int _clusterNumber,QObject *parent = nullptr);
    void startRouting();
    void createStarTopology(clockGenerator *clk, CommandReader *cmdr);
    void startThreads();
    void createMeshTopology(clockGenerator *clk, CommandReader *cmdr);
    void addStarToMesh(Cluster *starCluster);
private:
    int clusterNumber;
    std::vector<Router*> routers;
    std::vector<QThread*> threads;
    void connectTwoRouters(Router* r1, int p1, Router* r2, int p2);
    void connectHost(Router* rt, int rp, Host* ht);
    // void connectChangeRoutingProtocolSignal();
    std::vector<std::string> host_ip1 = {"192.168.1.4.1", "192.168.1.5.1"};
    std::vector<std::string> host_ip2 = {"192.168.1.9.1", "192.168.1.13.1"};


// signals:
//     void changeRoutingProtocol(RoutingProtocol _rp);
};

#endif // CLUSTER_H
