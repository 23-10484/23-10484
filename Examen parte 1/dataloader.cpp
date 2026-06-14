#include "dataloader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadCSV(const std::string& filename, std::vector<Coord_3D>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string valX, valY, valZ;
        
        // Asumiendo formato estándar CSV: x,y,z o separado por comas
        if (std::getline(ss, valX, ',') && 
            std::getline(ss, valY, ',') && 
            std::getline(ss, valZ, ',')) {
            
            Coord_3D pt;
            pt.x = std::stod(valX);
            pt.y = std::stod(valY);
            pt.z = std::stod(valZ);
            data.push_back(pt);
        }
    }
    file.close();
    return true;
}