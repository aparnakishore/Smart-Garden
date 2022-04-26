//humidity and oled includes and definition
  #include <DHT.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <ArduinoJson.h>
  Adafruit_SSD1306 display(-1);
  #define DHTPIN 3 // digital Pin 3, humidity sensor
  #define DHTTYPE DHT11
  DHT dht(DHTPIN,DHTTYPE);
  StaticJsonDocument<200> doc;
  StaticJsonDocument<200> out;
  StaticJsonDocument<200> notify;
  boolean manual = false;
  const unsigned long eventInterval = 3600000;
  unsigned long previousTime = 0;

//moisture and water pump includes and definition
//  const int AirValue = 620;   //you need to replace this value with Value_1
//  const int WaterValue = 310;  //you need to replace this value with Value_2
//  String plant1 = ""
//  String plant2 = ""
  int soilMoistureValue0 = 0;
  int soilMoistureValue1 = 0;
  int waterLevelInput = 0;
  int output_value0;
  int output_value1;
//  int soilmoisturepercent0=0;
//  int soilmoisturepercent1=0;
  int relayPin0 = 53;
  int relayPin1 = 52;
  int soil0 = A0;
  int soil1 = A1;
  int waterPin = A6;
  String message = "{" + (String)"\"action\": \"NOTIFY\",";

//define LED output and light sensor inputs
//LEDS
  int p1LED1 = 7;
  int p1LED2 = 6;
  int p2LED1 = 5;
  int p2LED2 = 4;
//Light sensors
  int light1Pin = A4;
  int light2Pin = A5;
  
//brightness levels dark = 0, low = 1, med = 2, high = 3
  int goalLVL1 = 3; //what we want for the plants
  int goalLVL2 = 3;
  int currentLVL1 = 0; //what we're at
  int currentLVL2 = 0;
//define brightness ranges (these we just kinda need to map 
  int medMax = 720;
  int lowMax = 550;
  int lowMin = 350;
//define moisture ranges (these we just kinda need to map 
  int MedMax = 549;
  int MedMin = 699;
  

void setup() {
  Serial.begin(9600);
//OLED setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(2);          // text size
  display.setTextColor(WHITE);     // text color
  display.setTextWrap(false);
//define LED pin modes to outputs
  pinMode(p1LED1, OUTPUT);
  pinMode(p1LED2, OUTPUT);
  pinMode(p2LED1, OUTPUT);
  pinMode(p2LED2, OUTPUT);
//define moisture and pump pin modes
  pinMode(relayPin0,OUTPUT);
  pinMode(relayPin1,OUTPUT);
  pinMode(soilMoistureValue0,INPUT);
  pinMode(soilMoistureValue1,INPUT);
  pinMode(waterLevelInput,INPUT);
//delay
  delay(2000);
}

