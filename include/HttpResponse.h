#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <map>

class HttpResponse {
public:
    HttpResponse();
    void postavi_status(int kod, std::string opis);
    void dodaj_zaglavlje(std::string kljuc, std::string vrijednost);
    void postavi_tijelo(std::string sadrzaj);
    std::string generiraj();
    int dohvati_status_kod();

private:
    int status_kod;            
    std::string status_opis;   
    std::string tijelo;        
    std::map<std::string, std::string> zaglavlja;
};

#endif
