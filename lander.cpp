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
void Lander::reset(const Position& posUpperRight)
{
   status = PLAYING;
   fuel = 5000.0;
   pos = Position(posUpperRight.getX() - 1, posUpperRight.getY() * random(0.75, 0.95));
   angle = Angle();
   velocity = Velocity(random(-10, -4), random(-2, 2));
}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 * Turn on flames as needed
 * Flame on
 ***************************************************************/
void Lander::draw(const Thrust& thrust, ogstream& gout) const
{
   bool bottom = thrust.isMain();
   bool clockwise = thrust.isClock();
   bool counterClockwise = thrust.isCounter();

   gout.drawLander(pos, angle.getRadians());
   if (fuel > 0.0)
      gout.drawLanderFlames(pos, angle.getRadians(), bottom, counterClockwise, clockwise);
}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 * Convert thrust and gravity into an acceleration object
 * Uses the angle
 ***************************************************************/
Acceleration Lander::input(const Thrust& thrust, double gravity)
{
   Acceleration acceleration = Acceleration();

   if (fuel > 0.0)
   {
      if (thrust.isClock() || thrust.isCounter())
      {
         fuel -= 1.0;
         angle.add(thrust.rotation());
      }
      if (thrust.isMain())
      {
         fuel -= 10.0;
         acceleration.set(angle, thrust.mainEngineThrust());
         acceleration.setDDX(-acceleration.getDDX());
      }
   }

   acceleration.addDDY(gravity);

   return acceleration;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Lander::coast(Acceleration& acceleration, double t)
{
   pos.addX(velocity.getDX() * t + 0.5 * acceleration.getDDX() * t * t);
   pos.addY(velocity.getDY() * t + 0.5 * acceleration.getDDY() * t * t);
   velocity.add(acceleration, t);
}
