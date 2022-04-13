//define LED output and light sensor inputs
//LEDS
  int p1LED1 = 7;
  int p1LED2 = 6;
  int p2LED1 = 5;
  int p2LED2 = 4;
//Light sensors
  int light1Pin = A0;
  int light2Pin = A1;
  
//brightness levels dark = 0, low = 1, med = 2, high = 3
  int goalLVL1 = 3; //what we want for the plants
  int goalLVL2 = 3;
  int currentLVL1 = 0; //what we're at
  int currentLVL2 = 0;
//define brightness ranges (these we just kinda need to map 
  int medMax = 800;
  int lowMax = 500;
  int lowMin = 450;

void setup() {
  Serial.begin(9600);
//define LED pin modes to outputs
  pinMode(p1LED1, OUTPUT);
  pinMode(p1LED2, OUTPUT);
  pinMode(p2LED1, OUTPUT);
  pinMode(p2LED2, OUTPUT);
}

void loop() {


//current brightness values
  int current1 = analogRead(light1Pin);
  int current2 = analogRead(light2Pin);

  Serial.print(current1);
  Serial.print("  ");
  Serial.println(current2);

//set current level for plant 1
  int LVL1 = setCurrentLevel(current1); //set these two with brightness sensor
  int LVL2 = setCurrentLevel(current2); 
  Serial.print(LVL1);
  Serial.print("  ");
  Serial.println(LVL2);
  int on1 = turnOnLights(p1LED1, p1LED2, goalLVL1, LVL1);
  int on2 = turnOnLights(p2LED1, p2LED2, goalLVL2, LVL2);

  delay(5000); //wait 5 seconds
 
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
