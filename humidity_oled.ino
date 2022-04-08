#include <dht.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(-1);

#define dht_apin A3 // Analog Pin sensor is connected to
 
dht DHT;
 
void setup(){
 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);          // text size
  display.setTextColor(WHITE);     // text color
  display.setTextWrap(false);
  //x = display.width();
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
    display.clearDisplay();
    display.setTextSize(0.7);
    display.setCursor(0,0);
    display.print("Temp = ");
    display.print(int(DHT.temperature));
    display.print(" C");
    display.setCursor(0,13);
    display.print("Humidity = ");
    display.print(int(DHT.humidity));
    display.print(" %");
    display.display();
 
  //Fastest should be once every two seconds.
 
}// end loop(
