#include "commandreader.h"
#include "clockgenerator.h"
#include "cluster.h"
#include "router.h"
#include "host.h"
#include <QThread>
#include <QtConcurrent>

Cluster::Cluster(int _clusterNumber,QObject *parent)
    : QObject{parent}
{
    clusterNumber = _clusterNumber;
}

void Cluster::connectHost(Router* rt, int rp, Host* ht){
    QObject::connect(rt->ports[rp], &Buffer::sendPacketSignal, ht->port, &Buffer::recievePacket);
    QObject::connect(ht->port, &Buffer::sendPacketSignal, rt->ports[rp], &Buffer::recievePacket);
    rt->setNeighbor(rp, ht->getIp());
}

void Cluster::connectTwoRouters(Router* r1, int p1, Router* r2, int p2){
        QObject::connect(r1->ports[p1], &Buffer::sendPacketSignal, r2->ports[p2], &Buffer::recievePacket);
        QObject::connect(r2->ports[p2], &Buffer::sendPacketSignal, r1->ports[p1], &Buffer::recievePacket);
        if (r1->getAS() == r2->getAS()){
            r1->setNeighbor(p1, r2->getIp());
            r2->setNeighbor(p2, r1->getIp());
        }
}

void Cluster::createStarTopology(clockGenerator* clk, CommandReader* cmdr,PacketSaver* packetSaver){
    std::vector<std::string> ipList = {"192.168.1.1","192.168.1.2","192.168.1.3","192.168.1.4"
                                  ,"192.168.1.5","192.168.1.6","192.168.1.7","192.168.1.8"};


    routers.push_back(new Router(0, ipList[0], clusterNumber,"0.0.0.0"));
    QThread* thread = new QThread();
    threads.push_back(thread);
    routers[0]->moveToThread(thread);
    QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[0], &Router::commandSlot);
    QObject::connect(clk, &clockGenerator::clockSignal, routers[0], &Router::processPacketsOnSignal);
    for(int i = 1; i  < 8 ; i++){
        Router* router = new Router(i, ipList[i], clusterNumber,"0.0.0.0");
        QThread* thread = new QThread();
        routers.push_back(router);
        threads.push_back(thread);
        router->moveToThread(thread);
        connectTwoRouters(routers[i-1], 0, routers[i%7], 1);
        QObject::connect(clk, &clockGenerator::clockSignal, routers[i], &Router::processPacketsOnSignal);
        QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[i], &Router::commandSlot);
    }

    QThread* threadMid = threads[7];
    Router* router = routers[7];
    router->moveToThread(threadMid);

    connectTwoRouters(router, 0, routers[1], 2);
    connectTwoRouters(router, 1, routers[2], 2);
    connectTwoRouters(router, 2, routers[4], 2);
    connectTwoRouters(router, 3, routers[6], 2);

    for (int i =0; i < routers.size(); i++){
        routers[i]->setibgpIps({"192.168.1.1","192.168.1.2","192.168.1.7"});
    }


    Host* h1 = new Host (host_ip1[0], 0.1, 0.1, clusterNumber, "0.0.0.0");
    QObject::connect(clk, &clockGenerator::clockSignal, h1, &Host::parteoSendPacket);
    QObject::connect(clk, &clockGenerator::clockSignal, h1, &Host::handlePackets);
    h1->setPartners(host_ip2);
    QObject::connect(h1,&Host::sendPacket,packetSaver, &PacketSaver::savePackets);
    QThread* thread1 = new QThread();
    h1->moveToThread(thread1);
    threads.push_back(thread1);

    Host* h2 = new Host (host_ip1[1], 0.1, 0.1, clusterNumber, "0.0.0.0");
    QObject::connect(clk, &clockGenerator::clockSignal, h2, &Host::parteoSendPacket);
    QObject::connect(clk, &clockGenerator::clockSignal, h2, &Host::handlePackets);
    h2->setPartners(host_ip2);
    QObject::connect(h2,&Host::sendPacket,packetSaver, &PacketSaver::savePackets);
    QThread* thread2 = new QThread();
    h2->moveToThread(thread2);
    threads.push_back(thread2);

    connectHost(routers[3], 4, h1);
    connectHost(routers[4], 4, h2);

}


