#include "server.h"

///////////////////////////////////////////////////////////////
/// HTTP 服务器
///////////////////////////////////////////////////////////////

HttpServer* HttpServer::instance = nullptr;

HttpServer *HttpServer::getInstance()
{
    if(instance == nullptr)
    {
        instance = new HttpServer();
    }
    return instance;
}