void loop() {
  unsigned long currentTime;
  digitalWrite(relayPin0, HIGH);
  digitalWrite(relayPin1, HIGH);
//current brightness values
  int current1 = analogRead(light1Pin);
  int current2 = analogRead(light2Pin);
//current humidity
  float h = dht.readHumidity();
  String HUMID_LEVEL = getHumidity(h);
//current temperature as Celsius
  float t = dht.readTemperature();
  String TEMP_LEVEL = getTemp(t);
// if we want to Read temperature as Fahrenheit (isFahrenheit = true)
//  float f = dht.readTemperature(true);

//  Serial.print(current1);
//  Serial.print("  ");
//  Serial.println(current2);

//set current level for plant 1
  int LVL1 = setCurrentLevel(current1); //set these two with brightness sensor
  int LVL2 = setCurrentLevel(current2); 
  String lvl1 = "not valid";
  String lvl2 = "not valid";
  lvl1 = getLight(LVL1);
  lvl2 = getLight(LVL2);
//  Serial.print(LVL1);
//  Serial.print("  ");
//  Serial.println(LVL2);

  //DHT.read11(dht_apin);
    
//  Serial.print("Current humidity = ");
//  Serial.print(h);
//  Serial.print("%  ");
//  Serial.print("temperature = ");
//  Serial.print(t); 
//  Serial.println("C  ");

//set moisture and activate pumps
  soilMoistureValue0 = analogRead(soil0);  //put Sensor insert into soil
  soilMoistureValue1 = analogRead(soil1);  //put Sensor insert into soil
  
  String MOISTURE_LEVEL0   = getMositure(soilMoistureValue0);
  String MOISTURE_LEVEL1   = getMositure(soilMoistureValue1);

//  soilmoisturepercent0 = map(soilMoistureValue0, 660, 390, 0, 100);
//  soilmoisturepercent1 = map(soilMoistureValue1, 660, 390, 0, 100); 

//  Serial.println((String)"soilMoistureValue0 - " + soilMoistureValue0); 
//  Serial.println((String)"soilMoistureValue1 - " + soilMoistureValue1); 
//  Serial.println((String)"soilmoisturepercent0 - " + soilmoisturepercent0); 
//  Serial.println((String)"soilmoisturepercent1 - " + soilmoisturepercent1); 
  
  waterLevelInput = analogRead(waterPin); //water level sensor
  if(waterLevelInput < 25){
//    String message1 = message + "\"message\": \"Fill your tank!\"}";
    notify["action"] = "NOTIFY";
    notify["type"] = "WATER_ALERT";
    char notifyStr[100];
    serializeJson(notify,notifyStr);
    Serial.println(notifyStr);
  }

   if (Serial.available() > 0)
   {
    String receivedString = (String)Serial.readStringUntil('\n');
    DeserializationError error = deserializeJson(doc, receivedString);
    if (error)
    {
      return;
    }
   
    const char *action = doc["action"];
    if (strcmp(action, "WATER") == 0 ){
        int rackId = doc["rackId"];
//        Serial.println("before if");
        if (rackId== 0){
//          Serial.println("in 0 if");
          digitalWrite(relayPin0, LOW);
          delay(5000);
          digitalWrite(relayPin0, HIGH);
          }
        if (rackId==1){
          digitalWrite(relayPin1, LOW);
          delay(5000);
          digitalWrite(relayPin1, HIGH);
        }
      }
   
    else if (strcmp(action, "LIGHT") == 0)
    {
      const char *state = doc["state"];
      int rackId = doc["rackId"];
      Serial.println("before if");

      if (rackId== 0){
        Serial.println("rack 0");

        if (strcmp(state, "ON") == 0)
        {
          Serial.println("inside on");
         // ldrMode = true;
         int on0 = turnOnLights(p1LED1, p1LED2, goalLVL1, LVL1);        
         currentTime = millis();
         manual = true;
         if (currentTime - previousTime >= eventInterval) {
           manual = false;
           previousTime = currentTime;
        }
        }
        else
        {
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          currentTime = millis();
          manual = true; 
          if (currentTime - previousTime >= eventInterval) {
            manual = false;
            previousTime = currentTime;
        //  ldrMode = false;
          }
        }
    }
    else if (rackId==1){
      Serial.println(state);
        if (strcmp(state, "ON") == 0)
        {
          Serial.println("inside on");
         // ldrMode = true;
         int on1 = turnOnLights(p1LED1, p1LED2, goalLVL2, LVL2);        
         currentTime = millis();
         manual = true;
         if (currentTime - previousTime >= eventInterval) {
           manual = false;
           previousTime = currentTime;
        }
        }
        else
        {
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);
          currentTime = millis();
          manual = true; 
          if (currentTime - previousTime >= eventInterval) {
            manual = false;
            previousTime = currentTime;
        //  ldrMode = false;
          }
        }
    }
  }
  else if (strcmp(action, "PLANT_INFO") == 0)
  {
    
    int rackId = doc["rackId"];
    if (rackId== 0)
    {
      //moisture, temp,humidity, light
//
      out["action"] = "NOTIFY";
      out["type"] = "PLANT_INFO";
      JsonObject data = out.createNestedObject("data");
      data["moisture"] = MOISTURE_LEVEL0;
      data["temperature"] = TEMP_LEVEL;
      data["humidity"] = HUMID_LEVEL;
      data["rackId"] = 0;
      data["light"] = lvl1;
      char message[500];
      serializeJson(out,message);
            Serial.println(message);
    }
    else if (rackId == 1)
    {
            //moisture, temp,humidity, light
      out["action"] = "NOTIFY";
      out["type"] = "PLANT_INFO";
      JsonObject data = out.createNestedObject("data");
      data["moisture"] = MOISTURE_LEVEL1;
      data["temperature"] = TEMP_LEVEL;
      data["humidity"] = HUMID_LEVEL;
      data["rackId"] = 1;
      data["light"] = lvl2;
      char message[500];
      serializeJson(out,message);
            Serial.println(message);
    }
  }
 }
