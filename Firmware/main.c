#include <stdbool.h> /* For true/false definition */
#include <stdint.h>  /* For uint8_t definition */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "debounce.h"
#include "millis.h"

const uint16_t PROGMEM gamma8[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   2,    2,
    2,   3,   3,   3,   4,   4,   5,   5,   5,   6,   7,   7,   8,   8,    9,
    10,  10,  11,  12,  13,  13,  14,  15,  16,  17,  18,  19,  20,  21,   22,
    23,  24,  25,  27,  28,  29,  30,  32,  33,  34,  36,  37,  39,  40,   42,
    43,  45,  46,  48,  50,  52,  53,  55,  57,  59,  61,  62,  64,  66,   68,
    70,  72,  75,  77,  79,  81,  83,  85,  88,  90,  92,  95,  97,  100,  102,
    105, 107, 110, 112, 115, 118, 121, 123, 126, 129, 132, 135, 138, 141,  144,
    147, 150, 153, 156, 159, 162, 166, 169, 172, 175, 179, 182, 186, 189,  193,
    196, 200, 203, 207, 211, 215, 218, 222, 226, 230, 234, 238, 242, 246,  250,
    254, 258, 262, 266, 271, 275, 279, 284, 288, 292, 297, 301, 306, 311,  315,
    320, 324, 329, 334, 339, 344, 348, 353, 358, 363, 368, 373, 378, 384,  389,
    394, 399, 405, 410, 415, 421, 426, 432, 437, 443, 448, 454, 459, 465,  471,
    477, 483, 488, 494, 500, 506, 512, 518, 524, 530, 537, 543, 549, 555,  562,
    568, 575, 581, 587, 594, 601, 607, 614, 620, 627, 634, 641, 648, 654,  661,
    668, 675, 682, 689, 697, 704, 711, 718, 725, 733, 740, 747, 755, 762,  770,
    777, 785, 793, 800, 808, 816, 824, 831, 839, 847, 855, 863, 871, 879,  887,
    896, 904, 912, 920, 929, 937, 945, 954, 962, 971, 980, 988, 997, 1005, 1014,
    1023};

#define BUTTON PA6
#define LED_WHITE_EN PB0
#define LED_WHITE_PWM PB2
#define LED_RED PA5

switch_t button = BUTTON_DEFAULT;

void buttonInit() {
  DDRA &= ~_BV(BUTTON);
  PUEA |= _BV(BUTTON);
}

static inline void ledWhiteSetBrightnes(uint16_t brightness) {
  OCR1A = brightness;
}

static inline void ledWhiteOn(uint16_t brightness) {
  ledWhiteSetBrightnes(brightness);
  PORTB &= ~_BV(LED_WHITE_EN);
  PORTB |= _BV(LED_WHITE_PWM);
  TOCPMCOE |= _BV(TOCC7OE);
}

static inline void ledWhiteOff() {
  ledWhiteSetBrightnes(0);
  PORTB |= _BV(LED_WHITE_EN);
  PORTB &= ~_BV(LED_WHITE_PWM);
  TOCPMCOE &= ~_BV(TOCC7OE);
}

#define BRIGHTNESS_DEFAULT 1023

static inline void ledRedOn() { PORTA |= _BV(LED_RED); }
static inline void ledRedOff() { PORTA &= ~_BV(LED_RED); }

void ledsInit() {
  DDRA |= _BV(LED_RED);
  DDRB |= _BV(LED_WHITE_EN);
  DDRB |= _BV(LED_WHITE_PWM);

  TOCPMSA1 = _BV(TOCC7S0);

  TCCR1A = 0;
  TCCR1A |= _BV(COM1A1);
  TCCR1A |= _BV(WGM10) | _BV(WGM11);
  TCCR1B = 0;
  TCCR1B |= _BV(WGM12);
  TCCR1B |= _BV(CS10) | _BV(CS11);

  ledWhiteSetBrightnes(0);
  ledWhiteOff();
  ledRedOff();
}

