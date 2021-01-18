#define Voltimetro A0

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x27


int Celsius = 1; // variable que define si se muestra la temperatura en Celsius o en Farenheit
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   lcd.init();

 // Turn on the blacklight and print a message.
  lcd.backlight();

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Inicializando...");
  delay(1000);

  // definir el pin del boton como entrada
  // ponerlo como entrada pullup
  
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned valorDigital;
  
  valorDigital = analogRead(Voltimetro);

 

  float voltios = 100.0 * (5.0/1024.0) * (float) valorDigital;

  if (Celsius == 1) {
  
    float Temperatura = (voltios/11.0);

  
  
    lcd.setCursor(0,0);
    lcd.print(Temperatura);
    lcd.print(" ");
    lcd.print("ºC");

    Serial.print(Temperatura);
    Serial.println(" ºC");
  } else { // Hacer la conversion a Farenheit

    
  }
  delay(1000); // Cambiar el delay, por una funcion de espera activa utilizando millis(), 
     //revisar si se ha cambiado la temperatura de Celsisus a Farenheit

}
