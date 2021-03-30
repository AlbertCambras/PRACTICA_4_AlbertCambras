## **PRÁCTICA 4: TAREAS**

### **EJERCICIO PRÁCTICO 2**
En este ejercicio he usado los conocimientos sobre tareas que he podido obtener mediante el ejercicio 1 y las páginas web mencionadas en la práctica para el uso de semáforos. El semáforo básicamente se encarga de apuntar a una tarea u otra según la prioridad y el uso que vaya a hacer de la CPU.
También he usado un trozo de código que usamos en la anterior práctica sobre el módulo wifi de la placa.

### **void setup()**
```cs

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

  xTaskCreatePinnedToCore(Tarea1,"Tarea1",10000,NULL,0,NULL,0);
  xTaskCreatePinnedToCore(Tarea2,"Tarea2",10000,NULL,0,NULL,1);

/**Hi ha una funció que va incrementant el semàfor, llavors, aquest bucle està fet per a que el semàfor reculli totes les tasques a executar. **/ 
    for(int i= 0; i<2; i++){ //Puesto que habrá las tareas que incrementan el semáforo, debemos garantizar que la función de configuración arduino sólo terminará después de que finalicen todas las tareas.
        xSemaphoreTake(barrierSemaphore, portMAX_DELAY); 
    }

}

```

Explicaré las partes de las tareas ya que todo lo otro lo vimos en la anterior práctica.
Primero establece conexión con el wifi de casa y crea un servidor local.
Una vez hecho esto, creo las tareas con la función vTaskCreate.
Las dos tareas tendrán la misma prioridad: *1*
Una tarea se ejecutará en un procesador y la otra en otro. Lo he hecho para también probar más metodologías del **FreeRTOS**.
Luego se crea un bucle desde 0 hasta el número total de tareas, en este caso 2 el cual sirve para que el semáforo sepa todas las tareas que tendrá que ir ejecutando.

### **TAREAS**

### **Tarea 1**
```cs

void Tarea1( void * parameter )
{

    for(;;) {
        Serial.println("this is another Task");
        delay(1000);
          // HTML & CSS contents which display on web server
          HTML = "<!DOCTYPE html>\
          <html>\
          <body>\
          <h1>PAGINA ESP32 DE ALBERT CAMBRAS </h1>\
          <h2> </h2>\
          </body>\
          </html>"+HTML;
          delay(100)
          xSemaphoreGive(barrierSemaphore); //El semàfor 'incrementa
      }

    }
    vTaskDelete(NULL);

}
```
Esta tarea se encarga de guardar en la variable string HTML el mensaje a imprimir en el servidor web.
Lo que va a hacer cada vez que entre en esta tarea guardar la frase *PAGINA ESP32 DE ALBERT CAMBRAS* en la variable HTML, por lo que cada vez que atualizemos la web nos va a salir esa frase repetida x veces.
La variable HTML la declaro arriba como variable global y así si se modifica no hará falta pasarla como parámetro de la tarea.
Una vez guardada la información en la variable, el semáforo se incrementa con la función *xSemaphoreGive(barrierSemaphore)*.
Todo esto está dentro de un bucle infinito para que nunca se borre la Tarea.

A la vez que se hace esta tarea, la CPU aprovechará para ir haciendo la tarea siguiente, *Tarea2*.

### **Tarea 2**

```cs
  void Tarea2(void * parameter){    
    while(1){
// Handle root url (/)
    handle_root();
      xSemaphoreGive(barrierSemaphore); //El semàfor 'incrementa
    }
    vTaskDelete(NULL);

  }

```
Esta tarea básicamente de lo que se encarga es de llamar a la función handle_root() que explicado por encima, se encarga de enviar la información de la variable HTML al servidor.
Una vez hecho esto, llama también al *xSemaphoreGive(barrierSemaphore);* y pasará a bhacer la anterior tarea otra vez.

## **CONCLUSIONES**  

El programa no funciona tan bien como yo quería debido a que cuando se actualiza la página web, aparecen errores de reseteo del servidor cosa que he intentado ir modificando pero no he conseguido arreglarlo.
Probé de hacer una nueva tarea que se encargara de crear la conexión Wifi y el servidor. De esta forma pretendía hacer que se hiciera una sola vez esa operación ya que una vez hecho eso borraba la tarea.
De esta forma una vez reiniciada la web, no debería haber ninguna falla con respecto eso.
Pero por lo contrario, cuando intentaba acceder a la web mediante la IP no la reconocía. Supuse que sería debido a que una vez borrada la tarea, no quedaba ninguna información sobre el servidor y lo borraba.
Remediando eso, el programa depende el momento que actualizes la página si que muestra sin problemas la frase múltiples veces.  

Salvo eso, me gustó mucho el poder ejecutar las dos tareas a la vez y me pareció muy interesante el uso de estas. Estoy haciendo un proyecto desde hace varios meses con arduino y alguna vez me había planteado el como hacer multitareas a la vez para hacer cálculos a la vez que movía unos motores y no tener que esperar ese tiempo de procesado para su movimiento.
