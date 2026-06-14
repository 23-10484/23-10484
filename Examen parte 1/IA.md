Yo: (Archivos adjuntos: pautas.pdf, datos.csv) Hola Gemini, tengo que desarrollar un programa modular en C++ para clasificar puntos tridimensionales (te adjunto las especificaciones del PDF y el archivo de datos). Uno de los requisitos obligatorios es que el vector original que almacena las coordenadas no sufra ninguna modificación. ¿Cómo deberíamos plantear las estructuras de datos base para los puntos y las etiquetas?

Gemini: ¡Hola! Es un excelente requerimiento para mantener la integridad de los datos. Para cumplir con esa condición de inmutabilidad y organizar el proyecto de forma modular, lo ideal es definir las estructuras en un archivo de cabecera común. De este modo, cualquier módulo podrá utilizarlas de manera estandarizada.

Este es el archivo types.h:
```cpp
#ifndef TYPES_H
#define TYPES_H

struct Coord_3D {
    double x;
    double y;
    double z;
};

struct Labeled {
    Coord_3D coord;
    char label; // Para guardar 'A', 'B', 'C', etc.
}

#endif
```
Yo: Entendido. Lo siguiente es el cargador de datos. Debe estructurarse como un módulo independiente que se encargue de abrir el archivo datos.csv que te adjunté y almacene la información en el vector, sin interferir con la lógica principal. ¿Cómo quedarían el .h y el .cpp?

Gemini: Para separar la lectura de archivos del flujo principal, crearemos una función que reciba la ruta del archivo y una referencia al vector de coordenadas. Utilizaremos las librerías estándar de C++ para abrir el documento y procesar las cadenas de texto separadas por comas.

Este sería el dataloader.h:
```cpp
#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "types.h"

bool loadCSV(const std::string& filename, std::vector<Coord_3D>& data);

#endif
```
Y aquí la lógica en dataloader.cpp:
```cpp
#include "dataloader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadCSV(const std::string& filename, std::vector<Coord_3D>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string x, y, z;
        
        if (std::getline(ss, x, ',') && std::getline(ss, y, ',') && std::getline(ss, z, ',')) {
            Coord_3D pt;
            pt.x = std::stod(x);
            pt.y = std::stod(y);
            pt.z = std::stod(z);
            data.push_back(pt);
        }
    }
    file.close();
    return true;
}
```
Yo: Perfecto. Ahora toca el algoritmo de K-means. En las pautas exigen estrictamente que la condición de parada esté definida en una función propia y bien explicada con comentarios. También necesito incorporar el cálculo de la dispersión como el promedio de las distancias de los puntos a su centroide asignado. ¿Cómo armamos este módulo?

Gemini: Desarrollaremos el algoritmo K-means aplicando el método iterativo estándar para reubicar los centros de masa. La condición de finalización se evaluará de forma aislada comparando la variación de posición de los centroides bajo un umbral de tolerancia, añadiendo además un límite de iteraciones para prevenir ciclos infinitos. La dispersión se calculará promediando las distancias calculadas.

Crea el archivo kmeans.h:
```cpp
#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "types.h"

void runKMeans(const std::vector<Coord_3D>& data, int k, 
               std::vector<Labeled>& outputPoints, 
               std::vector<Coord_3D>& centroids, 
               std::vector<double>& distortions);

bool shouldStop(const std::vector<Coord_3D>& oldCentroids, 
                 const std::vector<Coord_3D>& newCentroids, 
                 int currentIteration, int maxIterations);

double euclideanDistance(const Coord_3D& p1, const Coord_3D& p2);

#endif
```

Y la implementación en kmeans.cpp:

