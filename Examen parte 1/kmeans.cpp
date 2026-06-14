#include "kmeans.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

double euclideanDistance(const Coord_3D& p1, const Coord_3D& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + 
                     std::pow(p1.y - p2.y, 2) + 
                     std::pow(p1.z - p2.z, 2));
}

/**
 * EXPLICACIÓN DE LA CONDICIÓN DE PARADA:
 * Esta función encapsula el criterio de parada evaluando dos factores:
 * 1. Estabilización de centroides: Si la diferencia de posición entre los centroides viejos
 * y nuevos es prácticamente cero (menor a un epsilon de 1e-6), el algoritmo convergió.
 * 2. Límite de iteraciones: Evita bucles infinitos en caso de oscilaciones por datos flotantes.
 * Es una buena condición porque equilibra precisión matemática y eficiencia computacional.
 */
bool shouldStop(const std::vector<Coord_3D>& oldCentroids, 
                const std::vector<Coord_3D>& newCentroids, 
                int currentIteration, int maxIterations) {
    
    if (currentIteration >= maxIterations) return true;
    if (oldCentroids.empty()) return false;

    double epsilon = 1e-6;
    for (size_t i = 0; i < oldCentroids.size(); ++i) {
        if (euclideanDistance(oldCentroids[i], newCentroids[i]) > epsilon) {
            return false; // Al menos un centroide se movió significativamente
        }
    }
    return true; // Todos los centroides están estables
}

void runKMeans(const std::vector<Coord_3D>& data, int k, 
               std::vector<Labeled>& outputPoints, 
               std::vector<Coord_3D>& centroids, 
               std::vector<double>& distortions) {
    
    size_t n = data.size();
    outputPoints.resize(n);
    centroids.resize(k);
    distortions.assign(k, 0.0);

    // 1. Inicialización aleatoria de centroides (Forzando puntos existentes de la data)
    std::srand(std::time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[std::rand() % n];
    }

    std::vector<Coord_3D> oldCentroids;
    int iteration = 0;
    int maxIterations = 100;

    // Bucle principal de K-means
    while (!shouldStop(oldCentroids, centroids, iteration, maxIterations)) {
        oldCentroids = centroids;
        iteration++;

        // Paso de Asignación: Asociar cada punto al centroide más cercano
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
            outputPoints[i].label = 'A' + bestCluster; // Genera letras A, B, C...
        }

        // Paso de Actualización: Recalcular los centroides (Media de los puntos)
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

    // Calcular la medida de dispersión (MD): Distancia media absoluta al centroide
    std::vector<double> sumDistances(k, 0.0);
    std::vector<int> counts(k, 0);

    for (size_t i = 0; i < n; ++i) {
        int idx = outputPoints[i].label - 'A';
        double dist = euclideanDistance(outputPoints[i].coord, centroids[idx]);
        sumDistances[idx] += dist;
        counts[idx]++;
    }

    for (int j = 0; j < k; ++j) {
        if (counts[j] > 0) {
            distortions[j] = sumDistances[j] / counts[j]; // MD = Promedio de distancias
        }
    }
}