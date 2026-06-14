#ifndef TYPES_H
#define TYPES_H

// Estructura obligatoria para los puntos 3D
struct Coord_3D {
    double x;
    double y;
    double z;
};

// Estructura para asociar un punto con su etiqueta de cluster
struct Labeled {
    Coord_3D coord;
    char label; // Etiquetas de 'A' a 'F'
};

#endif // TYPES_H