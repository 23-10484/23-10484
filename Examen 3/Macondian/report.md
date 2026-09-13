# Cálculo del macondo a partir de microsensores

Un sensor no entrega un solo dato: entrega uno por cada microsensor que
lo compone, y de ese grupo de lecturas hay que sacar un valor único, el
macondo. El desafío no es trivial porque, ocasionalmente, algún
microsensor se desconfigura y arroja un número que no tiene nada que ver
con la realidad. Promediar directamente todas las lecturas dejaría que
ese ruido contamine el resultado.

La forma de evitarlo fue apoyarse primero en la mediana en lugar del
promedio. La mediana tiene la ventaja de que un valor extremo casi no la
mueve de lugar, así que sirve como un punto de referencia confiable
incluso cuando hay una lectura defectuosa mezclada en el lote. A partir
de esa mediana se define un margen de tolerancia —fijado en 7.5%, dentro
del rango de 5% a 10% sugerido en la entrevista original— y toda lectura
que se aleje más que eso se descarta por considerarse una falla. Recién
con las lecturas que pasaron ese filtro se calcula el promedio, que es lo
que finalmente se reporta como macondo. Hay una excepción: si el margen
es tan ajustado que no sobrevive ninguna lectura, se usan todas las
originales para no dejar el cálculo vacío.

Dicho de forma resumida, el proceso tiene tres momentos: hallar la
mediana, filtrar según qué tan lejos está cada lectura de esa mediana, y
promediar lo que quedó.

En cuanto al costo, con N microsensores por sensor el único paso no
lineal es ordenar las lecturas para obtener la mediana (O(N log N)); el
resto es O(N). Como N es bajo —6 u 8, dependiendo del modelo— el cálculo
completo es casi instantáneo frente a los 2 segundos que separan un lote
del siguiente, lo que deja bastante margen incluso si la frecuencia de
muestreo aumenta más adelante.

Por último, toda esta lógica quedó encapsulada en `Macondo.ts`, fuera
del resto del sistema, con una interfaz simple: recibe el arreglo de
lecturas junto con el radio de tolerancia y devuelve el macondo. Gracias
a ese aislamiento, si la agencia trae su propio método de cálculo, se
puede probar o reemplazar sin modificar el resto del código.
