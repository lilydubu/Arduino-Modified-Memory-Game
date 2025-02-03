# Arduino Memory Game

This is a modified version of the classic memory game implemented on an Arduino platform. In this version, each round introduces a random sequence that the player must repeat, gradually increasing in difficulty. The game uses an LCD display for visual feedback and a buzzer for audio cues.

## Features:
- **LCD Display**: 
  - Press "PRESS TO START" to begin the game.
  - Displays the current round number.
  - Indicates if it’s the player’s turn to press.
  - Provides feedback on whether the player's press is correct or wrong.
  - If the player loses, the display shows the correct sequence.
  
- **Buzzer**:
  - Each button has a unique tone to indicate a correct press.
  - Plays a song when the player wins or loses.

- **Debugging**:
  - Updates on the serial monitor for game states and player inputs.
  - Button-presses will sound/light up before the game starts.

- **Game Mechanics**:
  - The game is based on the classic memory game but with an added challenge: each round features a random sequence of increasing length.
  - The player must repeat the sequence correctly to advance to the next round. If the player makes a mistake, the game will show the correct sequence and end.

## Setup and Circuit

The game uses an Arduino board, an LCD display, and a buzzer. The circuit design is available on Tinkercad for reference.

- **Tinkercad Circuit Design**: [Will add soon. I will attach some images of the circuit for now!]

## Files Included:

- **modified-memory-game.ini**: The complete program file, which includes the code for the game, LCD display, and buzzer functionality.

## How to Play:

1. **Start the Game**: Press the "Press Me to Start" button to begin.
2. **Game Rounds**: Each round, a new sequence of button presses is shown. The sequence gets longer with each round.
3. **Player’s Turn**: Repeat the sequence by pressing the buttons in the correct order.
4. **Feedback**: After each press, a tone will play, and the LCD will display whether your press was correct or incorrect.
5. **Winning or Losing**: If you repeat the entire sequence correctly, you win that round. If you make a mistake, you lose, and the buzzer will play a losing tune while the LCD shows the correct sequence.

## Requirements:
- Arduino Uno
- LCD Display (16x2)
- Active Buzzer
- Push Buttons
- LEDs
- RGB LED
- Breadboard
- Resistors

## How to Use:

1. Upload the **modified-memory-game.ini** file to your Arduino. / Upload the **modified-memory-game** folder to your Arduino folder.
2. Set up the circuit following the circuit design.
3. Power on your Arduino and start playing!
