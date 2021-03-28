#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"  
#define ldrPin 36
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "4z4BnZAhmnQZWmctW69kZftjgawnJ_4c"; 
char ssid[] = "helloworld"; 
char pass[] = "admin123"; 

int sliderLux;
int sliderSuhu;
int ledPin = 5; 
int ledPin2 = 18; 
float t;
int tamp;
BlynkTimer timer;

BLYNK_WRITE(V4)
  {
  float valueLux = param.asInt(); 
  sliderLux=valueLux;
  }

  BLYNK_WRITE(V5)
  {
  int valueSuhu = param.asInt(); 
  sliderSuhu=valueSuhu;
  }
  
BLYNK_WRITE(V8)  
{
  int button = param.asInt();
  tamp=button;
}

void sensor()
{
  t = dht.readTemperature();
  int sensorCahaya = analogRead(ldrPin);
  int  ldr = ((0.009758 * sensorCahaya) + 10);
  int Suhu = t;

    
  Blynk.virtualWrite(V0, Suhu);
  Blynk.virtualWrite(V1, ldr);
  Blynk.virtualWrite(V2, ldr);
  Blynk.virtualWrite(V3, Suhu);

  
  if (isnan(t)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }
  
  Serial.println("Sistem Monitoring Pencahayaan dan Suhu Kandang Burung Walet");
  Serial.print("Temperature : "); 
  Serial.print(t); 
  Serial.println("*C\t");
  Serial.print("Kecerahan : ");
  Serial.print(ldr);
  Serial.println(" LUX\t");
  
 
  
  if(Suhu > sliderSuhu){
    Blynk.setProperty(V7,"color","#D3435C");
    digitalWrite(ledPin2, LOW);
  }else{
    Blynk.setProperty(V7,"color","#23C48E");
    digitalWrite(ledPin2, HIGH);
  }
     if(ldr > sliderLux){
    Blynk.setProperty(V6,"color","#D3435C");
    digitalWrite(ledPin, LOW);

  }else{
    Blynk.setProperty(V6,"color","#23C48E");
    digitalWrite(ledPin, HIGH);
  }
}


void setup()
{
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");  
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,43,114), 8080);
  dht.begin();
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  timer.setInterval(1000L, sensor);
}



  
void loop()
{
  Blynk.run();
  if(tamp == 1){
      timer.run();
    }else {
      Serial.println("System mati"); 
    }
}
