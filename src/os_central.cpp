// Standart lib
#include "Arduino.h"
#include "main.h"

extern QueueHandle_t display_queue;
extern QueueHandle_t tone_queue;
extern QueueHandle_t light_queue;
extern EventGroupHandle_t inputs_event;


void init_central_task(void * parameter){
  
  // avoid warning
  (void) parameter;
  // Display msg
  display_msg_t display_msg = {0, "" };
  // tone msg
  uint8_t play_tone = 0;
  // Enum the events arrived
  uint8_t index_short = 0, index_long = 0, index_double = 0;

  while (1)
  {
    EventBits_t bits = xEventGroupWaitBits(
      inputs_event,
      (EVENT_BTN_1_SHORT | EVENT_BTN_1_LONG | EVENT_BTN_1_DOUBLE),
      pdTRUE,     // Borra los bits al salir
      pdFALSE,    // Espera cualquier bit
      portMAX_DELAY
    );

    if (bits & EVENT_BTN_1_SHORT) {
      // Send diferents animations
      if( index_short < sizeof(play_tone_t))
      {
        index_short++;
      }
      else
      {
        // Next sequence
        index_short = LIGHT_OFF + 1;
      }

      xQueueSend( light_queue, ( void * ) &index_short, ( TickType_t ) 0 );
    }

    if (bits & EVENT_BTN_1_LONG) {
      // Send diferents animations
      if( index_long < sizeof(play_tone_t))
      {
        index_long++;
      }
      else
      {
        // Next sequence
        index_long = TONE_OFF + 1;
      }

      xQueueSend( tone_queue, ( void * ) &index_long, ( TickType_t ) 0 );
    }

    // When a double pulsation arrived -> new display animation
    if (bits & EVENT_BTN_1_DOUBLE) {

      // Send diferents animations
      if( index_double < sizeof(face_animation_t))
      {
        index_double++;
      }
      else
      {
        // Next sequence
        index_double = DISPLAY_FACE_OFF + 1;
      }

      // Send display notification
      display_msg.ucMessageID = index_double;
      xQueueSend( display_queue, ( void * ) &display_msg, ( TickType_t ) 0 );
    }



      
 

    // Next task
    portYIELD();

  }
  
}
