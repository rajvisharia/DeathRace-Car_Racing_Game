/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 *          FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 *          @see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "io.hpp"
#include "stdio.h"
#include "printf_lib.h"
#include "math.h"
#include "FreeRTOS.h"
#include "handlers.hpp"
#include "FreeRTOSConfig.h"
#include "uart3.hpp"
#include "uart2.hpp"

#define Player1_Tx_Address 180
#define Player2_Tx_Address 182
#define Master_Rx_Address 181
//Uart2 &data_uart2 = Uart2::getInstance();
//Uart3 &data_uart3 = Uart3::getInstance();

QueueHandle_t q;


/*
 *  Enumeration containing left and right orientation parameters declared.
 */
typedef enum
{
    l,          // Left
    r,          // Right
    s,          // Straight/Up
    i           // Invalid
}direction_t;


/*
 *  Function sendPlayerCarDirection:
 *  Polls  accelerometer values.
 *  Sends direction of the Player board to the queue every second.
 */
void sendPlayerCarDirection(void *p)
{
    //direction_t steer;
    char steer;
    double angle;


    /*
     ** Instructions on how to hold the board:
     ** For this particular angle value, hold the board in such a way that
     ** X-axis is pointing to the right, and Y-axis pointing in the front direction.
     ** Also, you should be able to correctly read the LED display values facing you.
     ** Tilt the board to the left or right, to get left & right orientation value.
     */
    while(1)
    {
        int16_t x = AS.getX();      // Get X-axis values
        int16_t y = AS.getY();      // Get Y-axis values
        int16_t z = AS.getZ();      // Get Z-axis values
        //u0_dbg_printf("X: %d, Y: %d, Z: %d\n", x, y, z);


        /*
         ** With the above placement of board and tilting left or right,
         ** We rotate the board across the Y-axis. Thus, Y-axis remains constant.
         ** Only x and Z axes change. So, Angle depends upon x and z.
         */
        angle = sin(z/x);           // Compute the angle sin(z/x)
        //u0_dbg_printf("Angle:%f\n", angle);

        char out;

        // LEFT: (x is positive) AND (angle = 0).
        if ((angle == 0) && (x > 0))
        {
            steer = 'l';
            LE.on(1);
            LE.off(4);
            LE.off(2);
            //u0_dbg_printf("Left.\n");
           // data_uart3.putChar(steer, portMAX_DELAY);
            //data_uart2.putChar(steer, portMAX_DELAY);
        }

        // RIGHT: (x is negative) AND (angle = 0).
        else if ((angle == 0) && (x < 0))
        {
            steer = 'r';
            LE.on(4);
            LE.off(1);
            LE.off(2);
            //u0_dbg_printf("Right.\n");
           // data_uart3.putChar(steer, portMAX_DELAY);
            //data_uart2.putChar(steer, portMAX_DELAY);
        }

        // UP/Straight: (z is positive) AND (-1 < angle < 1).
        else if((angle > -1) && (angle < 1) && (angle != 0) && (z > 0))
        {
            steer = 's';
            LE.on(2);
            LE.off(1);
            LE.off(4);
            //u0_dbg_printf("Straight/Up.\n");
            //data_uart3.putChar(steer, portMAX_DELAY);
            //data_uart2.putChar(steer, portMAX_DELAY);
        }

        // Invalid orientation or up/down orientation.
        else
        {
            steer = 'i';
            //u0_dbg_printf("Hold the board correctly..\n");
          //  data_uart3.putChar(steer, portMAX_DELAY);
           // data_uart2.putChar(steer, portMAX_DELAY);
        }
       // u0_dbg_printf("%c\n",steer);
        if(wireless_send(Master_Rx_Address, mesh_pkt_ack, &steer, sizeof(steer), 0)==0)
            LE.on(3);
        //xQueueSend(q, &steer, 0);

        vTaskDelay(0);
    }
}



int main(void)
{
    // Queue handle is not valid until you create it
    // q = xQueueCreate(20, sizeof(int));


    int baud_rate   = 38400;    // Baud rate for UART communication.
    int tx_que_size = 30;      // Transmit queue size. Default: 64
    int rx_que_size = 30;     // Receive queue size. Default: 32

    // data_uart2.init(baud_rate, rx_que_size, tx_que_size);   // UART2 initialized.
    //data_uart3.init(baud_rate, rx_que_size, tx_que_size);   // UART3 initialized.
    wireless_init();
    wireless_service();

    TaskHandle_t xHandle = NULL;
    //scheduler_add_task(new terminalTask(PRIORITY_HIGH));

        /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
   scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    // Initialize the on-board accelerometer sensor.
    bool start = AS.init();

    /*
     ** If initialized, create the tasks.
     ** If not initialized, then do nothing and print
     */
    if (!start)
        u0_dbg_printf("Accelerometer has not yet been initialized!\n");
    else
    {
        xTaskCreate(sendPlayerCarDirection, "Sends orientation values", 1024, (void *) 1, PRIORITY_CRITICAL, &xHandle);
        //xTaskCreate(consumer, "Waits for orientation values", 1024, (void *) 1, 1, &xHandle);
    }

    // Start Scheduler - This will not return, and your tasks will start to run their while(1) loop.
    //vTaskStartScheduler();
    scheduler_start(); ///< This shouldn't return

    return 0;
}
