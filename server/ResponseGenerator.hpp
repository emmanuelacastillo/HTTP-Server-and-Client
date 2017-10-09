#ifndef RESPONSEGENERATOR_HPP
#define RESPONSEGENERATOR_HPP

#include "Request.hpp"
#include "Response.hpp"

class ResponseGenerator
{
    public:
        virtual Response *generateResponse(Request *request) = 0;
        virtual ~ResponseGenerator() {};
};

class GetResponseGenerator : public ResponseGenerator
{
    public:
        Response *generateResponse(Request *request);
};

class PostResponseGenerator : public ResponseGenerator
{
    public:
        Response *generateResponse(Request *request);
};

class HeadResponseGenerator : public ResponseGenerator
{
    public:
        Response *generateResponse(Request *request);
};

class DefaultResponseGenerator : public ResponseGenerator
{
    public:
        Response *generateResponse(Request *request);
};

ResponseGenerator *getResponseGenerator(REQUESTTYPE generator);

#endif // RESPONSEGENERATOR_HPP
