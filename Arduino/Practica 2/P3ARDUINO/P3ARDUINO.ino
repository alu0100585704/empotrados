/*
 * Programa que carga un menú en una pantalla tft
 * con diferentes opciones seleccionables a través 
 * del dispositivo joystick externo.
 * 
 * @author Felipe Esquivel Delgado
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ImageReader.h> // Image-reading functions

#include "perro.c"
#include "piedra.c"
#include "papel.c"
#include "tijera.c"

#define TFT_CS         10
#define TFT_RST        9
#define TFT_DC         8

#define pinX A0 ///Pin del eje X del joystick
#define pinY A1 ///Pin del eje Y del joystick
#define button A3 ///Pin de presionar boton del joystick

#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#define TFT_MOSI 12  // Data out
#define TFT_SCLK 13  // Clock out

// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int opcion = 1;

/********************************
 *    Funciones al clicar
 *******************************/
void click_temperatura ()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(28, 25);
  tft.println("Temperatura\n");
#ifndef Voltimetro
  tft.setCursor(4, 55);
  tft.println("No esta conectado el ");
  tft.setCursor(4,70);
  tft.println("dispositivo sensor");
#endif  
  tft.setCursor(6,146);
  tft.println("Presione para salir");
  int aux = 500;
  if (aux != 0) {
    while (aux != 0) {
      aux = analogRead(button);
      delay(200);
    }
  }
}

void click_imagenR ()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(30, 4);
  tft.println("Foto Pecera\n");

  tft.drawLine(0, 15, 128, 15, BLUE);
  tft.drawRGBBitmap(35,60,pecera,55,31);

  delay(500);
  tft.setCursor(6,146);
  tft.println("Presione para salir");
  int aux = 500;
  if (aux != 0) {
    while (aux != 0) {
      aux = analogRead(button);
      delay(200);
    }
  }
}

void game_finish (int opcion)
{
  tft.setTextSize(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(25, 50);
  tft.setTextColor(YELLOW);
  tft.println("PIEDRA..");
  delay(800);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(25, 50);
  tft.setTextColor(WHITE);
  tft.println("PAPEL..");
  delay(800);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(15, 50);
  tft.setTextColor(BLUE);
  tft.println("TIJERA..");
  delay(800);

  tft.setTextSize(5);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(20, 50);
  tft.setTextColor(RED);
  tft.println("YA!");
  delay(800);

  tft.fillScreen(ST77XX_BLACK);
  
  short ran = random(1,4);
  tft.setTextSize(2);
  if (ran == opcion)
  {
    //EMPATE
    if (ran == 1)
    {
      tft.drawRGBBitmap(37,12,piedra,55,37);
      tft.drawRGBBitmap(37,100,piedra,55,37);
    } else if (ran == 2)
    {
      tft.drawRGBBitmap(37,12,papel,55,37);
      tft.drawRGBBitmap(37,100,papel,55,37);
    } else
    {
      tft.drawRGBBitmap(37,12,tijera,55,37);
      tft.drawRGBBitmap(37,100,tijera,55,37);
    }
    tft.setCursor(25, 68);
    tft.setTextColor(WHITE);
    tft.println("EMPATE");
  } else if ((ran == 1 && opcion == 3) || (ran == 2 && opcion == 1) || (ran == 3 && opcion == 2))
  {
    //PERDER
    if (ran == 1)
    {
      tft.drawRGBBitmap(35,12,piedra,55,37);
      tft.drawRGBBitmap(35,100,tijera,55,37);
    } else if (ran == 2)
    {
      tft.drawRGBBitmap(35,12,papel,55,37);
      tft.drawRGBBitmap(35,100,piedra,55,37);
    } else
    {
      tft.drawRGBBitmap(35,12,tijera,55,37);
      tft.drawRGBBitmap(35,100,papel,55,37);
    }
    tft.setCursor(30, 67);
    tft.setTextColor(RED);
    tft.println("PIERDES");
  } else 
  {
    //GANAR
    if (ran == 1)
    {
      tft.drawRGBBitmap(35,12,piedra,55,37);
      tft.drawRGBBitmap(35,100,papel,55,37);
    } else if (ran == 2)
    {
      tft.drawRGBBitmap(35,12,papel,55,37);
      tft.drawRGBBitmap(35,100,tijera,55,37);
    } else
    {
      tft.drawRGBBitmap(35,12,tijera,55,37);
      tft.drawRGBBitmap(35,100,piedra,55,37);
    }
    tft.setCursor(30, 67);
    tft.setTextColor(GREEN);
    tft.println("GANAS");
  }
  delay(4000);
}

void menu_game (int seleccion) 
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(45, 4);
  tft.setTextColor(RED);
  tft.println("ELIGE:\n");
  tft.drawLine(0, 15, 128, 15, BLUE);

  int posicion[4] = {0, 31, 55, 81};
  tft.fillRect(10, posicion[seleccion], 100, 25, GREEN);

  tft.setTextColor(WHITE);
  tft.setCursor(16, 40);
  tft.println("Piedra");
  tft.setCursor(16, 65);
  tft.println("Papel");
  tft.setCursor(16, 90);
  tft.println("Tijera");
}