```cpp
#include "kmeans.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

double euclideanDistance(const Coord_3D& p1, const Coord_3D& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}

/* CONDICIÓN DE PARADA:
   Frenamos el algoritmo si los centroides viejos y los nuevos están en el mismo sitio 
   (diferencia menor a 1e-6) o si ya alcanzamos las 100 iteraciones. Esto evita 
   bucles infinitos y detiene el programa cuando ya no hay mejoras.
*/
bool shouldStop(const std::vector<Coord_3D>& oldCentroids, 
                 const std::vector<Coord_3D>& newCentroids, 
                 int currentIteration, int maxIterations) {
    if (currentIteration >= maxIterations) return true;
    if (oldCentroids.empty()) return false;

    double epsilon = 1e-6;
    for (size_t i = 0; i < oldCentroids.size(); ++i) {
        if (euclideanDistance(oldCentroids[i], newCentroids[i]) > epsilon) {
            return false;
        }
    }
    return true;
}

void runKMeans(const std::vector<Coord_3D>& data, int k, 
               std::vector<Labeled>& outputPoints, 
               std::vector<Coord_3D>& centroids, 
               std::vector<double>& distortions) {
    
    size_t n = data.size();
    outputPoints.resize(n);
    centroids.resize(k);
    distortions.assign(k, 0.0);

    std::srand(std::time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[std::rand() % n];
    }

    std::vector<Coord_3D> oldCentroids;
    int iteration = 0;

    while (!shouldStop(oldCentroids, centroids, iteration, 100)) {
        oldCentroids = centroids;
        iteration++;

        // Asignar puntos al centroide más cercano
        for (size_t i = 0; i < n; ++i) {
            double minDist = 1e9;
            int bestCluster = 0;
            for (int j = 0; j < k; ++j) {
                double dist = euclideanDistance(data[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }
            outputPoints[i].coord = data[i];
            outputPoints[i].label = 'A' + bestCluster;
        }

        // Recalcular centroides
        std::vector<Coord_3D> sumCentroids(k, {0.0, 0.0, 0.0});
        std::vector<int> counts(k, 0);

        for (size_t i = 0; i < n; ++i) {
            int idx = outputPoints[i].label - 'A';
            sumCentroids[idx].x += outputPoints[i].coord.x;
            sumCentroids[idx].y += outputPoints[i].coord.y;
            sumCentroids[idx].z += outputPoints[i].coord.z;
            counts[idx]++;
        }

        for (int j = 0; j < k; ++j) {
            if (counts[j] > 0) {
                centroids[j].x = sumCentroids[j].x / counts[j];
                centroids[j].y = sumCentroids[j].y / counts[j];
                centroids[j].z = sumCentroids[j].z / counts[j];
            }
        }
    }

    // Calcular la dispersión media de cada cluster
    std::vector<double> sumDistances(k, 0.0);
    std::vector<int> counts(k, 0);

    for (size_t i = 0; i < n; ++i) {
        int idx = outputPoints[i].label - 'A';
        sumDistances[idx] += euclideanDistance(outputPoints[i].coord, centroids[idx]);
        counts[idx]++;
    }

    for (int j = 0; j < k; ++j) {
        if (counts[j] > 0) {
            distortions[j] = sumDistances[j] / counts[j];
        }
    }
}

```

Yo: Ya casi estamos. Falta el main.cpp. Tiene que recibir los argumentos por consola como cluster <k> <datos>, validar que k no sea mayor a 6, y al final crear los archivos clasificados.csv y el reporte summary.txt con el formato exacto de "Letra: N, (x, y, z), MD".

Gemini: Integraremos todos los componentes en el punto de entrada principal. El código validará los parámetros de ejecución provenientes de la terminal, invocará los submódulos de carga y procesamiento, y finalmente exportará los resultados gestionando el formato de salida numérico de acuerdo a lo requerido.

Este es el main.cpp
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include "types.h"
#include "dataloader.h"
#include "kmeans.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <k> <archivo.csv>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[1]);
    std::string filename = argv[2];

    if (k < 1 || k > 6) {
        std::cerr << "El numero de clusters (k) debe ser entre 1 y 6." << std::endl;
        return 1;
    }

    std::vector<Coord_3D> rawData;
    if (!loadCSV(filename, rawData)) {
        return 1;
    }

    std::vector<Labeled> classifiedPoints;
    std::vector<Coord_3D> centroids;
    std::vector<double> distortions;

    runKMeans(rawData, k, classifiedPoints, centroids, distortions);

    // Guardar clasificados.csv
    std::ofstream outFile("clasificados.csv");
    if (outFile.is_open()) {
        for (const auto& pt : classifiedPoints) {
            outFile << pt.coord.x << "," << pt.coord.y << "," << pt.coord.z << "," << pt.label << "\n";
        }
        outFile.close();
    }

    // Guardar summary.txt
    std::ofstream outSummary("summary.txt");
    if (outSummary.is_open()) {
        outSummary << std::fixed << std::setprecision(4);
        
        std::vector<int> counts(k, 0);
        for (const auto& pt : classifiedPoints) {
            counts[pt.label - 'A']++;
        }

        for (int j = 0; j < k; ++j) {
            outSummary << (char)('A' + j) << ": " 
                       << counts[j] << ", ("
                       << centroids[j].x << ", " 
                       << centroids[j].y << ", " 
                       << centroids[j].z << "), " 
                       << distortions[j] << "\n";
        }
        outSummary.close();
        std::cout << "Listo. Archivos creados correctamente." << std::endl;
    }

    return 0;
}
```
