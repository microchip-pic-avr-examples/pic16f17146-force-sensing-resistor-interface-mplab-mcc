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
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FSR_APPLICATION_H
#define	FSR_APPLICATION_H

#include <xc.h> // include processor files - each processor file is guarded.  

/**
  Section: FSR Application APIs
*/

/**
 * @ingroup fsr_application
 * @brief This API initializes the FSR sensor. This routine must be called on power up to calculate the offset count of ADCC,
 * when no force is applied on FSR sensor.
 * @param none
 * @return none
*/

void FSR_Initilialize(void);

/**
 * @ingroup fsr_application
 * @brief This API measures the force applied on FSR sensor and converts it to equivalent percentage of
 * maximum force and displays on terminal/graphical format using data visualizer.
 * @param none
 * @return none
*/
void FSRApplication_Task(void);

/**
 * @ingroup fsr_application
 * @brief This API sets the positive input channels of ADCC
 * @param channel - ADCC positive channel
 * @return none
*/
void SetADCCPositiveChannel(adcc_channel_t channel);

/**
 * @ingroup fsr_application
 * @brief User ISR function to indicate that ADCC conversion has completed
 * @param none
 * @return none
*/
void ADCC_UserInterruptHandler(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* FSR_APPLICATION_H */

