// Kompajliranje:  g++ -std=c++11 -I include src/*.cpp -o web_server
// Pokretanje:     ./web_server 8080
// Pristup:        http://localhost:8080

#include "Server.h"
#include "StaticFileHandler.h"
#include <iostream>
#include <cstdlib> // atoi()

int main(int argc, char* argv[]) {
    // Zadani port ako korisnik ne specificira
    int port = 8080;

    if (argc >= 2) {
        port = atoi(argv[1]); // atoi = ASCII to Integer
        if (port <= 0 || port > 65535) {
            std::cout << "Neispravan port: " << argv[1] << std::endl;
            std::cout << "Port mora biti broj izmedju 1 i 65535." << std::endl;
            return 1; // Greska
        }
    }

    StaticFileHandler handler("www");
    Server server(port, &handler);

    server.pokreni();

    return 0;
}
