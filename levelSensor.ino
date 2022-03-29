int button = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
//  int buttonState = digitalRead(button);
  Serial.println(analogRead(A0));
  delay(100);
}
