// Standart lib
#include "Arduino.h"
#include "main.h"

extern QueueHandle_t display_queue;
extern QueueHandle_t tone_queue;
extern QueueHandle_t light_queue;
extern EventGroupHandle_t inputs_event;


typedef void (*CommandHandler)(const String& arg);

struct Command {
  const char* name;
  CommandHandler handler;
};

// ==== Handlers ====

void handleTone(const String& arg) {
  uint8_t play_tone = 0;

  if (arg == "success") {
    Serial.println("tone sucess");
    play_tone = TONE_SUCCESS;
  } else if (arg == "error") {
    Serial.println("tone error");
    play_tone = 	TONE_ERROR;
  } else {
    Serial.println("❌ tone no reconocida.");
    play_tone = 	TONE_OFF;
  }

  xQueueSend( tone_queue, ( void * ) &play_tone, ( TickType_t ) 0 );
}

void handleFace(const String& arg) {
  display_msg_t display_msg = {0, "" };

  if (arg == "angry") {
    Serial.println("Modo angry");
    display_msg.ucMessageID = DISPLAY_FACE_ANGRY;
  } else if (arg == "looking") {
    Serial.println("Modo looking");
    display_msg.ucMessageID = 	DISPLAY_FACE_LOOKING;
  } else if (arg == "nervous") {
    Serial.println("Modo nervous");
    display_msg.ucMessageID = 	DISPLAY_FACE_NERVOUS;
  } else {
    Serial.println("❌ Emoción no reconocida.");
    display_msg.ucMessageID = 	DISPLAY_FACE_OFF;
  }

  // Send display notification
  xQueueSend( display_queue, ( void * ) &display_msg, ( TickType_t ) 0 );
}

void handleReboot(const String& arg) {
  Serial.println("♻️ Reiniciando...");
  delay(1000);
  esp_restart();
}

// ==== Tabla de comandos ====

Command commands[] = {
  {"tone", handleTone},
  {"face", handleFace},
  {"reboot", handleReboot},
};

const int numCommands = sizeof(commands) / sizeof(commands[0]);

// ==== Procesador de comandos ====

void processCommand(String input) {
 String arg = "";
  input.trim();
  if (input.length() == 0) return;

  int spaceIndex = input.indexOf(' ');
  String cmd = (spaceIndex == -1) ? input : input.substring(0, spaceIndex);
  if (spaceIndex == -1) {
    arg = "";
  } else {
    arg = input.substring(spaceIndex + 1);
    arg.trim();
  }
  
  for (int i = 0; i < numCommands; ++i) {
    if (cmd.equalsIgnoreCase(commands[i].name)) {
      commands[i].handler(arg);
      return;
    }
  }

  Serial.println("❓ Comando desconocido.");
}


void init_console_task(void *pvParameters) {
  String buffer = "";
  while (true) {
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        buffer.trim();
        processCommand(buffer);
        buffer = "";
      } else {
        buffer += c;
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Pequeño descanso para el scheduler
  }
}