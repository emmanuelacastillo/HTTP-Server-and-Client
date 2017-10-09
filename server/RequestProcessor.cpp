#include "HttpServer.hpp"
#include "RequestProcessor.hpp"
#include "ResponseGenerator.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

RequestProcessor::RequestProcessor()
{
    nextProcessor = NULL;
}

Response *RequestProcessor::process(Request  *request)
{
    Response *response = this->handleRequest(request);

    if(response->getStatus().code != BAD_REQUEST_CD
        && response->getStatus().code != NOT_FOUND_CD
        && response->getStatus().code != VERSION_NOT_SUPPORTED_CD
        && nextProcessor != NULL)
    {
        return this->nextProcessor->process(request);
    }

    return response;
}

void RequestProcessor::setNextProcessor(RequestProcessor *nextProcessor)
{
    RequestProcessor::nextProcessor = nextProcessor;
}

inline bool isValuesExist(Request *request)
{
    return !request->getPath().empty() && !request->getHttp().empty();
}

inline bool isPathExist(const string& path)
{
    return (access( path.c_str(), F_OK ) != -1);
}

inline bool isValidHttpVersion(const string& httpVersion)
{
    return (httpVersion.compare("HTTP/1.0") == 0) || (httpVersion.compare("HTTP/1.1") == 0);
}


Response *ValidateRequestProcessor::handleRequest(Request *request)
{
    Response *response = new Response();

    if(!isValuesExist(request))
    {
        response->setStatus(BADREQUEST);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/badrequest.html");
    }
    else if(!isPathExist(request->getPath()))
    {
        response->setStatus(NOTFOUND);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH +  "/notfound.html");
    }
    else if(!isValidHttpVersion(request->getHttp()))
    {
        response->setStatus(VERNOTSUPPORTED);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH +  "/versionnotsupported.html");
    }

    response->setHeaderLines();
    return response;
}

Response *ProcessRequestProcessor::handleRequest(Request *request)
{
    ResponseGenerator *responseGenerator = getResponseGenerator(request->getRequestType());
    Response *response = responseGenerator->generateResponse(request);
    delete responseGenerator;
    return response;
}
