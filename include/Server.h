#ifndef SERVER_H
#define SERVER_H

#include "RequestHandler.h"
#include "Logger.h"
#include <string>

class Server {
public:
    Server(int port, RequestHandler* handler);
    ~Server();
    void pokreni();

private:
    int port;               
    int soket_servera;       
    RequestHandler* handler; 
    Logger logger;           

    bool inicijaliziraj_soket();
    void obradi_klijenta(int soket_klijenta);
};

#endif
