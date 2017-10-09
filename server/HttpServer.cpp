#include "HttpServer.hpp"
#include <string>

using namespace std;

string getFileRelativePath(string &fileName)
{
    size_t pos = fileName.find(".");
    string fileExtension = fileName.substr(pos, fileName.size());

    if(fileExtension.compare(".html") == 0 || fileName.compare("/") == 0) {
        return WEBPAGE_REL_PATH;
    }
    else if(fileExtension.compare(".jpg") == 0) {
        return IMAGE_REL_PATH;
    }
    else {
        return "";
    }
}

string getContentType(string &fileName)
{
    size_t pos = fileName.find(".");
    string fileExtension = fileName.substr(pos, fileName.size());

    if(fileExtension.compare(".html") == 0) {
        return "text/html";
    }
    else if(fileExtension.compare(".jpg") == 0) {
        return "image/jpeg";
    }
    else {
        return "";
    }
}
