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
#include "uart3.hpp"
#include "uart2.hpp"
#include "printf_lib.h"
#include "io.hpp"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include <stdint.h>
#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "masterController.hpp"
#include "player.hpp"
#include "stdio.h"
#include "led_mat.hpp"
#include "lpc_rit.h"
#include "stdbool.h"
/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */

masterController myMaster;
ledMat myMatrix;
Player myPlayer;
#define Player1_Tx_Address 180
#define Player2_Tx_Address 182

#if 0
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
#endif
/*
 * ISR for update
 */
void interruptToDisplay(void){
    myMatrix.updateDisplay();
}
bool startGame =false;
/*
 * based on switch start the game
 */
void startApplication(){
    u0_dbg_printf("game started \n");
    myMaster.createObstacle();

}
/*
 *check the switch 1 value condition
 *If the switch is pressed then turn the start condition
 */
bool  switchPressed =false;

void checkSTart(){

    if(true == SW.getSwitch(1)){
        switchPressed= true;
    }
    if(true == switchPressed)
        startGame=true;
}


/*
 * A function which increments the score
 */
int calculateScore(){

    static int score =0;
//
//    if(1 == HIT_SQUARE){
//        score++;
//        //HIT_SQUARE=0;
//    }
//
//    if(1 == HIT_CIRCLE){
//        //HIT_CIRCLE=0;
//        score=0;
//    }

     return score;

}
/*
 * A function which dislay the score on RGB LED Matrix
 */
void displayScore(int scoreL){

    LD.setNumber(scoreL);
}
/*
 * a function to do the initialization of player1 which takes the input from uart3
 */
void player1_init(){

    //Uart3::getInstance().init(38400,10,10);
    //wireless_init();
    //wireless_service();
}
/*
 * a function to do the initialization of player2 which takes the input from uart3
 */
void player2_init(){

    //Uart2::getInstance().init(38400,10,10);
   // wireless_init();
    //wireless_service();
}
/*
 * A function which takes the input from the uart and then lights up the LED
 */
void Display1(char recvdDataL){

//    static int state=0;
//    if(recvdDataL == 'l'){
//           state =1;
//    }
//    else if(recvdDataL == 'r'){
//          state =2;
//    }
   // if(state ==1 )
        //designCarleft(10,1);

   // if(state ==2 )
        //designCarRight(10,1);

    u0_dbg_printf("1:%c\n",recvdDataL);
}
/*
*A function which takes input from player 1
 */
char player1_input(){

    char player1Data = ' ';
    mesh_packet_t rcvPkt;

    if(wireless_get_rx_pkt(&rcvPkt, 1)){

        if(Player1_Tx_Address == rcvPkt.nwk.src){
            player1Data= (char)(rcvPkt.data[0]);
         }
    }
   // wireless_flush_rx();
    return player1Data;

}
/*
 * A function which takes the player2 input
 */

char player2_input(){

    char player2Data = ' ';
    mesh_packet_t rcvPkt;

    if(wireless_get_rx_pkt(&rcvPkt, 0)){
        if(Player2_Tx_Address == rcvPkt.nwk.src){
            player2Data= (char)(rcvPkt.data[0]);
        }
    }
   // wireless_flush_rx();
    return player2Data;


}

/*
 * a function which takes the input from uart and lights up led
 */
void Display2(char recvdDataL){

//    static int state=0;
//       if(recvdDataL == 'l'){
//              state =1;
//              LE.on(1);
//              LE.off(4);
//       }
//       else if(recvdDataL == 'r'){
//             state =2;
//             LE.on(4);
//             LE.off(1);
//       }
    u0_dbg_printf("2:%c\n",recvdDataL);


}
/*
 * Task to create function which takes the input from player1
 */
