/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkEngine.h
 * Author: arnaubigas
 *
 * Created on March 19, 2016, 7:17 PM
 */

#ifndef NETWORKENGINE_H
#define NETWORKENGINE_H

#define MAXPACKETLENGTH 1024

namespace NetworkEngine {

    bool init(bool client);
    
    bool startServer();
    
    bool connectToServer(const char* ip, int port);
    
    bool checkInboundConnections();
    
    void sendPacket();
    
    void acceptIncomingConnections(bool accept);
    
    void kill();
    
}

#endif /* NETWORKENGINE_H */

