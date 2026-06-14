#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "types.h"

// Carga los datos desde un archivo CSV a un vector de Coord_3D
bool loadCSV(const std::string& filename, std::vector<Coord_3D>& data);

#endif // DATALOADER_H