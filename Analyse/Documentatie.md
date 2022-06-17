# Documentatie Robotwagen
### Jelte Boumans - 2ITIOT

# Probleemstelling
Ontwerp een nieuwe en betere versie van een robotwagentje. Deze moet uitgerust worden met een "main" bordje en een sensorbordje. Dit wagentje zal dan gebruikt moeten worden door leerlingen van het eerste jaar voor het vak IoT expiriments.

# As-Is Situatie
### Oud robotwagentje: 
<div "display=flex;"> 
  <img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Old%20car1.png" width="300"/>
  <img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Old%20car2.png" width="300"/>
  <img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Old%20car3.png" width="300"/>
</div>

### Specificaties:
- ESP32 DevKit V1
- LM7805 Voltage regulator
- SN754410 Dual H-bridge motor driver
- HC-SR04 Ultrasonic afstand sensor
- LCD + I2C controller
- 4 LEDs (1x Rood, 2x Geel en 1x Groen)
- 7V LiPo batterij
- Extra connectors voor I2C, 5V, 3V3, GND, GPIO, ...

# To-be Situatie
### Inhoud:
- Mindmap
- Hardware analyse
- Software analyse
- Schema's
- PCB's

# Mindmap
Dit is een mindmap van alle doelen die het nieuwe robotwagentje moet bereiken. Het is verdeeld in een hardware en software gedeelte. In beide delen laat ik de benodigdheden zien.

<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Mindmap.png"/>


# Hardware analyse
Een analyse van de gemaakte keuzes voor de hardware.

## Blokdiagram
In dit diagram zie je alle componenten voorgesteld als blokken, je kan ook zien hoe elke groep componenten met elkaar zullen communiceren en samenwerken.

<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Blokschema.png"/>

## Specificatie tabel
Hier is een tabel met elk component van het vorige blokdiagram. Over elk component wordt er wat info getoond.