uint8_t mcusr_mirror __attribute__((section(".noinit")));
void wdt_init(void) {
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

#define BUTTON_DEBOUNCE_INTERVAL_MS 10
uint32_t buttonDebounceTs = 0;

void processButtons() {
  if (millis() - buttonDebounceTs > BUTTON_DEBOUNCE_INTERVAL_MS) {
    debounce(&button, 0 == (PINA & _BV(BUTTON)));
    buttonDebounceTs = millis();
  }
  serviceButtonWithAutorepeat(&button, 0, 0, 0, millis());
}

inline static void pinChangeIntDisable() { GIMSK &= ~_BV(PCIE0); }

inline static void pinChangeIntEnable() {
  GIFR |= _BV(PCIF0);
  GIMSK |= _BV(PCIE0);
}

void pinChangeIntSetup() { PCMSK0 |= _BV(PCINT6); }

ISR(PCINT0_vect) { pinChangeIntDisable(); }

inline static void sleep() {
  buttonReset(&button);

  // DDRA |= _BV(PA0);
  // PORTA &= ~_BV(PA0);

  DDRA |= _BV(PA1);
  PORTA &= ~_BV(PA1);

  DDRA |= _BV(PA2);
  PORTA &= ~_BV(PA2);

  DDRA |= _BV(PA3);
  PORTA &= ~_BV(PA3);

  DDRA |= _BV(PA4);
  PORTA &= ~_BV(PA4);

  DDRA |= _BV(PA5);
  PORTA &= ~_BV(PA5);

  DDRA &= ~_BV(PA7);
  PUEA |= _BV(PA7);

  DDRB &= ~_BV(PB1);
  PUEB |= _BV(PB1);

  pinChangeIntEnable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  // wdt_disable();
  sleep_cpu();
  sleep_disable();
  // wdt_enable(WDTO_2S);
}

uint8_t getBatteryLevel();
void bargraphStart(uint8_t level);
void bargraphStop();

typedef enum {
  STATE_IDLE,
  STATE_BATT_LEVEL,
  STATE_WHITE_ON,
  STATE_RED_ON,
  STATE_FADE_WHITE
} State_t;
State_t state = STATE_IDLE;

void onExit_STATE_IDLE() {}

void onExit_STATE_WHITE_ON() { ledWhiteOff(); }

void onEnter_STATE_WHITE_ON(uint16_t brightness) {
  state = STATE_WHITE_ON;
  ledWhiteOn(brightness);
}

void onEnter_STATE_IDLE() {
  state = STATE_IDLE;
  sleep();
}

void onEnter_STATE_RED_ON() {
  state = STATE_RED_ON;
  ledRedOn();
}

void onExit_STATE_RED_ON() { ledRedOff(); }

void onEnter_STATE_BATT_LEVEL() {
  bargraphStart(getBatteryLevel());
  state = STATE_BATT_LEVEL;
}

void onExit_STATE_BATT_LEVEL() { bargraphStop(); }

uint32_t fade_ts = 0;
uint8_t br = 0;
bool fadeUp = true;

void fadeWhite() {
  if (millis() - fade_ts > 10) {
    ledWhiteSetBrightnes(pgm_read_word(&gamma8[br]));
    fade_ts = millis();
    if (fadeUp) {
      br++;
      if (br == 255) {
        fadeUp = false;
      }
    } else {
      br--;
      if (30 == br) {
        fadeUp = true;
      }
    }
  }
}

void onExit_STATE_FADE_WHITE() {}

void onEnter_STATE_FADE_WHITE() {
  br = 255;
  // ledWhiteOn(&gamma8[br]);
  fadeUp = false;
  fade_ts = millis();
  state = STATE_FADE_WHITE;
}

void processStateMachine() {
  switch (state) {
  case STATE_IDLE:
    if (isButtonEvent(&button, BUTTON_PRESSED)) {
      onExit_STATE_IDLE();
      onEnter_STATE_BATT_LEVEL();
    }
    break;
  case STATE_BATT_LEVEL:
    if (isButtonEvent(&button, BUTTON_RELEASED)) {
      onExit_STATE_BATT_LEVEL();
      onEnter_STATE_WHITE_ON(BRIGHTNESS_DEFAULT);
    }
    break;
  case STATE_FADE_WHITE:
    fadeWhite();
    if (isButtonEvent(&button, BUTTON_RELEASED)) {
      onExit_STATE_FADE_WHITE();
      onEnter_STATE_WHITE_ON(OCR1A);
    }
    break;
  case STATE_WHITE_ON:
    if (isButtonEvent(&button, BUTTON_RELEASED)) {
      onExit_STATE_WHITE_ON();
      onEnter_STATE_RED_ON();
    } else if (isLongPress(&button, millis())) {
      onEnter_STATE_FADE_WHITE();
    }
    break;

  case STATE_RED_ON:
    if (isButtonEvent(&button, BUTTON_PRESSED)) {
      onExit_STATE_RED_ON();
      while (!isButtonEvent(&button, BUTTON_RELEASED)) {
        processButtons();
      }
      onEnter_STATE_IDLE();
    }
    break;

  default:
    break;
  }
}

#define CHARLIE1 PA1
#define CHARLIE2 PA2
#define CHARLIE3 PA3

void charlie(uint8_t x) {
  switch (x) {
  case 0:
    DDRA &= ~_BV(CHARLIE1);
    DDRA &= ~_BV(CHARLIE2);
    DDRA &= ~_BV(CHARLIE3);

    PORTA &= ~_BV(CHARLIE1);
    PORTA &= ~_BV(CHARLIE2);
    PORTA &= ~_BV(CHARLIE3);
    break;
  case 1:
    DDRA |= _BV(CHARLIE1);
    DDRA &= ~_BV(CHARLIE2);
    DDRA |= _BV(CHARLIE3);

    PORTA &= ~_BV(CHARLIE1);
    PORTA &= ~_BV(CHARLIE2);
    PORTA |= _BV(CHARLIE3);
    break;
  case 2:
    DDRA |= _BV(CHARLIE1);
    DDRA &= ~_BV(CHARLIE2);
    DDRA |= _BV(CHARLIE3);

    PORTA |= _BV(CHARLIE1);
    PORTA &= ~_BV(CHARLIE2);
    PORTA &= ~_BV(CHARLIE3);
    break;
  case 3:
    DDRA &= ~_BV(CHARLIE1);
    DDRA |= _BV(CHARLIE2);
    DDRA |= _BV(CHARLIE3);

    PORTA &= ~_BV(CHARLIE1);
    PORTA &= ~_BV(CHARLIE2);
    PORTA |= _BV(CHARLIE3);
    break;
  case 4:
    DDRA |= _BV(CHARLIE1);
    DDRA |= _BV(CHARLIE2);
    DDRA &= ~_BV(CHARLIE3);

    PORTA |= _BV(CHARLIE1);
    PORTA &= ~_BV(CHARLIE2);
    PORTA &= ~_BV(CHARLIE3);
    break;
  case 5:
    DDRA |= _BV(CHARLIE1);
    DDRA |= _BV(CHARLIE2);
    DDRA &= ~_BV(CHARLIE3);

    PORTA &= ~_BV(CHARLIE1);
    PORTA |= _BV(CHARLIE2);
    PORTA &= ~_BV(CHARLIE3);
    break;
  case 6:
    DDRA &= ~_BV(CHARLIE1);
    DDRA |= _BV(CHARLIE2);
    DDRA |= _BV(CHARLIE3);

    PORTA &= ~_BV(CHARLIE1);
    PORTA |= _BV(CHARLIE2);
    PORTA &= ~_BV(CHARLIE3);
    break;
  default:
    break;
  }
}

uint8_t currCharlie = 0;
uint8_t bargraphLevel = 0;
void bargraphStart(uint8_t level) {
  bargraphLevel = level;
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= _BV(WGM22);
  TCCR2B |= _BV(CS20);
  TIMSK2 |= _BV(OCIE2A);
  OCR2A = 10000;
}

void bargraphStop() {
  TCCR2B = 0;
  charlie(0);
}

ISR(TIMER2_COMPA_vect) {
  if (currCharlie <= bargraphLevel) {
    charlie(currCharlie);
  } else {
    charlie(0);
  }

  currCharlie++;
  if (currCharlie > 6) {
    currCharlie = 0;
  }
}

#define CHANNEL_BATTERY 0

uint8_t adcInProgress = 0;

inline static void sleepWhileADC() {
  adcInProgress = 1;
  while (adcInProgress) {
    set_sleep_mode(SLEEP_MODE_ADC);
    sleep_mode();
  }
}

ISR(ADC_vect) {
  adcInProgress = 0;
  // nothing, just wake up
}

uint16_t adcReadChannel(uint8_t channel) {
  ADMUXA = channel;
  ADCSRA |= _BV(ADSC);
  // sleepWhileADC();
  loop_until_bit_is_clear(ADCSRA, ADSC);
  uint16_t ret = ADC;
  return ret;
}

inline static void adcSetup() {
  PRR &= ~_BV(PRADC);
  DIDR0 |= _BV(ADC0D);
  ADCSRA = 0;
  ADMUXB = _BV(REFS1); // use internal 2.2V reference
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADIE);
}

