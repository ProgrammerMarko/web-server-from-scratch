#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>

#include <map>

class HttpRequest {
public:
    HttpRequest(std::string sirovi_zahtjev);

    std::string dohvati_metodu();   // Vraca npr. "GET"
    std::string dohvati_putanju();  // Vraca npr. "/index.html"
    std::string dohvati_verziju();  // Vraca npr. "HTTP/1.1"
    std::string dohvati_zaglavlje(std::string kljuc);
    bool je_valjan();

private:
    std::string metoda;
    std::string putanja;
    std::string verzija;  
    bool valjan; 
    std::map<std::string, std::string> zaglavlja;
    void parsiraj(std::string sirovi_zahtjev);
};

#endif
