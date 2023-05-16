#include <DHT.h>
#define DHT11PIN 2
#define DHTTYPE DHT11

DHT dht(DHT11PIN, DHTTYPE);
float h,tc,tf;

void setup()
{
  delay(200);
  Serial.begin(9600)
  dht.begin();
  delay(1000);
  Serial.println("DHT11 Temperature and Humidity ");
}

void loop()
{
  delay(5000);
  h = dht.readHumidity();
  tc = dht.readTemperature();
  tf = dht.readTemperature(true);
  
  Serial.print('\n');
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.print("%,  ");
  Serial.print("Temperature = ");
  Serial.print(tc);
  Serial.print("°C, ");
  Serial.print(tf);
  Serial.println("°F");
}