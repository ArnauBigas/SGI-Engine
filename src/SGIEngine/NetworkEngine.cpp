/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkEngine.cpp
 * Author: arnaubigas
 * 
 * Created on March 19, 2016, 7:17 PM
 */

#include "NetworkEngine.h"
#include "Logger.h"
#include "CrashHandler.h"
#include "Config.h"
#include "SDL_stdinc.h"
#include <SDL2/SDL_net.h>

//This instance is a server
bool isServer;

//This instance is a client with a local server
bool localServer;

bool acceptIncoming = false;

/**
 Socket of the local server
 */
TCPsocket serverSocket;

/**
 Socket of the remote server
 */
TCPsocket remoteServer;

static int serverConnectionRoutine(void *data){
    TCPsocket socket = static_cast<TCPsocket>(data);
    bool connectionActive = true;
    Uint8 buff[MAXPACKETLENGTH];
    
    Logger::info << "Listening to client..." << std::endl;
    
    while(connectionActive){
        if(SDLNet_TCP_Recv(socket,buff,MAXPACKETLENGTH-1) > 0){
            Logger::info << "Client says " << buff << std::endl;
        } else {
            Logger::warning << "There was a network error with the client, connection terminated" << std::endl;
            connectionActive = false;
        }
    }
}

namespace NetworkEngine {
    
    bool init(bool client){
        
        isServer = !client;
        
        if (SDLNet_Init() < 0)
	{
            CrashHandler::crash("Couldn't initialize SDL_Net");
	}
        
        if(isServer){
            return startServer();
        }
        
        return true;
    }
    
    bool startServer(){
        
        Logger::info << "Starting server..." << std::endl;
        
        if(!isServer){
            localServer = true;
        }
        
        IPaddress localIP;
        
        if (SDLNet_ResolveHost(&localIP, NULL, Config::network.port) < 0)
        {
            CrashHandler::crash("Couldn't resolve local host: %s", SDLNet_GetError());
            return false;
        }

        if (!(serverSocket = SDLNet_TCP_Open(&localIP)))
        {
            CrashHandler::crash("Couldn't open: %s", SDLNet_GetError());
            return false;
        }
        
        //The application should set this, it's here for debugging purposes
        acceptIncoming = true;
        
        return true;
    }
    
    bool connectToServer(const char* ip, int port){
        if(isServer){
            Logger::error << "A server can't connect to another server..." << std::endl;
            return false;
        }
        if(localServer){
            Logger::error << "Can't connect to server, local server is running." << std::endl;
            return false;
        }
        
        IPaddress ipAdd;
        
        if (SDLNet_ResolveHost(&ipAdd, ip, port) < 0) {
		Logger::error << "Couldn't resolve host address: " << SDLNet_GetError() << std::endl;
		return false;
	}
        
        if (!(remoteServer = SDLNet_TCP_Open(&ipAdd))) {
		Logger::error << "Couldn't open a socket: " << SDLNet_GetError() << std::endl;
		return false;
	}
        
        SDLNet_TCP_Send(remoteServer, "test", 4);
    }
    
    void acceptIncomingConnections(bool accept){
        acceptIncoming = accept;
    }
    
    bool checkInboundConnections(){
        //Server started
        if((isServer || localServer) && acceptIncoming){
            TCPsocket newSocket;

            newSocket = SDLNet_TCP_Accept(serverSocket);
            if(newSocket) {
                IPaddress *remoteIP;
                if ((remoteIP = SDLNet_TCP_GetPeerAddress(newSocket))){
                    Logger::info << "Client connected: " << SDLNet_Read32(&remoteIP->host) << " " << SDLNet_Read16(&remoteIP->port) << std::endl;
                }
                if(NULL == SDL_CreateThread(serverConnectionRoutine, "ConnectionThread", (void *)newSocket)){
                    Logger::error << "Couldn't create a thread for an incoming connection." << std::endl;
                }
            }
        }
        return false;
    }
    
    void kill(){
        if(serverSocket){
            SDLNet_TCP_Close(serverSocket);
        }
        if(remoteServer){
            SDLNet_TCP_Close(remoteServer);
        }
        SDLNet_Quit();
    }
    
}