//*******************************ACTUATION DOWN HERE***************************//
  delay(5000); //wait 5 seconds
// turn on lights!
  if (!manual)
  {
    int on1 = turnOnLights(p1LED1, p1LED2, goalLVL1, LVL1);
    int on2 = turnOnLights(p2LED1, p2LED2, goalLVL2, LVL2);
  }

// turn on water pumps
//  if(soilmoisturepercent0<20){
//    digitalWrite(relayPin0, LOW);
//  }
//  else{
//    digitalWrite(relayPin0, HIGH);
//  }
//  if(soilmoisturepercent1<20){
//    digitalWrite(relayPin1, LOW);
//  }
//  else{
//    digitalWrite(relayPin1, HIGH);
//  }

//turn on water pumps based on soil moisture seonsor values
  if(soilMoistureValue0>=MedMin){
    digitalWrite(relayPin0, LOW);
    
    }
  else{
    digitalWrite(relayPin0, HIGH);
  }
  if(soilMoistureValue0>MedMax){
    digitalWrite(relayPin1, LOW);
  }
  else{
    digitalWrite(relayPin1, HIGH);
  }
  
  
  // display humidity and temp
    display.clearDisplay();
    display.setTextSize(0.7);
    display.setCursor(0,0);
    display.print("Temp = ");
    display.print(int(t));
    display.print(" C");
    display.setCursor(0,13);
    display.print("Humidity = ");
    display.print(int(h));
    display.print(" %");
    display.display(); 
}

int setCurrentLevel(int current){
  //sets current level based on current analog value read from light sensor
  int currentLVL = 0;
  if (current > medMax) currentLVL = 3; //high
  else if (current > lowMax && current <= medMax) currentLVL = 2; //medium
  else if (current > lowMin && current <= lowMax) currentLVL = 1; //low
  else currentLVL = 0; // dark

  //return current level
  return currentLVL;
}

String getTemp(float t){

    if(t<15.0)
    {
      return "Low";
    }
    else if(t>=15.0 && t<=35)
    {
      return "Medium";
    }
    else
    {
      return "High";
    }
}

 String getHumidity(float h){
    if(h<20)
    {
      return "Low";
    }
    else if (h>=20 && h<=85)
    {
      return "Medium";
    }
    else
    {
      return "High";
    }
  
}

String getMositure(int m){

    if(m>700)
    {
      return "Low";
    }
    else if(m<=700 && m >=550)
    {
      return "Medium";
    }
    else
    {
      return "High";
    }
}

String getLight(int l){
  
  if(l ==1)
  {
    return "Low";
  }else if(l ==2)
  {
    return"Medium";
  }
  else if(l ==3)
  {
    return "High";
  }
  else
  {
    return "Invalid Input";
  }

}

int turnOnLights(int pin1, int pin2, int goal, int current){
//turn on lights if there's a difference between what we have and what we want
  int diff = goal - current;
//  Serial.print(goal);
//  Serial.print("  ");
//  Serial.println(diff);
//thresholding max and min
  if(diff < 0) diff = 0;
  if(diff > 2 ) diff = 2;
//turn on lights depending on level differences
  switch (diff){
    case 2: //BOTH LIGHTS
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    break;
    case 1: //ONE LIGHT
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    break;
    case 0: //ZERO LIGHTS
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    break;  
    } 
  return 0;
}
