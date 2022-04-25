//humidity and oled includes and definition
  #include <DHT.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display(-1);
  #define DHTPIN 3 // digital Pin 3, humidity sensor
  #define DHTTYPE DHT11
  DHT dht(DHTPIN,DHTTYPE);
  
//moisture and water pump includes and definition
  const int AirValue = 620;   //you need to replace this value with Value_1
  const int WaterValue = 310;  //you need to replace this value with Value_2
  int soilMoistureValue0 = 0;
  int soilMoistureValue1 = 0;
  int waterLevelInput = 0;
  int output_value0;
  int output_value1;
  int soilmoisturepercent0=0;
  int soilmoisturepercent1=0;
  int relayPin0 = 52;
  int relayPin1 = 53;
  int soil0 = A0;
  int soil1 = A1;
  int waterPin = A6;


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


//current brightness values
  int current1 = analogRead(light1Pin);
  int current2 = analogRead(light2Pin);
//current humidity
  float h = dht.readHumidity();
//current temperature as Celsius
  float t = dht.readTemperature();
// if we want to Read temperature as Fahrenheit (isFahrenheit = true)
//  float f = dht.readTemperature(true);

  Serial.print(current1);
  Serial.print("  ");
  Serial.println(current2);

//set current level for plant 1
  int LVL1 = setCurrentLevel(current1); //set these two with brightness sensor
  int LVL2 = setCurrentLevel(current2); 
  Serial.print(LVL1);
  Serial.print("  ");
  Serial.println(LVL2);

  //DHT.read11(dht_apin);
    
  Serial.print("Current humidity = ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(t); 
  Serial.println("C  ");

//set moisture and activate pumps
  soilMoistureValue0 = analogRead(soil0);  //put Sensor insert into soil
  soilMoistureValue1 = analogRead(soil1);  //put Sensor insert into soil
  
  soilmoisturepercent0 = map(soilMoistureValue0, AirValue, WaterValue, 0, 100);
  soilmoisturepercent1 = map(soilMoistureValue1, AirValue, WaterValue, 0, 100);
  
  waterLevelInput = analogRead(waterPin); //water level sensor
  Serial.print("waterLevelInput = ");
  Serial.println(waterLevelInput); 
 
  //output_value0=analogRead(soilMoistureValue0);
  //output_value1=analogRead(soilMoistureValue1);
  
  //output_value0 = map(output_value0,550,10,0,100);
  //output_value1 = map(output_value1,550,10,0,100);
  
  //Serial.println((String)"A0: "+ soilmoisturepercent0 + " A1: " + soilmoisturepercent1);

//*******************************ACTUATION DOWN HERE***************************//
  delay(5000); //wait 5 seconds
// turn on lights!
  int on1 = turnOnLights(p1LED1, p1LED2, goalLVL1, LVL1);
  int on2 = turnOnLights(p2LED1, p2LED2, goalLVL2, LVL2);
// turn on water pumps  
  if(soilmoisturepercent0<20){
    digitalWrite(relayPin0, LOW);
  }
  else{
    digitalWrite(relayPin0, HIGH);
  }
  if(soilmoisturepercent1<20){
    digitalWrite(relayPin1, LOW);
  }
  else{
    digitalWrite(relayPin1, HIGH);
  }
  
  if (waterLevelInput = 0) {
    Serial.println("Fill your tank");
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

int turnOnLights(int pin1, int pin2, int goal, int current){
//turn on lights if there's a difference between what we have and what we want
  int diff = goal - current;
  Serial.print(goal);
  Serial.print("  ");
  Serial.println(diff);
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
