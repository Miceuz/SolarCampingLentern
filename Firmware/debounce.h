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

#endif /* DEBOUNCE_H_ */
