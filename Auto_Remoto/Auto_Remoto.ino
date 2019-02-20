/* 
 *  Proyecto de vehiculo controlado por arduino.
 *  Escrito para nodeMCU LoLin.
 *  Escrito por WhitesoundCL (Luis Esteban Clavijo Fuentes).
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EEPROM.h>


const char *ssid = "Tu_SSID";
const char *password = "****";
const int PUERTO = 80;


Servo controladorMotor;
Servo servoDireccion;

ESP8266WebServer server(PUERTO);

int posicionDirTarget = 0;
int posicionDirActual = 0;
int impulso = 0;


// Este struct será almacenado en la memoria EEPROM posteriormente
struct {
    byte dirServoMin = 35;
    byte dirServoMax = 93;
}datos;

void setup() {  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  { 
    delay(100);
  }


  controladorMotor.attach(D3);
  servoDireccion.attach(D4);

  server.on("/controlar", manejarControlar);
  server.on("/configuracion", manejarConfiguracion);
  server.begin();

  EEPROM.begin(10);
  EEPROM.get(0, datos);
}

void loop() {
  server.handleClient();
  // La direción se actualiza de a poco hasta llegar al punto requerido.
  // Esto es así por que encontré que da mejores resultados con el servomotor.
  if (posicionDirActual > posicionDirTarget) {
    posicionDirActual--;
    servoDireccion.write(posicionDirActual);
  } else if ( posicionDirActual < posicionDirTarget) {
    posicionDirActual++;
    servoDireccion.write(posicionDirActual);
  } 

}

// Recibe las peticiones html para controlar los servomotores
void manejarControlar() {
  if (server.arg("dir") != "") {
    //servoDireccion.write(map(server.arg("dir").toInt(),0, 255, DIR_MIN, DIR_MAX));
    posicionDirTarget = map(server.arg("dir").toInt(), 0, 255, datos.dirServoMax, datos.dirServoMin);
  }
  if (server.arg("motor") != "") {
    controladorMotor.write(server.arg("motor").toInt());
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", String(posicionDirTarget));
}

void manejarConfiguracion() {
  bool modificado = false;
  if (server.arg("max_dir") != "") {
    datos.dirServoMax = server.arg("max_dir").toInt();
    modificado = true;
  }
  if (server.arg("min_dir") != "") {
    datos.dirServoMin = server.arg("min_dir").toInt();
    modificado = true;
  }

  if(modificado) {
    EEPROM.put(0,datos);
  }

  if(server.arg("guardar") == "si"){
    EEPROM.commit();
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "{\"min_dir\" : "+String(datos.dirServoMin)+", \"max_dir\": "+String(datos.dirServoMax)+"}");
}
