#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <WiFi.h>

class WebServer
{
private:
    WiFiServer* serverPointer;
public:
    WebServer();
    WebServer(WiFiServer* serverPointer);
    ~WebServer();
    
    void serverLoop();
    void setServerPointer(WiFiServer* serverPointer);
};




#endif