Power Framework Expansion Card
==============================

Non-isolated 5V output with a 500 mA current limit.


## Pinout

To connect to this board, one has to use a 2-pin JST XH connector. The following
table represents the pinout, pin 1 being on the left as looking into the
expansion card.

| # | Ref   | Description                     |
|--:|:-----:|---------------------------------|
| 1 | GND   | Ground connection               |
| 2 | 5V    | Positive voltage                |


## LEDs

Left LED signals if input is powered. It is expected to be on at all times.

Right LED signals if output is powered and there are no faults. It will turn off
if there is an over-current state (typically at around `525 mA`).


---
*You can check my blog and other projects at [www.medo64.com](https://www.medo64.com/electronics/).*