void click_game ()
{
  short cambio = checkbutton();
  opcion = 1;
  menu_game(opcion);
  delay(1000);
  while (cambio == 0)
  {
    cambio = checkbutton();
    unsigned x = updateXPos();
    unsigned y = updateYPos();

    if(y >= 0 && y <= 300)
    {
      opcion++;
      if(opcion == 4)
      {  
        opcion = 1;
      }
      menu_game(opcion);
    }
    else if(y >= 900 && y <= 1024)
    {
      opcion--;
      if(opcion == 0)
      {
        opcion = 3;
      }
      menu_game(opcion);
    }
  }
  game_finish(opcion);
  opcion = 3;
}

void click_info ()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(WHITE);
  tft.setCursor(30, 25);
  tft.println("Arduino UNO");
  tft.setCursor(3, 47);
  tft.println("Programa ejemplo que");
  tft.setCursor(3, 59);
  tft.println("carga un menu con");
  tft.setCursor(3, 71);
  tft.println("diferentes opciones");
  tft.setCursor(3, 83);
  tft.println("seleccionables con ");
  tft.setCursor(3, 95);
  tft.println("el mando.\n\n");
  tft.setCursor(30,148);
  tft.println("Felipe Esquivel\n");
  int aux = 500;
  if (aux != 0) {
    while (aux != 0) {
      aux = analogRead(button);
      delay(200);
    }
  }
}

short checkbutton ()
{
  unsigned aux;
  short cambio = 0;
  delay(500);
  aux = analogRead(button);
  if (aux == 0)
  {
    delay(500);
    cambio = 1;
  }    
  return cambio;
}

unsigned updateXPos ()
{
  unsigned x;
  x = analogRead(pinX); 
  return x;
}

unsigned updateYPos ()
{
  unsigned y;
  y = analogRead(pinY); 
  return y;
}

void menu(int seleccion){
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(18, 3);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("Menu principal");
  tft.drawLine(0, 14, 128, 14, BLUE);
  //RECTANGULO DE SELECCION
  int posicion[6] = {0, 18, 41, 66, 91, 116};
  tft.fillRect(10, posicion[seleccion], 100, 25, BLUE);
  //OPCIONES DEL MENU PRINCIPAL
  tft.setTextColor(WHITE);
  tft.setCursor(16, 25);
  tft.println("Temperatura");
  tft.setCursor(16, 50);
  tft.println("Foto Pecera");
  tft.setCursor(16, 75);
  tft.println("Jueguito");
  tft.setCursor(16, 100);
  tft.println("Informacion");
}

void setup(void) {
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(90);

  menu(1);
  delay(500);
}

void loop() {
  int cambio = checkbutton();
  unsigned x = updateXPos();
  unsigned y = updateYPos();
  
  if (cambio == 1)
  {
    switch (opcion)
    {
      case 1:
              click_temperatura();
              break;
      case 2:
              click_imagenR();
              break;
      case 3:
              click_game();
              break;
      case 4:
              click_info();
              break;
      default:
              break;
    }
    menu(opcion);
  }
  else if(y >= 0 && y <= 300)
  {
    opcion++;
    if(opcion == 5)
    {
      opcion = 1;
    }
    menu(opcion);
  }
  else if(y >= 900 && y <= 1024)
  {
    opcion--;
    if(opcion == 0)
    {
      opcion = 4;
    }
    menu(opcion);
  }
}
