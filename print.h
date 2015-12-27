#ifndef PRINT_H
#define PRINT_H

#include "grid.h"
#include <fstream>
#include <iostream>

class print
{
  public:
    print();
    ~print();

    std::ofstream m_my_water_output_file;
    std::ofstream m_my_plant_output_file;

    void water_output_to_file_and_screen(
        const double t,
        const grid& new_water_concentrations
    );

    void plant_output_to_file_and_screen(
        const double t,
        const grid& new_plant_densities
    );
};

#endif // PRINT_H
