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

#define GRAVITY 1.62

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
      //posLander(posUpperRight.getX() / 2.0, posUpperRight.getY() / 2.0),
      width(posUpperRight.getX()), height(posUpperRight.getY()),
      lander(posUpperRight),
      thrust()
   {
      // Create 50 new stars with random positions
      for (int i = 0; i < 50; i++)
      {
         Star newStar;
         newStar.reset(width, height);
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

   // get width and height
   double getWidth() { return width; }
   double getHeight() { return height; }

private:
   unsigned char phase = 0;
   Angle a;
   Ground ground;
   Lander lander;
   Thrust thrust;
   Position posUpperRight;
   //Position posLander;
   //Position posStar;
   vector<Star> starVect;
   double width;
   double height;
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

   // draw the lander
   lander.coast(lander.input(thrust, GRAVITY), 0.1);
   lander.draw(thrust, gout);
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

   // draw the game
   pSimulator->display();

   // handle input
   pSimulator->move(pUI);

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
