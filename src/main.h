
// LED 
//#define LED_PIN   8
#define NUM_LEDS    4
#define DATA_PIN    8
// The ESP32C3 pin connected to buzzer
#define BUZZZER_PIN   0
// The ESP32C3 pin connected to buttom.
#define BUTTON_PIN    1
//Display pinout
#define DISPLAY_SCL   3
#define DISPLAY_SDA   2
// SD
#define PIN_MISO 5
#define PIN_MOSI 6
#define PIN_SCK  4
#define PIN_CS   7

typedef enum
{
	DISPLAY_FACE_OFF = 0,
	DISPLAY_FACE_ANGRY,
	DISPLAY_FACE_LOOKING,
	DISPLAY_FACE_NERVOUS
}face_animation_t;


typedef enum
{
	TONE_OFF = 0,
	TONE_SUCCESS,
	TONE_ERROR
}play_tone_t;

typedef enum{
    LIGHT_OFF = 0,
    LIGHT_BLINK,
    LIGHT_CASCADE
}light_show_t;

struct display_msg_t
{
    uint8_t ucMessageID;
    uint8_t ucData[ 1024 ];
};

// BTN task
void init_inputs_task(void * parameter);
// Display task
void init_display_task(void * parameter);
// Tone Task
void init_tone_task(void * parameter);
// Light Task
void init_light_task(void * parameter);