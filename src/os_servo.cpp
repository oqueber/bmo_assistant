// Standart lib
#include "Arduino.h"
#include "main.h"
#include <ESP32Servo.h>

QueueHandle_t servo_queue;

// Servo object
Servo myServo;

//
// Moves
// 


// notes in the melody:
void servo_scanning()
{
  for (int pos = 0; pos <= 180; pos += 10) {
    myServo.write(pos);
    vTaskDelay(50);
  }

  // Luego de 180 a 0
  for (int pos = 180; pos >= 0; pos -= 10) {
    myServo.write(pos);
    vTaskDelay(50);
  }
}


void init_servo_task(void * parameter){
  
  // avoid warning
  (void) parameter;

  myServo.setPeriodHertz(50); // Estándar de servos
  myServo.attach(SERVO_PIN, 500, 2400); // Rango de pulso en microsegundos

  /* Attempt to create the event group. */
  servo_queue = xQueueCreate( 10, sizeof( uint8_t ) );

  while (1)
  {
    #if 0
    if( xQueueReceive( servo_queue,
                      &( tone_play ),
                      ( TickType_t ) portMAX_DELAY) == pdPASS )

    #endif

    servo_scanning();

    // Next task
    portYIELD();
  }
}
