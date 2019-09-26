#include "WebServer.h"

WebServer::WebServer(){

}

WebServer::WebServer(WiFiServer* serverPointer){
    this->serverPointer = serverPointer;
}

WebServer::~WebServer(){
}

void WebServer::setServerPointer(WiFiServer* serverPointer){
    this->serverPointer = serverPointer;
}

void WebServer::serverLoop(){

	    while(1){

        WiFiClient client = this->serverPointer->available();
        if (client) {
            
            // an http request ends with a blank line
            bool currentLineIsBlank = true;
            while (client.connected()) {
                
                // int cpmNo = this->count->getQueueUtilisation();
                
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    // if you've gotten to the end of the line (received a newline
                    // character) and the line is blank, the http request has ended,
                    // so you can send a reply
                    if (c == '\n' && currentLineIsBlank) {
                        // send a standard http response header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close");  // the connection will be closed after completion of the response
                        client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");

                        for(int i = 0; i < 30; i++){
                            client.println(millis());
                            client.println("<br />");
                        }

                        client.println("</html>");
                        break;
                    }
                    if (c == '\n') {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    } else if (c != '\r') {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
            // give the web browser time to receive the data
            delay(1);

            // close the connection:
            client.stop();
            Serial.println("client disonnected");
        }

    }

}