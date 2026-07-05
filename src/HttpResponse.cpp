#include "HttpResponse.h"

HttpResponse::HttpResponse() : status_kod(200), status_opis("OK"), tijelo("") {
}

void HttpResponse::postavi_status(int kod, std::string opis) {
    status_kod = kod;
    status_opis = opis;
}

void HttpResponse::dodaj_zaglavlje(std::string kljuc, std::string vrijednost) {
    zaglavlja[kljuc] = vrijednost;
}


void HttpResponse::postavi_tijelo(std::string sadrzaj) {
    tijelo = sadrzaj;
    zaglavlja["Content-Length"] = std::to_string(tijelo.size());
}

// generiraj() sastavlja kompletan HTTP odgovor kao string.
// Format:
//   "HTTP/1.1 200 OK\r\n"           <- status linija
//   "Content-Type: text/html\r\n"   <- zaglavlja (moze ih biti vise)
//   "\r\n"                          <- prazan red = kraj zaglavlja
//   "<html>...</html>"              <- tijelo = sadrzaj stranice
// "\r\n" je standardni separator u HTTP protokolu (carriage return + line feed).
std::string HttpResponse::generiraj() {
    std::string odgovor = "";

    odgovor += "HTTP/1.1 " + std::to_string(status_kod) + " " + status_opis + "\r\n";

    std::map<std::string, std::string>::iterator it;
    for (it = zaglavlja.begin(); it != zaglavlja.end(); ++it) {
        odgovor += it->first + ": " + it->second + "\r\n";
    }

    odgovor += "\r\n";

    odgovor += tijelo;

    return odgovor;
}

// Vraca status kod. Npr. 200, 404. Koristi ga Logger za ispis.
int HttpResponse::dohvati_status_kod() {
    return status_kod;
}
