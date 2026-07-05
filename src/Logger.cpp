#include "Logger.h"
#include <iostream>
#include <ctime>

Logger::Logger(std::string ime_servera) : ime_servera(ime_servera) {
}

void Logger::info(std::string poruka) {
    ispisi("\033[32m", "INFO", poruka);
}

void Logger::upozorenje(std::string poruka) {
    ispisi("\033[33m", "UPOZORENJE", poruka);
}

void Logger::greska(std::string poruka) {
    ispisi("\033[31m", "GRESKA", poruka);
}

void Logger::zahtjev(std::string metoda, std::string putanja, int status_kod) {
    std::string poruka = metoda + " " + putanja + " -> " + std::to_string(status_kod);
    ispisi("\033[34m", "ZAHTJEV", poruka);
}

std::string Logger::dohvati_vrijeme() {
    time_t sada = time(nullptr);
    struct tm* lokalno_vrijeme = localtime(&sada);

    char spremnik[20];
    strftime(spremnik, sizeof(spremnik), "%Y-%m-%d %H:%M:%S", lokalno_vrijeme);

    return std::string(spremnik);
}

void Logger::ispisi(std::string ansi_boja, std::string razina, std::string poruka) {
    std::cout << ansi_boja
              << "[" << dohvati_vrijeme() << "] "
              << "[" << ime_servera << "] "
              << "[" << razina << "] "
              << poruka
              << "\033[0m"
              << std::endl;
}
