#include <iostream> // Incluye la biblioteca para entrada y salida estándar
#include <vector>   // Incluye la biblioteca para usar vectores
#include <cstdlib>  // Incluye funciones para generación de números aleatorios
#include <ctime>    // Incluye la función time para semilla aleatoria
#include <conio.h>  // Incluye funciones como _kbhit() y _getch()

using namespace std; // Evita usar el prefijo std:: en cada función estándar

// Dimensiones del tablero
const int ANCHO = 10; // Número de columnas del tablero
const int ALTO = 20;  // Número de filas del tablero
// Lista de colores ANSI para las piezas
const string colores[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m", "\033[91m"};
int puntaje = 0; // Puntaje global del juego

// Definición de las piezas (Tetrominos)
const vector<vector<vector<int>>> tetrominos = {
    {{1, 1, 1, 1}},         // Pieza I
    {{1, 1, 1}, {0, 1, 0}}, // Pieza T
    {{1, 1, 0}, {0, 1, 1}}, // Pieza S
    {{0, 1, 1}, {1, 1, 0}}, // Pieza Z
    {{1, 1}, {1, 1}},       // Pieza O
    {{1, 0, 0}, {1, 1, 1}}, // Pieza L
    {{0, 0, 1}, {1, 1, 1}}  // Pieza J
};

// Clase que gestiona el juego Tetris
class Tetris {
private:
    // Atributos 
    vector<vector<int>> tablero;       // Matriz que representa el tablero
    vector<vector<int>> piezaActual;  // Matriz de la pieza que está cayendo
    int pActualX, pActualY;           // Posición de la pieza actual en el tablero
    int colorActual;                  // Índice del color de la pieza actual
    int velocidad;                    // Velocidad de caída de las piezas
    bool pausa;                       // Bandera para indicar si el juego está en pausa

    // Métodos privados
    void dibujar();               // Dibuja el tablero y las piezas
    void entrada();               // Gestiona la entrada del usuario
    void logica();                // Contiene la lógica del juego
    bool juegoTerminado();        // Verifica si el juego ha terminado
    void limpiarLineas();         // Elimina líneas completas
    void generarPieza();          // Genera una nueva pieza
    bool puedeMoverse(int dx, int dy); // Verifica si una pieza puede moverse
    void lugarPieza();            // Coloca una pieza en el tablero
    void rotarPieza();            // Rota la pieza actual
    void mostrarPausa();          // Muestra el mensaje de pausa

public:
    Tetris();                     // Constructor para inicializar el juego
    void correr();                // Método principal que ejecuta el juego
    static bool preguntarReinicio(); // Pregunta si el usuario quiere reiniciar el juego
};

int calcularPuntaje(int lineas);
void mensajes();

// Constructor de la clase Tetris
Tetris::Tetris() {
    tablero.resize(ALTO, vector<int>(ANCHO, 0)); // Inicializa el tablero vacío, .resize() reserva memoria
    srand(time(0)); // Inicializa la semilla para números aleatorios
    velocidad = 600; // Velocidad inicial
    pausa = false;   // El juego empieza sin estar en pausa
    generarPieza();  // Genera la primera pieza
}
//El constructor garantiza un estado inicial valido para el juego, incluyendo la inicialización del tablero, la generación de la primera pieza y la configuración de la velocidad y la bandera de pausa.

// Genera una nueva pieza para el juego
void Tetris::generarPieza() {
    piezaActual = tetrominos[rand() % tetrominos.size()]; // Selecciona una pieza aleatoria
    colorActual = rand() % colores->size(); // Selecciona un color aleatorio,-> se utiliza para acceder al vector de colores
    pActualX = ANCHO / 2 - piezaActual[0].size() / 2; // Centra la pieza en el tablero
    //Ejemplo para la figura I: ANCHO = 10, piezaActual[0].size() = 4, pActualX = 3
    pActualY = 0; // La pieza empieza desde arriba
}

// Dibuja el tablero y la pieza actual
void Tetris::dibujar() {
    system("cls"); // Limpia la pantalla
    mensajes();    // Muestra los mensajes y el puntaje
    for (int i = 0; i < ALTO + 2; i++) {        //For para dibujar las filas
        for (int j = 0; j < ANCHO + 2; j++) {   //For para dibujar las columnas
            if (i == 0 || i == ALTO + 1 || j == 0 || j == ANCHO + 1) { //Verifica si estamos en el borde
                cout << "\033[90m" << char(219) << "\033[0m"; // Dibuja el borde
            } else if (tablero[i - 1][j - 1]) {//la condicion que se tiene que cumplir es que la posicion en el tablero sea diferente a 0
                cout << colores[tablero[i - 1][j - 1] - 1] << "#" << "\033[0m"; // Dibuja piezas fijas, se le resta 1 a tablero para ajustar las coordenadas
            } else if (i - 1 >= pActualY && i - 1 < pActualY + piezaActual.size() && //Verifica si la coordenada actual esta dentro del rango de la pieza
                       j - 1 >= pActualX && j - 1 < pActualX + piezaActual[0].size() &&//Verifica si la coordenada actual esta dentro del rango de la pieza
                       piezaActual[i - 1 - pActualY][j - 1 - pActualX]) {//Verifica si una celda especifica del tablero forma part de la pieza en movimiento
                cout << colores[colorActual] << char (219) << "\033[0m"; // Dibuja la pieza actual
            } else {
                cout << " "; // Espacio vacío
            }
        }
        cout << endl;
    }
/*se resta 1 a i y j para ajustar las coordenadas en el tablero, ya que el for comienza desde 0,
y el tablero comienza desde 1 (fila 0 y columna 0)
*/
}
// Captura y gestiona la entrada del usuario
void Tetris::entrada() {
    if (_kbhit()) { // Verifica si hay una tecla presionada
        char tecla = tolower(_getch()); // Captura la tecla y la convierte a minúscula
        if (tecla == 'p') { // Si la tecla es 'P', alterna la pausa
            pausa = !pausa;
        } else if (!pausa) { // Si no está en pausa, permite el movimiento de la pieza
            if (tecla == 'a' && puedeMoverse(-1, 0)) { // Mover a la izquierda
                pActualX--;
            } else if (tecla == 'd' && puedeMoverse(1, 0)) { // Mover a la derecha
                pActualX++;
            } else if (tecla == 's' && puedeMoverse(0, 1)) { // Bajar la pieza
                pActualY++;
            } else if (tecla == 'q') { // Rotar la pieza
                rotarPieza();
            }
        }
    }
}

// Contiene la lógica principal del juego
void Tetris::logica() {
    if (pausa) { // Si el juego está en pausa
        mostrarPausa(); // Muestra el mensaje de pausa
        return; // Detiene la lógica mientras está en pausa
    }

    if (puedeMoverse(0, 1)) { // Si la pieza puede seguir bajando
        pActualY++; // La pieza baja una posición
    } else { // Si la pieza no puede bajar más
        lugarPieza(); // Fija la pieza en el tablero
        limpiarLineas(); // Verifica y elimina lineas completas
        if (juegoTerminado()) { // Verifica si el juego ha terminado
            dibujar(); // Dibuja el estado final del tablero
            cout << "GAME OVER!" << endl; // Muestra el mensaje de fin de juego
            cout << "Puntaje Final: " << puntaje << endl; // Muestra el puntaje final
            if (preguntarReinicio()) { // Pregunta si el usuario quiere reiniciar el juego
                tablero.assign(ALTO, vector<int>(ANCHO, 0)); // Reinicia el tablero, .assign() asigna un valor a un vector
                puntaje = 0; // Reinicia el puntaje
                generarPieza(); // Genera una nueva pieza
                velocidad = 600; // Restablece la velocidad inicial
            } else {
                exit(0); // Sale del programa
            }
        }
        generarPieza(); // Genera una nueva pieza
    }
}

// Verifica si una pieza puede moverse a una nueva posición
bool Tetris::puedeMoverse(int dx, int dy) {
    for (int i = 0; i < piezaActual.size(); i++) { // Itera por las filas de la pieza
        for (int j = 0; j < piezaActual[0].size(); j++) { // Itera por las columnas de la pieza
            if (piezaActual[i][j]) { // Si hay un bloque en la pieza
                int nx = pActualX + j + dx; // Calcula la nueva posición en X
                int ny = pActualY + i + dy; // Calcula la nueva posición en Y
                if (nx < 0 || nx >= ANCHO || ny >= ALTO || // Si se sale del tablero
                    (ny >= 0 && tablero[ny][nx])) { // O si hay un bloque fijo en esa posición
                    return false; // No puede moverse
                }
            }
        }
    }
    return true; // Si no hay colisión, puede moverse
}

// Fija la pieza actual en el tablero
void Tetris::lugarPieza() {
    for (int i = 0; i < piezaActual.size(); i++) { // Itera por las filas de la pieza
        for (int j = 0; j < piezaActual[0].size(); j++) { // Itera por las columnas de la pieza
            if (piezaActual[i][j]) { // Si hay un bloque en la pieza
                tablero[pActualY + i][pActualX + j] = colorActual + 1; // Lo fija en el tablero
            }
        }
    }
}

// Limpia las líneas completas del tablero
void Tetris::limpiarLineas() {
    int lineasEliminadas = 0; // Contador de líneas eliminadas
    for (int i = ALTO - 1; i >= 0; i--) { // Itera desde la última fila hacia arriba
        bool completa = true; // Bandera para verificar si la línea está completa
        for (int j = 0; j < ANCHO; j++) { // Itera por las columnas
            if (!tablero[i][j]) { // Si encuentra un espacio vacío
                completa = false; // La línea no está completa
                break; // Sale del bucle
            }
        }
        if (completa) { // Si la línea está completa
            tablero.erase(tablero.begin() + i); // Elimina la línea del tablero
            tablero.insert(tablero.begin(), vector<int>(ANCHO, 0)); // Añade una fila vacía al inicio
            lineasEliminadas++; // Incrementa el contador de líneas eliminadas
            i++; // Repite la verificación en la misma fila
        }
    }
    puntaje += calcularPuntaje(lineasEliminadas); // Actualiza el puntaje según las líneas eliminadas
    if (lineasEliminadas > 0) { // Si se eliminaron líneas
        velocidad = max(100, velocidad - 50); // Aumenta la velocidad del juego
        //max devuelve el valor más grande entre los dos números que se le pasan como argumentos
    }
}

// Verifica si el juego ha terminado
bool Tetris::juegoTerminado() {
    for (int j = 0; j < ANCHO; j++) { // Itera por la primera fila del tablero
        if (tablero[0][j]) return true; // Si de las coordenadas (0, j) hay un bloque, el juego termina
    }
    return false; // Si no hay bloques, el juego continúa
}

// Rota la pieza actual en sentido horario
void Tetris::rotarPieza() {
    vector<vector<int>> rotada(piezaActual[0].size(), vector<int>(piezaActual.size(), 0)); // Crea una matriz para la pieza rotada
    for (int i = 0; i < piezaActual.size(); i++) { // Itera por las filas de la pieza, piezaActual.size() es la cantidad de filas
        for (int j = 0; j < piezaActual[0].size(); j++) { // Itera por las columnas de la fila actual de la pieza, piezaActual[0].size() es la cantidad de columnas osea el ancho
            rotada[j][piezaActual.size() - 1 - i] = piezaActual[i][j]; // Toma los elementos de la pieza actual y los coloca en la matriz rotada
            //la formula realiza la rotación de la pieza en sentido horario de 90 grados
            /*Los elementos de la primera fila se colocan en la primera columna de la matriz rotada, 
            los elementos de la segunda fila se colocan en la segunda columna, y asi sucesivamente*/
        }
    }
    if (puedeMoverse(0, 0)) piezaActual = rotada; // Si puede moverse, actualiza la pieza actual
}

// Muestra el mensaje de pausa
void Tetris::mostrarPausa() {
    //system("cls"); // Limpia la pantalla
    cout << "Juego en PAUSA. Presiona 'P' para continuar..."<< endl; // Muestra el mensaje de pausa
}

// Pregunta al usuario si quiere reiniciar el juego
bool Tetris::preguntarReinicio() {
    char respuesta; // Variable para almacenar la respuesta del usuario
    do {
        cout << endl <<"Deseas jugar de nuevo? (S/N): ";
        cin >> respuesta;
        respuesta = tolower(respuesta); // Convierte la respuesta a minúscula
    } while (respuesta != 's' && respuesta != 'n'); // Repite mientras la respuesta no sea válida
    return respuesta == 's'; // Devuelve true si la respuesta es 's'
}

// Ejecuta el ciclo principal del juego
void Tetris::correr() {
    while (true) { // Bucle infinito del juego
        dibujar(); // Dibuja el tablero y las piezas
        entrada(); // Gestiona la entrada del usuario
        logica(); // Aplica la lógica del juego
        _sleep(velocidad); // Controla la velocidad de caída de las piezas
    }
}

// Función principal
int main() {
    Tetris juego; // Crea una instancia del juego
    juego.correr(); // Ejecuta el juego
    return 0; // Finaliza el programa
}

// Función para calcular el puntaje según el número de líneas eliminadas
int calcularPuntaje(int lineas) {
    int base = 100; // Puntaje base por línea
    int multiplicadores[] = {1, 3, 5, 8}; // Multiplicadores para cada número de líneas
    return lineas > 0 ? base * lineas * multiplicadores[lineas - 1] : 0;
    //Ejemplo si se eliminan 2 lineas al mismo tiempo: 100 * 2 * 3 = 600
}

// Muestra los mensajes iniciales y los controles
void mensajes() {
    cout << "Controles: " << endl;
    cout << "Mover Izquierda: A" << endl;
    cout << "Mover Derecha:   D" << endl;
    cout << "Rotar: Q" << endl;
    cout << "Bajar: S" << endl;
    cout << "Pausar/Continuar: P" << endl << endl;
    cout << "Puntaje: " << puntaje << endl;
}