void Player1(void* pv){

    player1_init();

    //initRGBPins();
    //myPlayer.init(1);

    char playerOrientation;
    int score;
    Lane myLane;
    Collision myCarcollision;
    int carXcoordinate;

    while(1){
        //playerOrientation = myPlayer.getPlayerMovement(1);

       // if(startGame == true){

            playerOrientation = player1_input();

            myLane = myMaster.decideLane1(playerOrientation);

            carXcoordinate=myMaster.moveCar1(myLane);

            myCarcollision = myMaster.obstacleCollisionPlayer1(carXcoordinate);

            score = myMaster.calculateScorePlayer1(myCarcollision);

            myMaster.displayScorePlayer1(score);

     //   }

        vTaskDelay(50);
    }
}
/*
 * Task to create function which takes the input from payer2
 */
void Player2(void* pv){

    player2_init();
    //myPlayer.init(2);

    char playerOrientation;
    int score;
    Lane myLane;
    Collision myCarcollision;
    int carXcoordinate;
    Collision CollisionInput[6] ={hit_square,hit_square,hit_square,hit_square,hit_square,hit_square};
    static int temp_index=0;
    while(1){
        //if(startGame ==1){
        //playerOrientation = myPlayer.getPlayerMovement(2);

            playerOrientation=player2_input();

            //u0_dbg_printf("2:%c\n",playerOrientation);

            myLane = myMaster.decideLane2(playerOrientation);

            carXcoordinate=myMaster.moveCar2(myLane);

            myCarcollision=myMaster.obstacleCollisionPlayer2(carXcoordinate);


            score = myMaster.calculateScorePlayer2(myCarcollision);


            myMaster.displayScorePlayer2(score);
       // }

        vTaskDelay(50);

    }
}
/*
 * Task to create function which design border
 */
void Display(void* pv){

    static int temp_timer=0;
    while(1){
        myMaster.createObstacle();
        myMaster.calculateTime();
        vTaskDelay(1);
    }
}
/*
 * Task to create function which design border
 */
void obstacle(void* pv){
    myMaster.layoutInit();
    while(1){
        myMaster.moveObstacleDown();
        vTaskDelay(70);
    }

}
void check(void *pv){
    char playerOrientation1;
    char playerOrientation2;
    player1_init();
    player2_init();
    while(1){
        playerOrientation1=player1_input();
        playerOrientation2=player2_input();
        if(playerOrientation1=='l'){
            LE.on(1);
            LE.off(2);
        }
        else if(playerOrientation1=='r'){
            LE.on(2);
            LE.off(1);
        }
        if(playerOrientation2=='l'){
            LE.on(3);
            LE.off(4);
        }
        else if(playerOrientation2=='r'){
            LE.on(4);
            LE.off(3);
        }
        vTaskDelay(10);

    }
}
int main(void)
{
    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */
   wireless_init();
   wireless_service();
//    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
//    scheduler_add_task(new wirelessTask(PRIORITY_HIGH));
//    scheduler_add_task(new terminalTask(PRIORITY_LOW));
//    scheduler_add_task(new remote_wireless(PRIORITY_MEDIUM));
    //wireless_init();
    //wireless_service();

    //scheduler_add_task(new remote_wireless(PRIORITY_MEDIUM));


    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
   // scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

   ///scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_HIGH));

    xTaskCreate(Display,(const char*)"border",STACK_BYTES(1024),0,PRIORITY_CRITICAL,0);

    xTaskCreate(obstacle,(const char*)"obstacle",STACK_BYTES(1024),0,PRIORITY_CRITICAL,0);

    xTaskCreate(Player1,(const char*)"player 1",STACK_BYTES(1024),0,PRIORITY_HIGH,0);

    xTaskCreate(Player2,(const char*)"player 2",STACK_BYTES(1024),0,PRIORITY_HIGH,0);
    //xTaskCreate(check,(const char*)"check",STACK_BYTES(1024),0,3,0);

   // xTaskCreate(Player2,(const char*)"player 2",STACK_BYTES(4096),0,3,0);
   // xTaskCreate(check,(const char*)"check",STACK_BYTES(4096),0,3,0);
    //rit_enable(interruptToDisplay,1);

   // xTaskCreate(check,(const char*)"player 2",STACK_BYTES(2048),0,3,0);
   //vTaskStartScheduler();
   scheduler_start(); ///< This shouldn't return

    return -1;
}
