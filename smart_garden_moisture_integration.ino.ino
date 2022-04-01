const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2

int soilMoistureValue0 = 0;
int soilMoistureValue1 = 0;

int waterLevelInput = 0;

int output_value0;
int output_value1;

int soilmoisturepercent0=0;
int soilmoisturepercent1=0;

int relayPin0 = 8;
int relayPin1 = 9;

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  pinMode(relayPin0,OUTPUT);
  pinMode(relayPin1,OUTPUT);
  pinMode(soilMoistureValue0,INPUT);
  pinMode(soilMoistureValue1,INPUT);
  pinMode(waterLevelInput,INPUT);
  Serial.println("Reading sensor..");
  delay(2000);
}

void loop() {
soilMoistureValue0 = analogRead(A0);  //put Sensor insert into soil
soilMoistureValue1 = analogRead(A1);  //put Sensor insert into soil

soilmoisturepercent0 = map(soilMoistureValue0, AirValue, WaterValue, 0, 100);
soilmoisturepercent1 = map(soilMoistureValue1, AirValue, WaterValue, 0, 100);

waterLevelInput = analogRead(A2); //water level sensor

//output_value0=analogRead(soilMoistureValue0);
//output_value1=analogRead(soilMoistureValue1);

//output_value0 = map(output_value0,550,10,0,100);
//output_value1 = map(output_value1,550,10,0,100);

//Serial.println((String)"A0: "+ soilmoisturepercent0 + " A1: " + soilmoisturepercent1);

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
//Notification
}
delay(1000);
}
