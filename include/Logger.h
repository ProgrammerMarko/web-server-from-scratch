#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    Logger(std::string ime_servera);

    void info(std::string poruka);
    void upozorenje(std::string poruka);
    void greska(std::string poruka);
    void zahtjev(std::string metoda, std::string putanja, int status_kod);

private:
    std::string ime_servera;
    std::string dohvati_vrijeme();
    void ispisi(std::string ansi_boja, std::string razina, std::string poruka);
};

#endif
