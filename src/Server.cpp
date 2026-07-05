#include "Server.h"
#include "HttpRequest.h"

// <sys/socket.h> daje funkcije za rad sa soketima:
//   socket()  - kreira novi soket (komunikacijski kanal)
//   bind()    - veze soket na adresu i port
//   listen()  - pocinje slusati dolazece veze
//   accept()  - prihvaca jednu klijentsku vezu
//   recv()    - cita podatke iz soketa
//   send()    - salje podatke kroz soket
//   setsockopt() - postavlja opcije soketa
#include <sys/socket.h>

// <netinet/in.h> daje strukture za internet adrese:
//   struct sockaddr_in - struktura koja sadrzi IP adresu i port
//   INADDR_ANY - konstanta 0.0.0.0 (slusa na svim mreznim suceljima)
//   htons()    - pretvara port iz host byte ordera u network byte order
#include <netinet/in.h>

// <unistd.h> daje POSIX funkcije:
//   close()  - zatvara datotecni deskriptor (soket)
//   fork()   - stvara kopiju trenutnog procesa (child proces)
//   _exit()  - zavrsava child proces bez ciscenja parentovih resursa
#include <unistd.h>

// <sys/wait.h> daje funkcije za cekanje child procesa.
#include <sys/wait.h>

// <signal.h> daje funkcije za rad sa signalima operativnog sustava:
//   signal(SIGCHLD, SIG_IGN) - kaze OS-u da automatski pocisti child procese
#include <signal.h>

#include <iostream>

// <cstring> daje memset() - popunjava blok memorije zadanom vrijednoscu.
// Npr. memset(buf, 0, 100); popunjava 100 bajtova nulama.
#include <cstring>

Server::Server(int port, RequestHandler* handler)
    : port(port), soket_servera(-1), handler(handler), logger("WebServer") {
}

Server::~Server() {
    if (soket_servera != -1) {
        close(soket_servera);
        logger.info("Soket servera zatvoren.");
    }
}

void Server::pokreni() {
    if (!inicijaliziraj_soket()) {
        logger.greska("Nije moguce inicijalizirati soket. Server se gasi.");
        return;
    }

    logger.info("Server pokrenut na portu " + std::to_string(port));
    logger.info("Otvorite preglednik na http://localhost:" + std::to_string(port));

    signal(SIGCHLD, SIG_IGN);

    while (true) {
        struct sockaddr_in adresa_klijenta;
        socklen_t velicina_adrese = sizeof(adresa_klijenta);

        int soket_klijenta = accept(
            soket_servera,
            (struct sockaddr*)&adresa_klijenta,
            &velicina_adrese
        );

        if (soket_klijenta < 0) {
            logger.greska("Greska pri prihvacanju veze klijenta.");
            continue; // "continue" preskace ostatak petlje i vraca se na pocetak.
        }

        logger.info("Novi klijent spojen.");

        pid_t pid = fork();

        if (pid < 0) {
            logger.greska("Greska pri stvaranju child procesa (fork).");
            close(soket_klijenta);
        } else if (pid == 0) {
            close(soket_servera);
            obradi_klijenta(soket_klijenta);
            close(soket_klijenta);
            _exit(0);
        } else {
            close(soket_klijenta);
        }
    }
}

bool Server::inicijaliziraj_soket() {
    soket_servera = socket(AF_INET, SOCK_STREAM, 0);
    if (soket_servera < 0) {
        logger.greska("Nije moguce kreirati soket.");
        return false;
    }

    int opcija = 1;
    setsockopt(soket_servera, SOL_SOCKET, SO_REUSEADDR, &opcija, sizeof(opcija));

    struct sockaddr_in adresa_servera;
    memset(&adresa_servera, 0, sizeof(adresa_servera));

    adresa_servera.sin_family = AF_INET;
    adresa_servera.sin_addr.s_addr = INADDR_ANY;
    adresa_servera.sin_port = htons(port);

    int rezultat = bind(soket_servera, (struct sockaddr*)&adresa_servera, sizeof(adresa_servera));
    if (rezultat < 0) {
        logger.greska("Nije moguce vezati soket na port " + std::to_string(port));
        return false;
    }

    if (listen(soket_servera, 10) < 0) {
        logger.greska("Nije moguce poceti slusati na soketu.");
        return false;
    }

    logger.info("Soket uspjesno inicijaliziran na portu " + std::to_string(port));
    return true;
}

void Server::obradi_klijenta(int soket_klijenta) {
    char spremnik[8192];
    memset(spremnik, 0, sizeof(spremnik));

    int procitano = recv(soket_klijenta, spremnik, sizeof(spremnik) - 1, 0);
    if (procitano <= 0) {
        logger.upozorenje("Klijent se odspojio bez slanja podataka.");
        return;
    }

    std::string sirovi_zahtjev(spremnik);
    HttpRequest zahtjev(sirovi_zahtjev);

    if (!zahtjev.je_valjan()) {
        logger.upozorenje("Primljen neispravan HTTP zahtjev.");
        return;
    }

    HttpResponse odgovor = handler->handle(zahtjev);

    logger.zahtjev(zahtjev.dohvati_metodu(), zahtjev.dohvati_putanju(), odgovor.dohvati_status_kod());

    std::string odgovor_str = odgovor.generiraj();

    send(soket_klijenta, odgovor_str.c_str(), odgovor_str.size(), 0);
}
