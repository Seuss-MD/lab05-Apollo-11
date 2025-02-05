/***********************************************************************
 * Header File:
 *    LANDER
 * Author:
 *    Br. Helfrich
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#pragma once

#include "position.h"  // for POINT
#include "velocity.h"  // for VELOCITY
#include "thrust.h"    // for THRUST
#include "uiDraw.h"    // for DRAW* and RANDOM
#include "angle.h"     // for ANGLE

enum Status { PLAYING, SAFE, DEAD };

class TestLander;

/****************************************************************
 * LANDER
 * The position and status of the lander
 ***************************************************************/
class Lander
{
   friend TestLander;
private:
   Status   status;      // are we dead or not?
   Position pos;         // the location of the lander
   Velocity velocity;           // the speed of the lander
   Angle    angle;       // the orientation of the lander
   double   fuel;        // amount of fuel remaining
   
public:
  // to create a lander, we need to know something about the board
  Lander(const Position & posUpperRight) : status(PLAYING), fuel(0.0), 
                                           pos(posUpperRight.getX(), posUpperRight.getY()),
                                           angle(Angle()), velocity(Velocity()){}

  // get the status of the lander
  bool     isDead()         const { if (status == DEAD) return true; }
  bool     isLanded()       const { if (status == SAFE) return true; }
  bool     isFlying()       const { if (status == PLAYING) return true; }
  Position getPosition()    const { return pos;}
  double   getSpeed()       const { return velocity.getSpeed(); }
  int      getFuel()        const { return fuel;  }
  int      getWidth()       const { return 20;   } //in meters
  double   getMaxSpeed()    const { return 4; }    //in m/s

  // reset the lander and its position
  void reset(const Position& posUpperRight);
 
  // straighten the lander and put it on the ground
  void land()  {status = SAFE; angle.setUp(); }
  
  // we are dead. Draw the lander upside down
  void crash() {status = DEAD; angle.setDown(); }

  // draw the lander on the screen
  void draw(const Thrust & thrust, ogstream & gout) const;
 
  // handle input
  Acceleration input(const Thrust & thrust, double gravity);

  // what happens when we coast?
  void coast(Acceleration & acceleration, double time);

};