| Component                 | U      | I       | Winkel                    | Prijs (06/05/2022) |
| ------------------------- | ------ | ------- | ------------------------- | ------------------ |
| ESP32                     | 3V3    | 0.5A    | [ESP32-WROVER-I](https://www.tinytronics.nl/shop/nl/communicatie-en-signalen/draadloos/wi-fi/modules/esp32-wrover-wifi-module-pcb-antenne-aansluiting)        | €5.50              |
| ATMega328P                | 5V     | 15mA    | [ATMEGA328P-AN](https://be.farnell.com/microchip/atmega328p-an/mcu-8bit-atmega-20mhz-tqfp-32/dp/2443178)         | €3.25              |
| Dual motor driver         | 3V3 | 16.5mA  | [TC78H621FNG](https://be.farnell.com/toshiba/tc78h621fng-o-el/motor-driver-ic-h-bridge-tssop/dp/3869999?st=motordriver)           | €0.70              |
| Dual motor driver         | 7V | 1.1A  | [TC78H621FNG](https://be.farnell.com/toshiba/tc78h621fng-o-el/motor-driver-ic-h-bridge-tssop/dp/3869999?st=motordriver)           | €0.70              |
| OLED Display              | 3V3    | 15mA    | [OLED Display](https://www.tinytronics.nl/shop/nl/displays/oled/0.96-inch-oled-display-128*64-pixels-wit)          | €7.00              |
| USB-C                     | /      | /       | [USB4085-GF-A](https://be.farnell.com/gct-global-connector-technology/usb4085-gf-a/usb-connector-2-0-type-c-rcpt/dp/2924867?st=usb%20c%202.0)          | €0.70              |
| USB to UART bridge        | 3V3    | 20mA    | [CP2102N-A02-GQFN28R](https://be.farnell.com/silicon-labs/cp2102n-a02-gqfn28r/usb-uart-bridge-40-to-85deg-c/dp/3387003?st=cp2102n-a02-gqfn28r)   | €2.53              |
| Batterij                  | 7V     | 1200mAh | [Conrad LiPo accupack ](https://www.conrad.be/nl/p/conrad-energy-lipo-accupack-7-4-v-2400-mah-aantal-cellen-2-20-c-softcase-xt60-1344133.html?t=1&utm_source=google&utm_medium=surfaces&utm_term=1344133&utm_content=free-google-shopping-clicks&utm_campaign=shopping-feed&vat=true&gclid=CjwKCAjwxOCRBhA8EiwA0X8hi6Dpvaew0u-kTnIyrUmKE2RRHzkksSaw41QoJ36AmjOCY1n-dP7VnRoC240QAvD_BwE&gclsrc=aw.ds&tid=13894944235_122657379817_pla-301443522443_pla-1344133&WT.srch=1) | €20.24             |
| 5V converter              | 7V     | /       | [LD1117AS50TR]()          | €?                 |
| 3V3 converter             | 7V     | /       | [LD1117AS33TR](https://www.digikey.com/en/products/detail/stmicroelectronics/LD1117AS33TR/585752)          | €1.04              |
| Lijnvolger                | 5V     | ?       | [MIR-3.0Y](https://www.tinytronics.nl/shop/en/sensors/optical/infrared/8x-ir-line-tracking-module-40mm-range)              | €6.00              |
| Ultrason afstand sensor   | 5V     | 2mA     | [HC-SR04](https://www.tinytronics.nl/shop/nl/sensoren/afstand/ultrasonische-sensor-hc-sr04)               | €3.00              |
| IR afstand sensor         | 5V     | 30mA    | ?                         | €?                 |
| Crystal oscillator        | /      | /       | [HC49SM-16](https://be.farnell.com/multicomp/hc49sm-16-30-50-60-16-atf/crystal-hc-49-4hsmx-16-0mhz/dp/1667017?st=16mhz%20crystal)             | €0.50              |

# Software analyse
## Data In -en Outputs
### Main bordje 

| Component    | Input                              | Output                                                                            |
| ------------ | ---------------------------------- | --------------------------------------------------------------------------------- |
| ESP          | UART via USB, I2C van sensorbordje | Low power PWM naar motor driver, I2C requests naar sensor bordje en SPI naar OLED |
| Motor driver | Low power PWM van ESP              | High power PWM naar de motoren                                                    |
| OLED Display | SPI van ESP                        | /                                                                                 |

### Sensorbordje

| Component       | Input                                                                                                  | Output                            |
| --------------- | ------------------------------------------------------------------------------------------------------ | --------------------------------- |
| ATMega          | Analoge waarde van IR sensor, pulsen van ultrason sensor, clock van crystal en een byte van lijnvolger | I2C data naar ESP                 |
| Lijnvolger      | 5V voeding                                                                                             | Byte digitale waardes naar ATMega |
| IR sensor       | 5V voeding                                                                                             | Analoge waarde naar ATMega        |
| Ultrason sensor | Puls van ATMega (trigger)                                                                              | Puls naar ATMega (echo)           |

## Flowcharts
Hier zijn mijn flowcharts van de software voor het sensor bordje en de ESP. Voor de ESP is het manueel besturen en automatisch rijden verdeeld in aparte charts. Maar de chart voor automatisch rijden is nog niet gemaakt.

### Sensor bordje
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Flowchart%20Sensor%20Bord.png"/>

### ESP bordje
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Flowchart%20ESP%20Bord.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Flowchart%20ESP%20Bord%20manueel.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Flowchart%20ESP%20Bord%20automatisch.png"/>

## State Diagrams
Hier zijn mijn state diagrams voor van de software voor het sensor bordje en de ESP. Voor de ESP is de callback een aparte diagram voor de leesbaarheid te behouden.

### Sensor bordje
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/State%20diagram%20Sensor%20bord.png"/>

### ESP bordje
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/State%20diagram%20ESP%20bord.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/State%20diagram%20ESP%20bord%20callback.png"/>

## NodeRED Mockups
Hier zijn mijn ruwe schetsen hoe ik het NodeRED controlepaneel zou designen voor de manuele controle en automatisch rijden.
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Mockup%20Automatisch.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Mockup%20Manueel.png"/>

## NodeRED Dashboard
Hier is een screenshot van het eindresultaat van het NodeRed Dashboard
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Node%20Red%20Dashboard.png">

## Behuizing Mockups
Hier zijn mijn ruwe schetsen hoe ik de behuizing van de auto zou designen.
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Behuizing%20Mockup%201.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/Behuizing%20Mockup%202.png"/>

# Schema's
Dit is de 4de iteratie van mijn schema.
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ESP%20Schema.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ATMega%20Schema.png"/>

# PCB's
Dit is de 4de iteratie van mijn PCB.
<div "display=flex;"> 
  <img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ESP%20PCB.png"/>
  <img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ATMega%20PCB.png"/>
</div>

# Resultaat
Dit zijn de resultaten van de schemas:
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ESP%20PCB%20Scan.png"/>
<img src="https://raw.githubusercontent.com/JelteBoumansAP/Smart-Car/main/Analyse/Fotos/ATMega%20PCB%20Scan.png"/>
