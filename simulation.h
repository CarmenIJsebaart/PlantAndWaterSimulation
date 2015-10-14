#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

class Simulation {
public:
  using grid = std::vector<std::vector<double>>;
  const grid& get_water_grid() const noexcept { return m_water_grid; }
  const grid& get_plant_grid() const noexcept { return m_plant_grid; }
  void goto_next_timestep();
  double get_current_time() const noexcept { return m_time; }
private:
  grid m_plant_grid;
  grid m_water_grid;
  double m_time;
};

#endif // SIMULATION_H
