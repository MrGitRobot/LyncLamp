# Purpose
This is a simple Arduino project for indicating the status of an employee concerning questions
- green: Available for questions.
- orange: Think if it is an important question.
- red: Busy. Come again later.
- pink: I'm in for "Kicker" 

# Usage 
Flash to Arduino. Use the connected buttons to change the state. 
If concentration time (in red state) is over, the single red LED will light up.
From then on you cannot change to the red state any more.

## Author
Christian Schuller

## Prototype Picture
![picture of the prototype](https://raw.githubusercontent.com/MrGitRobot/LyncLamp/addedPinkState/LyncLamp_Prototype_pink.jpeg)
