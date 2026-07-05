# web-server-from-scratch

HTTP server za statične datoteke, pisan od nule u C++-u preko POSIX socketa,
bez vanjskih biblioteka. Nastao kao završni projekt na kolegiju Objektno
orijentirano programiranje (Veleučilište u Bjelovaru).

Server sluša na zadanom portu, za svakog klijenta forka novi proces, parsira
HTTP zahtjev i vraća traženu datoteku iz `www/` mape s ispravnim MIME tipom,
odnosno 404 stranicu ako datoteka ne postoji. Ispis u konzolu ide kroz vlastiti
logger s vremenom i bojama po razini poruke.

## Pokretanje

```
make
./web_server 8080
```

pa u pregledniku otvoriti http://localhost:8080.

## Struktura

- `Server` — socket/bind/listen/accept petlja, fork po klijentu
- `HttpRequest` / `HttpResponse` — parsiranje zahtjeva i slaganje odgovora
- `StaticFileHandler` — čitanje datoteka, MIME tipovi, 404 stranica, zaštita od path traversala
- `Logger` — ispis s vremenom i bojama

Ograničenja: podržan je samo GET, nema HTTPS-a ni keep-alive veza.
