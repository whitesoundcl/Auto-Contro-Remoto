/* 
 *  Proyecto de vehiculo controlado por arduino.
 *  Escrito para nodeMCU LoLin.
 *  Código con manejo de pantalla OLED incluida.
 *  Escrito por WhitesoundCL (Luis Esteban Clavijo Fuentes).
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8x8lib.h>
#include <Servo.h>
#include <EEPROM.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

const char *ssid = "Tu_SSID";
const char *password = "****";
const int PUERTO = 80;

// Constructor de la pantalla
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);

// Para que la pantalla actue como un terminal.
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];
U8X8LOG u8x8log;

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
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  u8x8log.print("Wifi:");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }
  u8x8log.println("OK");
  u8x8log.println("Direc. IP:");
  u8x8log.println(WiFi.localIP());

  controladorMotor.attach(D3);
  servoDireccion.attach(D4);

  server.on("/controlar", manejarControlar);
  server.on("/configuracion", manejarConfiguracion);
  server.begin();
  u8x8log.println("Server iniciado");

  EEPROM.begin(10);
  EEPROM.get(0, datos);
  u8x8log.println("EEPROM MIN:"+ String(datos.dirServoMin));
  u8x8log.println("EEPROM MAX:"+ String(datos.dirServoMax));
  
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
    u8x8log.println("max_dir:" + String(datos.dirServoMax));
    modificado = true;
  }
  if (server.arg("min_dir") != "") {
    datos.dirServoMin = server.arg("min_dir").toInt();
    u8x8log.println("min_dir:" + String(datos.dirServoMin));
    modificado = true;
  }

  if(modificado) {
    EEPROM.put(0,datos);
  }

  if(server.arg("guardar") == "si"){
    EEPROM.commit();
    u8x8log.println("conf. guard.");
  }
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "{\"min_dir\" : "+String(datos.dirServoMin)+", \"max_dir\": "+String(datos.dirServoMax)+"}");
}
