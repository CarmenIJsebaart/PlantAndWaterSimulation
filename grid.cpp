#include <cmath>
#include "grid.h"

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
  assert(delta_t > 0.0);
  grid water_concentration_changes(width, height);
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      const double w = water_concentrations.get(x, y);
      const double p = plant_densities.get(x, y);
      const double water_concentration_change
          = delta_t * ( a - w - (w * pow(p,2.0)) + (v * downflow_water_concentrations.get(x, y)));
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
      double new_plant_density = current_plant_density + plant_density_change;
      if(new_plant_density < 0)
        new_plant_density = 0;
      assert(current_plant_density >= 0.0);
      assert(new_plant_density >= 0.0);
      new_plant_densities.set(x, y, new_plant_density);
    }
  }
  return new_plant_densities;
}