inline static void adcDisable() {
  ADCSRA = 0;
  PRR |= _BV(PRADC);
}

uint16_t adcGetBatteryVoltage() {
  adcSetup();
  adcReadChannel(CHANNEL_BATTERY);
  uint32_t lsb = adcReadChannel(CHANNEL_BATTERY);
  adcDisable();
  return lsb * 220 * 2 / 1024;
}

uint8_t getBatteryLevel() {
  uint16_t voltage = adcGetBatteryVoltage();
  if (voltage <= 320) {
    return 1;
  } else if (voltage <= 340) {
    return 2;
  } else if (voltage <= 360) {
    return 3;
  } else if (voltage <= 370) {
    return 4;
  } else if (voltage <= 400) {
    return 5;
  } else if (voltage <= 420) {
    return 6;
  } else if (voltage > 420) {
    return 6;
  }
}

uint8_t c = 0;

void main() {
  // wdt_init();
  // wdt_reset();
  buttonInit();
  millisInit();
  ledsInit();
  pinChangeIntSetup();
  sei();

  // wdt_enable(WDTO_2S);
  // ledRedOn();

  onEnter_STATE_IDLE();

  while (1) {
    processButtons();
    processStateMachine();
    wdt_reset();
  }

  // bargraphStart(0);
  // while (1) {
  //   bargraphLevel = 0;
  //   _delay_ms(500);
  //   bargraphLevel = 1;
  //   _delay_ms(500);
  //   bargraphLevel = 2;
  //   _delay_ms(500);
  //   bargraphLevel = 3;
  //   _delay_ms(500);
  //   bargraphLevel = 4;
  //   _delay_ms(500);
  //   bargraphLevel = 5;
  //   _delay_ms(500);
  //   bargraphLevel = 6;
  //   _delay_ms(500);
  // }
}
