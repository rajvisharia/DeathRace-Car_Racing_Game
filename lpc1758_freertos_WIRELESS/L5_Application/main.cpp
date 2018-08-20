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
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include"printf_lib.h"
#include<stdio.h>

/*Nordic Wireless, Player & Master integrated*/
#if 0
/*Player Sender*/
#define Master_Rx_Address 180

class remote_wireless: public scheduler_task
{
    public:
        remote_wireless(uint8_t priority) :
                scheduler_task("SampleWireless", 2048, priority)
        {
        }

        bool run(void *p)
        {
            int16_t x = AS.getX();      // Get X-axis values
            int16_t y = AS.getY();      // Get Y-axis values
            int16_t z = AS.getZ();      // Get Z-axis values
            double angle = sin(z/x);
            mesh_packet_t ackPkt;
            char steer;

            // LEFT: (x is positive) AND (angle = 0).
            if ((angle == 0) && (x > 0))
            {
                steer = 'l';
                u0_dbg_printf("Left.\n");
                //data_uart3.putChar(steer, portMAX_DELAY);

            }

            // RIGHT: (x is negative) AND (angle = 0).
            else if ((angle == 0) && (x < 0))
            {
                steer = 'r';
                u0_dbg_printf("Right.\n");
                //data_uart3.putChar(steer, portMAX_DELAY);
            }

            // UP/Straight: (z is positive) AND (-1 < angle < 1).
            else if((angle > -1) && (angle < 1) && (angle != 0) && (z > 0))
            {
                steer = 's';
                u0_dbg_printf("Straight/Up.\n");
                //data_uart3.putChar(steer, portMAX_DELAY);
            }

            // Invalid orientation or up/down orientation.
            else
            {
                steer = 'i';
                u0_dbg_printf("Hold the board correctly..\n");
                //data_uart3.putChar(steer, portMAX_DELAY);
            }


            if (wireless_send(Master_Rx_Address, mesh_pkt_ack, &steer, sizeof(steer), 2))
            {
                puts("Sent 2 Master");
                if(wireless_get_ack_pkt(&ackPkt,100) && Master_Rx_Address==ackPkt.nwk.src)
                {
                    puts("\nReceived ACK from Master");
                }
             }
             else
                 puts("\n send failed");

            vTaskDelay(500);
            return true;
        }
    private:
};
#endif


/*Master Receiver*/
#define Player1_Tx_Address 181
#define Player2_Tx_Address 182
class remote_wireless: public scheduler_task
{
    public:
        remote_wireless(uint8_t priority) :
                scheduler_task("SampleWireless", 2048, priority)
        {
        }

        bool run(void *p)
        {
            //const char max_hops = 2;

            mesh_packet_t rcvPkt, ackPkt;
            //gConsole_message_t messageToSend, messageReceived;
            int timeout_ms = 500;

            while (wireless_get_rx_pkt(&rcvPkt, timeout_ms))
            {
                u0_dbg_printf("Received data from %i\n", rcvPkt.nwk.src);
                // Identifying Player 1 or 2 has sent the data
                if(Player1_Tx_Address == rcvPkt.nwk.src)
                    {
                        for (int i = 0; i < rcvPkt.info.data_len; i++)
                            {
                                putchar(rcvPkt.data[i]);
                            }
                        u0_dbg_printf("\n");
                    }

                if(Player2_Tx_Address == rcvPkt.nwk.src)
                    {
                        for (int i = 0; i < rcvPkt.info.data_len; i++)
                            {
                                putchar(rcvPkt.data[i]);
                            }
                        u0_dbg_printf("\n");
                    }

            }
            if (wireless_get_ack_pkt(&ackPkt, 100) && Player1_Tx_Address == ackPkt.nwk.src)
            {
                u0_dbg_printf(" Received ACK from Player1\n");
            }

            if (wireless_get_ack_pkt(&ackPkt, 100) && Player2_Tx_Address == ackPkt.nwk.src)
            {
                u0_dbg_printf(" Received ACK from Player2\n");
            }
            wireless_flush_rx();

            vTaskDelay(500);
            return true;
        }
    private:
};


int main()
{
        wireless_init();
        wireless_service();
        scheduler_add_task(new wirelessTask(PRIORITY_HIGH));
        scheduler_add_task(new terminalTask(PRIORITY_LOW));
        scheduler_add_task(new remote_wireless(PRIORITY_MEDIUM));
        scheduler_start();
}



