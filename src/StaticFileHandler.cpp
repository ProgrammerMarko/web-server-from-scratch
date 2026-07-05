#include "StaticFileHandler.h"
#include <fstream>
#include <sstream>

StaticFileHandler::StaticFileHandler(std::string korijenska_mapa)
    : korijenska_mapa(korijenska_mapa) {
}

HttpResponse StaticFileHandler::handle(HttpRequest zahtjev) {
    HttpResponse odgovor;

    std::string putanja_zahtjeva = zahtjev.dohvati_putanju();

    // Zastita od path traversala - ".." u putanji ne smije izaci iz korijenske mape
    if (putanja_zahtjeva.find("..") != std::string::npos) {
        odgovor.postavi_status(403, "Forbidden");
        odgovor.dodaj_zaglavlje("Content-Type", "text/html");
        odgovor.dodaj_zaglavlje("Server", "OOP-WebServer/1.0");
        odgovor.postavi_tijelo("<html><body><h1>403 - Zabranjen pristup</h1></body></html>");
        return odgovor;
    }

    std::string putanja = korijenska_mapa + putanja_zahtjeva;

    if (datoteka_postoji(putanja)) {
        std::string sadrzaj = procitaj_datoteku(putanja);
        std::string mime_tip = odredi_mime_tip(putanja);

        odgovor.postavi_status(200, "OK");
        odgovor.dodaj_zaglavlje("Content-Type", mime_tip);
        odgovor.dodaj_zaglavlje("Server", "OOP-WebServer/1.0");
        odgovor.postavi_tijelo(sadrzaj);
    } else {
        std::string stranica_greske = procitaj_datoteku(korijenska_mapa + "/404.html");

        odgovor.postavi_status(404, "Not Found");
        odgovor.dodaj_zaglavlje("Content-Type", "text/html");
        odgovor.dodaj_zaglavlje("Server", "OOP-WebServer/1.0");

        if (!stranica_greske.empty()) {
            odgovor.postavi_tijelo(stranica_greske);
        } else {
            odgovor.postavi_tijelo("<html><body><h1>404 - Stranica nije pronadjena</h1></body></html>");
        }
    }

    return odgovor;
}

std::string StaticFileHandler::procitaj_datoteku(std::string putanja_datoteke) {
    std::ifstream datoteka(putanja_datoteke, std::ios::binary);
    if (!datoteka.is_open()) {
        return "";
    }

    std::stringstream spremnik;
    spremnik << datoteka.rdbuf();

    datoteka.close();

    return spremnik.str();
}

std::string StaticFileHandler::odredi_mime_tip(std::string putanja_datoteke) {
    size_t pozicija_tocke = putanja_datoteke.rfind('.');

    if (pozicija_tocke == std::string::npos) {
        return "application/octet-stream";
    }

    std::string ext = putanja_datoteke.substr(pozicija_tocke);

    if (ext == ".html" || ext == ".htm") return "text/html";
    if (ext == ".css")  return "text/css";
    if (ext == ".js")   return "application/javascript";
    if (ext == ".png")  return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".gif")  return "image/gif";
    if (ext == ".ico")  return "image/x-icon";
    if (ext == ".txt")  return "text/plain";
    if (ext == ".json") return "application/json";

    return "application/octet-stream";
}

bool StaticFileHandler::datoteka_postoji(std::string putanja_datoteke) {
    std::ifstream datoteka(putanja_datoteke);
    bool postoji = datoteka.good();
    datoteka.close();
    return postoji;
}
