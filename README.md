Full project video can be found at: https://youtu.be/Ek4EhuD-6MM

[![Alcoholic Russian Roulette Gun](https://raw.githubusercontent.com/EngineeringDads/Alcoholic-Russian-Roulette/main/Images/received_2897660557159924.webp)](https://youtu.be/Ek4EhuD-6MM "Alcoholic Russian Roulette Gun")

# :gun: Alcoholic Russian Roulette :cocktail:

Modified version of the alcohol shot gun. It's a game inspired by the traditional russian roulette, with a slight kink. The game uses an arduino nano to select a random player/s in a game where an alcohol shot in loaded. When it is the selected players turn, a piezo buzzer indicates that player is receiving the shot, before a servo motor pulls the trigger of the gun. The game is capped at a maximum of 15 players and must have more than one player to proceed.

<img src="https://raw.githubusercontent.com/EngineeringDads/Alcoholic-Russian-Roulette/main/Images/Gun%20pull.gif" width="350" height="200"> <img src="https://raw.githubusercontent.com/EngineeringDads/Alcoholic-Russian-Roulette/main/Images/Intro%20message.jpg" width="350" height="200">
<img src="https://raw.githubusercontent.com/EngineeringDads/Alcoholic-Russian-Roulette/main/Images/Player.jpg" width="350" height="200"> <img src="https://raw.githubusercontent.com/EngineeringDads/Alcoholic-Russian-Roulette/main/Images/Selections.jpg" width="350" height="200">

## Materials

1. Customised box to mount to the gun (we used a 3D printer)
2. Arduino Nano
3. Servo Motor
4. Piezoelectric Buzzer
5. 3 x Push buttons
6. I2C LCD Screen
7. Mount for the Servo (3D Printed)
8. On/Off Switch
9. Alcoholic Shot Gun
10. Wire to pull the trigger
11. 9V Battery

## Composition

Below is the schematic of the electronic components. The piezoelectric buzzer did not use a transistor, rather it was an output directly to the arduino nano. All of the components were soldered directly to the arduino board, and placed in the junction box. Resistors were also not necessary, as we took advantage of the 'Resistor Pullup' function in the boards internal resistors. The servo motor was attached in bolted into a separate mount, which was attached to the shaft of the gun and fed into the side of the junction box. A 9V battery was then attached to the back of the junction box to power the arduino.

<img src="https://github.com/EngineeringDads/Alcoholic-Russian-Roulette/blob/main/Images/Circuit.JPG?raw=true" width="700" height="400">

## How it works

To program the game, we used the Arduino IDE. The LCD screen menu and selections were characterised by a 'count' for the number of times a button was selected. Different combinations of these selections dictated which menu was to be viewed, and how many players and shots per game. The select button was then used to send a character off to an analog seed, which would set the number of players and shots.

To duplicate this project, open the [I'm an inline-style link with title](https://github.com/EngineeringDads/Alcoholic-Russian-Roulette/blob/main/Alcoholic%20Russian%20Roulette.ino "Alcoholic Russian Roulette.ino") and upload it to the board, using your computer and a mini-usb cable.

The microcontroller will need to be switched on and off before proceeding to play another game. There are failsafes in the code that prevent you from playing on your own, having more shots in the game than players, going over 15 players and having zero shots. You will not be able to acheived the actions just mentioned, as the game will not make sense otherwise. The controller is constantly seeding "0"'s. It is not until you decide how many players are in the game that it locks in that number of "0"'s. When you decide the number of shots, it creates that number of "1"'s and randomly allocates it to a position in the array. E.G. if you pick 3 players and 1 shot, you would have an array that looks like "010", where the 1 is randomly placed and the second player would get 'shot'. Each time the select button is pressed, the controller moves a position in the array and if a one is detected, it sends a signal for the servo to pull the trigger of the gun. Once all players have had their turn, the LCD screen will display "game over".
