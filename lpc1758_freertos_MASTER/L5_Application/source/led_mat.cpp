/*
 * led_mat.cpp
 *
 *  Created on: Apr 28, 2018
 *      Author: Supradeep-PC
 */

#include "FreeRTOS.h"
#include "LPC17xx.h"
#include"led_mat.hpp"
#include <stdint.h>
#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "stdio.h"
#include "stdbool.h"
#include "printf_lib.h"
#include "string.h"
#include "stdlib.h"


GPIO R1(P1_19);
GPIO G1(P1_20);
GPIO B1(P1_22);
GPIO R2(P1_23);
GPIO G2(P1_28);
GPIO B2(P1_29);

GPIO A(P1_30);
GPIO B(P1_31);
GPIO C(P2_0);
GPIO D(P2_1);
GPIO E(P2_2);

GPIO clk(P2_3);
GPIO lat(P2_4);
GPIO oe(P2_5);

uint8_t matrixbuff[MATRIX_NROWS][MATRIX_HEIGHT];

/*
 ** Initialize pins as output/input, high/low.
 */
void ledMat::displayInit()
{
    R1.setAsOutput();
    G1.setAsOutput();
    B1.setAsOutput();
    R2.setAsOutput();
    G2.setAsOutput();
    B2.setAsOutput();
    A.setAsOutput();
    B.setAsOutput();
    C.setAsOutput();
    D.setAsOutput();
    E.setAsOutput();
    clk.setAsOutput();
    lat.setAsOutput();
    oe.setAsOutput();

    R1.setLow();
    G1.setLow();
    B1.setLow();
    R2.setLow();
    G2.setLow();
    B2.setLow();
    A.setLow();
    B.setLow();
    C.setLow();
    D.setLow();
    E.setLow();
    clk.setLow();
    lat.setLow();
    oe.setLow();

    memset(matrixbuff, 0, MATRIX_HEIGHT * MATRIX_NROWS);
}


/*
 ** Function to glow a pixel.
 */
void ledMat::drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    matrixbuff[x][y] |= color;
}


/*
 ** Function to clear a pixel.
 */
void ledMat::clearPixel(uint8_t x, uint8_t y, uint8_t color)
{
    matrixbuff[x][y] &= ~(color);
}


/*
 ** Function to update the display matrix.
 */
void ledMat::updateDisplay()
{
    for(uint8_t row = 0; row < 32; row++)
    {
        for(uint8_t col = 0; col < 64; col++)
        {

            if(matrixbuff[row][col] & 0x1)
            {
                B1.setHigh();
            }
            else
            {
                B1.setLow();
            }
            if(matrixbuff[row][col] & 0x2)
            {
                G1.setHigh();
            }
            else
            {
                G1.setLow();
            }
            if(matrixbuff[row][col] & 0x4)
            {
                R1.setHigh();
            }
            else
            {
                R1.setLow();
            }
            if(matrixbuff[row][col] & 0x8)
            {
                B2.setHigh();
            }
            else
            {
                B2.setLow();
            }
            if(matrixbuff[row][col] & 0x10)
            {
                G2.setHigh();
            }
            else
            {
                G2.setLow();
            }
            if(matrixbuff[row][col] & 0x20)
            {
                R2.setHigh();
            }
            else
            {
                R2.setLow();
            }

            clk.setHigh();
            clk.setLow();
        }

        oe.setHigh();
        lat.setHigh();

        A.setLow();
        B.setLow();
        C.setLow();
        D.setLow();
        E.setLow();

        if(row & 0x1)
        {
            A.setHigh();
        }

        if(row & 0x2)
        {
            B.setHigh();
        }

        if(row & 0x4)
        {
            C.setHigh();
        }

        if(row & 0x8)
        {
            D.setHigh();
        }

        if(row & 0x10)
        {
            E.setHigh();
        }

        lat.setLow();
        oe.setLow();
    }
}


/*
 ** Function to draw line between 2 points.
 */
void ledMat::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
      int16_t steep = abs(y1 - y0) > abs(x1 - x0);
      if (steep) {
        swap(x0, y0);
        swap(x1, y1);
      }

      if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
      }

      int16_t dx, dy;
      dx = x1 - x0;
      dy = abs(y1 - y0);

      int16_t err = dx / 2;
      int16_t ystep;

      if (y0 < y1) {
        ystep = 1;
      } else {
        ystep = -1;
      }

      for (; x0<=x1; x0++) {
        if (steep) {
          drawPixel(y0, x0, color);
        } else {
          drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
          y0 += ystep;
          err += dx;
        }
      }
}


/*
 ** Function to display dotted lanes.
 */
