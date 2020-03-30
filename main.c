/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  PIC24FJ128GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36b
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "mcc_generated_files/led.h"
#include "mcc_generated_files/sensors_handling.h"
#include "mcc_generated_files/cloud/cloud_service.h"
#include "mcc_generated_files/debug_print.h"

//This handles messages published from the MQTT server when subscribed
void receivedFromCloud(uint8_t *topic, uint8_t *payload)
{
	char *toggleToken = "\"LightSwitch\":";
    char *subString;
    
    if ((subString = strstr((char*)payload, toggleToken)))
    {
        LED_holdYellowOn( subString[strlen(toggleToken)] == '1' );
    }


    debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "topic: %s", topic);
    debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "payload: %s", payload);
}

// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
   static char json[200];
         
   // This part runs every CFG_SEND_INTERVAL seconds
   int rawTemperature = SENSORS_getTempValue();
   int light = SENSORS_getLightValue();
   uint8_t ledYellowStatus = LED_Yellow_Status();
   // int len = sprintf(json, "{\"id\":\"1\",\"version\":\"1.0\",\"params\":{\"Status\":1,\"Data\":\"Hello,
   // world!\",\"Light\":%d,\"Temp\":\"%d.%02d\"},\"method\":\"thing.event.property.post\"}",light,rawTemperature/100,abs(rawTemperature)%100);
   int len = sprintf(json,
	               "{\"id\":\"1\",\"version\":\"1.0\",\"params\":{\"LightSwitch\":%u,\"CurrentTemperature\":%d,\"CurrentHumidity\":\"%d."
	               "%02d\"},\"method\":\"thing.event.property.post\"}",
	               ledYellowStatus,
	               light,
	               rawTemperature / 100,
	               abs(rawTemperature) % 100);
   if (len > 0) {
      CLOUD_publishData((uint8_t *)json, len);
      //LED_flashYellow();
   }
}

#include "mcc_generated_files/application_manager.h"

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    application_init();

    while (1)
    {
        // Add your application code
        runScheduler();
    }

    return 1;
}
/**
 End of File
*/

