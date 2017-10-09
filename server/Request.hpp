#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>

using namespace std;

const string REQ_DELIMITER = "\r\n";
const string REQ_LINE_DELIMITER = " ";

enum REQUESTTYPE { ERROR, GET, POST, HEAD };

class Request
{
    private:
        REQUESTTYPE requestType;
        string path;
        string http;
        vector<string> headers;
        string body;
        REQUESTTYPE extractRequestType(string method);

    public:
        Request(char *buffer);
        REQUESTTYPE getRequestType();
        string getPath();
        string getHttp();
        vector<string> getHeaders();
        string getBody();
};

#endif // REQUEST_HPP
