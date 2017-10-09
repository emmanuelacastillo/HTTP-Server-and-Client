#ifndef REQUESTPROCESSOR_HPP
#define REQUESTPROCESSOR_HPP

#include "Request.hpp"
#include "Response.hpp"

class RequestProcessor
{
    private:
        RequestProcessor *nextProcessor;

    protected:
        virtual Response *handleRequest(Request *request) = 0;

    public:
        RequestProcessor();
        Response *process(Request  *request);
        void setNextProcessor(RequestProcessor *nextProcessor);
        virtual ~RequestProcessor() {};
};

class ValidateRequestProcessor : public RequestProcessor
{
    public:
        Response *handleRequest(Request *request);
};

class ProcessRequestProcessor : public RequestProcessor
{
    public:
        Response *handleRequest(Request *request);
};

#endif // REQUESTPROCESSOR_HPP
