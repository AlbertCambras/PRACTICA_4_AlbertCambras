
## **PRÁCTICA 2: TAREAS**
Para esta práctica he hecho dos main, uno con el código proporcionado en la misma y en la cual explicare las funciones básicas para poder trabajar con tareas.
Primero de todo, las tareas son una forma de realizar múltiples cosas en el microcontrolador.  

### ¿qué sucede si está utilizando una pantalla de tinta electrónica que tarda unos segundos en actualizarse?
En ese caso si no usamos tareas, lo que pasaría es que si los datos que se reciben para imprimir en la pantalla vinieran de forma muy rápida, al final no se acabaría de ver completamente los datos en la pantalla, ya que cuando se fueran a imprimir los datos habría cambiado el valor y no se mostraría nunca.
En ese caso sería imprescindible usar tareas ya que podríamos modificar el valor de la variable una vez realizada la tarea de mostrar en la pantalla.

### **Ejercicio práctico 1: main.cpp**
Para este ejercicio, importamos el código proporcionado en la práctica.

Podríamos separar el código en tres partes:

### **void setup()**
```cs

void setup() {

    Serial.begin(9600);
    /* we create a new task here */
    xTaskCreate( anotherTask,"another Task", 10000,NULL,1,NULL); 
}
```
Aquí lo que hacemos es inicializar el puerto serie, en mi caso en 9600 bauds.
Creamos las tareas a utilizar mediante la función **xTaskCreate()**, en este caso creamos una llamada anotherTask y la llamaremos con la función anotherTask. El tamaño reservado para esta tarea es 10000. No le pasamos ningún valor, lo podemos declarar poniendo el primer NULL. 
La prioridad de la tarea es 1. Las prioridades van de 0 a 24, contra más pequeño el número, mayor prioridad le damos.
Las prioridades nos sirven cuando dos o más tareas compiten por el uso de la CPU, en ese caso se ejecuta primero la de mayor prioridad.
Si dos tareas tienen el mismo grado de prioridad, el tiempo de CPU se repartirá.  

### **void loop()**
```cs

void loop() {

    Serial.println("this is ESP32 Task");
    delay(1000);
}
```
Esta función típica del IDE de Arduino es una tarea llamada por el mismo código de forma automática. Hay formas de anularla como ya veremos más adelante, pero en este caso la usamos solamente para mostrar por el puerto serie una frase.

### **DECLARACIÓN DE LA TAREA**
```cs
void anotherTask( void * parameter )
{
/* loop forever */
for(;;)
{
Serial.println("this is another Task");
delay(1000);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}
```
La forma de declarar una tarea es la misma que el declarar una función de las que ya hemos visto y pasándoles por parámetro un puntero que en este caso se llama *parameter*.
Esta tarea que hemos hecho es muy básica y tiene algunos detalles a comentar:
Hacemos un bucle infinito simplemente para luego poder llamar la función **vTaskDelete** y de esa forma nunca se ejecutará.
vTaskDelete nos sirve para detener las tareas.

### **CONCLUSIONES**
En este código de ejemplo considero que no tiene mucho sentido usar tareas, pero para intentar entender estas nos sirve.
El bucle for infinito si tuvieramos diferentes tareas, en este caso no nos ejecutaría nunca estas, ya que no hay ningún indicio para poder ejecutarlas.
Hay otro ejemplo en el cuál en ve de usar delay(1000) se usa la función vTaskDelay() la cuál en el tiempo establecido en esta, parará el bucle for y usaría ese tiempo para controlar otras tareas. En ese caso si tendría sentido ese bucle for(;;).

