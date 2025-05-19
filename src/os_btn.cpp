// Standart lib
#include "Arduino.h"
// Common cofig
#include "main.h"

#define DELAY_EACH_DELTA          10 // 10ms
#define WINDOW_MS                 600    // Ventana de 1 s

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
  // Variables de conteo en cada ventana
  uint8_t clickCount    = 0; // If arrived here, it's a pulsation already.
  uint8_t longDetected  = 0;
  bool    btn_n0 = false, btn_n1 = false;

  TickType_t windowStart = xTaskGetTickCount();

  // Escuchar durante WINDOW_MS
  while ((xTaskGetTickCount() - windowStart) < pdMS_TO_TICKS(WINDOW_MS)) {
    
    // BTN now
    btn_n0 = (digitalRead(BUTTON_PIN) == LOW);

    // Click detected 
    if ((btn_n0 == true) && (btn_n1 == false)) {
      clickCount++;
    }
    else if ((btn_n0 == true) && (btn_n1 == true))
    {
      longDetected++;
    }

    // save the last value 
    btn_n1 = btn_n0;

    // Next Sequence
    vTaskDelay(pdMS_TO_TICKS(DELAY_EACH_DELTA));
  }

  // Procesar resultados de la ventana
  if (longDetected >= (WINDOW_MS / DELAY_EACH_DELTA) - 5 ) {
    xEventGroupSetBits(inputs_event, EVENT_BTN_1_LONG);
    Serial.println("Botón presionado (LONG)");
  }
  else if (clickCount > 1) {
    xEventGroupSetBits(inputs_event, EVENT_BTN_1_DOUBLE);
    Serial.println("Botón presionado (DOUBLE)");
  }
  else {
    xEventGroupSetBits(inputs_event, EVENT_BTN_1_SHORT);
    Serial.println("Botón presionado (SHORT)");
  }
  
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_button_pressed, FALLING);
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