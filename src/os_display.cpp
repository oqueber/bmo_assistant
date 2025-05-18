// Standart lib
#include "Arduino.h"
// Common cofig
#include "main.h"

// I2C
#include <Wire.h>
// Display libraries
#include <Adafruit_GFX.h>      
#include <Adafruit_SSD1306.h>  
#include <Fonts/FreeMonoBoldOblique12pt7b.h>

QueueHandle_t display_queue;

// Flag button to receive irq
volatile bool flag_animation_stop = false;
// Allow switching between animations
face_animation_t display_now = DISPLAY_FACE_LOOKING;

// Create Display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

bool face_wait(TickType_t time)
{
  // RX msgs
  struct display_msg_t new_display_msg;

  if( xQueueReceive( display_queue,
                      &( new_display_msg ),
                      time ) == pdPASS )
  {
    // Update animation
    display_now = (face_animation_t)new_display_msg.ucMessageID;
    return true;
  }
  
  // Not change
  return false;
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

    if(face_wait(30)){return;};           
  }

  if(face_wait(1000)){return;};  

  for(int i = 45; i > 0; i--)
  {
    display.fillRoundRect(10,17,30,40,15,1);
    display.fillRoundRect(55,17,30,40,15,1);
    display.fillTriangle(0,0, display.width()-1,0, 45 , i, 0);
    
    //Refrescamos la pantalla para visualizarlos
    display.display(); 

    // If flag animation stop is enable -> stop
    if(flag_animation_stop){return;}

    if(face_wait(20)){return;};         
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

    if(face_wait(30)){return;};            
  }

  if(face_wait(1000)){return;};

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

    if(face_wait(20)){return;};           
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

    if(face_wait(30)){return;};            
  }

  if(face_wait(1000)){return;};  

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

    if(face_wait(20)){return;};            
  }
}

void face_disable(void)
{
  //Limpiamos la pantalla
  display.fillScreen(0);         
  
  //Refrescamos la pantalla para visualizarlos
  display.display();
}




void init_display_task(void * parameter){
  
  // avoid warning
  (void) parameter;

  /* Attempt to create the event group. */
  display_queue = xQueueCreate( 10, sizeof( display_msg_t ) );

  // Configure I2C pins before initializing display
  Wire.setPins(DISPLAY_SDA, DISPLAY_SCL);  // SDA, SCL

  // INIT and Clean DISPLAY 128x64
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  while (1)
  {
    // SELECT Animation
    switch (display_now)
    {
      case DISPLAY_FACE_OFF:
      {
        face_disable();
        (void)face_wait(500);
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
          display_now = DISPLAY_FACE_OFF;
      break;
    }

    // Next task
    portYIELD();
  }
}