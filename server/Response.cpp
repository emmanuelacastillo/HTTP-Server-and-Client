#include "HttpServer.hpp"
#include "Response.hpp"
#include "Request.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void Response::setStatus(STATUS status)
{
    Response::status = status;
}

STATUS Response::getStatus()
{
    return Response::status;
}

string Response::generateStatusLine()
{
    return Response::statusLine = "HTTP/1.0 " + this->getStatus().code + " " + this->getStatus().msg + REQ_DELIMITER;
}

void Response::addToHeaderLine(string line)
{
    Response::headerLines.append(line);
}

void Response::setContent(string contentPath)
{
    long fileSize = 0;
    ifstream fileToRead (contentPath.c_str());
    if(fileToRead.is_open())
    {
        /* Get file data */
        fileToRead.seekg(0, ios::end);
        fileSize = fileToRead.tellg();
        char *fileDataBuff = new char[fileSize];
        fileToRead.seekg(0, ios::beg);
        fileToRead.read(fileDataBuff, fileSize);
        fileToRead.close();
        this->content = string(fileDataBuff);
        delete fileDataBuff;
    }
    /* Set content type to be set on response header line */
    setContentType(contentPath);
}

string Response::getContent()
{
    return Response::content;
}

void Response::setContentType(string filePath)
{
    this->contentType = getContentType(filePath);
}

void Response::setHeaderLines()
{
    /* Set values for response header lines */
    time_t currentTime = time(0);
    stringstream timeStream;
    timeStream << ctime(&currentTime);
    string time = timeStream.str();
    size_t pos = time.find("\n");
    if(pos != string::npos) {
        time.erase(pos, time.size());
    }

    stringstream contentSizeStream;
    contentSizeStream << this->content.size();

    /* Set response header lines */
    this->addToHeaderLine("Date: " + time + REQ_DELIMITER);
    this->addToHeaderLine("Server: Customized" + REQ_DELIMITER);
    this->addToHeaderLine("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT" + REQ_DELIMITER);

    /* Only sets content length if file exist */
    if(!this->content.empty()) {
        this->addToHeaderLine("Content-Length: " + contentSizeStream.str() + REQ_DELIMITER);
    }
    this->addToHeaderLine("Content-Type: " + this->contentType + REQ_DELIMITER);
    this->addToHeaderLine("Connection: Closed" + REQ_DELIMITER);
    this->addToHeaderLine(REQ_DELIMITER);

    /* Clear stringstreams */
    timeStream.str(string());
    timeStream.clear();

    contentSizeStream.str(string());
    contentSizeStream.clear();
}

string Response::getResponseString()
{
    string response = "";
    response.append(this->generateStatusLine());

    if(!this->headerLines.empty()) {
        response.append(headerLines);
    }

    if(!this->content.empty()) {
        response.append(this->content);
    }

    return response;
}
