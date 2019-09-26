#include <Arduino.h>
#include <WiFi.h>

#include "settings.h"
#include "credentials.h"
#include "WebServer.h"

void serverLoop();
void software_Reset();
void altLoop(void * parameter);

WiFiServer server;
TaskHandle_t taskAltLoop;
WebServer webServer;


void setup() {
	Serial.begin(115200);

	Serial.print("setup() running on core ");
	Serial.println(xPortGetCoreID());

	xTaskCreatePinnedToCore(
    	altLoop, /* Function to implement the task */
    	"altLoop", /* Name of the task */
    	10000,  /* Stack size in words */
    	NULL,  /* Task input parameter */
    	0,  /* Priority of the task */
    	&taskAltLoop,  /* Task handle. */
    	0); /* Core where the task should run */


		WiFi.begin(mySSID, myPASSWORD);

    int wifi_loops=0;
	int wifi_timeout = WIFI_TIMEOUT_DEF;

    while (WiFi.status() != WL_CONNECTED) {
		wifi_loops++;
		Serial.print(".");
		delay(500);
		
		if (wifi_loops>wifi_timeout){
			software_Reset();
		}
	}

	server.begin();
	webServer.setServerPointer(&server);
}




void loop() {
    Serial.print("loop() running on core ");
	Serial.println(xPortGetCoreID());

	delay(1000);
}




void altLoop(void * parameter){
	while(1){

		Serial.print("altLoop() running on core ");
		Serial.println(xPortGetCoreID());
		
		delay(1100);

		webServer.serverLoop();
		// serverLoop();
	}

	vTaskDelete(taskAltLoop);
}



void software_Reset(){ // Restarts program from beginning but does not reset the peripherals and registers

	Serial.print("resetting");
	esp_restart(); 
}



void serverLoop(){

	    while(1){
        
        if (WiFi.status() != WL_CONNECTED){
            software_Reset();
        }

        WiFiClient client = server.available();
        // Serial.println(client.);
        if (client) {
            
            // an http request ends with a blank line
            bool currentLineIsBlank = true;
            while (client.connected()) {
                
                // int cpmNo = this->count->getQueueUtilisation();
                
                if (client.available()) {
                    char c = client.read();
                    // Serial.write(c);
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