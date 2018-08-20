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
#include "FreeRTOS.h"
#include "LPC17xx.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include <stdint.h>
#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "stdio.h"
#include "stdbool.h"
#include "printf_lib.h"
#include "led_mat.hpp"
#include "lpc_rit.h"

ledMat mat;


/*
 ** Task to update LED Matrix.
 */
void run(void *p)
{
    while(1)
    {
        mat.updateDisplay();
        vTaskDelay(1);
    }
}


/*
 ** Update LED Matrix every 1ms by calling an interrupt.
 ** Currently, not using this. Using run task.
 */
void interruptToDisplay(void)
{
    mat.updateDisplay();
}


/*
 ** Task to display car for Player 1.
 */
void Player1Ready(void *p)
{
    uint8_t x1 = 14;         // Row: 5 (4+1 = 5)
    uint8_t x2 = 24;        // Row: 15
    uint8_t x3 = 4;        // Row: 25
    uint8_t y = 2;          // Col: 3
    uint8_t color1 = red1;
    uint8_t color2 = red1;

    mat.drawPlayerReady(2, 12, color1);


    while(1)
    {
        /*
         ** Clear car's last position.
         ** Display car at specified position.
         ** Give a delay of 1 sec before switching.
         */
        mat.clearCar(x3, y, color1, color2);
        mat.drawCar(x1, y, color1, color2);
        delay_ms(1000);

        mat.clearCar(x1, y, color1, color2);
        mat.drawCar(x2, y, color1, color2);
        delay_ms(1000);

        mat.clearCar(x2, y, color1, color2);
        mat.drawCar(x3, y, color1, color2);
        delay_ms(1000);

        vTaskDelay(1);
    }
}


/*
 ** Task to display car for Player 2.
 */
void Player2Ready(void *p)
{
    uint8_t x1 = 14;         // Row: 36 (31+1+4 = 36)
    uint8_t x2 = 24;        // Row: 46 (32+14 = 46)
    uint8_t x3 = 4;        // Row: 56 (32+24 = 56)
    uint8_t y = 2;          // Col: 3
    uint8_t color1 = blue2;
    uint8_t color2 = blue2;

    mat.drawPlayerReady(2, 12, color1);


    while(1)
    {
        /*
         ** Clear car's last position.
         ** Display car at specified position.
         ** Give a delay of 1 sec before switching.
         */
        mat.clearCar(x3, y, color1, color2);
        mat.drawCar(x1, y, color1, color2);
        delay_ms(1000);

        mat.clearCar(x1, y, color1, color2);
        mat.drawCar(x2, y, color1, color2);
        delay_ms(1000);

        mat.clearCar(x2, y, color1, color2);
        mat.drawCar(x3, y, color1, color2);
        delay_ms(1000);

        vTaskDelay(1);
    }
}


/*
 ** Task to display triangular obstacles for car 1 & 2.
 */
void showObstacleTriangle(void *p)
{
    uint8_t x = 2;
    uint8_t y = 40;
    uint8_t w = 4;
    uint8_t h = 4;


    while(1)
    {
        mat.drawObstacleTriangle(x, y, w, h, skyblue1, yellow1);
        mat.drawObstacleTriangle(x, y, w, h, skyblue2, yellow2);

        vTaskDelay(1);
    }
}


/*
 ** Task to display square obstacles for car 1 & 2.
 */
void showObstacleSquare(void *p)
{
    uint8_t x = 12;
    uint8_t y = 40;
    uint8_t s = 3;

    while(1)
    {
        mat.drawObstacleSquare(x, y, s, yellow1, skyblue1);
        mat.drawObstacleSquare(x, y, s, yellow2, skyblue2);

        vTaskDelay(1);
    }
}


/*
 ** Task to display scores for Player 1.
 */
void showScores(void *p)
{
    uint8_t count = 0;
    uint8_t count1 = 99;
    uint8_t x = 7;          // Row: 8 (7+1 = 8)
    uint8_t y = 54;         // Col: 55

    while(1)
    {

        count = (count == 100) ? 0 : count;
        count1 = (count1 == 0) ? 99 : count1;

        mat.clearNumbers(5, y, red1);
        mat.drawNumbers(5, y, count, red1);

        mat.clearNumbers(16, y, red2);
        mat.drawNumbers(16, y, count, red2);

        mat.clearTime(25, y, red1, red2);
        mat.drawTime(25, y, count1, red1, red2);

        count++;
        count1--;
        vTaskDelay(1000);
    }
}


void showDown(void *p)
{
    while (1)
    {
        mat.moveDisplayDown();
        vTaskDelay(1000);
    }
}



/*
 ** Task to update LED Matrix.
 */
void showStartCondition(void *p)
{

    mat.drawStartCondition(3,48);

    while(1)
    {
        vTaskDelay(1);
    }
}

void showCountDown(void *p)
{

    while(1)
    {
        mat.drawCountDown(2, 30);
        vTaskDelay(10);
    }
}



int main(void)
{
    mat.displayInit();

    //mat.drawGameName(10,40);
    //mat.drawStartCondition(2,40);           // To display start condition for the game.

    //mat.drawWinner(8, 40, '2');
    //mat.drawDraw(20, 36);


    //xTaskCreate(Player1Ready, "Display car", 1024, NULL, 3, NULL);     // Task to show Player 1 ready.
    //xTaskCreate(Player2Ready, "Display car", 1024, NULL, 3, NULL);     // Task to show Player 2 ready.

    //xTaskCreate(showCountDown, "Start the game", 1024, NULL, 3, NULL);
    //mat.drawStartWord(10,30);

    xTaskCreate(run, "", 1024, NULL, 3, NULL);

    mat.drawFrame();
    //mat.drawLineDotted(10, 3, cyan);
    //mat.drawLineDotted(21, 3, cyan);
    //mat.drawLine(1, 53, 30, 53, purple);
    //mat.drawLine(1, 52, 30, 52, purple);

    //mat.drawLine(31, 0, 31, 63, purple1);    // Row: 31 & 63
    //mat.drawLine(0, 0, 0, 63, purple2);    // Row: 31 & 63

   // mat.drawObstacleSquare(12, 40, 3, yellow1, skyblue1);


    //xTaskCreate(showStartCondition, "Start the game", 1024, NULL, 3, NULL);
    //xTaskCreate(showScores, "Display score numbers", 1024, NULL, 3, NULL);
    //xTaskCreate(showObstacleTriangle, "Display obstacles", 1024, NULL, 3, NULL);
    //xTaskCreate(showObstacleSquare, "Display obstacles", 1024, NULL, 3, NULL);
    //xTaskCreate(showDown, "Bring down", 1024, NULL, 3, NULL);

    // tbd: Update LED matrix every 1ms by calling rit interrupt. Not being used atm.
    //rit_enable(interruptToDisplay, 1);

    vTaskStartScheduler();

    return -1;
}
