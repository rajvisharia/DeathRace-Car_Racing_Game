# DeathRace-Car_Racing_Game
SJSU CMPE 244 (Embedded Software) Project

Death Race - A Car Racing Game running on FreeRTOS

Introduction:
It is a two-player, real-time car racing game running on FreeRTOS using SJOne board (NXP LPC1758). There are dedicated race tracks for each player and the player can shift horizontally on the track. The players can move horizontally on the track (RGB LED Matrix Display) by tilting the Player consoles (SJone boards), either left or right. Each player will encounter two kinds of obstacles during the game - square and triangular obstacles. The score of the player is incremented for collecting the square boxes and decremented every time when it hits the triangular obstacles. The obstacles are generated randomly and the criticality of the game (randomness of obstacles) increases with time. The individual player score will be displayed and updated from time to time on the LED Display Matrix. The game ends at the end of 60 seconds and the last 10 seconds of the game are indicated by changing the color of the score to red and blinking them. The player with a greater score wins the game.

About the game:
* 2-Player Car Racing Game
* 60 seconds game timer
* Press switch on the console (SJOne board) to start
* Tilt the console left or right to move the car
* Collect square obstacles
* Avoid triangular obstacles
* Higher score player wins after the timeout

Link to project details and more information: http://socialledge.com/sjsu/index.php/S18:_Death_Race

Link to GitLab project repository: https://gitlab.com/Magnumopus244/DeathRace

Link to project video: https://www.youtube.com/watch?v=jbGvOdyTFEc&feature=youtu.be

Technologies used:
* FreeRTOS
* C/C++
* Device drivers
* LED display Matrix
* PCB design
* GIT
