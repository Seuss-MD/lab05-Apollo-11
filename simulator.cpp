/**********************************************************************
 * LAB 06
 * Lunar Lander simulation. This is the Game class and main()
 **********************************************************************/

#include "position.h"    // everything should have a point
#include "angle.h"       // angle of the lander
#include "uiInteract.h"  // for INTERFACE
#include "uiDraw.h"      // for RANDOM and DRAW*
#include "ground.h"      // for GROUND
#include "test.h"        // for the unit tests
#include "star.h"        // for drawStar
#include "lander.h"      // for lander
#include "thrust.h"      // for thrust
#include <cmath>         // for SQRT
#include <cassert>       // for ASSERT

#include <iostream>   // for now
#include <vector>
using namespace std;

#define GRAVITY -1.62
#define LANDER_WIDTH 20

/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   // set up the simulator and element positions
   Simulator(const Position& posUpperRight) :
      ground(posUpperRight),
      lander(posUpperRight),
      thrust()
   {
      this-> posUpperRight = posUpperRight;

      // Create 50 new stars with random positions
      for (int i = 0; i < 50; i++)
      {
         Star newStar;
         newStar.reset(posUpperRight.getX(), posUpperRight.getY());
         starVect.push_back(newStar);
      }

      // Create Lander
      lander.reset(posUpperRight);

   }

   // display stuff on the screen
   void display();

   // move the lander based on input
   void move(const Interface* pUI)
   {
      thrust.set(pUI);
      lander.input(thrust, GRAVITY);  //gravity
   }

   

   // blink phase for star
   void blink() { phase++; }


   //get start
   Position getStartPos() {return posUpperRight;}

private:
   unsigned char phase = 0;
   Angle a;
   Ground ground;
   Lander lander;
   Thrust thrust;
   Position posUpperRight;
   vector<Star> starVect;
};

/**********************************************************
 * DISPLAY
 * Draw on the screen
 **********************************************************/
void Simulator::display()
{
   ogstream gout;

   // draw a star
   for (int i = 0; i < 50; i++)
   {
      starVect[i].draw(gout);
   }

   // draw the ground
   ground.draw(gout);

   if (ground.onPlatform(lander.getPosition(), LANDER_WIDTH))
   {
      if (lander.getSpeed() <= lander.getMaxSpeed())
      {

         lander.land();
         cout << "landed " <<endl;
      }
      else
      {
        lander.crash();
        cout << "Lander has crashed bcause speed was " <<lander.getSpeed() << endl;
      }
   };

   // draw the lander

   if (ground.hitGround(lander.getPosition(), LANDER_WIDTH))
   {
      lander.crash();
   }

   if (lander.isFlying())
   {
      lander.coast(lander.input(thrust, GRAVITY), 1.0/30.0);
      lander.draw(thrust, gout);
   }

   if (lander.isDead() || lander.isLanded())
   {
      /*if (pUI->isSpaceBar())
      lander.reset(getStartPos());*/
   }


   
}


/*************************************
 * CALLBACK
 * Handle one frame of the simulator
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 
   Simulator* pSimulator = (Simulator*)p;

   pSimulator->move(pUI);
   // draw the game
   pSimulator->display();

   // handle input

   // change phase
   pSimulator->blink();

}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my LM type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run the unit tests
   testRunner();


   // Initialize OpenGL
   Position posUpperRight(400, 400);
   Interface ui("Lunar Lander", posUpperRight);

   // Initialize the game class
   Simulator simulator(posUpperRight);

   // set everything into action
   ui.run(callBack, (void*)&simulator);

   return 0;
}
