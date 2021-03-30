#include <Arduino.h>

int nTasks=2;
char Str2[8] = {'a', 'r', 'd', 'u', 'i', 'n', 'o'};
String HTML;
#include <WiFi.h>
#include <WebServer.h>
// SSID & Password
const char* ssid = "MOVISTAR_AB50"; // Enter your SSID here
const char* password = "935888966"; //Enter your Password here
WebServer server(80); // Object of WebServer(HTTP port, 80 is defult)

// Handle root url (/)
void handle_root(void);

SemaphoreHandle_t barrierSemaphore = xSemaphoreCreateCounting( nTasks, 0 );

void Tarea1( void * parameter ){

    for(;;) {       
      // HTML & CSS contents which display on web server
      HTML = "<!DOCTYPE html>\
      <html>\
      <body>\
      <h1>PAGINA ESP32 DE ALBERT CAMBRAS </h1>\
      <h2> </h2>\
      </body>\
      </html>"+HTML;

      delay(100);
      xSemaphoreGive(barrierSemaphore); //El semàfor 'incrementa
    }
    vTaskDelete(NULL);

}

  void Tarea2(void * parameter){    
    while(1){
      Serial.println(HTML);
// Handle root url (/)
    handle_root();
      xSemaphoreGive(barrierSemaphore); //El semàfor 'incrementa
    }
    vTaskDelete(NULL);

  }


void setup() {

    Serial.begin(9600);
    Serial.println("Try Connecting to ");
    Serial.println(ssid);

    // Connect to your wi-fi modem
    WiFi.begin(ssid, password);

    // Check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP()); //Show ESP32 IP on serial
    server.on("/", handle_root);
    server.begin();
    Serial.println("HTTP server started");
    delay(100);

  delay(1000);

  xTaskCreatePinnedToCore(Tarea1,"Tarea1",10000,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(Tarea2,"Tarea2",10000,NULL,1,NULL,1);

/**Hi ha una funció que va incrementant el semàfor, llavors, aquest bucle està fet per a que el semàfor reculli totes les tasques a executar. **/ 
    for(int i= 0; i<2; i++){ //Puesto que habrá las tareas que incrementan el semáforo, debemos garantizar que la función de configuración arduino sólo terminará después de que finalicen todas las tareas.
        xSemaphoreTake(barrierSemaphore, portMAX_DELAY); 
    }

}

void loop() {
server.handleClient();
}


void handle_root(){

    server.send(200, "text/html", HTML);
    }