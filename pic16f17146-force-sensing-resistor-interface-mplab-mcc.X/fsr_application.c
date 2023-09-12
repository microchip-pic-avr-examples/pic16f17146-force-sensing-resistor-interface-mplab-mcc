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

// Macros for sending data to data visualizer graph
#define START_OF_FRAME (0x5F)
#define END_OF_FRAME (0xA0)

#define ADCC_MAX_COUNT  (4095) // 12-bit ADCC in single ended mode
#define ADCC_COUNT_WHEN_NO_CLICK (ADCC_MAX_COUNT - 40) // ADCC count when proto click is not connected (99% of ADCC_MAX_COUNT)

// Different state of ADCC conversion
typedef enum {
        RESTART,
        COMPLETED,
        INITIATE
}adccConversionState_t;

adccConversionState_t adccConversion = INITIATE;
uint16_t adccCountSpan = 0;
uint16_t adccCountAtNoForce = 0;

void FSR_Initilialize(void)
{
    SetADCCPositiveChannel(channel_OPA1OUT);

    PIE6bits.ADTIE = 0; // Disable ADCC threshold interrupt 
    PIR6bits.ADTIF = 0; // Clear ADCC threshold interrupt flag

    ADCON0bits.ADGO = 1; // Start ADCC conversion
    while (!(PIR6bits.ADTIF)); // Wait for ADCC conversion to complete
    
    adccCountAtNoForce = ADCC_GetFilterValue();
    adccCountSpan = ADCC_MAX_COUNT - adccCountAtNoForce;

    ADCC_SetADTIInterruptHandler(ADCC_UserInterruptHandler);

    PIR6bits.ADTIF = 0; // Clear ADCC threshold interrupt flag
    PIE6bits.ADTIE = 1; // Enable ADCC threshold interrupt
    
    Timer2.PeriodCountSet(TIMER2_PERIOD); // Set ADCC auto trigger interval
    Timer2.Start(); //Start ADCC sampling
}

void SetADCCPositiveChannel(adcc_channel_t channel)
{
    ADPCH = channel;
}

void FSRApplication_Task(void)
{
    uint16_t adccCount = 0;
    uint16_t effectiveADCCCount = 0; // Actual ADCC count effective for applied force
    float fractionForce = 0.0;
    uint8_t percentageForce = 0;

    if (adccConversion == COMPLETED)
    {
        adccConversion = RESTART;

        LED0_Toggle(); // To indicate whether conversion is done 

        adccCount = ADCC_GetFilterValue();

        if (adccCount >= ADCC_COUNT_WHEN_NO_CLICK)
        {
            printf("ADCC Count : %u \r\n", adccCount);
            printf("Hardware Fault \r\n\n");
        }
        else
        {
            if (adccCount <= adccCountAtNoForce)
            {
                percentageForce = 0;
            }
            else
            {
                effectiveADCCCount = adccCount - adccCountAtNoForce;
                fractionForce = (float) effectiveADCCCount / adccCountSpan;
                percentageForce = (uint8_t) (fractionForce * 100);
            }

#ifdef GRAPH_FSR      
            while (!(UART1.IsTxReady()));
            UART1.Write(START_OF_FRAME);
            while (!(UART1.IsTxReady()));
            UART1.Write((uint8_t)(adccCount)); 
            while (!(UART1.IsTxReady()));
            UART1.Write((uint8_t)(adccCount >> 8)); 
            while (!(UART1.IsTxReady()));
            UART1.Write(percentageForce);
            while (!(UART1.IsTxReady()));
            EUSART1_Write(END_OF_FRAME);
#else 
            printf("ADCC Count : %u \r\n", adccCount);
            printf("Force applied :  %u %% \r\n\n", percentageForce);
#endif
        }
    }
}

void ADCC_UserInterruptHandler(void)
{
    adccConversion = COMPLETED;
}