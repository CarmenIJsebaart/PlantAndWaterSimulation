#include <cassert>
#include <random>
#include <iostream>
#include "simulation.h"

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
  : plant_densities(1,1),
    water_concentrations(1,1),
    t{0.0}
{

  const int width = 100; //Horizontal number of steps
  const int height = 100; //Vertical number of steps

  //Create a 2D grid of water
  //Initialize the grid with zeros
  //grid water_concentrations = create_initial_water_concentrations(parameters);
  water_concentrations = grid(width, height);

  //Create a 2D grid of plants
  //Initialize the grid with random numbers between 0 and 10 (not higher, because otherwise the
  //time steps have to be really small).
  //grid plant_densities = create_initial_plant_densities(parameters);
  plant_densities = grid(width, height);
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
      {
        const double initial_plant_density = GetRandomUniform() * 10.0;
        assert(initial_plant_density >= 0.0 && initial_plant_density <= 10.0);
        std::cout << initial_plant_density << '\n';
        plant_densities.set(x, y, initial_plant_density);
      }
  }
  t = 0.0;
}

void simulation::goto_next_timestep() {
  ///////////////////////////////////////////////
  /// Calculation of the water concentrations ///
  ///////////////////////////////////////////////
  const double delta_height = 0.5; //Stepsize downhill
  const double delta_width = 0.5;  //Stepsize horizontally
  const double m = 0.45; //Grass (see article)
  const int height = water_concentrations.get_height();
  const int width = water_concentrations.get_width();
  //Formula to calculate the change of water concentration in time: dw/dt = a - w - w * n^2 + v * (dw/dx)

  //Create a 2D grid where the downflow of water concentrations can be stored
  //and calculate the downflow_water_concentration (dw/dx)
  const grid downflow_water_concentrations = d_value_div_dx(
    height,
    width,
    delta_height,
    water_concentrations
  );

  const int a = 2;          //Water is supplied uniformly at rate a (Grass (see article))
  const double v = 182.5;   //Water flows downhill at speed v

  //Create a 2D grid where the water concentration changes can be stored
  //and calculate the change of water concentration (dw/dt)
  const grid water_concentration_changes = calculate_water_concentration_changes(
    a,
    v,
    height,
    width,
    water_concentrations,
    plant_densities,
    downflow_water_concentrations,
    delta_t
  );

  //Create a 2D grid where the new water concentrations can be stored
  //and calculate the new water concentrations
  const grid new_water_concentrations = calculate_new_water_concentrations(
    height,
    width,
    water_concentrations,
    water_concentration_changes
  );

  ///////////////////////////////////////////////
  /// Calculation of the plant densities      ///
  ///////////////////////////////////////////////

  //Formula to calculate the change of plant density in time: dn/dt = w * n^2 - m * n + (d^2n/dx^2) + (d^2n/dy^2)

  //Create double differential equation (d^2n/dx^2)
  const grid dndx = d_value_div_dx(height, width, delta_height, plant_densities);   // dn/dx+
  const grid d2ndx2 = d_value_div_dx(height, width, delta_height, dndx);            // d^2n/dx^2
  //Create double differential equation (d^2n/dy^2)
  const grid dndy = d_value_div_dy(height, width, delta_width, plant_densities);   // dn/dy
  const grid d2ndy2 = d_value_div_dy(height, width, delta_width, dndy);            // d^2n/dy^2

  //Create a 2D grid where the plant density changes can be stored
  //and calculate the plant_density_changes (dn/dt)
  const grid plant_density_changes = calculate_plant_density_changes(
    m,
    height,
    width,
    water_concentrations,
    plant_densities,
    d2ndx2,
    d2ndy2,
    delta_t
  );

  //Create a 2D grid where the new plant densities can be stored
  //and calculate next plant densities
  const grid new_plant_densities = calculate_new_plant_densities(
    height,
    width,
    plant_densities,
    plant_density_changes
  );

  //Replace current grids by new grids
  water_concentrations = new_water_concentrations;
  plant_densities = new_plant_densities;
}

