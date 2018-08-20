/*File         : MasterController.hpp
 *Author       : Sneha Shahi
 *Email        : snehashahi900@gmail.com
 *Description  :
 */

#ifndef MASTERCONTROLLER_HPP_
#define MASTERCONTROLLER_HPP_
#include "led_mat.hpp":
typedef enum{
    left,
    right,
    straight,
}Lane;

typedef enum{
    initialCondition,
    player1left,
    player1right,
    player2left,
    player2right,
}playerlane;
//
//typedef enum{
//   no_collision,
//   hit_square,
//   hit_circle,
//}Collision;

struct score{
    int Player1score;
    int Player2score;
};

class masterController{

        public:

                void createObstacle();

                Collision  obstacleCollisionPlayer1(int carCoordinate);

                Collision  obstacleCollisionPlayer2(int carCoordinate);

                void displayScorePlayer1(int score);
                void displayScorePlayer2(int score);

                int  calculateScorePlayer1(Collision carCollision);
                int  calculateScorePlayer2(Collision carCollision);

                Lane decideLane1(char Orientation);

                Lane decideLane2(char Orientation);
                int moveCar1(Lane carLane);
                int moveCar2(Lane carLane);

                void moveObstacleDown(void);

                void calculateTime(void);

                void layoutInit(void);

        private:

};




#endif /* MASTERCONTROLLER_HPP_ */
