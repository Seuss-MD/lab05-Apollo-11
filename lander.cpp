/***********************************************************************
 * Source File:
 *    LANDER
 * Author:
 *    Br. Helfrich
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#include "lander.h"
#include "acceleration.h"

 /***************************************************************
  * RESET
  * Reset the lander and its position to start the game over
  ***************************************************************/
void Lander :: reset(const Position & posUpperRight)
{
   status = PLAYING;
   fuel = 5000.0;
   pos = Position(posUpperRight.getX()-1, posUpperRight.getY() * random(0.75,0.95));
   angle = Angle();
   velocity = Velocity(random(-2, 2), random(-10, -4));
}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 * Turn on flames as needed
 * Flame on
 ***************************************************************/
void Lander :: draw(const Thrust & thrust, ogstream & gout) const
{
   bool bottom             =  thrust.isMain();
   bool clockwise          =  thrust.isClock();
   bool counter            =  thrust.isCounter();

   gout.drawLander(pos, angle.getRadians());
   gout.drawLanderFlames(pos, angle.getDegrees(), bottom, clockwise, counter );
}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 * Convert thrust and gravity into an acceleration object
 * Uses the angle 
 ***************************************************************/
Acceleration Lander :: input(const Thrust& thrust, double gravity)
{
   if (thrust.isClock())
   {
      angle.add(thrust.rotation());
   }
   if (thrust.isCounter())
   {
      angle.add(-1 * thrust.rotation());
   }
   
   Acceleration acceleration = Acceleration();
   acceleration.set(angle, thrust.mainEngineThrust());
   acceleration.addDDY(gravity);

   return acceleration;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Lander :: coast(Acceleration & acceleration, double t)
{
   pos.addX(velocity.getDX() * t + 0.5 * acceleration.getDDX() * t * t);
   pos.addY(velocity.getDY() * t + 0.5 * acceleration.getDDY() * t * t);

}
