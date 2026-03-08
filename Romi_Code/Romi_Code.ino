#include <Arduino.h>
#include "Chassis.h"
#include "Romi32U4Buttons.h"

// --- STANDARD TURNS (L / R) ---
// encoder count targets, tune by turning 16 times and changing numbers untill offset is 0
#define NIGHTY_LEFT_TURN_COUNT -717
#define NIGHTY_RIGHT_TURN_COUNT 717 //earlier 715

// --- LOADED TURNS (l / r) ---
// NEW: Tune these specific values for when the robot has the water bottle
#define LOADED_LEFT_TURN_COUNT -738 // <--- TUNE THIS FOR LOWERCASE 'l'
#define LOADED_RIGHT_TURN_COUNT 738 // <--- TUNE THIS FOR LOWERCASE 'r'3435

// F and B go forward/backwards 50 cm by default, but other distances can be easily specified by adding a number after the letter
// S and E go the start/end distance
// L and R are left and right (Standard Turns)
// l and r are left and right (Loaded Turns with Water Bottle)
// R R for 180 
// targetTime is target time (duh)
char moves[200] = "F33.89 r r F B R R F100 B100 R R R R R R F100 r r F100 E";  // Example: "R F10 l F10" (lowercase 'l' for bottle turn) F33.89 r r F B R R F100 B100 R R R R R R F r r F E
double targetTime = 35.5;
double endDist = -33.89;
double startDist = -16;
double disttttt = 33.89;

//6.994936972
// parameters are wheel diam, encoder counts, wheel track (tune these to your own hardware)
// default values of 7, 1440, 14 can't go wrong
Chassis chassis(7.12, 1440, 14);
Romi32U4ButtonA buttonA;

// define the states (I LOVE state machines) (I made the state machine for Jacob's flappy bird in desmos)
// this state machine is not actually useful in any way
enum ROBOT_STATE { ROBOT_IDLE,
                   ROBOT_MOVE,
                   MOVING };
ROBOT_STATE robotState = ROBOT_IDLE;


// a helper function to stop the motors
void idle(void) {
  Serial.println("idle()");
  chassis.idle();
  robotState = ROBOT_IDLE;
}

/*
 * This is the standard setup function that is called when the board is rebooted
 * It is used to initialize anything that needs to be done once.
 */
void setup() {
  // This will initialize the Serial at a baud rate of 115200 for prints
  // Be sure to set your Serial Monitor appropriately
  Serial.begin(115200);
  // Serial1 is used to receive data from K210

  // initialize the chassis (which also initializes the motors)
  chassis.init();
  idle();

  // these can be undone for the student to adjust
  // tuned like shit, very good numbers to change
  // it's actually a PI controller where first number is P and second is I
  chassis.setMotorPIDcoeffs(5, 0.5);
}

void turnLeft() {
  chassis.turnFor(89, 60);
  delay(100);
}

void turnRight() {
  chassis.turnFor(-89, 60);
  delay(100);
}

// Standard turn wrappers
void left(float seconds) {
  chassis.turnWithTimePosPid(NIGHTY_LEFT_TURN_COUNT, seconds);
}

void right(float seconds) {
  chassis.turnWithTimePosPid(NIGHTY_RIGHT_TURN_COUNT, seconds);
}

// NEW: Loaded turn wrappers (use the new LOADED definitions)
void loadedLeft(float seconds) {
  chassis.turnWithTimePosPid(LOADED_LEFT_TURN_COUNT, seconds);
}

void loadedRight(float seconds) {
  chassis.turnWithTimePosPid(LOADED_RIGHT_TURN_COUNT, seconds);
}

// I wrote most of this in a meditative state the night before states lol
void loop() {
  if (buttonA.getSingleDebouncedPress()) {
    delay(300); // wait a little before starting to move so it doesn't hit the pencil or smth idk
    robotState = ROBOT_MOVE;
  }

  if (robotState == ROBOT_MOVE) {
    int count = 1; // count the number of moves (turns and straights)
    for (int i = 0; i < strlen(moves); i++)
      if (isSpace(moves[i])) count++;

    // constucts *movesList, each element is pointer to the first character of a move string
    // i.e. if moves is "S R F100 B L E" then *movesList[2] is a pointer to "F" and moveslist[2] is "F100"
    char *movesList[count];
    char *ptr = NULL;

    // tokenize moves with space as delimiter, each token is one move
    byte index = 0;
    ptr = strtok(moves, " ");
    while (ptr != NULL) {
      movesList[index] = ptr;
      index++;
      ptr = strtok(NULL, " ");
    }

    int numTurns = 0;
    int numLoadedTurns = 0; // NEW: Counter for bottle turns
    double totalDist = 0;
    char currentChar;
    String st;

    // count number of turns and total distance travelled
    // instead of *movesList[i] I could've just done st[0]... but pointers are cool ig
    for (int i = 0; i < count; i++) {
      currentChar = *movesList[i];
      st = movesList[i];
      
      // Standard Turns
      if (currentChar == 'R' || currentChar == 'L') {
        numTurns++;
      }
      // NEW: Loaded Turns (Bottle)
      else if (currentChar == 'r' || currentChar == 'l') {
        numLoadedTurns++;
      }
      else if (currentChar == 'F' || currentChar == 'B') {   
        if (st.length() > 1) {
          totalDist += st.substring(1).toDouble();
        } else {
          totalDist += 50;
        }
      } else if (currentChar == 'S') {
        totalDist += abs(startDist);
      } else if (currentChar == 'E') {
        totalDist += abs(endDist);
      }
    }

    double turnTime = 0.80;       // target time for a standard turn
    double loadedTurnTime = 2.00; // NEW: Increased time for water bottle turns (TUNE THIS)

    // NEW MATH: Account for both turn types in the time budget
    // I kept your 0.90 fudge factor for normal turns, and used the full loaded time for loaded ones
    double totalTurnTime = (0.90 * numTurns) + (2.10 * numLoadedTurns); 
    
    double totalDriveTime = targetTime - totalTurnTime - 0.0029*totalDist; // this also always went over hence the 0.0029*totalDist
    double dist;

    // execute the moves (this really should've been a switch case kind of thing)
    for (int i = 0; i < count; i++) {
      currentChar = *movesList[i];
      st = movesList[i];

      if (currentChar == 'R') {
        right(turnTime);
      } else if (currentChar == 'L') {
        left(turnTime);
      }
      // NEW: Execution for Loaded Turns (Now uses separate functions)
      else if (currentChar == 'r') {
        loadedRight(loadedTurnTime);
      } else if (currentChar == 'l') {
        loadedLeft(loadedTurnTime);
      }
      else if (currentChar == 'F' || currentChar == 'B') {      
        if (st.length() > 1) {
          dist = st.substring(1).toDouble();
        } else {
          dist = 50;
        }
        if (currentChar == 'F') {
          chassis.driveWithTime(dist, dist/totalDist * totalDriveTime);
        } else {
          chassis.driveWithTime(0-dist, dist/totalDist * totalDriveTime);
        } 
      } else if (currentChar == 'S') {
        chassis.driveWithTime(startDist, abs(startDist)/totalDist * totalDriveTime);
      } else if (currentChar == 'E') {
        chassis.driveWithTime(endDist, abs(endDist)/totalDist * totalDriveTime);
      }
    }
    idle(); // go back to idling after finish
  }
}