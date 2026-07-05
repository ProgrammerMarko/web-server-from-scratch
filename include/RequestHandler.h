#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"

class RequestHandler {
public:
    virtual ~RequestHandler() {
    }
    virtual HttpResponse handle(HttpRequest zahtjev) = 0;
};

#endif
