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


class grid {
public:
    grid(const int width, const int height)
      : m_v(height,std::vector<double>(width,0.0))
  {
      assert(width == get_width());
      assert(height == get_height());
  }
  int get_height() const noexcept { return static_cast<int>(m_v.size()); }
  int get_width() const {
    assert(!m_v.empty());
    return static_cast<int>(m_v[0].size());
  }
  double get(const int x, const int y) const {
    assert(x >= 0);
    assert(x < get_height());
    assert(y >= 0);
    assert(y < get_width());
    return m_v[x][y];
  }
  void set(const int x, const int y, const double value) {
    assert(x >= 0);
    assert(x < get_height());
    assert(y >= 0);
    assert(y < get_width());
    m_v[x][y] = value;
    assert(get(x, y) == value);
  }

  private:
  //an x-y-ordered 2D vector
  std::vector<std::vector<double>> m_v;
};

//Differentiate equation to x
grid d_value_div_dx(const int height, const int width, const double delta_height, grid input_grid);
//Differentiate equation to y
grid d_value_div_dy(const int height, const int width, const double delta_width, grid input_grid);
grid calculate_water_concentration_changes(const int a, const double v, const int height, const int width,
                                           grid water_concentrations, grid plant_densities,
                                           grid downflow_water_concentrations, const double delta_t);
grid calculate_new_water_concentrations(const int height, const int width, grid water_concentrations,
                                        grid water_concentration_changes);
grid calculate_plant_density_changes(const double plant_losses, const int height, const int width,
                                     grid water_concentrations, grid plant_densities, grid d2ndx2,
                                     grid d2ndy2, const double delta_t);
grid calculate_new_plant_densities(const int height, const int width, grid plant_densities,
                                   grid plant_density_changes);

int main()
{
  const int width = 10; //Horizontal number of steps
  const int height = 10; //Vertical number of steps
  const double delta_height = 0.5; //Stepsize downhill
  const double delta_width = 0.5;  //Stepsize horizontally

  //Create a 2D grid of water
  //Initialize the grid with zeros
  //grid water_concentrations = create_initial_water_concentrations(parameters);
  grid water_concentrations (width, height);

  //Create a 2D grid of plants
  //Initialize the grid with random numbers between 0 and 10 (not higher, because otherwise the
  //time steps have to be really small).
  //grid plant_densities = create_initial_plant_densities(parameters);
  grid plant_densities (width, height);
  const double initial_plant_density = 1;
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
      {
        plant_densities.set(x, y, initial_plant_density);
      }
  }

  const double m = 0.45; //Grass (see article)
  const double simulation_time = 100;
  const double delta_t = 0.1;

  //For each time step
  for (double t = 0.0; t < simulation_time; t += delta_t)
  {
    ///////////////////////////////////////////////
    /// Calculation of the water concentrations ///
    ///////////////////////////////////////////////

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

  } //End of the time loop

  return 0;
} //End of the main function

grid d_value_div_dx(
    const int height,
    const int width,
    const double delta_height,
    grid input_grid)
{
  grid output_grid (width, height);
  double output_value;
  for(int y = 0; y < width; ++y)
  {
    for(int x = 0; x < height; ++x)
    {
      if((x > 0) && (x < height - 1))
      {
       output_value = (input_grid.get(x + 1, y) - input_grid.get(x - 1, y)) / (2 * delta_height);
        output_grid.set(x, y, output_value);
      }
      else if(x == 0) //When x == 0 you cannot take x - 1, then you work outside the vector (pretend grid is a cilinder)
      {
        output_value = (input_grid.get(x + 1, y) - input_grid.get(x + (height - 1), y)) / (2 * delta_height);
        output_grid.set(x, y, output_value);
      }
      else if(x == height) //When x == height you cannot take x + 1, then you work outside the vector (pretend grid is a cilinder)
      {
        output_value = (input_grid.get(x - x, y) - input_grid.get(x - 1, y)) / (2 * delta_height);
        output_grid.set(x, y, output_value);
      }
    }
  }
  return output_grid;
}

grid d_value_div_dy(
    const int height,
    const int width,
    const double delta_width,
    grid input_grid)
{
  grid output_grid (width, height);
  double output_value;
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      if((y > 0) && (y < width - 1))
      {
        output_value = (input_grid.get(x, y + 1) - input_grid.get(x, y - 1)) / (2 * delta_width);
        output_grid.set(x, y, output_value);
      }
      else if(y == 0) //When y == 0 you cannot take y - 1, then you work outside the vector (pretend grid is a cilinder)
      {
        output_value = (input_grid.get(x, y + 1) - input_grid.get(x, y + (width - 1))) / (2 * delta_width);
        output_grid.set(x, y, output_value);
      }
      else if(y == width) //When y == height you cannot take y + 1, then you work outside the vector (pretend grid is a cilinder)
      {
        output_value = (input_grid.get(x, y - y) - input_grid.get(x, y - 1)) / (2 * delta_width);
        output_grid.set(x, y, output_value);
      }
    }
  }
  return output_grid;
}

grid calculate_water_concentration_changes(
    const int a,
    const double v,
    const int height,
    const int width,
    grid water_concentrations,
    grid plant_densities,
    grid downflow_water_concentrations,
    const double delta_t)
{
  grid water_concentration_changes (width, height);
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      const double water_concentration_change
          = delta_t
          *
          (
            a
            - water_concentrations.get(x, y)
            - (water_concentrations.get(x, y) * pow(plant_densities.get(x, y),2))
            + (v * downflow_water_concentrations.get(x, y))
          )
          ;
      water_concentration_changes.set(x, y, water_concentration_change);
    }
  }
  return water_concentration_changes;
}

grid calculate_new_water_concentrations(
    const int height,
    const int width,
    grid water_concentrations,
    grid water_concentration_changes)
{
  grid new_water_concentrations (width, height);
  double new_water_concentration;
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      new_water_concentration = water_concentrations.get(x, y) + water_concentration_changes.get(x, y);
      new_water_concentrations.set(x, y, new_water_concentration);
    }
  }
  return new_water_concentrations;
}

grid calculate_plant_density_changes(
    const double plant_losses, // m in article
    const int height,
    const int width,
    const grid water_concentrations,
    const grid plant_densities,
    const grid d2ndx2,
    const grid d2ndy2,
    const double delta_t
)
{
  grid plant_density_changes (width, height);
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      const double plant_density_change
          = delta_t
          *
          (
            (water_concentrations.get(x, y) * pow(plant_densities.get(x, y),2))
            - (plant_losses * plant_densities.get(x, y))
            + d2ndx2.get(x, y)
            + d2ndy2.get(x, y)
          )
      ;
      plant_density_changes.set(x, y, plant_density_change);
    }
  }
  return plant_density_changes;
}

grid calculate_new_plant_densities(
    const int height,
    const int width,
    const grid plant_densities,
    const grid plant_density_changes
)
{
  grid new_plant_densities(width, height);
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      const double current_plant_density = plant_densities.get(x, y);
      const double plant_density_change = plant_density_changes.get(x, y);
      const double new_plant_density = current_plant_density + plant_density_change;
      assert(current_plant_density >= 0.0);
      assert(new_plant_density >= 0.0);
      new_plant_densities.set(x, y, new_plant_density);
    }
  }
  return new_plant_densities;
}
