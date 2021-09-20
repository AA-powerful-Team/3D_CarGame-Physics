# Physics library implementations mini games

Projects created for Physics II subject. 
- First game Re-created is a 2D Pinball Game done with BoxD library. 
- Second game created is a racing track Mini-Game with obstacles, time trial & jumps. Done with Bullet 3D library

Both games created only with C and C++ and the implmented libraries.

These projects have been done by two students of CITM - UPC (Barcelona, Spain).

# ROLLING X PINBALL

Project for Physics II subject. The goal is to recreate a pinball game with BoxD library. 

This project has been done by two students of CITM - UPC (Barcelona, Spain). 

* [Github repository](https://github.com/AA-powerful-Team/Pinball_physics)

         https://github.com/AA-powerful-Team/Pinball_physics

## About the game 

The original pinball game can be found [here] (http://www.freewebarcade.com/game/rollin-x-pinball/index.php?act=Arcade&do=newscore).

         http://www.freewebarcade.com/game/rollin-x-pinball/index.php?act=Arcade&do=newscore

 ## Installation instructions 

Download the Zip file and extract it. Open the folder and double click on the executable (.exe) 

_IMPORTANT: do not modify, change or add any folder or file, or else the game may not work correctly._ 

## Controls Player controls: 

```markdown

- RIGHT FLIPPER: Right arrow key 
- LEFT FLIPPER: Right left key 
- LAUNCH BALL: Down arrow key (hold to charge kicker)

General/developer controls: 

- EXIT GAME: ESC key or click close button in window 
- DEBUG MODE & DRAW OBJECTS: F1 key 
- CREATE BALL: 1 key 
- MOVE BALL: click and drag the ball around while on debug mode
- A: restart the game 


 ```

**CAUTION: the game is not thought to be played in debug mode. Some bugs may appear while on debug mode. 

## How to play 

The goal of the game is score as many points as you can before losing four balls. 

You may score by: 
* Impacting one of the two circle bouncers. 
* Impacting one of the two triangular bouncers. 
* Impacting the blue bouncer after the tunnel. 
* Going in the 2x or 4x entrance, multiplying your score by the  
same amount. 
* Entering the Ball lock ramp. 


# Combos 

```markdown

* Going through large tunnel on the left part of the screen, activating all lights. 
* Touching the 3 sensor right of the circle bouncers. 
* Touching the 3 above sensors that lead to the bouncers. 
* Touching the 3 above sensors that lead to the bouncers, plus the uppermost sensor. 
* Touching the upper sensor and the one in the middle of the stage, connecting the upper and lower part. 
* Touching the 5 small sensors in the lower part of the board. 

 ```

**CAUTION: losing the ball and touching the sideways sensors will rest you points.

Score and balls left can be seen on window title.

## Game Video

<html>
<body>
	
<iframe width="1031" height="580" src="https://www.youtube.com/embed/y39BIvoNypc" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

</body>
</html>


## Game Differences 

* Ball doesn't disappear when entering the 2x and 4x ramps or add falling velocity. 
* Kickers when the ball tries to fall in the sideways rails. 
* No instructions shown. 
* Score differs from the original. 
* Lights do not disappear with time, just when ball falls or the combo is performed. 

## Team members 

_Adrian Font Romero Github account:_ 
* [Github account](https://github.com/AdrianFR99) 

_Andrés Ricardo Saladrigas Pérez Github account:_ 
* [Github account](https://github.com/TheArzhel) 


## Tools used 
* Microsoft Visual Studio 2017 
* Language: C++ * Graphics and audio: SDL
2.0.4 
* Physics: Box2D 
* Code repository: GitHub 
* Others: Adobe Photoshop and Audacity 


### Disclaimer

All sprites and sounds are property of the games creator


# PrimitiveRacing3D

Project for Physics II subject. The goal is to create a racing game using Bullet 3D library

This project has been done by two students of CITM - UPC (Barcelona, Spain). 

	
* [Github repository](https://github.com/AA-powerful-Team/3D_CarGame-Physics) 
     
       https://github.com/AA-powerful-Team/3D_CarGame-Physics
	
### Installation

## Installation instructions 

Download the Zip file and extract it. Open the folder and double click on the executable (.exe) 

_IMPORTANT: do not modify, change or add any folder or file, or else the game may not work correctly._ 

Download [Here](https://github.com/AA-powerful-Team/3D_CarGame-Physics/releases) 
    
       https://github.com/AA-powerful-Team/3D_CarGame-Physics/releases
       
### Controls


```markdown
- E: Start game

Arrows:
- UP: Accelerate
- RIGHT: Turn Right 
- LEFT: Turn Left 

- LEFT CONTROL: brake.
- SPACE: Flip car when turn over
- A: restart the game cycle
- P: look backwards

General/developer controls: 

- EXIT GAME: ESC key or click close button in window 
- DEBUG MODE & DRAW OBJECTS: F1 key 
- CLICK AND DRAG: Move camera

 ```

## How to play 

The goal of the game is to make 3 laps before 240 seconds (4 min) and avoid getting out of the road.

Lose: get out of the road or make 3 laps with a time greater then 240 sec.

Win: make 3 laps in less than 240 seconds.


## Game Video

<html>
<body>
	
<iframe width="938" height="750" src="https://www.youtube.com/embed/qY-RwlDkLss" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

</body>
</html>

# Objects 

* Joints with movement. 
* Statick obstacle
* Joints wiithout movement.
* Start line sensor
* Boost sensor

## Team members 

_Adrian Font Romero Github account:_ 
* [Github account](https://github.com/AdrianFR99) 

_AndrÃ©s Ricardo Saladrigas PÃ©rez Github account:_ 
* [Github account](https://github.com/TheArzhel) 


## Tools used 
* Microsoft Visual Studio 2017 
* Language: C++ * Graphics and audio: SDL 2.0.4 
* Physics: Bullet3D 
* Code repository: GitHub 
* Others: Adobe Photoshop and Audacity 


### License


Licensed under the [MIT License](https://github.com/AA-powerful-Team/3D_CarGame-Physics/blob/master/LICENSE.txt)

	https://github.com/AA-powerful-Team/3D_CarGame-Physics/blob/master/LICENSE.txt




