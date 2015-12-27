#include "print.h"
#include <iomanip>

print::print() :
  m_my_water_output_file("WaterConcentrations.csv"),
  m_my_plant_output_file("plantdensities.csv")
{
   //Create an output file where the water concentration data can be stored
   //Create an output file where the plant density data can be stored
}

print::~print()
{
  m_my_water_output_file.close();
  m_my_plant_output_file.close();
}

void print::water_output_to_file_and_screen(
    const double t,
    const grid& new_water_concentrations
)
{
  for(int x = 0; x < grid::height; ++x)
  {
    m_my_water_output_file << t;
    for(int y = 0; y < grid::width; ++y)
    {
      std::cout << std::setw(10) << new_water_concentrations.get(x, y) << ' ';
      m_my_water_output_file << ';' << new_water_concentrations.get(x, y);
    }
    m_my_water_output_file << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void print::plant_output_to_file_and_screen(
    const double t,
    const grid& new_plant_densities
)
{
  for(int x = 0; x < grid::height; ++x)
  {
    m_my_plant_output_file << t;
    for(int y = 0; y < grid::width; ++y)
    {
      std::cout << std::setw(10) << new_plant_densities.get(x, y) << ' ';
      m_my_plant_output_file << ';' << new_plant_densities.get(x, y);
    }
    m_my_plant_output_file << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
