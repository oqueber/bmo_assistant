#include <MPU6050.h>
#include <Wire.h>
#include "main.h"

MPU6050 mpu;

bool mpu_log_enable = false;

void init_mpu_task(void *pvParameters) {
  // ya lo hace el display
  //Wire.begin(DISPLAY_SDA, DISPLAY_SCL); // SDA, SCL
  vTaskDelay(pdMS_TO_TICKS(2000)); // cada 500ms

  mpu.initialize();

  while (!mpu.testConnection()) {
    printf("MPU6050 no conectado correctamente\n");
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  printf("MPU6050 inicializado correctamente\n");

  while (true) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if(mpu_log_enable)
    {
      printf("Accel: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d\n", ax, ay, az, gx, gy, gz);
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
