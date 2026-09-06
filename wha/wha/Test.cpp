#include <iostream>
#include <windows.h> // Para activar el color ANSI en Windows

struct Pixel {
    int r, g, b;
    bool transparente; // true si no se debe pintar
};

// Activa el soporte de secuencias ANSI en la consola de Windows
void activarANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void imprimirPixelArtASCII(Pixel imagen[15][15]) {
    char bloque = 219;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            Pixel p = imagen[i][j];

            // Si es transparente, movemos el cursor un espacio sin pintar bloque
            if (p.transparente) {
                std::cout << "\033[0m  "; // Restablece color y avanza 2 espacios
            }
            else {
                std::cout << "\033[38;2;" << p.r << ";" << p.g << ";" << p.b << "m";
                std::cout << bloque << bloque;
            }
        }
        std::cout << "\033[0m" << std::endl;
    }
}

int main() {
    activarANSI(); // Importante para que no salgan los símbolos <-[38;2;...

    Pixel pixelArt[15][15] = {
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {81, 73, 25, false}, {81, 73, 25, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {81, 73, 25, false}, {255, 228, 79, false}, {255, 228, 79, false}, {114, 204, 186, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {114, 204, 186, false}, {114, 204, 186, false}, {114, 204, 186, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {85, 153, 139, false}, {114, 204, 186, false}, {114, 204, 186, false}, {114, 204, 186, false}, {85, 153, 139, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {85, 153, 139, false}, {255, 255, 255, false}, {255, 255, 255, false}, {255, 255, 255, false}, {85, 153, 139, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {114, 204, 186, false}, {255, 255, 255, false}, {255, 255, 255, false}, {229, 215, 137, false}, {255, 255, 255, false}, {255, 255, 255, false}, {114, 204, 186, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {114, 204, 186, false}, {144, 158, 64, false}, {144, 158, 64, false}, {144, 158, 64, false}, {144, 158, 64, false}, {144, 158, 64, false}, {114, 204, 186, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {56, 61, 25, false}, {144, 158, 64, false}, {226, 189, 154, false}, {247, 228, 190, false}, {181, 198, 81, false}, {247, 228, 190, false}, {181, 198, 81, false}, {247, 228, 190, false}, {226, 189, 154, false}, {144, 158, 64, false}, {56, 61, 25, false}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {56, 61, 25, false}, {226, 189, 154, false}, {247, 228, 190, false}, {144, 158, 64, false}, {226, 189, 154, false}, {144, 158, 64, false}, {247, 228, 190, false}, {226, 189, 154, false}, {56, 61, 25, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {56, 61, 25, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {56, 61, 25, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {114, 204, 186, false}, {47, 84, 76, false}, {114, 204, 186, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {112, 175, 151, false}, {114, 204, 186, false}, {114, 204, 186, false}, {114, 204, 186, false}, {112, 175, 151, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {226, 189, 154, false}, {128, 128, 128, false}, {255, 255, 255, false}, {114, 204, 186, false}, {255, 255, 255, false}, {128, 128, 128, false}, {226, 189, 154, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {47, 84, 76, false}, {81, 73, 25, false}, {255, 228, 79, false}, {128, 128, 128, false}, {255, 228, 79, false}, {81, 73, 25, false}, {47, 84, 76, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}}
    };

    std::cout << "--- RENDERIZADO 15x15 ASCII COLOR ---\n\n";
    imprimirPixelArtASCII(pixelArt);

    std::cout << std::endl << std::endl << "Presiona Enter para cerrar el programa...";
    std::cin.get();

    return 0;
}