void Cluster::startThreads(){
    for(int i=0; i < threads.size(); i++){
        threads[i]->start();
    }
}

void Cluster::createMeshTopology(clockGenerator* clk, CommandReader* cmdr,PacketSaver* packetSaver){
    std::vector<std::string> ipList = {"192.168.1.9","192.168.1.10","192.168.1.11","192.168.1.12"
                                       ,"192.168.1.13","192.168.1.14","192.168.1.15","192.168.1.16","192.168.1.17","192.168.1.18","192.168.1.19", "192.168.1.20","192.168.1.21","192.168.1.22","192.168.1.23","192.168.1.24"};

    for (int i =0; i < 4; i++){
        Router* router1 = new Router(i*4,ipList[i*4],clusterNumber,"0.0.0.0");
        QThread* thread1 = new QThread();
        routers.push_back(router1);
        threads.push_back(thread1);
        router1->moveToThread(thread1);
        if (i != 0){
            connectTwoRouters(router1, 3, routers[(i-1)*4], 2);
        }
        QObject::connect(clk, &clockGenerator::clockSignal, router1, &Router::processPacketsOnSignal);
        QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, router1, &Router::commandSlot);
        for (int j = 1 ; j < 4; j++){
            Router* router = new Router((i*4)+j,ipList[(i*4)+j],clusterNumber,"0.0.0.0");
            QThread* thread = new QThread();
            routers.push_back(router);
            threads.push_back(thread);
            router->moveToThread(thread);
            if (i != 0){
                connectTwoRouters(router, 3, routers[((i-1)*4)+j], 2);
            }
            connectTwoRouters(router, 1, routers[((i)*4)+j-1], 0);
            QObject::connect(clk, &clockGenerator::clockSignal, router, &Router::processPacketsOnSignal);
            QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, router, &Router::commandSlot);
        }
    }
    for (int i =0; i < routers.size(); i++){
        routers[i]->setibgpIps({"192.168.1.16","192.168.1.20","192.168.1.24"});
    }
    Host* h1 = new Host (host_ip2[0], 0.1, 0.1, clusterNumber, "0.0.0.0");
    QObject::connect(clk, &clockGenerator::clockSignal, h1, &Host::parteoSendPacket);
    QObject::connect(clk, &clockGenerator::clockSignal, h1, &Host::handlePackets);
    h1->setPartners(host_ip1);
    QObject::connect(h1,&Host::sendPacket,packetSaver, &PacketSaver::savePackets);
    QThread* thread1 = new QThread();
    h1->moveToThread(thread1);
    threads.push_back(thread1);

    Host* h2 = new Host (host_ip2[1], 0.1, 0.1, clusterNumber, "0.0.0.0");
    QObject::connect(clk, &clockGenerator::clockSignal, h2, &Host::parteoSendPacket);
    QObject::connect(clk, &clockGenerator::clockSignal, h2, &Host::handlePackets);
    QObject::connect(h2,&Host::sendPacket,packetSaver, &PacketSaver::savePackets);
    h2->setPartners(host_ip1);
    QThread* thread2 = new QThread();
    h2->moveToThread(thread2);
    threads.push_back(thread2);

    connectHost(routers[8], 4, h1);
    connectHost(routers[12], 4, h2);

}


void Cluster::addStarToMesh(Cluster* starCluster){

    connectTwoRouters(routers[7], 4, starCluster->routers[6], 4);
    routers[7]->setAsBorder();
    starCluster->routers[6]->setAsBorder();

    connectTwoRouters(routers[11], 4, starCluster->routers[0], 4);
    routers[11]->setAsBorder();
    starCluster->routers[0]->setAsBorder();

    connectTwoRouters(routers[15], 4, starCluster->routers[1], 4);
    routers[15]->setAsBorder();
    starCluster->routers[1]->setAsBorder();
}

void Cluster::startRouting(){
    QtConcurrent::run(&Router::StartRIPProtocol, routers[0]);
    // for (int i = 0; i < routers.size(); ++i) {
    //     QtConcurrent::run(&Router::StartOSPFProtocol, routers[i]);
    // }
    // QThread::sleep(1);
    // for (int i =0; i < 8; i++){
    //     routers[i]->printRoutingTable();
    // }
}
