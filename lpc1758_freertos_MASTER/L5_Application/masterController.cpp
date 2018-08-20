/*File          :MasterController.cpp
 *Author        :Sneha Shahi
 *Email         :snehashahi900@gmail.com
 *Description   :This file contains the description for API required for the
 *               drivers that Master module used for designing the algorithm to
 *               make decision for player module and drive the RGB LED matrix API
 */
#include "masterController.hpp"
#include "printf_lib.h"
#include "io.hpp"
#include "led_mat.hpp"
#include "lpc_sys.h"
#include "soft_timer.hpp"
volatile bool GAME_OVER_PLAYER1=false;
volatile bool GAME_OVER_PLAYER2=false;
static int count=0;
ledMat matrix;
/*
 * create obstacle and make sure that proper distance is maintained
 */
void masterController::createObstacle(){

    static int timer500=0;
    static int timer650=0;
    static int timer750=0;
    static int indexObstaclePath2=0;
    static int indexObstaclePath1=0;
    static int indexObstaclePath3=0;

    matrix.updateDisplay();
    if(GAME_OVER_PLAYER1 == false && GAME_OVER_PLAYER2 == false){

    if( timer650 == 0){
        if(indexObstaclePath1 ==0){

            matrix.drawObstacleSquare(2, 49, 3, yellow1, green1);
            //matrix.drawObstacleSquare(2, 49, 3, yellow1, green1);
        }
        if(indexObstaclePath1 ==1){
            matrix.drawObstacleTriangle(14, 49, 4,4, skyblue1, yellow1);
            //matrix.drawObstacleTriangle(2, 49, 4,4, skyblue1, yellow1);

        }
        if(indexObstaclePath1 ==2){
            matrix.drawObstacleSquare(24, 49, 3, yellow1, green1);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath1 ==3){
            matrix.drawObstacleTriangle(3, 49, 4,4, skyblue1, yellow1);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath1 ==4){
            matrix.drawObstacleSquare(16, 49, 3, yellow1, green1);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath1 ==5){
            matrix.drawObstacleTriangle(23, 49, 4,4, skyblue1, yellow1);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
    }
    if( timer500== 0){
        if(indexObstaclePath2 ==0){

            matrix.drawObstacleSquare(2, 49, 3, yellow2, green2);
            //matrix.drawObstacleSquare(2, 49, 3, yellow1, green1);
        }
        if(indexObstaclePath2 ==1){
            matrix.drawObstacleTriangle(14, 49, 4,4, skyblue2, yellow2);
            //matrix.drawObstacleTriangle(2, 49, 4,4, skyblue1, yellow1);

        }
        if(indexObstaclePath2 ==2){
            matrix.drawObstacleSquare(24, 49, 3, yellow2, green2);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath2 ==3){
            matrix.drawObstacleSquare(1, 49, 3, yellow2, green2);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath2 ==4){
            matrix.drawObstacleSquare(16, 49, 3, yellow2, green2);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
        if(indexObstaclePath2 ==5){
            matrix.drawObstacleTriangle(26, 49, 4,4, skyblue2, yellow2);
            //matrix.drawObstacleSquare(25, 49, 3, yellow1, green1);

       }
    }
//    if( timer500 == 0){
//        if(indexObstaclePath2 ==0){
//            matrix.drawObstacleSquare(14, 49, 3, yellow1, green1);
//            matrix.drawObstacleSquare(14, 49, 3, yellow2, green2);
//
//        }
//        if(indexObstaclePath2 ==1){
//
//
//            matrix.drawObstacleTriangle(15, 49, 4,4, skyblue1, yellow1);
//             matrix.drawObstacleTriangle(15, 49, 4,4, skyblue2, yellow2);
//
//        }
//        if(indexObstaclePath2 ==2){
//            matrix.drawObstacleSquare(24, 49, 3, yellow1, green1);
//            matrix.drawObstacleSquare(25, 49, 3, yellow2, green2);
//
//        }
//    }
//
//    if( timer750 == 0){
//        if(indexObstaclePath3 ==0){
//            matrix.drawObstacleSquare(24, 49, 3, yellow1, green1);
//            matrix.drawObstacleSquare(25, 49, 3, yellow2, green2);
//
//        }
//        if(indexObstaclePath3 ==1){
//
//            matrix.drawObstacleTriangle(24, 49, 4,4, skyblue1, yellow1);
//            matrix.drawObstacleTriangle(25, 49, 4,4, skyblue2, yellow2);
//
//        }
//        if(indexObstaclePath3 ==2){
//
//            matrix.drawObstacleSquare(24, 49, 3, yellow1, green1);
//            matrix.drawObstacleSquare(25, 49, 3, yellow2, green2);
//        }
//    }

    timer500++;
    timer650++;
    timer750++;

    indexObstaclePath1++;
    indexObstaclePath2++;
    indexObstaclePath3++;

    if(indexObstaclePath3==6)
        indexObstaclePath3=0;

    if(indexObstaclePath2==6)
        indexObstaclePath2=0;

    if(indexObstaclePath1==6)
        indexObstaclePath1=0;

    if(timer650 >300)
        timer650=0;

    if(timer500 >300)
        timer500=0;

    if(timer750 >530)
        timer750=0;

    }
}

/*
 * obstacle collision
 */
Collision masterController::obstacleCollisionPlayer1(int carCoordinate){

    Collision myCar;
    myCar =matrix.checkHitPlayer1(carCoordinate);
    return myCar;
}
/*
 * obstacle collision
 */
Collision masterController::obstacleCollisionPlayer2(int carCoordinate){

    Collision myCar;
    myCar =matrix.checkHitPlayer2(carCoordinate);
    return myCar;
}
/*
 * display the score
 */
void masterController::displayScorePlayer1(int score){

    // LD.setNumber(score);
    static int scoreNew=0;
    static int counttemp=0;


    if(GAME_OVER_PLAYER1 == false && count<91){
        scoreNew=score/5;
        if(count>=80){
            matrix.drawNumbers(7, 54, scoreNew, red1);
            counttemp++;
            if(counttemp>10){
                matrix.clearNumbers(7, 54,red1);
                counttemp=0;
             }
        }
        else{
            matrix.clearNumbers(7, 54,red1);
            matrix.drawNumbers(7, 54, scoreNew, red1);
        }


    }


}
/*
 * display the score
 */
void masterController::displayScorePlayer2(int score){

    //LD.setNumber(score);
    static int scoreNew=0;
    static int counttemp=0;
    scoreNew=score/5;
    if(GAME_OVER_PLAYER2 == false && count<91){
        scoreNew=score/5;
        if(count>=80){
                    matrix.drawNumbers(7, 54, scoreNew, red2);
                    counttemp++;
                    if(counttemp>10){
                        matrix.clearNumbers(7, 54,red2);
                        counttemp=0;
                     }
                }
                else{
                    matrix.clearNumbers(7, 54,red2);
                    matrix.drawNumbers(7, 54, scoreNew, red2);
                }
    }
}
/*
 * calculate the score
 */
int masterController::calculateScorePlayer1(Collision carCollision){

    static int score=0 ;
    if(carCollision==hit_square)
        score++;
    if(carCollision==hit_triangle){
        if(score>0)
        score=score-1;
    }

    if(carCollision==no_collide)
        score=score*1;

    return score;


}
int masterController::calculateScorePlayer2(Collision carCollision){

    static int score=0 ;
       if(carCollision==hit_square)
           score++;
       if(carCollision==hit_triangle ){
           if(score>0)
           score=score-1;
       }
       if(carCollision==no_collide)
           score=score*1;

    return score;

}
/*
 * Decide car lane left or right based on the player input
 * @ Orientation the orientation message which player is sending
 */
Lane masterController::decideLane1(char Orientation){

    static Lane myLane =straight;

    switch(Orientation){

        case 'l':myLane=left;
                 break;

        case 'r':myLane =right;
                 break;

        case 's':myLane =straight;
                 break;
    }

    return myLane;
}
/*
 * Decide car lane left or right based on the player input
 * @ Orientation the orientation message which player is sending
 */
Lane masterController::decideLane2(char Orientation){
    static Lane myLane =straight;

    switch(Orientation){

        case 'l':myLane=left;
                 break;

        case 'r':myLane =right;
                 break;

        case 's':myLane =straight;
                 break;
    }
    return myLane;
}
/*
 * move car left or right based on the player input
 * @ carLane the car lane at which player wants to move
 * @ PlayerNum The player number who is requesting to change lane
 */
int masterController::moveCar1(Lane carLane){
    int nOnce=0;
    static uint8_t x1 = 4;         // Row: 36 (31+1+4 = 36)
    static uint8_t x2 = 14;        // Row: 46 (32+14 = 46)
    static uint8_t x3 = 24;        // Row: 56 (32+24 = 56)
    uint8_t y = 2;          // Col: 3
    uint8_t color1 = red1;
    uint8_t color2 = red1;
    Collision myCar;
    //static playerlane myLane =initialCondition;
    if(GAME_OVER_PLAYER1 == false ){
    switch(carLane){

        case left:  //matrix.drawCar(4, 2, 32, 0);
                    if(x2 >1){
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(x2-1, y, color1, color2);
                        x2--;
                    }
                    else{
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(1, y, color1, color2);
                    }


                    break;

        case right: //matrix.drawCar(14, 2, 32, 0);
                    if(x2 < 24){
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(x2+1, y, color1, color2);
                        x2++;
                    }
                    else{
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(24, y, color1, color2);
                    }
                    //u0_dbg_printf("r:%d\n",x2);
//                    LE.on(2);
//                    LE.off(1);
                    break;

        case straight: //matrix.drawCar(24, 2, 32, 0);
                    if(nOnce ==0){
                       // matrix.drawCar(x2, y, color1, color);
                        matrix.drawCar(14, y, color1, color2);
                        nOnce=1;
                    }
                    matrix.clearCar(14, y, color1, color2);
                    matrix.drawCar(x2, y, color1, color2);
                    //u0_dbg_printf("s:%d\n",x2);

                    break;
    }
    }
    return x2;
}

int masterController::moveCar2(Lane carLane){
    int nOnce=0;
    static uint8_t x1 = 4;         // Row: 36 (31+1+4 = 36)
    static uint8_t x2 = 14;        // Row: 46 (32+14 = 46)
    static uint8_t x3 = 24;        // Row: 56 (32+24 = 56)
    uint8_t y = 2;          // Col: 3
    uint8_t color1 = blue2;
    uint8_t color2 = blue2;
    if(GAME_OVER_PLAYER2 == false){
    //static playerlane myLane =initialCondition;
    switch(carLane){

        case left:  //matrix.drawCar(4, 2, 32, 0);
                    if(x2 >1){
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(x2-1, y, color1, color2);
                        x2--;
                    }
                    else{
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(1, y, color1, color2);
                    }
                    //u0_dbg_printf("l:%d\n",x2);
//                    LE.on(3);
//                    LE.off(4);
                    break;

        case right: //matrix.drawCar(14, 2, 32, 0);
                    if(x2 < 24){
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(x2+1, y, color1, color2);
                        x2++;
                    }
                    else{
                        matrix.clearCar(x2, y, color1, color2);
                        matrix.drawCar(24, y, color1, color2);
                    }
                    //u0_dbg_printf("r:%d\n",x2);
//                    LE.on(4);
//                    LE.off(3);
                    break;

        case straight: //matrix.drawCar(24, 2, 32, 0);
                    if(nOnce ==0){
                       // matrix.drawCar(x2, y, color1, color);
                        matrix.drawCar(14, y, color1, color2);
                        nOnce=1;
                    }
                    matrix.clearCar(14, y, color1, color2);
                    matrix.drawCar(x2, y, color1, color2);

                    //u0_dbg_printf("s:%d\n",x2);
                    break;
    }
    }
    return x2;
   // matrix.checkHit(0,x2);
}
/*
 * Initialises the layout for the matrix
 */
void masterController::layoutInit(){
    matrix.displayInit();
    matrix.drawFrame();
    matrix.drawLineDotted(10,3,63);
    matrix.drawLineDotted(21,3,63);
}
/*
 * Move the obstacle down
 */
void masterController::moveObstacleDown(){

    if(GAME_OVER_PLAYER1 == false && GAME_OVER_PLAYER2 == false)
    matrix.moveDisplayDown();

}
/*
 * Function calculate time,every 1 second update LD display
 */
void masterController::calculateTime(void){
    static uint64_t currentTime=0 ;
    static uint64_t initialtime=0;
    static uint64_t difftime=0;


    currentTime= sys_get_uptime_ms();
    difftime = currentTime-initialtime;
    if(GAME_OVER_PLAYER1 == false && GAME_OVER_PLAYER2 == false){
    if(difftime >1000){
        count++;
        LD.setNumber(count);
        difftime=0;
        initialtime=currentTime;
    }
    }
    if(count ==90){
        GAME_OVER_PLAYER1=true;
        GAME_OVER_PLAYER2=true;
    }

}
