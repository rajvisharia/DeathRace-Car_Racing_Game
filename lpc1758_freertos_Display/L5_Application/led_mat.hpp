/*
 * led_mat.hpp
 *
 *  Created on: Apr 28, 2018
 *      Author: Supradeep-PC
 */

#ifndef LED_MAT_HPP_
#define LED_MAT_HPP_

#define MATRIX_HEIGHT 64
#define MATRIX_WIDTH 64
#define MATRIX_NROWS 32
#include "stdint.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

typedef enum{
   no_collide,
   hit_square,
   hit_triangle,
}Collision;

/*
uint8_t black = 0;
uint8_t blue1 = 2;
uint8_t blue2 = 16;
uint8_t blue = 18;
uint8_t cyan1 = 7;
uint8_t cyan2 = 56;
uint8_t cyan = 63;
uint8_t green1 = 1;
uint8_t green2 = 8;
uint8_t green = 9;
uint8_t purple1 = 6;
uint8_t purple2 = 48;
uint8_t purple = 54;
uint8_t red1 = 4;
uint8_t red2 = 32;
uint8_t red = 36;
uint8_t skyblue1 = 3;
uint8_t skyblue2 = 24;
uint8_t skyblue = 27;
uint8_t yellow1 = 5;
uint8_t yellow2 = 40;
uint8_t yellow = 45;
*/


typedef enum
{
    black = 0,

    blue1 = 2,
    blue2 = 16,
    blue = 18,

    cyan1 = 7,
    cyan2 = 56,
    cyan = 63,

    green1 = 1,
    green2 = 8,
    green = 9,

    purple1 = 6,
    purple2 = 48,
    purple = 54,

    red1 = 4,
    red2 = 32,
    red = 36,

    skyblue1 = 3,
    skyblue2 = 24,
    skyblue = 27,

    yellow1 = 5,
    yellow2 = 40,
    yellow = 45

}color_codes;

class ledMat
{
public:

    void displayInit();

    void drawGameName(uint8_t x, uint8_t y);
    void drawStartCondition(uint8_t x, uint8_t y);
    void drawPlayerReady(uint8_t x, uint8_t y, uint8_t color);
    void drawCountDown(uint8_t x, uint8_t y);
    void drawStartWord(uint8_t x, uint8_t y);

    void drawFrame();

    void drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void clearPixel(uint8_t x, uint8_t y, uint8_t color);
    void clearDisplay(uint8_t color);

    void updateDisplay();
    void moveDisplayDown();

    void drawCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void clearCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void moveCar(bool leftRight);

    void drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t color);
    void drawLineDotted(uint8_t x, uint8_t w, uint8_t color);

    void drawDigit(uint8_t x, uint8_t y, uint8_t value, uint8_t color);
    void drawNumbers(uint8_t x, uint8_t y, uint8_t value, uint8_t color);
    void clearNumbers(uint8_t x, uint8_t y, uint8_t color);
    void drawTime(uint8_t x, uint8_t y, uint8_t value, uint8_t color1, uint8_t color2);
    void clearTime(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);

    void drawLetter(uint8_t x, uint8_t y, char alpha, uint8_t color);
    void drawText(uint8_t x, uint8_t y, uint8_t color);

    void drawObstacleSquare(uint8_t x, uint8_t y, uint8_t s, uint8_t color, uint8_t color1);
    void drawObstacleCircle(uint8_t x, uint8_t y, uint8_t color);
    void drawObstacleTriangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color1,uint8_t color2);


    //void checkHit(uint8_t x1, uint8_t x2);

    Collision checkHitPlayer1(uint8_t x);
    Collision checkHitPlayer2(uint8_t x);
    //void drawCrash(uint8_t x, uint8_t y, uint8_t color);
    void drawCrash(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void drawWinner(uint8_t x, uint8_t y, char player_number);
    void drawDraw(uint8_t x, uint8_t y);

private:

};



#endif /* LED_MAT_HPP_ */
