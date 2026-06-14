#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "types.h"
#include "dataloader.h"
#include "kmeans.h"

int main(int argc, char* argv[]) {
    // Validar argumentos por línea de comandos
    if (argc < 3) {
        std::cerr << "Uso correcto: " << argv[0] << " <k> <archivo_datos.csv>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[1]);
    std::string filename = argv[2];

    // Restricción preventiva dada por el Dr. Szilard
    if (k < 1 || k > 6) {
        std::cerr << "Error: El numero de clusters (k) debe estar entre 1 y 6." << std::endl;
        return 1;
    }

    std::vector<Coord_3D> rawData;
    
    // Cargar los datos (El módulo de carga trabaja de forma independiente)
    if (!loadCSV(filename, rawData)) {
        return 1;
    }

    std::vector<Labeled> classifiedPoints;
    std::vector<Coord_3D> centroids;
    std::vector<double> distortions;

    // Ejecutar el algoritmo sin alterar el vector original 'rawData'
    runKMeans(rawData, k, classifiedPoints, centroids, distortions);

    // --- GENERAR ARCHIVO clasificados.csv ---
    std::ofstream outFile("clasificados.csv");
    if (outFile.is_open()) {
        for (const auto& pt : classifiedPoints) {
            outFile << pt.coord.x << "," 
                    << pt.coord.y << "," 
                    << pt.coord.z << "," 
                    << pt.label << "\n";
        }
        outFile.close();
        std::cout << "Archivo 'clasificados.csv' generado exitosamente." << std::endl;
    } else {
        std::cerr << "Error al crear clasificados.csv" << std::endl;
    }

    // --- GENERAR ARCHIVO summary.txt ---
    std::ofstream outSummary("summary.txt");
    if (outSummary.is_open()) {
        outSummary << std::fixed << std::setprecision(4);
        
        // Contar cuántos puntos cayeron en cada cluster para el reporte
        std::vector<int> counts(k, 0);
        for (const auto& pt : classifiedPoints) {
            counts[pt.label - 'A']++;
        }

        // Escribir bajo el estricto molde: Letra: N, (x, y, z), MD
        for (int j = 0; j < k; ++j) {
            outSummary << (char)('A' + j) << ": " 
                       << counts[j] << ", ("
                       << centroids[j].x << ", " 
                       << centroids[j].y << ", " 
                       << centroids[j].z << "), " 
                       << distortions[j] << "\n";
        }
        outSummary.close();
        std::cout << "Archivo 'summary.txt' generado exitosamente." << std::endl;
    } else {
        std::cerr << "Error al crear summary.txt" << std::endl;
    }

    return 0;
}