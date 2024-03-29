
[![MCHP](images/microchip.png)](https://www.microchip.com)

# Force Sensing Resistor Interface Using PIC16F17146 Microcontroller

## Introduction
This example aims to interface the Force Sensing Resistor (FSR) using the analog peripherals such as Operational Amplifier (OPA) and 12-bit Analog-to-Digital Converter with Computation (ADCC) of PIC16F17146 microcontroller. The use of internal OPA helps to reduce BoM cost and minimizes number of external components used.

## Related Documentation
- [PIC16F17146 Product Page](https://www.microchip.com/product/PIC16F17146)
- [PIC16F17146 Data Sheet](https://www.microchip.com/DS40002343)

## Software Used
- [MPLAB® X IDE v6.15](https://www.microchip.com/mplab/mplab-x-ide) or newer
- [XC8 Compiler 2.45](https://www.microchip.com/mplab/compilers) or newer
- [MPLAB® Code Configurator (MCC) 5.3.7](https://www.microchip.com/mplab/mplab-code-configurator) or newer
- [Microchip PIC16F1xxxx Series Device Support 1.20.366](https://packs.download.microchip.com/) or newer

## Hardware Used
- [PIC16F17146 Curiosity Nano Board](https://www.microchip.com/en-us/development-tool/EV72J15A)
- [Curiosity Nano base board](https://www.microchip.com/development-tool/AC164162)
- [Proto Click](https://www.mikroe.com/proto-click)
- [FSR 400 Series](https://www.interlinkelectronics.com/fsr-400-series)
- Resistor (1.5 kΩ)

## Setup
![setup](images/setup.png)

PIC16F17146 Curiosity Nano board is used as development platform in this example. Curiosity Nano Base board is used for connecting the PROTO click to the Curiosity Nano board. The Click board™, placed in click slot three, is used as general-purpose PCB to connect FSR and resistors to the OPA terminals. A feedback resistor RG of 1.5 kΩ is connected from OPA output (pin RC2) to the negative terminal of OPA (pin RA2). One terminal of FSR is connected to pin RA2 and the other terminal is grounded. OPA is connected to ADCC internally, without any external jumper cable.

### Pin Connection Table
|Microcontroller Pin| Signal Description |
|:------------------:|:-----------------:|
| RC2| OPA Output |
| RA2 |OPA Inverting Terminal* |
| RB7| UART TX|
| RB5 | UART RX |
| RC1 | LED0|

**Note:** Connection from PIC16F17146 Curiosity Nano to the click slot is not available for this pin. Jumper cable is used to make the required connections. Thus, short AN1 (RA2) and INT3 of Curiosity Nano Base Board.

## Implementation
The implementation of this example is carried out in three stages.

- Current to Voltage conversion using OPA
- Sampling of OPA output using ADCC
- Conversion of ADCC results to equivalent force and printing the results

![block_diagram](images/block-diagram.png)

### Current to Voltage Conversion Using OPA
A Force Sensing Resistor (FSR) converts the applied force into equivalent resistance. The current flowing through FSR is converted to corresponding voltage levels by the OPA module which acts as a current to voltage converter. The FSR output is connected to the negative terminal of OPA. A feedback resistor RG of 1.5 kΩ is also connected to the negative terminal of OPA. Vdd/2 is connected to the positive terminal of OPA. The voltage swing of OPA output ranges from 1.65V to 3.3V as Vdd is set to 3.3V.
The output of OPA is described by the equation:

`VOUT = VDD/2 * [1 + RG/RFSR].  ------------------------------------------------------(1)`

### Sampling of the OPA Output Using ADCC
The output of OPA is fed to the ADCC internally. The ADCC count is used to calculate the percentage of maximum force applied.

### Force Calculation
As per the specifications of the FSR (Refer [FSR Data sheet](https://www.interlinkelectronics.com/fsr-400-series)), ideally a maximum of 10N, which corresponds to 1kg, can be applied to the FSR. When no force is applied, FSR resistance is greater than 10 MΩ and output voltage of OPA is 1.65V *(Refer equation 1)*. When maximum force is applied, FSR resistance is around 1.25 kΩ and the corresponding OPA output voltage is calculated as 3.63V. As the OPA output is limited to 3.3V in this implementation, corresponding resistance of FSR is calculated approximately as 1.5 kΩ. Refer to the equation below.

`RFSR = RG/((2 * VOUT/VDD) – 1)   -----------------------------------------(2)`

Therefore, the maximum force that can be applied and sensed using this implementation is approximately limited to 700g (corresponding to 1.5 kΩ FSR resistance).

In this example, the equivalent percentage of maximum force is calculated from ADCC count and is sent to serial terminal via EUSART.
The Data visualizer is used to illustrate the results which displays the ADCC count and the percentage of maximum force that has been applied.

## Demo Operation
Force applied on the surface of FSR is sensed continuously. The ADCC count and the Force Applied are displayed using Data Visualizer. LED0 toggles every time data is sent to data visualizer using EUSART.

By default, ADCC count and Force Applied are displayed in graphical format. To set up Data Visualizer to view the result in a graphical format refer to [Steps to Open the Time Plot Window in Data Visualizer](https://github.com/microchip-pic-avr-examples/pic16f17146-force-sensing-resistor-interface-mplab-mcc#steps-to-open-time-plot-window-in-data-visualizer).

![graph](images/graph.gif)

*ADCC count* and *Force Applied* increase and decrease as per the applied force.

To view result in text format, comment the `#define GRAPH_FSR` line present in *fsr_application.c* file. To set up the Data Visualizer to view the result in terminal view refer to [Steps to Open the Terminal Window in Data Visualizer](https://github.com/microchip-pic-avr-examples/pic16f17146-force-sensing-resistor-interface-mplab-mcc#steps-to-open-terminal-window-in-data-visualizer).

![text](images/text.png)

When a connection error occurs or the PROTO click is removed from the Curiosity Nano Base board, "Hardware Fault" is printed on the terminal.

![hardware-falut](images/hardware-fault.png)

### Steps to Open the Terminal Window in Data Visualizer
To see the messages in terminal window, the Curiosity Nano board needs to be connected to terminal emulator. Data Visualizer can be used as a terminal emulator. Follow the procedure mentioned below to open the Terminal window in Data Visualizer.

Open the Data Visualizer tool which is available as a plugin in MPLAB X IDE.
1.	Open **COM7** Settings window. Set the Baud Rate to 9600.
**Note:** The COM port number can be different depending on the availability of the port.
2.	Click the right aligned triangle symbol on **COM7** tab to start capturing.

### Steps to Open the Time Plot Window in Data Visualizer
To visualize the data in graphical format, time plot of Data Visualizer is used. Follow the procedure mentioned below to open Graph/Time plot in Data Visualizer.

Open the Data Visualizer tool which is available as a plugin in MPLAB X IDE.
1.	Click Connections--> **Serial Ports** tab.
2.	Open **COM7** Settings window. Set the Baud Rate to 9600.
**Note:** The COM port number can be different depending on the availability of the port.
3.	Click the right aligned triangle symbol on **COM7** tab.
4.	Click Variable Streamer.

![data-visualizer-1](images/data-visualizer-1.png)

5.  Click New Variable Streamer.
6.	Add new variables for ADCC count (Uint16) and Force Applied (Uint8) as shown in the image below and click Save.

![data-visualizer-2](images/data-visualizer-2.png)

7.  Select source of the plot as selected CNANO port (in our case its COM7).
8.	Add plot for ADCC count and Force Applied.

![data-visualizer-3](images/data-visualizer-3.png)

## Peripheral Configuration Using MCC
This section explains how to configure the peripherals using MPLAB X IDE with MCC plugin to recreate this project.

Refer to the [Software Used](https://github.com/microchip-pic-avr-examples/pic16f17146-force-sensing-resistor-interface-mplab-mcc#software-used) section to install required tools to recreate the project.

Additional Links: [MCC Melody Technical Reference](https://onlinedocs.microchip.com/v2/keyword-lookup?keyword=MCC.MELODY.INTRODUCTION&redirect=true)

##### Peripheral Configuration Summary

| Peripherals               | Configuration                                                                                                                                                                                                                                                                                                                                                                                                  | Usage                                                                         |
|---------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------|
|    Clock Control    |    Clock source –   HFINTOSC<br>HF Internal Clock – 4 MHz<br>Clock Divider   – 1                                                                                                                                                                                                                                                                                                         |    System  clock                                                                     |
|	 Pin Settings		  |    *Pin Grid View* <br>OPA1<br>OPA1INx- : RA2 <br>OPA1OUT : RC2<br><br>EUSART1<br>RX1 : RB5<br>TX1 : RB7<br><br>GPIO<br> Output : RC1 <br> <br> *Pins* <br>RC1<br>Custom Name : LED0 <br>  																																															|    Pin Configurations																	|
|    OPA1               |    Enable Op Amp<br>Op Amp Configuration – Direct connection to pins<br>Positive Channel – Vdd/2<br>Negative Channel – OPA1IN-<br>Negative Source Selection – OPA1IN2-<br>                |    Used as current to voltage convertor of FSR output                                         |                             |
|    ADCC               |    Enable ADC<br>Input Configuration – Single-Ended mode<br>Operating mode – Burst average mode<br>Result alignment – Right justified<br>Positive reference – VDD<br>Auto-conversion trigger – TMR2<br>Acquisition count – 5 <br><br>Clock source – Fosc<br>Clock divider – Fosc/4<br><br>Threshold Interrupt Mode – Enabled <br>Repeat – 32 <br> Accumulator Right shift – 5<br><br>ADTI Interrupt Enable – Enabled<br>   |   Used for sampling of OPA output <br>   <br> |                                                              
|    TMR2               |    Custom Name - Timer2 <br>Disable Timer <br>Control Mode – Roll over pulse<br>Start/Reset Option – Software Control<br>Clock Source – MFINTOSC 32 kHz<br>Polarity – Rising edge<br>Prescaler – 1:64<br>Postscaler – 1:3<br>Time Period – 1s<br>|    Used to auto-trigger ADCC                                           |
|    UART1                | *UART1 Driver* <br> Requested Baudrate – 9600 <br>Enable Redirect STDIO to EUSART<br> UART PLIB Selector – EUSART1<br>      <br>*EUSART1 PLIB*   <br>Enable Receive<br> Enable Transmit<br>  Enable Serial Port         |    Send data to PC terminal   |

**Note:** The on-board debugger present on Curiosity Nano board has a virtual serial port (CDC) connected to EUSART on the PIC16F17146 and provides an easy way to communicate with the target application through terminal software. Refer to the [Curiosity Nano board user guide](https://www.microchip.com/DS50003388) for more details.

## Summary
This example demonstrates how a force sensor is interfaced to PIC16F17146 using a built-in OPA and ADCC peripherals and how to measure the percentage of the maximum force applied.
