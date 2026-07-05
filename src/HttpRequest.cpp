#include "HttpRequest.h"
#include <sstream>

HttpRequest::HttpRequest(std::string sirovi_zahtjev) : valjan(false) {
    parsiraj(sirovi_zahtjev);
}

std::string HttpRequest::dohvati_metodu() { return metoda; }
std::string HttpRequest::dohvati_putanju() { return putanja; }
std::string HttpRequest::dohvati_verziju() { return verzija; }
bool HttpRequest::je_valjan() { return valjan; }

std::string HttpRequest::dohvati_zaglavlje(std::string kljuc) {
    std::map<std::string, std::string>::iterator it = zaglavlja.find(kljuc);
    if (it != zaglavlja.end()) {
        return it->second;
    }
    return "";
}

// parsiraj() - parsira sirovi HTTP zahtjev
// Primjer:
//   GET /index.html HTTP/1.1\r\n
//   Host: localhost\r\n
//   User-Agent: Mozilla/5.0\r\n
//   \r\n
void HttpRequest::parsiraj(std::string sirovi_zahtjev) {
    std::istringstream stream(sirovi_zahtjev);

    std::string prva_linija;
    std::getline(stream, prva_linija);

    std::istringstream stream_linije(prva_linija);
    stream_linije >> metoda >> putanja >> verzija;

    if (metoda.empty() || putanja.empty() || verzija.empty()) {
        return;
    }

    if (putanja == "/") {
        putanja = "/index.html";
    }

    std::string linija;
    while (std::getline(stream, linija)) {
        // Uklanjamo '\r' (HTTP koristi \r\n za kraj reda)
        if (!linija.empty() && linija[linija.size() - 1] == '\r') {
            linija.erase(linija.size() - 1);
        }

        // Prazan red oznacava kraj zaglavlja
        if (linija.empty()) {
            break;
        }

        // Trazimo dvotocku koja odvaja kljuc od vrijednosti
        size_t pozicija = linija.find(':');
        if (pozicija != std::string::npos) {
            std::string kljuc = linija.substr(0, pozicija);
            std::string vrijednost = "";
            if (pozicija + 2 < linija.size()) {
                vrijednost = linija.substr(pozicija + 2);
            }
            zaglavlja[kljuc] = vrijednost;
        }
    }

    // Parsiranje uspjelo
    valjan = true;
}
