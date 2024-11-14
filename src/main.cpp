/*
 ESP8266 - Ejemplo de uso de pantalla OLED 128x64 monocroma por Dani No www.esploradores.com

 Este ejemplo utiliza el núcleo de la librería gráfica de Adafruit GFX y 
 la librería SSD1306 específica para pantallas monocromas 128x64 y 128x32.
 Estas librerías deben estar instaladas para su funcionamiento.
 Para instalarlas se debe seleccionar:
   -Programa -> Incluir librería -> Gestionar Librerías -> Adafruit GFX Library
   -Programa -> Incluir librería -> Gestionar Librerías -> Adafruit SSD1306

 Cuando se instala la librería SSD1306 está configurada por defecto para pantallas 128x32.
 Al utilizarla para una pantalla 128x64 es preciso reconfigurarla.
 Para hacerlo debe abrir el fichero Adafruit_SSD1306.h con un editor y 
 descomentar la línea 69 (// #define SSD1306_128_64  --->  #define SSD1306_128_64) y 
 comentar la línea 70 (#define SSD1306_128_32  --->  //#define SSD1306_128_32).
 Así mismo se debe verificar en la línea 51 que la dirección de I2C de la pantalla es correcta 
 (#define SSD1306_I2C_ADDRESS   0x3C  // 128x64 0x3D (default) or 0x3C (if SA0 is grounded))

Por último, también se debe verificar que en la línea 108 de este sketch
la dirección I2C de la pantalla también es correcta.
 */

#include "Arduino.h"
#include "pitches.h"

#include <Wire.h>
#include <Adafruit_GFX.h>      //Nucleo de la librería gráfica.
#include <Adafruit_SSD1306.h>  //Librería para pantallas OLED monocromas de 128x64 y 128x32
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

// The ESP8266 pin connected to piezo buzzer
#define BUZZZER_PIN   15
// The ESP8266 pin connected to buttom.
#define BUTTOM_PIN    10

int button_status = 0;
int play_sound_select = 0;

#define OLED_RESET -1 
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// notes in the melody:
void play_sound_alarm()
{
  int num_beep = 3;

  while (num_beep)
  {
    // PLAY DURATION
    tone(BUZZZER_PIN, 10000, 100);
    delay(100);

    // PAUSE
    noTone(BUZZZER_PIN);
    delay(50);
    num_beep--;
  }
}

// notes in the melody:
void play_sound_success()
{
  int num_beep = 2;

  while (num_beep)
  {
    // PLAY DURATION
    tone(BUZZZER_PIN, 2700, 100);
    delay(100);

    // PAUSE
    noTone(BUZZZER_PIN);
    delay(25);
    num_beep--;
  }
}

// notes in the melody:
void play_sound_error()
{
  int num_beep = 4;

  while (num_beep)
  {
    // PLAY DURATION
    tone(BUZZZER_PIN, 100, 100);
    delay(100);

    // PAUSE
    noTone(BUZZZER_PIN);
    delay(100);
    num_beep--;
  }
}

// notes in the melody:
void play_sound()
{
  int num_beep = 2;
    
  while (num_beep)
  {
    // PLAY DURATION
    tone(BUZZZER_PIN, 1000, 100);
    delay(100);

    // PAUSE
    noTone(BUZZZER_PIN);
    delay(200);
    num_beep--;
  }
}

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize Buzzer pin as an output.
  pinMode(BUZZZER_PIN, OUTPUT);

  // Initialize Buttom pin as a input
  pinMode(BUTTOM_PIN, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // //Inicializa pantalla con en la dirección 0x3D para la conexión I2C.
  display.clearDisplay();
}

void face_brave(void)
{
  display.fillScreen(0);         //Limpiamos la pantalla
  
  display.fillRoundRect(10,17,30,40,15,1);
  display.fillRoundRect(55,17,30,40,15,1);

  for(int i = 0; i < 45; i++)
  {
    display.fillTriangle(0,0, display.width()-1,0, 45 , i, 0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(30);           
  }

  delay(1000); 

  for(int i = 45; i > 0; i--)
  {
    display.fillRoundRect(10,17,30,40,15,1);
    display.fillRoundRect(55,17,30,40,15,1);
    display.fillTriangle(0,0, display.width()-1,0, 45 , i, 0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(20);           
  }
}


void face_nervius(void)
{
  display.fillScreen(0);         //Limpiamos la pantalla
  
  display.fillRoundRect(10,17,30,40,15,1);
  display.fillRoundRect(55,17,30,40,15,1);

  for(int i = 0; i < 20; i++)
  {
    display.fillTriangle( 0 ,display.height()-1,
                          display.width()-1 , display.height()-1 , 
                          display.width()/2, (display.height()-1) - i ,
                          0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(30);           
  }

  delay(1000); 

  for(int i = 20; i > 0; i--)
  {
    display.fillRoundRect(10,17,30,40,15,1);
    display.fillRoundRect(55,17,30,40,15,1);
    display.fillTriangle( 0 ,display.height()-1,
                          display.width()-1 , display.height()-1 , 
                          display.width()/2, (display.height()-1) - i ,
                          0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(20);           
  }
}

void face_looking(void)
{
  
  for(int i = 0; i < 30; i++)
  {
    //Limpiamos la pantalla
    display.fillScreen(0);         

    display.fillRoundRect(i + 10,17,30,40,15,1);
    display.fillRoundRect(i + 55,17,30,40,15,1);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(30);           
  }

  delay(1000); 

  for(int i = 30; i > 0; i--)
  {
    //Limpiamos la pantalla
    display.fillScreen(0);         

    display.fillRoundRect(i + 10,17,30,40,15,1);
    display.fillRoundRect(i + 55,17,30,40,15,1);
    
    //Refrescamos la pantalla para visualizarlos
    display.display();  
    delay(20);           
  }
}

void loop()
{

  // WAIT TO PRESS BUTTOM
  if(digitalRead(BUTTOM_PIN) != button_status)
  {
    // ONLY WHEN BUTTOM IS PRESSING
    if(digitalRead(BUTTOM_PIN) == 0)
    {
      play_sound_select++;
    }
    
    // KEEP THE CHANGE
    button_status = digitalRead(BUTTOM_PIN);
  }

  // SELECT SOUND
  switch (play_sound_select)
  {
    case 0:
    {
      play_sound_success();
      break;
    }
    case 1:
    {
      play_sound_error();
      break;
    }
    case 2:
    {
      play_sound_alarm();
      break;
    }
    default:
      play_sound_select = 0;
      break;
  }

  //PUNTOS EN LAS CUATRO ESQUINAS DE LA PANTALLA
  //display.fillScreen(0);         //Limpiamos la pantalla
  
  //display.drawCircle(32,32,10,1);
  //display.drawCircle(64,32,10,1);
  //display.fillCircle(80,40,20,1);
  //display.fillCircle(32,40,20,1);
  //display.drawRoundRect(0,0,127,31,10,1);
  
  //face_brave();
  //face_nervius();
  face_looking();

  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(500);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  delay(500);
}