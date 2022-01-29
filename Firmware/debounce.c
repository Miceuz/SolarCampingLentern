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

#include "debounce.h"

void debounce(volatile switch_t *button, volatile uint8_t value) {
  button->pinBuffer = (button->pinBuffer) << 1;
  button->pinBuffer |= value;
  if (0xFF == button->pinBuffer) {
    button->pressed = BUTTON_PRESSED;
  } else if (0x00 == button->pinBuffer) {
    button->pressed = BUTTON_RELEASED;
  }
}

void clear(volatile switch_t *button) {
  button->pinBuffer = 0x00;
  button->pressed = 0;
}

void buttonReset(volatile switch_t *button) {
  button->pressed = BUTTON_RELEASED;
  button->pinBuffer = 0x00;
  button->lastState = BUTTON_RELEASED;
}

void serviceButton(volatile switch_t *button, void (*onPressed)(void),
                   void (*onReleased)(void)) {
  if (button->pressed != button->lastState) {
    if (button->pressed) {
      if (0 != onPressed) {
        onPressed();
      }
    } else {
      if (0 != onReleased) {
        onReleased();
      }
    }
    button->changeFlag = 1;
    button->lastState = button->pressed;
  }
}

bool isButtonEvent(volatile switch_t *button, uint8_t state) {
  if (button->changeFlag && button->pressed == state) {
    button->changeFlag = 0;
    return true;
  }
  return false;
}

uint8_t isLongPress(volatile switch_t *button, uint32_t timestamp) {
  return button->pressed &&
         ((timestamp - button->pressedAt) > button->pressTimeout);
}

void serviceButtonWithAutorepeat(volatile switch_t *button,
                                 void (*onPressed)(void),
                                 void (*onLongPressed)(void),
                                 void (*onReleased)(void), uint32_t timestamp) {
  if (button->pressed != button->lastState) {
    if (button->pressed) {
      button->pressedAt = timestamp;
      if (0 != onPressed) {
        onPressed();
      }
    } else {
      if (0 != onReleased) {
        onReleased();
      }
      button->pressTimeout = PRESS_TIMEOUT_INITIAL;
    }
    button->lastState = button->pressed;
    button->changeFlag = 1;
  }
  // else if(isLongPress(button, timestamp)) {
  //     if(0 != onLongPressed) {
  //         onLongPressed();
  //     }
  //     button->pressedAt = timestamp;
  //     // if(PRESS_TIMEOUT_INITIAL == button->pressTimeout) {
  //         button->pressTimeout = PRESS_TIMENOUT_REPEAT;
  //     // }
  // }
}
