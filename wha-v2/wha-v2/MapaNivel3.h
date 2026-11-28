#pragma once

#include <vector>
#include <string>
#include "Configuracion.h"

class MapaNivel3 {
public:
	static void cargarMatriz(std::vector<std::string>& matriz) {
		int filas = Configuracion::FILAS_MAPA_NIVEL3;
		int cols = Configuracion::COLUMNAS_MAPA_NIVEL3;
		matriz.clear();
		matriz.reserve(filas);
		for (int r = 0; r < filas; r++) {
			matriz.push_back(std::string(cols, '#'));
		}

		for (int r = 14; r <= 30; r++) {
			for (int c = 4; c <= 26; c++) {
				matriz[r][c] = '.';
			}
		}
		for (int r = 20; r <= 24; r++) {
			for (int c = 9; c <= 26; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 16; r <= 17; r++) {
			for (int c = 10; c <= 12; c++) {
				matriz[r][c] = 'C';
			}
		}
		matriz[26][14] = 'C';
		matriz[26][15] = 'C';
		matriz[18][21] = 'O';
		matriz[27][20] = 'O';

		for (int r = 20; r <= 24; r++) {
			for (int c = 27; c <= 67; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 6; r <= 19; r++) {
			for (int c = 29; c <= 33; c++) matriz[r][c] = ';';
			for (int c = 58; c <= 62; c++) matriz[r][c] = ';';
		}
		for (int r = 6; r <= 13; r++) {
			for (int c = 29; c <= 62; c++) {
				matriz[r][c] = '.';
			}
		}
		matriz[8][38] = 'O';
		matriz[12][38] = 'O';
		matriz[7][54] = 'O';

		for (int r = 25; r <= 38; r++) {
			for (int c = 29; c <= 33; c++) matriz[r][c] = ';';
			for (int c = 58; c <= 62; c++) matriz[r][c] = ';';
		}
		for (int r = 31; r <= 38; r++) {
			for (int c = 29; c <= 62; c++) {
				matriz[r][c] = '.';
			}
		}
		matriz[32][39] = 'O';
		matriz[36][53] = 'O';

		for (int r = 6; r <= 38; r++) {
			for (int c = 45; c <= 48; c++) {
				bool esPuenteNorte = (r >= 9 && r <= 11);
				bool esPuenteCentro = (r >= 20 && r <= 24);
				bool esPuenteSur = (r >= 33 && r <= 35);
				if (esPuenteNorte || esPuenteCentro || esPuenteSur) matriz[r][c] = '=';
				else matriz[r][c] = '~';
			}
		}
		matriz[20][44] = 'P';
		matriz[24][44] = 'P';
		matriz[20][49] = 'P';
		matriz[24][49] = 'P';

		for (int r = 8; r <= 36; r++) {
			for (int c = 68; c <= 98; c++) {
				if (r == 8 || r == 36 || c == 68 || c == 98) matriz[r][c] = '#';
				else matriz[r][c] = '.';
			}
		}
		for (int r = 20; r <= 24; r++) {
			matriz[r][68] = 'X';
		}

		for (int r = 12; r <= 32; r++) {
			for (int c = 72; c <= 95; c++) {
				matriz[r][c] = '_';
			}
		}
		matriz[13][74] = 'P';
		matriz[31][74] = 'P';
		matriz[13][83] = 'P';
		matriz[31][83] = 'P';
		matriz[13][93] = 'P';
		matriz[31][93] = 'P';

		for (int r = 19; r <= 21; r++) {
			for (int c = 89; c <= 90; c++) {
				matriz[r][c] = 'T';
			}
		}
	}
};
