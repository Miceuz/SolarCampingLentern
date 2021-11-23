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

static uint8_t isLongPress(volatile switch_t *button, uint32_t timestamp) {
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
