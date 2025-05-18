// Standart lib
#include "Arduino.h"
#include "main.h"
//LEDS
#include <Adafruit_NeoPixel.h>

// Define the array of leds
Adafruit_NeoPixel pixels(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
// light queue
QueueHandle_t light_queue;


void light_cascade(void)
{
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();  
    vTaskDelay(100);

    // Send the updated pixel colors to the hardware.
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.show();   
    vTaskDelay(500);

    pixels.setPixelColor(1, pixels.Color(0, 150, 0));
    pixels.show();   
    vTaskDelay(500);

    pixels.setPixelColor(2, pixels.Color(0, 150, 0));
    pixels.show();   
    vTaskDelay(500);

    pixels.setPixelColor(3, pixels.Color(0, 150, 0));
    pixels.show();   
    vTaskDelay(500);

    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();  
    vTaskDelay(100);

}

void light_blink(void)
{
    // Send the updated pixel colors to the hardware.
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.setPixelColor(1, pixels.Color(150, 0, 0));
    pixels.show();   
    vTaskDelay(500);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();  
    vTaskDelay(500);
}

void light_off(void)
{
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();  
}

void init_light_task(void * parameter){
  
  // avoid warning
  (void) parameter;

  uint8_t light_show;

  /* Attempt to create the event group. */
  light_queue = xQueueCreate( 10, sizeof( uint8_t ) );

  // INITIALIZE NeoPixel strip object
  pixels.begin(); 

  while (1)
  {
    if( xQueueReceive( light_queue,
                      &( light_show ),
                      ( TickType_t ) portMAX_DELAY) == pdPASS )
    {
      
      switch (light_show)
      {
        case LIGHT_CASCADE:
        {
            light_cascade();
          break;
        }
        case LIGHT_BLINK:
        {
            light_blink();
          break;
        }
        case LIGHT_OFF:
        default:
            light_off();
          break;
      }
    }

    // Next task
    portYIELD();

  }
  
}
