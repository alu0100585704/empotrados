#define Voltimetro A0
#define Boton 7 ///uso entrada pint 7 digital

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x27 o 0x3F
int celsius = 0; // variable que define si se muestra la temperatura en Celsius o en Farenheit. Cero celsius, otro valor farenheit
int boton = 0; ///variable que indica a 1 boton apretado, a 0 botón sin apretar.

unsigned long tiempoInicial = 0; ///tiempo desde que se arrancó el arduino
unsigned long tiempoFinal = 0; ///cuando tiempo inicial >= tiempo final, se actualiza las pantalla tanto por Serial.print como la LCD. Esta variable se incremente en 1000 ms cada vez que se actualiza la pantalla.

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
  lcd.setCursor(0,0);
  lcd.print("                ");
  
  // definir el pin del boton como entrada
  // ponerlo como entrada pullup

  pinMode(Boton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned valorDigital;   
  int actualizarPantalla; ///se pone a 1 para actualizar la pantall cuando ha pasado 1000 milisegundos.
  
  tiempoInicial = millis();
  if (tiempoInicial > tiempoFinal)
  {
      tiempoFinal = tiempoFinal + 1000; //para espera de 1 segundo
      actualizarPantalla = 1;
  }
  else 
      actualizarPantalla = 0;
  
  valorDigital = analogRead(Voltimetro);
    
  ///revisar si se ha presionado el boton
  if ((digitalRead(Boton) == LOW) && (boton == 0))
   {
    boton = 1;
    celsius = ~celsius; ///invierto entre celsius y farenheit
   }
   else if (digitalRead(Boton) == HIGH)
                boton = 0;
    
 

  float voltios = 100.0 * (5.0/1024.0) * (float) valorDigital;

if (actualizarPantalla == 1)
  if (celsius == 0) {
  
    float Temperatura = (voltios/11.0); ///11 es el valor que el apmplificador operacional en modo inversor aplica a la señal de entrada en base a las resistencia de 10Kh y 1Kh
  
  
    lcd.setCursor(0,0);
    lcd.print(Temperatura);
    lcd.print(" ");
    lcd.print("C");
    
    Serial.print(Temperatura);
    Serial.println(" C");

  } else { // Hacer la conversion a Farenheit

    float Temperatura = (voltios/11.0) / 0.556 + 32  ; ///11 es el valor que el apmplificador operacional en modo inversor aplica a la señal de entrada en base a las resistencia de 10Kh y 1Kh
  
  
    lcd.setCursor(0,0);
    lcd.print(Temperatura);
    lcd.print(" ");
    lcd.print("F");

    Serial.print(Temperatura);
    Serial.println(" F");
    
  }
 

}
