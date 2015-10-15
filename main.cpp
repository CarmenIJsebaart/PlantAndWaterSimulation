/* Final assignment
   Written by Carmen IJsebaart on 12-10-2015 till 16-10-2015
   A program that visualizes regular and irregular patterns
   in semiarid vegetation. */

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <cassert>
#include <ctime>
#include "grid.h"
#include "simulation.h"

//  Floor map of the grid
//
//                          ----->
//                       y-direction
//                         (width)
//                  +---+---+---+---+---+       +---+
//                  |   |   |   |   |   |       |   |<-----     0.5 meter * 0.5 meter
//                  +---+---+---+---+---+       +---+       (grid -> 50 meter * 50 meter)
//               |  |   |   |   |   |   |
//  x-direction  |  +---+---+---+---+---+
//   (height)    |  |   |   |   |   |   |
//               v  +---+---+---+---+---+
//                  |   |   |   |   |   |
//                  +---+---+---+---+---+
//                  |   |   |   |   |   |
//                  +---+---+---+---+---+
//
//


int main()
{
  simulation my_simulation;

  const double simulation_time = 500;

  //For each time step
  while (my_simulation.get_current_time() < simulation_time)
  {
    std::cout << '.' << std::flush;
    my_simulation.goto_next_timestep();
  }

  return 0;
}