void ledMat::drawLineDotted(uint8_t x, uint8_t w, uint8_t color)
{
    for (int i = 10; i < 52; )
    {
        drawLine(x, i, x, i+w-1, color);
        i +=5;
    }
}
void ledMat::moveDisplayDown()
{
    static uint8_t count = 0;
    uint8_t x, y;

    for (x=1; x<31; x++)
    {
        for (y=7; y<53; y++)
        {
            if (y < 52)
            {
                matrixbuff[x][y] = matrixbuff[x][y+1];
            }

            if (y == 52)
            {
                clearPixel(x, y, 63);

                count = (x == 9) ? count+1 : count;

                if (x == 10 || x == 21)
                {

                    if (count == 4 || count ==5)
                    {
                        clearPixel(x, y, 63);
                        count = (count == 5) ? 0 : count;
                    }
                    else
                    {
                        drawPixel(x, y,cyan);
                    }

                }
            }

        }
    }
}

/*
 ** Function to display the border grid and center partition.
 */
void ledMat::drawFrame()
{
    drawLine(0, 0, 0, 63, purple);      // Row: 0 & 32
    //drawLine(1, 0, 1, 63, 6);       // Row: 1
    drawLine(31, 0, 31, 63, purple);    // Row: 31 & 63
    //drawLine(30, 0, 30, 63, 48);    // Row: 62

    drawLine(0, 0, 31, 0, purple);      // Column: 0
    drawLine(1, 53, 30, 53, purple);      // Column: 0
    drawLine(0, 63, 31, 63, purple);    // Column: 63
}


/*
 ** Function to display circular obstacle.
 ** Not using this for the moment.
 */
void ledMat::drawObstacleCircle(uint8_t x, uint8_t y, uint8_t color)
{
    drawPixel(x, y+2, color);
    drawPixel(x-1, y+1, color); drawPixel(x, y+1, color); drawPixel(x+1, y+1, color);
    drawPixel(x-2, y, color); drawPixel(x-1, y, color); drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color);
    drawPixel(x-1, y-1, color); drawPixel(x, y-1, color); drawPixel(x+1, y-1, color);
    drawPixel(x, y-2, color);

}


/*
 ** Function to draw triangular obstacle.
 */
void ledMat::drawObstacleTriangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color1, uint8_t color2)
{
    // Hollow Triangle
    drawLine(x, y, x+w, y, color1);    // Bottom row
    drawPixel(x, y+1, color1);
    drawPixel(x+w, y+1, color1);
    drawPixel(x+1, y+2, color1);
    drawPixel(x+w-1, y+2, color1);
    drawPixel(x+w/2, y+3, color1);

    // To fill triangle
    drawLine(x+1, y+1, x+w-1, y+1, color2);
    drawPixel(x+2, y+2, color2);

}


/*
 ** Function to draw square obstacle.
 */
void ledMat::drawObstacleSquare(uint8_t x, uint8_t y, uint8_t s, uint8_t color, uint8_t color1)
{
    //drawPixel(x+1, y,color1);  drawPixel(x+2,y, color1);   drawPixel(x+3, y, color1);  drawPixel(x+4, y, color1);
    //drawPixel(x+1,y+1,color1); drawPixel(x+2,y+1,color1);  drawPixel(x+3,y+1,color1);  drawPixel(x+4,y+1,color1);
    //drawPixel(x+1,y+2,color1); drawPixel(x+2,y+2,color1);  drawPixel(x+3,y+2,color1);  drawPixel(x+4,y+2,color1);
    //drawPixel(x+1,y+3,color1); drawPixel(x+2,y+3,color1);  drawPixel(x+3,y+3,color1);  drawPixel(x+4,y+3,color1);
    //drawPixel(x+1,y+4,color1); drawPixel(x+2,y+4,color1);  drawPixel(x+3,y+4,color1);  drawPixel(x+4,y+4,color1);

    drawLine(x, y, x+s, y, color);
    drawLine(x, y, x, y+s, color);
    drawLine(x+s, y, x+s, y+s, color);
    drawLine(x, y+s, x+s, y+s, color);
    drawPixel(x+1,y+1, color1);
    drawPixel(x+2, y+1, color1);
    drawPixel(x+1,y+2, color1);
    drawPixel(x+2, y+2, color1);
}


/*
 ** Function to display car.
 */
void ledMat::drawCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2)
{
        drawPixel(x, y,color1);                                                 drawPixel(x+3, y, color1);
                             drawPixel(x+1,y+1,color1);  drawPixel(x+2,y+1,color1);
                             drawPixel(x+1,y+2,color1);  drawPixel(x+2,y+2,color1);
                             drawPixel(x+1,y+3,color1);  drawPixel(x+2,y+3,color1);
        drawPixel(x,y+4,color1);                                                drawPixel(x+3,y+4,color1);

}


/*
 ** Function to clear car's last position.
 */
void ledMat::clearCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2)
{
    for(uint8_t i = x; i <= (x+3); i++)
    {
        for(uint8_t j = y; j <= (y+4); j++)
        {
            clearPixel(i, j, color1);
            //clearPixel(i, j, color2);     // If we are using 2 colors for car, clear other colour too.
        }
    }
}


