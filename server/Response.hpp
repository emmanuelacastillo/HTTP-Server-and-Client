#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <queue>

using namespace std;

struct STATUS
{
    string code;
    string msg;
};

const static string BAD_REQUEST_CD = "400";
const static string NOT_FOUND_CD = "404";
const static string VERSION_NOT_SUPPORTED_CD = "505";

const static STATUS OK = { "200", "OK" };
const static STATUS MOVEDPERM = { "301", "Moved Permanently" };
const static STATUS BADREQUEST = { BAD_REQUEST_CD, "Bad Request" };
const static STATUS NOTFOUND = { NOT_FOUND_CD, "Not Found" };
const static STATUS INTERNALERROR = { "500", "Internal Server Error" };
const static STATUS NOTIMPLEMENTED = { "501", "Not Implemented" };
const static STATUS VERNOTSUPPORTED = { VERSION_NOT_SUPPORTED_CD, "HTTP Version Not Supported" };


class Response
{
    private:
        STATUS status;
        string statusLine;
        string headerLines;
        string content;
        string contentType;
        string generateStatusLine();
        void setContentType(string filePath);
    public:
        void setStatus(STATUS status);
        STATUS getStatus();
        void setContent(string contentPath);
        string getContent();
        void addToHeaderLine(string line);
        void setHeaderLines();
        string getResponseString();
};

#endif // RESPONSE_HPP
