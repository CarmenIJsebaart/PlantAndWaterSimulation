#include <cassert>
#include <random>
#include <iostream>
#include "simulation.h"
#include "print.h"

int step = 0; //Counts the time steps that are past

///GetRandomUniform draws a random number from 0.0 to and excluding 1.0.
///From http://www.richelbilderbeek.nl/CppGetRandomUniform.htm
double GetRandomUniform()
{
  //rd is used only to initialize mt with a truly random seed
  static std::random_device rd;
  //mt generates random numbers
  static std::mt19937 mt(rd());
  //d puts these random numbers in the correct distribution
  static std::uniform_real_distribution<double> d(0.0,1.0);
  //The random value x gets drawn here
  const double initial_plant_density{d(mt)};
  return initial_plant_density;
}

simulation::simulation()
  : plant_densities{},
    water_concentrations{},
    t{0.0}
{
  //Create a 2D grid of water
  //Initialize the grid with zeros
  //grid water_concentrations = create_initial_water_concentrations(parameters);
  //grid water_concentrations (grid::width, grid::height);

  //Create a 2D grid of plants
  //Initialize the grid with random numbers between 0 and 10 (not higher, because otherwise the
  //time steps have to be really small).
  //grid plant_densities = create_initial_plant_densities(parameters);
  //grid plant_densities (grid::width, grid::height);
  for(int x = 0; x < grid::height; ++x)
  {
    for(int y = 0; y < grid::width; ++y)
      {
        const double initial_plant_density = (GetRandomUniform() * 10.0);
        //Multiply random number by 10, because I want a random number between 1.0 and 10.0
        plant_densities.set(x, y, initial_plant_density);
      }
  }
  t = 0.0;
}

void simulation::goto_next_timestep() {
  ///////////////////////////////////////////////
  /// Calculation of the water concentrations ///
  ///////////////////////////////////////////////

  /////const double delta_height = 0.5; //Stepsize downhill
  /////const double delta_width = 0.5;  //Stepsize horizontally

  const double plant_mortality_rate = 0.9; //Grass (see article)
  //The mortality rate of Grass in the article is 0.45. If I use 0.45 in this simulation stipes of plants
  //and bare soil are formed, but no forks are visible (see article). With 0.9 as mortality rate, the forks
  //are visible.
  /////const int height = water_concentrations.get_height();
  /////const int width = water_concentrations.get_width();

  //Formula to calculate the change of water concentration in time: dw/dt = a - w - w * n^2 + v * (dw/dx)

  //Create a 2D grid where the downflow of water concentrations can be stored
  //and calculate the downflow_water_concentration (dw/dx)
  const grid downflow_water_concentrations = d_value_div_dx(
    grid::height,
    grid::width,
    grid::delta_height,
    water_concentrations
  );

  const double a = 2.0;    //Water is supplied uniformly at rate a (Grass (see article))
  const double v = 182.5;   //Water flows downhill at speed v

  //Create a 2D grid where the water concentration changes can be stored
  //and calculate the change of water concentration (dw/dt)
  const grid water_concentration_changes = calculate_water_concentration_changes(
    a,
    v,
    grid::height,
    grid::width,
    water_concentrations,
    plant_densities,
    downflow_water_concentrations,
    delta_t
  );

  //Create a 2D grid where the new water concentrations can be stored
  //and calculate the new water concentrations
  const grid new_water_concentrations = calculate_new_water_concentrations(
    grid::height,
    grid::width,
    water_concentrations,
    water_concentration_changes
  );

  ///////////////////////////////////////////////
  /// Calculation of the plant densities      ///
  ///////////////////////////////////////////////

  //Formula to calculate the change of plant density in time:
  //dn/dt = w * n^2 - m * n + (d^2n/dx^2) + (d^2n/dy^2)

  //Create a 2D grid where the plant density changes can be stored
  //and calculate the plant_density_changes (dn/dt)
  const grid plant_density_changes = calculate_plant_density_changes(
    plant_mortality_rate,
    grid::height,
    grid::width,
    water_concentrations,
    plant_densities,
    delta_t,
    grid::delta_height,
    grid::delta_width
  );

  //Create a 2D grid where the new plant densities can be stored
  //and calculate next plant densities
  const grid new_plant_densities = calculate_new_plant_densities(
    grid::height,
    grid::width,
    plant_densities,
    plant_density_changes
  );

  //Replace current grids by new grids
  water_concentrations = new_water_concentrations;
  plant_densities = new_plant_densities;
  t += delta_t;

  if(step % 5000 == 0) //Write to the screen every 5 time steps
  {
    print my_print;
    std::cout << t << '\t' << step << std::endl;
    my_print.water_output_to_file_and_screen(t,new_water_concentrations );
    my_print.plant_output_to_file_and_screen(t, new_plant_densities);
  }

  ++step;
}