/*
 ** Function to display numbers on the screen.
 */
void ledMat::drawNumbers(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color)
{
    uint8_t num[2] = {};

    num[0] = value / 10;
    num[1] = value % 10;

    for(uint8_t i = 0; i < 2; i++)
    {
        x0 = (i == 1)? x0+13 : x0;

        if(num[i] == 0)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+2, color);
            drawPixel(x0, y0+3, color);
            drawPixel(x0, y0+4, color);
            drawPixel(x0, y0+5, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+4, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
        }
        else if(num[i] == 1)
        {
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+6, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+1, color);
            drawPixel(x0+2, y0+2, color);
            drawPixel(x0+2, y0+3, color);
            drawPixel(x0+2, y0+4, color);
            drawPixel(x0+2, y0+5, color);
            drawPixel(x0+2, y0+6, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
        }
        else if(num[i] == 2)
        {
            drawPixel(x0, y0, color);
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+2, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+3, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+3, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
        }
        else if(num[i] == 3)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+4, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+4, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
        }
        else if(num[i] == 4)
        {
            drawPixel(x0, y0+2, color);
            drawPixel(x0, y0+3, color);
            drawPixel(x0, y0+4, color);
            drawPixel(x0+1, y0+2, color);
            drawPixel(x0+1, y0+5, color);
            drawPixel(x0+2, y0+2, color);
            drawPixel(x0+2, y0+6, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+1, color);
            drawPixel(x0+3, y0+2, color);
            drawPixel(x0+3, y0+3, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+2, color);
        }
        else if(num[i] == 5)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+5, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0, y0+7, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+4, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+4, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+7, color);
        }
        else if(num[i] == 6)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+2, color);
            drawPixel(x0, y0+3, color);
            drawPixel(x0, y0+4, color);
            drawPixel(x0, y0+5, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+4, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+4, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+6, color);
        }
        else if(num[i] == 7)
        {
            drawPixel(x0, y0+7, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+1, color);
            drawPixel(x0+2, y0+2, color);
            drawPixel(x0+2, y0+3, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
            drawPixel(x0+4, y0+7, color);
        }
        else if(num[i] == 8)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+2, color);
            drawPixel(x0, y0+3, color);
            drawPixel(x0, y0+5, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+4, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+4, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+4, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
        }
        else if(num[i] == 9)
        {
            drawPixel(x0, y0+1, color);
            drawPixel(x0, y0+4, color);
            drawPixel(x0, y0+5, color);
            drawPixel(x0, y0+6, color);
            drawPixel(x0+1, y0, color);
            drawPixel(x0+1, y0+3, color);
            drawPixel(x0+1, y0+7, color);
            drawPixel(x0+2, y0, color);
            drawPixel(x0+2, y0+3, color);
            drawPixel(x0+2, y0+7, color);
            drawPixel(x0+3, y0, color);
            drawPixel(x0+3, y0+3, color);
            drawPixel(x0+3, y0+7, color);
            drawPixel(x0+4, y0+1, color);
            drawPixel(x0+4, y0+2, color);
            drawPixel(x0+4, y0+3, color);
            drawPixel(x0+4, y0+4, color);
            drawPixel(x0+4, y0+5, color);
            drawPixel(x0+4, y0+6, color);
        }
    }
}


/*
 ** Function to clear numbers at specific location.
 */
void ledMat::clearNumbers(uint8_t x0, uint8_t y0, uint8_t color)
{
    for(uint8_t i = 0; i < 5; i++)
    {
        for(uint8_t j = 0; j < 8; j++)
        {
            clearPixel(i+x0, j+y0, color);
            clearPixel(i+x0+13, j+y0, color);
        }
    }
}
/*
 * for collision
 */
Collision ledMat::checkHitPlayer1(uint8_t x1)
{
    uint8_t temp=11;
    Collision mycarCollision;
    for (int i=x1; i <= (x1+3); i++)
    {
        temp = matrixbuff[i][7];
        if ( (temp & 0x07) == yellow1)
        {

            mycarCollision = hit_square;
            break;
            ///return mycarCollision;
        }
        if ( (temp & 0x07) == skyblue1)
        {
            mycarCollision = hit_triangle;
            break;
           // return mycarCollision;
        }
        else
        {
            mycarCollision=no_collide;
        }
    }
    return mycarCollision;
}
Collision ledMat::checkHitPlayer2(uint8_t x2)
{
    uint8_t temp=11;
    Collision mycarCollision;


    for (int i=x2; i <= (x2+3); i++)
    {
        temp = matrixbuff[i][7];

        if ( (temp & 0x38) == yellow2)
        {
            mycarCollision = hit_square;
            break;
            //return mycarCollision;
        }
        if ( (temp & 0x38) == skyblue2)
        {
            mycarCollision = hit_triangle;
            break;
            //return mycarCollision;

        }
        else
        {
            mycarCollision=no_collide;
        }
    }
    return mycarCollision;
}

