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
  assert(delta_height != 0.0);
  for(int y = 0; y < width; ++y)
  {
    for(int x = 0; x < height; ++x)
    {
      output_value = (input_grid.get( (x + height + 1) % height, y) -
                      input_grid.get( (x + height - 1) % height, y)) / (2.0 * delta_height);
      output_grid.set(x, y, output_value);
    }
  }
  return output_grid;
}

grid calculate_water_concentration_changes(
    const double a,
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
      const double n = plant_densities.get(x, y);
      const double water_concentration_change
          = delta_t * ( a - w - (w * (n * n)) + (v * downflow_water_concentrations.get(x, y)));
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
  for(int x = 0; x < height; ++x)
  {
    for(int y = 0; y < width; ++y)
    {
      double new_water_concentration = water_concentrations.get(x, y) + water_concentration_changes.get(x, y);
      if(new_water_concentration < 0.0005)
        new_water_concentration = 0;
      assert( water_concentrations.get(x, y) >= 0.0);
      assert(new_water_concentration >= 0.0);
      new_water_concentrations.set(x, y, new_water_concentration);
    }
  }
  return new_water_concentrations;
}

grid calculate_plant_density_changes(
    const double plant_losses,
    const int height,
    const int width,
    const grid water_concentrations,
    const grid plant_densities,
    const double delta_t,
    const double delta_height,
    const double delta_width
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
            (water_concentrations.get(x, y) * (plant_densities.get(x, y) * plant_densities.get(x, y)))
            - (plant_losses * plant_densities.get(x, y))
            + (((-2.0 * plant_densities.get(x, y)) + plant_densities.get((x + height + 1) % height, y) + plant_densities.get((x + height - 1) % height, y)) / (delta_height * delta_height))
            + (((-2.0 * plant_densities.get(x, y)) + plant_densities.get(x, (y + width + 1) % width) + plant_densities.get(x, (y + width - 1) % width)) / (delta_width * delta_width))
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
      assert(new_plant_density < 1000000);
      assert(current_plant_density >= 0.0);
      assert(new_plant_density >= 0.0);
      new_plant_densities.set(x, y, new_plant_density);
    }
  }
  return new_plant_densities;
}

