#define Voltimetro A0

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned valorDigital;
  
  valorDigital =  analogRead(Voltimetro);
  float voltios = (5.0 / 1024.0) * (float) valorDigital * 100;

  Serial.print(voltios);
  Serial.println(" ºC");
  delay(100);
}
