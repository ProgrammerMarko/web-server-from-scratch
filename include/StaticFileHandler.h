#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H

#include "RequestHandler.h"
#include <string>

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler(std::string korijenska_mapa);
    HttpResponse handle(HttpRequest zahtjev);

private:
    std::string korijenska_mapa;
    std::string procitaj_datoteku(std::string putanja_datoteke);
    std::string odredi_mime_tip(std::string putanja_datoteke);
    bool datoteka_postoji(std::string putanja_datoteke);
};

#endif
