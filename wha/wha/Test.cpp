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

void imprimirMiniASCII(Pixel imagen[2][2]) {
    char bloque = 219;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Pixel p = imagen[i][j];

            // Si es transparente, movemos el cursor un espacio sin pintar bloque
            if (p.transparente) {
                std::cout << "\033[0m  "; // Restablece color y avanza 2 espacios
            }
            else {
                std::cout << "\033[38;2;" << p.r << ";" << p.g << ";" << p.b << "m";
                std::cout << bloque;
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

    Pixel spriteCoustas2[15][15] = {
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {0, 0, 0, true}, {38, 25, 21, false}, {38, 25, 21, false}, {38, 25, 21, false}, {38, 25, 21, false}, {38, 25, 21, false}, {0, 0, 0, true}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {56, 43, 38, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {56, 43, 38, false}, {56, 43, 38, false}, {38, 25, 21, false}, {38, 25, 21, false}, {56, 43, 38, false}, {38, 25, 21, false}, {38, 25, 21, false}, {56, 43, 38, false}, {56, 43, 38, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {56, 43, 38, false}, {38, 25, 21, false}, {193, 149, 120, false}, {112, 86, 35, false}, {38, 25, 21, false}, {112, 86, 35, false}, {193, 149, 120, false}, {38, 25, 21, false}, {56, 43, 38, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {158, 111, 77, false}, {193, 149, 120, false}, {81, 57, 21, false}, {158, 111, 77, false}, {81, 57, 21, false}, {193, 149, 120, false}, {158, 111, 77, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {193, 149, 120, false}, {193, 149, 120, false}, {193, 149, 120, false}, {193, 149, 120, false}, {193, 149, 120, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {38, 25, 21, false}, {56, 43, 38, false}, {63, 38, 29, false}, {158, 111, 77, false}, {193, 149, 120, false}, {158, 111, 77, false}, {63, 38, 29, false}, {56, 43, 38, false}, {38, 25, 21, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {63, 38, 29, false}, {158, 140, 137, false}, {211, 187, 182, false}, {211, 187, 182, false}, {211, 187, 182, false}, {158, 140, 137, false}, {63, 38, 29, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {63, 38, 29, false}, {158, 111, 77, false}, {130, 97, 104, false}, {158, 140, 137, false}, {211, 187, 182, false}, {158, 140, 137, false}, {130, 97, 104, false}, {158, 111, 77, false}, {63, 38, 29, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {63, 38, 29, false}, {107, 96, 42, false}, {168, 148, 62, false}, {158, 140, 137, false}, {168, 148, 62, false}, {107, 96, 42, false}, {63, 38, 29, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {63, 38, 29, false}, {104, 73, 61, false}, {68, 30, 30, false}, {104, 73, 61, false}, {63, 38, 29, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}}
    };

    Pixel spriteTartah[15][15] = {
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {81, 73, 25, false}, {81, 73, 25, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {81, 73, 25, false}, {196, 173, 60, false}, {196, 173, 60, false}, {162, 196, 84, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {162, 196, 84, false}, {162, 196, 84, false}, {162, 196, 84, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {126, 153, 65, false}, {162, 196, 84, false}, {162, 196, 84, false}, {162, 196, 84, false}, {126, 153, 65, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {126, 153, 65, false}, {220, 224, 186, false}, {220, 224, 186, false}, {220, 224, 186, false}, {126, 153, 65, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {117, 119, 100, false}, {220, 224, 186, false}, {220, 224, 186, false}, {220, 224, 186, false}, {183, 188, 133, false}, {220, 224, 186, false}, {220, 224, 186, false}, {220, 224, 186, false}, {117, 119, 100, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {117, 119, 100, false}, {220, 224, 186, false}, {211, 134, 67, false}, {211, 134, 67, false}, {211, 134, 67, false}, {211, 134, 67, false}, {211, 134, 67, false}, {220, 224, 186, false}, {117, 119, 100, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {158, 88, 31, false}, {226, 189, 154, false}, {247, 228, 190, false}, {157, 158, 181, false}, {247, 228, 190, false}, {157, 158, 181, false}, {247, 228, 190, false}, {226, 189, 154, false}, {158, 88, 31, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {158, 88, 31, false}, {226, 189, 154, false}, {247, 228, 190, false}, {110, 111, 127, false}, {226, 189, 154, false}, {110, 111, 127, false}, {247, 228, 190, false}, {226, 189, 154, false}, {158, 88, 31, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {158, 88, 31, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {247, 228, 190, false}, {158, 88, 31, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {114, 62, 37, false}, {112, 78, 62, false}, {226, 189, 154, false}, {112, 78, 62, false}, {114, 62, 37, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {162, 196, 84, false}, {112, 78, 62, false}, {198, 175, 165, false}, {112, 78, 62, false}, {162, 196, 84, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {226, 189, 154, false}, {126, 153, 65, false}, {162, 196, 84, false}, {198, 175, 165, false}, {162, 196, 84, false}, {126, 153, 65, false}, {226, 189, 154, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}},
    {{0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {59, 81, 8, false}, {91, 55, 48, false}, {119, 92, 74, false}, {142, 110, 89, false}, {119, 92, 74, false}, {91, 55, 48, false}, {59, 81, 8, false}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}, {0, 0, 0, true}}
    };

    Pixel MiniCoco[2][2] = {
        {{181, 198, 81, false},{181, 198, 81, false}},
        {{114, 204, 186, false},{114, 204, 186, false}}
    };

    std::cout << "--- RENDERIZADO 15x30 ASCII COLOR ---\n\n";
    imprimirPixelArtASCII(pixelArt);

    std::cout << "--- RENDERIZADO 2x2 ASCII COLOR ---\n\n";
    imprimirMiniASCII(MiniCoco);

    std::cout << std::endl << std::endl << "Presiona Enter para cerrar el programa...";
    std::cin.get();

    return 0;
}