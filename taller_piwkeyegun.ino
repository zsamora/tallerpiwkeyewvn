// Librerias
#include <ESP8266WiFi.h>
#include <dht.h>
#include <BlynkSimpleEsp8266.h>

// Constantes
const int AirValue = 705;    // Sensor capacitivo en el aire
const int WaterValue = 285;  // Sensor capacitivo en agua

// Sensores y pines utilizados
#define DHT_pin D0
dht DHT;
const int moisture_pin = A0;

// Credenciales Blynk
#define BLYNK_PRINT Serial
char auth[] = "BLYNK_TOKEN"; // Auth token recibido en el mail
const char* ssid = "SSID_WIFI";                 // SSID (Red Wifi)
const char* password = "PASSWORD_WIFI";            // Password
BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  // Establecer el pin del sensor como entrada
  pinMode(moisture_pin, INPUT);  
  // Conexión a servidor Blynk
  Blynk.begin(auth, ssid, password);
  timer.setInterval(5000L, myTimerEvent);
}
void loop() {
  Blynk.run();
  timer.run();
}

void myTimerEvent()
{
  // Leer valores de los sensores y guardarlos
  DHT.read11(DHT_pin);
  float ta = readEnvTemperature();
  float ha = readEnvHumidity();
  float hs = readMoisture();
  Serial.println("");
  // Envío de datos
  Blynk.virtualWrite(V0, ta); // Temperatura ambiente
  Blynk.virtualWrite(V1, hs); // Humedad sustrato
  Blynk.virtualWrite(V2, ha); // Humedad ambiente
}

float readEnvTemperature() {
  float ta = DHT.getTemperature();
  Serial.print("Temperatura ambiente: ");
  Serial.print(ta);
  Serial.println(" °C");
  return ta;
}

float readEnvHumidity() {
  float ha = DHT.getHumidity();
  Serial.print("Humedad ambiente: ");
  Serial.print(ha);
  Serial.println(" %");
  return ha;
}

float readMoisture() {
  int moisture = analogRead(moisture_pin);
  float moisturePercent = map(moisture, AirValue, WaterValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0.0, 100.0);
  Serial.print("Humedad sustrato: ");
  Serial.print(moisturePercent);
  Serial.print(" %");
  Serial.print(" (");
  Serial.print(moisture); // Calibrar sensor de humedad
  Serial.println(")");
  return moisturePercent;
}
