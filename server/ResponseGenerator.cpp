#include "HttpServer.hpp"
#include "ResponseGenerator.hpp"
#include "Request.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Response *GetResponseGenerator::generateResponse(Request *request)
{
    Response *response = new Response();
    /* Sets up response content */
    response->setContent(request->getPath());

    /*  Check whether content was successful set */
    if(!response->getContent().empty()) {
        response->setStatus(OK);
    }
    /* If body was not set successfully an error response is sent back */
    else {
        response->setStatus(INTERNALERROR);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/internalerror.html");
    }

    response->setHeaderLines();
    return response;
}

Response *PostResponseGenerator::generateResponse(Request *request)
{
    Response *response = new Response();

    if(!request->getBody().empty())
    {
        ofstream fileToWrite (request->getPath().c_str(), ios::out | ios::trunc);
        if(fileToWrite.is_open())
        {
            fileToWrite << request->getBody().c_str();
            fileToWrite.close();
            response->setStatus(OK);
            response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/fileposted.html");
        }
    }
    else {
        response->setStatus(BADREQUEST);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/badrequest.html");
    }

    response->setHeaderLines();
    return response;
}

Response *HeadResponseGenerator::generateResponse(Request *request)
{
    Response *response = new Response();

    /* Sets up response content information*/
    ifstream fileToRead (request->getPath().c_str());
    if(fileToRead.is_open())
    {
        /* Get content size */
        fileToRead.seekg(0, ios::end);
        long fileSize = fileToRead.tellg();

        /* Set content-length size */
        stringstream contentSizeStream;
        contentSizeStream << fileSize;
        response->addToHeaderLine("Content-Length: " + contentSizeStream.str() + REQ_DELIMITER);
        contentSizeStream.str(string());
        contentSizeStream.clear();

        response->setStatus(OK);
    }
    else {
        response->setStatus(INTERNALERROR);
        response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/internalerror.html");
    }

    response->setHeaderLines();
    return response;
}

Response *DefaultResponseGenerator::generateResponse(Request *request)
{
    Response *response = new Response();
    response->setStatus(NOTIMPLEMENTED);
    response->setContent(SERVER_BASE_PATH + WEBPAGE_REL_PATH + "/notimplemented.html");
    response->setHeaderLines();
    return response;
}

ResponseGenerator *getResponseGenerator(REQUESTTYPE generator)
{
    switch(generator)
    {
        case GET:
            return new GetResponseGenerator();
            break;
        case POST:
            return new PostResponseGenerator();
            break;
        case HEAD:
            return new HeadResponseGenerator();
            break;
        default:
            return new DefaultResponseGenerator();
            break;
    }
}
