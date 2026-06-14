#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "types.h"

// Ejecuta el algoritmo K-means. El vector 'data' es constante (no se modifica)
void runKMeans(const std::vector<Coord_3D>& data, int k, 
               std::vector<Labeled>& outputPoints, 
               std::vector<Coord_3D>& centroids, 
               std::vector<double>& distortions);

// Condición de parada encapsulada
bool shouldStop(const std::vector<Coord_3D>& oldCentroids, 
                const std::vector<Coord_3D>& newCentroids, 
                int currentIteration, int maxIterations);

// Calcula la distancia Euclidiana entre dos puntos 3D
double euclideanDistance(const Coord_3D& p1, const Coord_3D& p2);

#endif // KMEANS_H