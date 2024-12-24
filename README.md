# Tetris en C++

Este proyecto es una implementación del clásico juego Tetris en C++. Utiliza gráficos de texto y permite al jugador mover y rotar las piezas para formar líneas completas y obtener puntos.

## Retos del proyecto

* **Gestión de la pantalla:** El uso de caracteres ANSI para representar las piezas y el tablero.
* **Lógica del juego:** Implementar la física de caída de las piezas, la detección de colisiones y la eliminación de líneas completas requiere una cuidadosa atención a los detalles.
* **Manejo de la entrada del usuario:** La detección de las teclas presionadas y la respuesta a ellas de manera oportuna es fundamental para una experiencia de juego fluida.

## Cómo jugar

1. **Compila el código:** Utiliza un compilador de C++ para generar el ejecutable.
2. **Ejecuta el programa:** El juego se iniciará automáticamente.
3. **Controles:**
   * **A:** Mover a la izquierda.
   * **D:** Mover a la derecha.
   * **S:** Bajar la pieza.
   * **Q:** Rotar la pieza.
   * **P:** Pausar/Reanudar el juego.

## Estructura del código

* **Clases:** Se utiliza una clase `Tetris` para encapsular la lógica del juego.
* **Vectores:** Se emplean vectores para representar el tablero y las piezas.
* **Funciones:** Se han definido funciones para dibujar la pantalla, manejar la entrada del usuario, actualizar el estado del juego, etc.

## Potenciales mejoras

* **Interfaz gráfica:** Utilizar una biblioteca gráfica como SFML o OpenGL para crear una interfaz más visual y atractiva.
* **Sonidos:** Agregar efectos de sonido para mejorar la experiencia del jugador.
* **Múltiples modos de juego:** Implementar un modo ataque.
* **Guardado de partidas:** Permitir al jugador guardar su progreso y reanudarlo más tarde.
