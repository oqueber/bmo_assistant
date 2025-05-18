// Standart lib
#include "Arduino.h"
// Common cofig
#include "main.h"

#define LONG_PRESS_DETECTION_AT   2000 // 2seconds
#define DELAY_EACH_DELTA          100 // 100ms

#define EVENT_BTN_PRESSED    (1 << 0)

//ISRs need to have IRAM_ATTR before the function definition to run the interrupt code in RAM.
IRAM_ATTR void ISR_button_pressed(void);

// Allow switching between animations
uint8_t index_select_animation = 0;

/* Declare a variable to hold the created event group. */
EventGroupHandle_t flags_irq;

extern QueueHandle_t display_queue;
extern QueueHandle_t tone_queue;
extern QueueHandle_t light_queue;

//
// BTN SEQUENCE
//

void btn_detect_press(void)
{
	int num_btn_press = 0;
  // Display msg
  display_msg_t display_msg = {0, "" };
  // tone msg
  uint8_t play_tone = 0;

  // Check if it is really pressed
  if(digitalRead(BUTTON_PIN) == 0)
  {
    // Check if it is a short or long press
    while(digitalRead(BUTTON_PIN) == 0)
    {
      num_btn_press++;
      vTaskDelay(DELAY_EACH_DELTA);

      // Detected long pressed button
      if( num_btn_press >= (LONG_PRESS_DETECTION_AT/DELAY_EACH_DELTA))
      {
        break;
      }
    }

    // Send diferents animations
    if( index_select_animation < sizeof(face_animation_t))
    {
      index_select_animation++;
    }
    else
    {
      // Next sequence
      index_select_animation = DISPLAY_FACE_OFF + 1;
    }

    // SHORT PULSE
    if ( num_btn_press < (LONG_PRESS_DETECTION_AT/DELAY_EACH_DELTA))
    {
      play_tone = TONE_SUCCESS;
      display_msg.ucMessageID = index_select_animation;
    }
    // LONG PULSES
    else
    {	
      // Display OFF
      index_select_animation = DISPLAY_FACE_OFF;
      display_msg.ucMessageID = index_select_animation;
      play_tone = TONE_ERROR;
    }

      
    xQueueSend( display_queue, ( void * ) &display_msg, ( TickType_t ) 0 );
    xQueueSend( tone_queue, ( void * ) &play_tone, ( TickType_t ) 0 );
    xQueueSend( light_queue, ( void * ) &index_select_animation, ( TickType_t ) 0 );

  }

	// Enable button
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING );
}


void init_inputs_task(void * parameter){
  
  // avoid warning
  (void) parameter;

  // Initialize Buttom pin as a input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING);

  /* Attempt to create the event group. */
  flags_irq = xEventGroupCreate();

  while (1)
  {
     EventBits_t bits = xEventGroupWaitBits(
      flags_irq,
      EVENT_BTN_PRESSED,
      pdTRUE,     // Borra los bits al salir
      pdFALSE,    // Espera cualquier bit
      portMAX_DELAY
    );

    if (bits & EVENT_BTN_PRESSED) {
      Serial.println("Botón presionado (evento)");
      
      btn_detect_press();

      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING);
    }

  }
  
}


//
// IRQ
//

IRAM_ATTR void ISR_button_pressed(void) 
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  // Señaliza evento al grupo
  xEventGroupSetBitsFromISR(flags_irq, EVENT_BTN_PRESSED, &xHigherPriorityTaskWoken);

  // Desactiva la interrupción hasta que se procese
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));

  // Si fue necesario hacer un cambio de contexto
  portYIELD_FROM_ISR();
}