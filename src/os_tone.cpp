// Standart lib
#include "Arduino.h"
#include "main.h"

QueueHandle_t tone_queue;


#if BUZZER_AS_VIBRATION 
#define  DELAY_PAUSE 800
#else
#define  DELAY_PAUSE 100
#endif
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
    ledcWriteTone(TONE_CHANNEL, 10000);
    vTaskDelay(DELAY_PAUSE);

    // PAUSE
    ledcWriteTone(TONE_CHANNEL, 0);
    vTaskDelay(DELAY_PAUSE/2);
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
    ledcWriteTone(TONE_CHANNEL, 2700);
    vTaskDelay(DELAY_PAUSE);

    // PAUSE
    ledcWriteTone(TONE_CHANNEL, 0);
    vTaskDelay(DELAY_PAUSE/4);
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
    ledcWriteTone(TONE_CHANNEL, 100);
    vTaskDelay(DELAY_PAUSE);

    // PAUSE
    ledcWriteTone(TONE_CHANNEL,0);
    vTaskDelay(DELAY_PAUSE);
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
    ledcWriteTone(TONE_CHANNEL, 1000);
    vTaskDelay(DELAY_PAUSE);

    // PAUSE
    ledcWriteTone(TONE_CHANNEL,0);
    vTaskDelay(DELAY_PAUSE*2);
    num_beep--;
  }
}



void init_tone_task(void * parameter){
  
  // avoid warning
  (void) parameter;

  uint8_t tone_play;

  // Configura el canal para el tono
  ledcSetup(TONE_CHANNEL, TONE_FREQUENCY, TONE_RESOLUTION);

  // Asocia el canal al pin del buzzer
  ledcAttachPin(BUZZER_PIN, TONE_CHANNEL);

  /* Attempt to create the event group. */
  tone_queue = xQueueCreate( 10, sizeof( uint8_t ) );

  while (1)
  {
    if( xQueueReceive( tone_queue,
                      &( tone_play ),
                      ( TickType_t ) portMAX_DELAY) == pdPASS )
    {
      
      switch (tone_play)
      {
        case TONE_SUCCESS:
        {
          play_sound_success();
          break;
        }
        case TONE_ERROR:
        {
          play_sound_error();
          break;
        }
        default:
          play_sound();
          break;
      }
    }

    // Next task
    portYIELD();

  }
  
}
