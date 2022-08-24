/*
© [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
 */
#include "mcc_generated_files/system/system.h"
#include "fsr_application.h"

// Uncomment below #define for graphical view of the ADCC results
//#define GRAPH_FSR

#ifdef GRAPH_FSR 
#define  TIMER2_PERIOD (17)       // For 100ms Timer period
#else
#define  TIMER2_PERIOD  (170)    // For 1s Timer period
#endif

//defines for sending data to data visualizer graph
#define START_OF_FRAME (0x5F)
#define END_OF_FRAME (0xA0)

#define ADCC_MAX_COUNT  (1650) // ADCC output varies from 0 to 1.65 (Vdd/2), ADCC reference is 2.048V
#define HARDWARE_FAULT_COUNT (ADCC_MAX_COUNT - 15) // Hardware fault will be raised once it crosses ~99% of ADCC full count

adccConversionState_t adccConversion = INITIATE;
int16_t adccBaseValue = 0;
int16_t adccOffsetValueForNoForce = 0;

void ADCC_UserInterruptHandler(void)
{
    adccConversion = COMPLETED;
}

void FSRApplication_Task(void)
{
    int16_t adccCount = 0;
    uint8_t percentageForce = 0;
    int16_t actualADCCount = 0;
    float fractionForce = 0.0;

    if (adccConversion == COMPLETED)
    {
        adccConversion = RESTART;

        LED0_Toggle(); // To verify whether conversion is done 

        adccCount = ADCC_GetFilterValue();

        if ((adccCount <= 0) || (adccCount >= HARDWARE_FAULT_COUNT))
        {
            printf("ADC Count : %d \r\n", adccCount);
            printf("Hardware Fault \r\n\n");
        }
        else
        {
            if (adccCount <= adccOffsetValueForNoForce)
            {
                percentageForce = 0;
            }
            else
            {
                actualADCCount = (adccCount - adccOffsetValueForNoForce);
                fractionForce = (float) actualADCCount / adccBaseValue;
                percentageForce = (uint8_t) (fractionForce * 100);
            }

#ifdef GRAPH_FSR      
            while (!(UART1.IsTxReady()));
            UART1.Write(START_OF_FRAME);
            while (!(UART1.IsTxReady()));
            UART1.Write(ADFLTRL);
            while (!(UART1.IsTxReady()));
            UART1.Write(ADFLTRH);
            while (!(UART1.IsTxReady()));
            UART1.Write(percentageForce);
            while (!(UART1.IsTxReady()));
            EUSART1_Write(END_OF_FRAME);
#else 
            printf("ADCC Count : %d \r\n", adccCount);
            printf("Force applied :  %u % \r\n\n", percentageForce);
#endif
        }
    }
}

void FSR_Initilialize(void)
{
    SetADCCInputChannel();

    PIE6bits.ADTIE = 0; // Disable ADCC threshold interrupt.

    Timer2.PeriodCountSet(TIMER2_PERIOD); // Set ADCC auto trigger interval

    Timer2.Start(); //Start ADCC sampling

    while (!(PIR6bits.ADTIF)); // Wait for ADCC conversion to complete

    adccOffsetValueForNoForce = ADCC_GetFilterValue();

    adccBaseValue = (ADCC_MAX_COUNT - adccOffsetValueForNoForce);

    ADCC_SetADTIInterruptHandler(ADCC_UserInterruptHandler);

    PIR6bits.ADTIF = 0;

    PIE6bits.ADTIE = 1; // Enable ADCC threshold interrupt
}

void SetADCCInputChannel(void)
{
    ADPCH = pChannel_OPA1OUT; // Set Positive Channel

    ADNCH = pChannel_DAC1OUT; // Set Negative Channel
}

