#include "Juego.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Juego ArbolDePlata;

    ArbolDePlata.menuPrincipal();

    while (ArbolDePlata.getEjecutando()) {
        ArbolDePlata.actualizar();

        if (ArbolDePlata.getNivelActualObj().verificarObjetivo()) {
            ArbolDePlata.cambiarNivel();
        }
    }

    std::cout << std::endl << std::endl << "Presiona Enter para cerrar el programa...";
    std::cin.get();
    return 0;
}
