/*
 *Programa demostración TFT y LCD. 
 * @author Juan Siverio Rojas
 */

///librerías para práctica 1(termómetro)
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

///Librerías para practica 2 (juego retro)

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "logo_ull.c"


///variable para práctica 1

#define Voltimetro A0
#define Boton 0 ///uso entrada pint 0 digital

LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x27 o 0x3F
int celsius = 0; // variable que define si se muestra la temperatura en Celsius o en Farenheit. Cero celsius, otro valor farenheit
int boton = 0; ///variable que indica a 1 boton apretado, a 0 botón sin apretar.
unsigned long tiempoInicial = 0; ///tiempo desde que se arrancó el arduino
unsigned long tiempoFinal = 0; ///cuando tiempo inicial >= tiempo final, se actualiza las pantalla tanto por Serial.print como la LCD. Esta variable se incremente en 1000 ms cada vez que se actualiza la pantalla.
unsigned option = 0; ///Indico que estoy en la primera opción de menu.

//variables para práctica 2

#define TFT_CS         10
#define TFT_RST        9
#define TFT_DC         8

//#define TFT_RST        4
//#define TFT_CS         6
//#define TFT_DC         5

#define joystickY      A1   ///eje X
#define joystickX      A2   ///Eje y
#define joystickButtom A3 ///utilizar como digital. 
        
#define TFT_MOSI 12  // Data out
#define TFT_SCLK 13  // Clock out

// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

float p = 3.1415926;

void termometro()
{
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
 {
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
}
void menuShow()
{

if (option == 0)
 {
  tft.fillScreen(ST7735_BLACK);
  tft.drawRGBBitmap(0,0,logo_ull,30,30);  
  tft.drawRGBBitmap(98,0,logo_ull,30,30);
  tft.setCursor(35, 3);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.println("Menu");
  tft.drawLine(0, 32, 128, 32,ST7735_BLUE);
  
  //OPCIONES 
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);  
  tft.setCursor(16, 35);  
  tft.println("Gracos Cent");
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(16, 60);
  tft.println("Grados Farenheit");
  tft.setCursor(16, 85);
  tft.println("Juego");
  tft.setCursor(16, 110);
  tft.println("Instrucciones");
  option =  1; //indico que la opción uno es la seleccionada para empezar, o sea, Centígrados.
 }
 
}


/* Devuelve un char con la siguiente configuración. Se analizará con operadores lógicos
 * bit 0: a 1 si se apretó botón
 * bit 1: a 1 si movimiento izquierda 
 * bit 2: a 1 si movimiento derecha
 * bit 3: a 1 si movimiento hacia arriba
 * bit 4: a 1 si movimiento hacia abajo
 */

char joystick()  
{
  char valor=0;   ///todo a cero significa que no se ha hecho ningún movimiento ni apretado el boton
  unsigned tmp,x,y;
  
  delay(200);
  tmp = analogRead(joystickButtom);

  if (tmp == 0)
    valor |= 0x01; ///pongo  bit 0  1
  else 
    valor &= 0xE; ///pongo bit 0 a 0, puesto que no está el boton apretado.

  y = analogRead(joystickY); 
    
  if(y >= 0 && y <= 300)
  {    
      valor |= 0x10; ///pongo bit 4 a 1 indicando movimiento hacia abajo-
      valor &= 0xF7;  ///pongo bit 3 a 0 indicando que no hay movimiento hacia arriba.
  }
  if(y >= 700 && y <= 1024)
  {
      valor |= 0x08; ///pongo bit 3 a 1 indicando movimiento hacia arriba
      valor &= 0xEF; ///pongo bit 4 a 0 indicando que no hay movimiento hacia abajo.
  }
  x = analogRead(joystickX); 

  if(x >= 0 && x <= 300)
   {
      valor |= 0x02; ///pongo bit 1 a 1 indicando movimiento hacia izquierda
      valor &= 0xFB; ///pongo bit 2 a cero indicando que no hay movimiento hacia la derecha
   }  
  if(x >= 700 && x <= 1024)
  {
      valor |= 0x04; ///pongo bit 2 a 1 indicando movimiento hacia derecha
      valor &= 0xFD;  ///pongo bit 1 a 0 indicando que no hay movimiento hacia la izquierda
  }
  if(x >= 300 && x <= 700)
    valor &= 0xF9; ///borro movimiento izquierda o derecha.
    
  if(y >= 300 && y <= 700)
    valor &= 0xE7; ///borro movimiento arriba o abajo
  
return valor;
}

void setup() {
  ///código para práctica 1
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

//código para práctica 2
   tft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab   
   tft.setRotation(90);

  
}

void loop() {
  

 termometro();
 menuShow();
 joystick();
 
//Serial.println("boton joystic");
//Serial.println(analogRead(joystickButtom));


 //  tft.invertDisplay(true);
 // delay(500);
 // tft.invertDisplay(false);
 // delay(500);
}


/*void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}
//*
///funciones para pruebas tft
///*
void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}*/
