/**
 * @file    main.c
 * @brief   Development project with ESP8266.
 *
 * @author  Oqueber Navarro
 * @date    15-11/2024
 *
 * @details This project involves the development of an application for the ESP8266 microcontroller 
 * using the Arduino programming language. It is compiled and designed to be executed 
 * in PlatformIO with Visual Studio Code.
 * 
 * For more information, visit: 
 * @see     https://oqueber.es/bmo
 */

// Standart lib
#include "Arduino.h"
#include <Wire.h>

// Display libraries
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>  
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

typedef enum
{
	DISPLAY_FACE_DISPABLE = 0,
	DISPLAY_FACE_ANGRY,
	DISPLAY_FACE_LOOKING,
	DISPLAY_FACE_NERVOUS
}face_animation_t;

// The ESP8266 pin connected to buzzer
#define BUZZZER_PIN   15
// The ESP8266 pin connected to buttom.
#define BUTTON_PIN    10

//ISRs need to have IRAM_ATTR before the function definition to run the interrupt code in RAM.
IRAM_ATTR void ISR_button_pressed(void);

// Flag button to receive irq
volatile bool flag_animation_stop = false;
volatile bool flag_btn_enable = false;
// Allow switching between animations
int index_select_animation = DISPLAY_FACE_ANGRY;

// Create Display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

//
// BUZZER
// 

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


//
// Display
//

void face_angry(void)
{
  display.fillScreen(0);         //Limpiamos la pantalla
  
  display.fillRoundRect(10,17,30,40,15,1);
  display.fillRoundRect(55,17,30,40,15,1);

  for(int i = 0; i < 45; i++)
  {
    display.fillTriangle(0,0, display.width()-1,0, 45 , i, 0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display(); 

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

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

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

    delay(20);           
  }
}

void face_nervous(void)
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

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

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
    
    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

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

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

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

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

    delay(20);           
  }
}

void face_disable(void)
{
    //Limpiamos la pantalla
    display.fillScreen(0);         
    
    //Refrescamos la pantalla para visualizarlos
    display.display();
}


//
// BUTTOM SEQUENCE
//

void btn_detect_press(void)
{
	int num_btn_press = 0;
        
	// Check if it is a short or long press
	while(digitalRead(BUTTON_PIN) == 0)
	{
		num_btn_press++;
		delay(100);

		// Detected long pressed button
		if( num_btn_press >= 10)
		{
			break;
		}
	}


	// SHORT PULSE
	if ( num_btn_press < 10)
	{
		index_select_animation++;
		play_sound_success();
	}
	// LONG PULSES
	else
	{	
		// DISABLE ANIMATION
		if (index_select_animation != DISPLAY_FACE_DISPABLE)
		{
			index_select_animation = DISPLAY_FACE_DISPABLE;
			play_sound_error();
		}
		// ENABLE ANIMATION
		else
		{
			index_select_animation = DISPLAY_FACE_ANGRY;
			play_sound_success();
		}
	}

	// Enable button
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING );
	// Enable animation 
	flag_animation_stop = false;
	// Clean flag btn 
	flag_btn_enable = false;
}

//
// SETUP
//

void setup()
{
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    // initialize Buzzer pin as an output.
    pinMode(BUZZZER_PIN, OUTPUT);

    // Initialize Buttom pin as a input
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING);

    // INIT and Clean DISPLAY 128x64
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
}

void loop()
{
    // SELECT Animation
    switch (index_select_animation)
    {
        case DISPLAY_FACE_DISPABLE:
        {
            face_disable();
            delay(500);
            break;
        }
        case DISPLAY_FACE_ANGRY:
        {
            face_angry();
            break;
        }

        case DISPLAY_FACE_LOOKING:
        {
            face_looking();
            break;
        }
		case DISPLAY_FACE_NERVOUS:
        {
			face_nervous();
            break;
        }
        default:
            index_select_animation = DISPLAY_FACE_ANGRY;
        break;
    }


    //if an interrup has occured
    if (flag_btn_enable)
    {
		// Check if it is really pressed
        if(digitalRead(BUTTON_PIN) == 0)
		{
			btn_detect_press();
		}
    }

    // Toggle the led
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

//
// IRQ
//

IRAM_ATTR void ISR_button_pressed(void) 
{
    // BTN IS PRESSING
    if (flag_btn_enable == false)
    {
		// Set btn flag to enable
		flag_btn_enable = true;
		// Stop all animation 
		flag_animation_stop = true;
		// Disable btn
		detachInterrupt( digitalPinToInterrupt(BUTTON_PIN) );
    }
}