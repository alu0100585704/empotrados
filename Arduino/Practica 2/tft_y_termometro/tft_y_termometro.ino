/*
 *Programa demostración TFT y LCD. 
 * @author Juan Siverio Rojas
 */
/*
 * Las imágenes del PUZZLE se han convertido a monocromo para que pudieran caber en la memoria de programa, pero el resultado es que apenas se distinguen bien.
 * Se supone que son partes del logo de la ULL. 
 * 
 * Además se ha utilizado la versión de selección manual de pines, que no es tan rápida como la SPI, pero es más flexible permitiéndome elegir los pines.
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
#include "puzzle0M.c"
#include "puzzle1M.c"
#include "puzzle2M.c"
#include "puzzle3M.c"

///variable para práctica 1

#define Voltimetro A0
#define Boton 0 ///uso entrada pint 0 digital

LiquidCrystal_I2C lcd(0x3F, 16, 2); // 0x27 o 0x3F
int celsius = 0; // variable que define si se muestra la temperatura en Celsius o en Farenheit. Cero celsius, otro valor farenheit
int boton = 0; ///variable que indica a 1 boton apretado, a 0 botón sin apretar.
unsigned long tiempoInicial = 0; ///tiempo desde que se arrancó el arduino
unsigned long tiempoFinal = 0; ///cuando tiempo inicial >= tiempo final, se actualiza las pantalla tanto por Serial.print como la LCD. Esta variable se incremente en 1000 ms cada vez que se actualiza la pantalla.
unsigned opcionMenuActual = 0; ///Indico a que posicion de menu me voy a mover. 0, indica repintar todo el menu.
unsigned opcionMenuAnterior; ///Indico la posicion anterior

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

///Con un And lógico, comprueba con los siguientes valores su se apretó el botón o se movió hacia alguna dirección
#define JOYSTICK_LEFT  0x02
#define JOYSTICK_RIGHT 0x04
#define JOYSTICK_UP    0x08
#define JOYSTICK_DOWN   0x10
#define JOYSTICK_BUTTOM  0x01

// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Funciones prototipo
void termometro();
void menuShow();
int joystick();
void intrucciones();
void puzzle();

/*
 * SETUP
 */
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

/*
 * LOOP
 */
void loop() {

 termometro();
 menuShow();
 
int valor = joystick();


if (valor != 0 )
  if (valor & JOYSTICK_BUTTOM)
      switch (opcionMenuActual)
      {
        case 1:  
              celsius = 0;
              break;
              
        case 2:
              celsius = 1;
              break;
              
        case 3:
              puzzle();
              opcionMenuActual = 0;
              menuShow();
              break;
    
        case 4:
              instrucciones();
              opcionMenuActual = 0;
              menuShow();
        
              break;
        default:
              break;
      }
  else 
      {
        if (valor & JOYSTICK_UP)
          opcionMenuActual++;
      
      if (valor & JOYSTICK_DOWN)
          opcionMenuActual--;
        
      }
      

///dar la vuelta al menu
if (opcionMenuActual == 5)
  opcionMenuActual = 1;
 
if (opcionMenuActual == 0)
  opcionMenuActual = 4;



}

/*
 * FUNCIONAS INTERNAS
 */

 /*
  * Termómetro 
 */
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


/*
 * Menu. Muestra el menu completo si opcionMenuActual  = 0. De lo contrario, actualiza solo la opcion de menu en concreto mediante SPRITE.
 */
void menuShow()
{

if (opcionMenuActual == 0)
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
  tft.println("Juego PUZZLE");
  tft.setCursor(16, 110);
  tft.println("Instrucciones");
  opcionMenuActual =  1; //indico que la opción uno es la seleccionada para empezar, o sea, Centígrados.
  opcionMenuAnterior = 1;
 }

else if (opcionMenuActual != opcionMenuAnterior)
{
  ///selecciono la nueva opción y anula la anterior, lo hago con la técnica SPRITES, que solo actualizo lo modificado y necesario, en lugar de toda la pantalla
  tft.setTextSize(1);
 tft.setTextColor(ST7735_WHITE);  
  switch (opcionMenuAnterior)
  {
    case 1:  
          tft.setCursor(16, 35);  
          tft.println("Gracos Cent");          
          break;
          
    case 2:
          tft.setCursor(16, 60);
          tft.println("Grados Farenheit");
          break;
          
    case 3:
          tft.setCursor(16, 85);
          tft.println("Juego PUZZLE");

          break;

    case 4:
          tft.setCursor(16, 110);
          tft.println("Instrucciones");
    
          break;
    default:
          break;
  }

opcionMenuAnterior = opcionMenuActual;
 
 tft.setTextColor(ST7735_YELLOW);  
  switch (opcionMenuActual)
  {
    case 1:  
          tft.setCursor(16, 35);  
          tft.println("Gracos Cent");          
          break;
          
    case 2:
          tft.setCursor(16, 60);
          tft.println("Grados Farenheit");
          break;
          
    case 3:
          tft.setCursor(16, 85);
          tft.println("Juego PUZZLE");

          break;

    case 4:
          tft.setCursor(16, 110);
          tft.println("Instrucciones");
    
          break;
    default:
          break;
  }

  
}
  

}


/* Devuelve un char con la siguiente configuración. Se analizará con operadores lógicos
 * bit 0: a 1 si se apretó botón
 * bit 1: a 1 si movimiento izquierda 
 * bit 2: a 1 si movimiento derecha
 * bit 3: a 1 si movimiento hacia arriba
 * bit 4: a 1 si movimiento hacia abajo
 */

int joystick()  
{
  int valor = 0;   ///todo a cero significa que no se ha hecho ningún movimiento ni apretado el boton
  int tmp,x,y;  
  delay(50);
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

void instrucciones()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(0, 0);
  tft.println("Opcion 1: Muestra en la LCD la temperatura actual en Celsius");
  //tft.setCursor(5, 47);
  ///tft.setCursor(3, 59);
  tft.println("\nOpcion 2: Muestra en la LCD en temperatura actual en Farenhait");
//  tft.setCursor(3, 71);
 // tft.println("Farenhait");
 // tft.setCursor(3, 83);
  tft.println("\nOpcion 3:Juego PUZZLE");
  //tft.setCursor(3, 95);
  tft.println("\nProgramado por Juan Siverio Rojas");
  //tft.setCursor(30,148);
  tft.println("\nMueva o apriete el boton del mando");
  while (joystick()== 0);
}
void puzzle()
{
  
  const uint8_t * imagen[4];  
  imagen[0] =  puzzle0;
  imagen[1] = puzzle1;
  imagen[2] = puzzle2;
 imagen[3] = puzzle3;

  int finish[4];
  finish[0] = 0;
  finish[1] = 1;
  finish[2] = 2;
  finish[3] = 3;
  
  int posActual[4];
  posActual[0] = 2;
  posActual[1] = 0;
  posActual[2] = 1;
  posActual[3] = 3;
  
  tft.fillScreen(ST7735_BLACK);    
  tft.drawGrayscaleBitmap(0,0,imagen[posActual[0]],64,80); 
  tft.drawGrayscaleBitmap(65,0,imagen[posActual[1]],64,80);  
  tft.drawGrayscaleBitmap(0,81,imagen[posActual[2]],64,80);  
  tft.drawGrayscaleBitmap(65,81,imagen[posActual[3]],64,80);  
  while (joystick()== 0);
}
