#include "HttpServer.hpp"
#include "Request.hpp"
#include <cstdlib>
#include <cstring>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

inline string translatePath(string path)
{
    if(path.compare("/") == 0) {
        return "/index.html";
    }

    return path;
}

inline string translateHttp(string http)
{
    if(http.empty()) {
        return "HTTP/1.0";
    }

    return http;
}

Request::Request(char *buffer)
{
    queue<string> requestLines;
    string requestStream(buffer);
    size_t pos = 0;
    string requestSubStr;

    /* Create a queue of each line of the request based on the delimited (\r\n) */
    while(requestStream.size() > 0)
    {
        pos = requestStream.find(REQ_DELIMITER);

        if(pos == string::npos) {
            requestLines.push(requestStream);
            requestStream.clear();
            break;
        }

        requestSubStr = requestStream.substr(0, pos);
        requestLines.push(requestSubStr);
        requestStream.erase(0, pos + REQ_DELIMITER.length());
    }

    if(requestLines.size() == 0)
        return;

    string requestLine = requestLines.front();
    requestLines.pop();

    /* Extract the request type within the first line, which is the request line of the HTTP protocol */
    /* The first string seperated by a space is expected to be the request type. */
    pos = requestLine.find(REQ_LINE_DELIMITER);

    if(pos == string::npos) {
        return;
    }

    requestSubStr = requestLine.substr(0, pos);
    this->requestType = this->extractRequestType(requestSubStr);
    requestLine.erase(0, pos + REQ_LINE_DELIMITER.length());

    /* Extract the requested path that is expected to be the second string within the request line. */
    pos = requestLine.find(REQ_LINE_DELIMITER);

    /* Set defaults if nothing else found */
    if(pos == string::npos) {
        this->path = SERVER_BASE_PATH + getFileRelativePath(requestLine) + translatePath(requestLine);
        this->http = translateHttp("");
        return;
    }

    requestSubStr = translatePath(requestLine.substr(0, pos));
    this->path = SERVER_BASE_PATH + getFileRelativePath(requestSubStr) + requestSubStr;
    requestLine.erase(0, pos + REQ_LINE_DELIMITER.length());

    /* Extract the http version that is expected to be the third (remaining) string within the request line. */
    this->http = translateHttp(requestLine);
    requestLine.clear();

    /* Extract headers */
    int requestLineSize = requestLines.size();
    for(int count = 0; count < requestLineSize; count++)
    {
        requestLine = requestLines.front();

        /* headers end after an empty line */
        if(requestLine.empty()) {
            requestLines.pop();
            break;
        }

        this->headers.push_back(requestLine);
        requestLines.pop();
    }

    /* Extract body if exist */
    string requestBody = "";
    for(int count = 0; count < requestLines.size(); count++)
    {
        requestBody += requestLines.front();
        requestLines.pop();
    }
    this->body = requestBody;
}

REQUESTTYPE Request::extractRequestType(string method)
{
    if(method.compare("GET") == 0)
        return GET;
    else if(method.compare("POST") == 0)
        return POST;
    else if(method.compare("HEAD") == 0)
        return HEAD;

    return ERROR;
}

REQUESTTYPE Request::getRequestType()
{
    return Request::requestType;
}

string Request::getPath()
{
    return Request::path;
}

string Request::getHttp()
{
    return Request::http;
}

vector<string> Request::getHeaders()
{
    return Request::headers;
}

string Request::getBody()
{
    return Request::body;
}
