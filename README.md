# ino-light-show
Multi-mode light show, with analog input for rate of light flicker

## Setup

On an Arduino board:

- Connect a potentiometer (1k is fine) to A0

- Connect on/off switches to pins 2, 3 

- Connect a (dumb) buzzer to pin 13.

- Connect leds to pins 7, 8, 9, 10, 11, 12

## Shows

For on/off switch positions:
- 0,0 -> Emergency Vehicle. Three left-most leds light up in succession, then turn off, then three right-most leds light up in succession, then turn off.
- 0,1 -> Straight Six Engine (BMW) firing order visualizer. Lights up in correct order of a six cylinder engine
- 1,0 -> Formula One rev limiter simulator. Potentiometer controls RPM. At redline (max potentiometer resistance), light flashes indicating a need to switch gears
- 1,1 -> Random. For each led, a random chance to toggle on, off, or blink for a certain amount of time. Chances for a led group to also have these random effects applied across all of them.

For all of these shows, the potentiometer controls their speed/rate of effect

