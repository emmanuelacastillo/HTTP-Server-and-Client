#include <string>
#include <stdlib.h>

using namespace std;

const string SERVER_BASE_PATH = string(getenv("PWD")) + "/";
const string WEBPAGE_REL_PATH = "webpages";
const string IMAGE_REL_PATH = "images";

string getFileRelativePath(string &fileName);
string getContentType(string &fileName);
