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


## Commands

Each char is a separate command. If it is recognized, output will be tilde
character (`~`), original command, and `CR``LF`.

| Char | Description                                                   |
|------|---------------------------------------------------------------|
| `0`  | No smoothing for voltage and current                          |
| `1`  | Voltage and current read are smoothed over 2 readings         |
| `2`  | Voltage and current read are smoothed over 4 readings         |
| `3`  | Voltage and current read are smoothed over 8 readings         |
| `4`  | Voltage and current read are smoothed over 16 readings        |
| `5`  | Voltage and current read are smoothed over 32 readings        |
| `6`  | Voltage and current read are smoothed over 64 readings        |
| `7`  | Voltage and current read are smoothed over 128 readings       |
| `8`  | Voltage and current read are smoothed over 256 readings       |
| `9`  | Voltage and current read are smoothed over 512 readings       |
| `d`  | Disable output                                                |
| `D`  | Disable output and allow saving for startup                   |
| `e`  | Enable output                                                 |
| `E`  | Enable output and allow saving for startup                    |
| `S`  | Saves current settings                                        |

Any command state that can be saved permanently (e.g. `E`) needs to be followed
by `S` command in order to be saved.


---
*You can check my blog and other projects at [www.medo64.com](https://www.medo64.com/electronics/).*
