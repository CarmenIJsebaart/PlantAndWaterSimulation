#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

#include "grid.h"

class simulation {
public:
  simulation();
  const grid& get_water_grid() const noexcept { return water_concentrations; }
  const grid& get_plant_grid() const noexcept { return plant_densities; }
  void goto_next_timestep();
  double get_current_time() const noexcept { return t; }
private:
  grid plant_densities;
  grid water_concentrations;
  const double delta_t = 0.001;
  double t;
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

#endif // SIMULATION_H
