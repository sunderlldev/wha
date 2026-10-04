#include "Juego.h"
#include <clocale>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    Juego ArbolDePlata;

    ArbolDePlata.menuPrincipal();

    while (ArbolDePlata.getEjecutando()) {
        ArbolDePlata.actualizar();
    }

    std::cout << std::endl << std::endl << "Presiona Enter para cerrar el programa...";
    std::cin.get();
    return 0;
}
