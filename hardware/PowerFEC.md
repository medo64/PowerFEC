### PowerFEC Parts

|  # | Part                                      | RefDes  | Preferred Part Number         |
|---:|-------------------------------------------|---------|-------------------------------|
|  1 | C 10nF X7R 16V (0805)                   † | C1      | 399-17617-1-ND                |
|  3 | C 100nF X7R 16V (0805)                    | C2-C4   | 478-5311-1-ND                 |
|  1 | C 100nF X7R 16V (0805)                  † | C5      | 478-5311-1-ND                 |
|  1 | C 1uF 16V X7R (0805)                      | C6      | 1276-6470-1-ND                |
|  2 | C 10uF X5R 16V (0805)                     | C7-C8   | 1276-1096-1-ND                |
|  2 | DS LED (0805)                             | DS1-DS2 | 475-1415-1-ND                 |
|  1 | P USB C, plug, straddle 0.8mm             | J1      | WM12855-ND                    |
|  1 | J JST XH Vertical (2w)                    | J2      | 455-B2B-XH-A-ND               |
|  1 | L Ferrite 600R@100Mhz                     | L1      | 240-2390-1-ND                 |
|  1 | R 0.1, 0.5%, 0.5W (1206)                  | R1      | 273-KDV12DR100ETCT-ND         |
|  4 | R 1K 0.125W (0805)                        | R2-R5   | RMCF0805FT1K00CT-ND           |
|  2 | R 5.1K 0.125W (0805)                      | R6-R7   | RMCF0805FT5K10CT-ND           |
|  2 | R 5.1K 0.125W (0805)                    † | R8-R9   | RMCF0805FT5K10CT-ND           |
|  1 | R 56K 0.125W (0805)                       | R10     | RMCF0805FT56K0CT-ND           |
|  1 | U PIC16F1455 (SOIC-14)                    | U1      | PIC16F1455-I/SL-ND            |
|  1 | U AP22652                                 | U2      | 31-AP22652AW6-7CT-ND          |
|  1 | U ZXCT1009 (SOT23)                        | U3      | ZXCT1109QSA-7DICT-ND          |
|  1 | U MCP3422 (SOIC-8)                      † | U4      | MCP3422A1T-E/SNCT-ND          |
|  2 | H Screw M2x3mm                            | -       | -                             |

† optional 18-bit ADC


#### Board Size

|       |      Dimensions | Area    | Thickness |
|-------|-----------------|---------|-----------|
| PCB   |  24.0 x 27.9 mm | 1.1 in² |    0.8 mm |


#### ADC

ADC uses internally available PIC `1.024 V` voltage reference and all
measurements are done in `10 bits` (optionally, at 18-bit resolution).

Since current sense resistor is `100 mΩ`, having `1 kΩ` resistor on Iout gives
us nice 1:1 V/A ratio. Resolution is about `0.98 mA` per bit (at 18-bit,
`3.8 uA` per bit with 2x gain).

Voltage measurement for output uses `5.1 kΩ` and `1 kΩ` voltage divider thus
having 6.1:1 ratio. Resolution is about `6 mV` per bit (at 18-bit, `23.3 uV` per
bit with 2x gain).


#### Specification

|                 | Minimum | Typical | Maximum |
|-----------------|--------:|--------:|--------:|
| Input voltage:  |         |   5.0 V |         |
| Input current:  |         |  560 mA |         |
| Output voltage: |  4.75 V |   5.0 V |  545 mA |
| Output current: |  490 mA |  525 mA |  545 mA |
