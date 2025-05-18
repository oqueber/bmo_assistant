// Standart lib
#include "Arduino.h"
// Common cofig
#include "main.h"

#define LONG_PRESS_DETECTION_AT   2000 // 2seconds
#define DELAY_EACH_DELTA          100 // 100ms

#define EVENT_BTN_PRESSED    (1 << 0)

//ISRs need to have IRAM_ATTR before the function definition to run the interrupt code in RAM.
IRAM_ATTR void ISR_button_pressed(void);

/* Declare a variable to hold the created event group. */
EventGroupHandle_t flags_irq;
// Press type
EventGroupHandle_t inputs_event;


//
// BTN SEQUENCE
//

void btn_detect_press(void)
{
	int num_btn_press = 0;

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

    // SHORT PULSE
    if ( num_btn_press < (LONG_PRESS_DETECTION_AT/DELAY_EACH_DELTA))
    {
      // Send short pressed
      xEventGroupSetBits(inputs_event, EVENT_BTN_1_SHORT);
    }
    // LONG PULSES
    else
    {	
      // Send short pressed
      xEventGroupSetBits(inputs_event, EVENT_BTN_1_LONG);
    }

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
  inputs_event = xEventGroupCreate();

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