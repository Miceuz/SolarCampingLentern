/*
 ------------------------------------------------------------------------------
| Copyright Albertas Mickėnas 2022                                             |
|                                                                              |
| This source describes Open Hardware and is licensed under the CERN-OHL-S v2. |
|                                                                              |
| You may redistribute and modify this source and make products using it under |
| the terms of the CERN-OHL-S v2 (https://ohwr.org/cern_ohl_s_v2.txt).         |
|                                                                              |
| This source is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,          |
| INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A         |
| PARTICULAR PURPOSE. Please see the CERN-OHL-S v2 for applicable conditions.  |
|                                                                              |
| Source location: https://https://github.com/Miceuz/SolarCampingLentern       |
|                                                                              |
| As per CERN-OHL-S v2 section 4, should You produce hardware based on this    |
| source, You must where practicable maintain the Source Location visible      |
| on the external case of the device or other products you make using this     |
| source.                                                                      |
 ------------------------------------------------------------------------------
*/

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_
#include <stdbool.h>
#include <stdint.h>
#define FALSE 0
#define TRUE 1
#define BUTTON_PRESSED 1
#define BUTTON_RELEASED 0
#define PRESS_TIMEOUT_INITIAL 1000
#define PRESS_TIMENOUT_REPEAT 10000

typedef struct {
  uint8_t pinBuffer;
  uint8_t pressed;
  uint8_t lastState;
  uint32_t pressedAt;
  uint16_t pressTimeout;
  uint8_t changeFlag;
} switch_t;

#define BUTTON_DEFAULT                                                         \
  { 0x00, BUTTON_RELEASED, BUTTON_RELEASED, 0, 1000, FALSE }

extern void debounce(volatile switch_t *button, volatile uint8_t value);
extern void serviceButton(volatile switch_t *button, void (*onPressed)(void),
                          void (*onReleased)(void));
extern void serviceButtonWithAutorepeat(volatile switch_t *button,
                                        void (*onPressed)(void),
                                        void (*onLongPressed)(void),
                                        void (*onReleased)(void),
                                        uint32_t timestamp);
extern void clear(volatile switch_t *button);
extern bool isButtonEvent(volatile switch_t *button, uint8_t state);
extern void buttonReset(volatile switch_t *button);
extern uint8_t isLongPress(volatile switch_t *button, uint32_t timestamp);

#endif /* DEBOUNCE_H_ */
