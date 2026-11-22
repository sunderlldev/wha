#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <gcroot.h>
#include "Configuracion.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Imaging;

class MapaTileMatrix {
private:
	std::vector<std::string> matriz;
	int filas;
	int columnas;
	int tileSize;
	int nivelActual;
	bool esPozo;

	gcroot<Bitmap^> bmpMadera;
	gcroot<Bitmap^> bmpMuro;
	gcroot<Bitmap^> bmpCamino;
	gcroot<Bitmap^> bmpRoca;
	gcroot<Bitmap^> bmpPuerta;
	gcroot<Bitmap^> bmpAgua;
	gcroot<Bitmap^> bmpAgua2;
	gcroot<Bitmap^> bmpBosque;
	gcroot<Bitmap^> bmpSendero;
	gcroot<Bitmap^> bmpPuente;
	gcroot<Bitmap^> bmpEstante;
	gcroot<Bitmap^> bmpPilar;
	gcroot<Bitmap^> bmpLampara;
	gcroot<Bitmap^> bmpAlfombra;
	gcroot<Bitmap^> bmpEscalera;
	gcroot<Bitmap^> bmpArbolBorde;
	gcroot<Bitmap^> bmpFlor;
	gcroot<Bitmap^> bmpMesa;
	gcroot<Bitmap^> bmpCaja;

	static unsigned int SiguienteAleatorio(unsigned int& semilla) {
		semilla = semilla * 1664525u + 1013904223u;
		return semilla;
	}

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		if (System::IO::File::Exists(nombreArchivo)) {
			return gcnew Bitmap(nombreArchivo);
		}
		String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
		String^ ruta1 = System::IO::Path::Combine(baseDir, nombreArchivo);
		if (System::IO::File::Exists(ruta1)) {
			return gcnew Bitmap(ruta1);
		}
		String^ ruta2 = System::IO::Path::Combine(baseDir, "..\\..\\wha-v2\\" + nombreArchivo);
		if (System::IO::File::Exists(ruta2)) {
			return gcnew Bitmap(ruta2);
		}
		String^ ruta3 = System::IO::Path::Combine(baseDir, "..\\..\\..\\wha-v2\\wha-v2\\" + nombreArchivo);
		if (System::IO::File::Exists(ruta3)) {
			return gcnew Bitmap(ruta3);
		}
		return gcnew Bitmap(32, 32);
	}

	static Bitmap^ CrearLienzo32() {
		return gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
	}

	static void PintarFondo(Bitmap^ bmp, Color color) {
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(color);
		delete g;
	}

	static Bitmap^ ClonarTile(Bitmap^ fuente) {
		Bitmap^ copia = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(copia);
		g->InterpolationMode = InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = PixelOffsetMode::Half;
		g->DrawImage(fuente, 0, 0, 32, 32);
		delete g;
		return copia;
	}

	static void QuitarFondoComparando(Bitmap^ imagen, Bitmap^ fondo, int tolerancia) {
		for (int y = 0; y < imagen->Height; y++) {
			for (int x = 0; x < imagen->Width; x++) {
				Color a = imagen->GetPixel(x, y);
				Color b = fondo->GetPixel(x % fondo->Width, y % fondo->Height);
				int dr = a.R - b.R;
				int dg = a.G - b.G;
				int db = a.B - b.B;
				if (dr * dr + dg * dg + db * db <= tolerancia * tolerancia) {
					imagen->SetPixel(x, y, Color::FromArgb(0, 0, 0, 0));
				}
			}
		}
	}

	static Bitmap^ MezclarTinte(Bitmap^ fuente, Color tinte, int alpha) {
		Bitmap^ copia = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(copia);
		g->InterpolationMode = InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = PixelOffsetMode::Half;
		g->DrawImage(fuente, 0, 0, 32, 32);
		SolidBrush^ pincel = gcnew SolidBrush(Color::FromArgb(alpha, tinte));
		g->FillRectangle(pincel, 0, 0, 32, 32);
		delete pincel;
		delete g;
		return copia;
	}

	static Bitmap^ CrearTileBosque() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 22, 34, 27));
		unsigned int semilla = 481516u;
		for (int i = 0; i < 130; i++) {
			int x = (int)(SiguienteAleatorio(semilla) % 32);
			int y = (int)(SiguienteAleatorio(semilla) % 32);
			int tono = (int)(SiguienteAleatorio(semilla) % 3);
			Color c = Color::FromArgb(255, 26, 42, 32);
			if (tono == 1) c = Color::FromArgb(255, 18, 29, 23);
			else if (tono == 2) c = Color::FromArgb(255, 31, 48, 36);
			g->FillRectangle(gcnew SolidBrush(c), x, y, 1, 1);
		}
		for (int i = 0; i < 7; i++) {
			int x = 3 + (int)(SiguienteAleatorio(semilla) % 26);
			int y = 5 + (int)(SiguienteAleatorio(semilla) % 24);
			g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 15, 26, 20)), x, y, 1, 4);
			g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 15, 26, 20)), x + 2, y + 2, 1, 3);
		}
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileSendero() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 55, 43, 32));
		unsigned int semilla = 90210u;
		for (int i = 0; i < 100; i++) {
			int x = (int)(SiguienteAleatorio(semilla) % 32);
			int y = (int)(SiguienteAleatorio(semilla) % 32);
			Color c = (SiguienteAleatorio(semilla) % 2 == 0)
				? Color::FromArgb(255, 48, 37, 28)
				: Color::FromArgb(255, 64, 50, 37);
			g->FillRectangle(gcnew SolidBrush(c), x, y, 1, 1);
		}
		for (int i = 0; i < 5; i++) {
			int x = 4 + (int)(SiguienteAleatorio(semilla) % 24);
			int y = 5 + (int)(SiguienteAleatorio(semilla) % 22);
			g->FillEllipse(gcnew SolidBrush(Color::FromArgb(255, 74, 62, 48)), x, y, 2, 1);
		}
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTilePuente() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 92, 62, 36));
		SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(255, 61, 41, 26));
		for (int x = 0; x < 32; x += 8) {
			g->FillRectangle(linea, x, 0, 2, 32);
		}
		g->FillRectangle(linea, 0, 4, 32, 2);
		g->FillRectangle(linea, 0, 26, 32, 2);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(255, 120, 84, 48));
		for (int x = 2; x < 32; x += 8) {
			g->FillRectangle(brillo, x, 7, 1, 18);
		}
		delete linea;
		delete brillo;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileEstante(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ oscuro = gcnew SolidBrush(Color::FromArgb(255, 42, 27, 18));
		SolidBrush^ libroA = gcnew SolidBrush(Color::FromArgb(255, 150, 52, 48));
		SolidBrush^ libroB = gcnew SolidBrush(Color::FromArgb(255, 62, 108, 76));
		SolidBrush^ libroC = gcnew SolidBrush(Color::FromArgb(255, 64, 84, 138));
		SolidBrush^ libroD = gcnew SolidBrush(Color::FromArgb(255, 176, 142, 74));
		g->FillRectangle(oscuro, 3, 5, 26, 22);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 78, 50, 30)), 4, 6, 24, 6);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 78, 50, 30)), 4, 14, 24, 6);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 78, 50, 30)), 4, 22, 24, 6);
		int colores[4] = { 0, 1, 2, 3 };
		array<SolidBrush^>^ pinceles = gcnew array<SolidBrush^>{ libroA, libroB, libroC, libroD };
		for (int i = 0; i < 7; i++) {
			int x = 5 + i * 3;
			g->FillRectangle(pinceles[colores[i % 4]], x, 7, 2, 5);
			g->FillRectangle(pinceles[colores[(i + 2) % 4]], x, 15, 2, 5);
			g->FillRectangle(pinceles[colores[(i + 1) % 4]], x, 23, 2, 5);
		}
		g->FillRectangle(oscuro, 3, 5, 26, 1);
		g->FillRectangle(oscuro, 3, 12, 26, 1);
		g->FillRectangle(oscuro, 3, 20, 26, 1);
		g->FillRectangle(oscuro, 3, 28, 26, 1);
		delete oscuro;
		delete libroA;
		delete libroB;
		delete libroC;
		delete libroD;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTilePilar(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ piedra = gcnew SolidBrush(Color::FromArgb(255, 96, 100, 106));
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(255, 62, 66, 72));
		SolidBrush^ luz = gcnew SolidBrush(Color::FromArgb(255, 130, 134, 140));
		g->FillRectangle(sombra, 10, 3, 14, 27);
		g->FillRectangle(piedra, 11, 4, 12, 25);
		g->FillRectangle(luz, 13, 5, 3, 23);
		g->FillRectangle(sombra, 8, 2, 18, 3);
		g->FillRectangle(sombra, 8, 28, 18, 3);
		delete piedra;
		delete sombra;
		delete luz;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileLampara(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, 255, 214, 120));
		SolidBrush^ metal = gcnew SolidBrush(Color::FromArgb(255, 70, 58, 44));
		SolidBrush^ llama = gcnew SolidBrush(Color::FromArgb(255, 255, 210, 110));
		g->FillEllipse(brillo, 3, 3, 26, 26);
		g->FillRectangle(metal, 14, 14, 4, 12);
		g->FillEllipse(metal, 10, 24, 12, 4);
		g->FillEllipse(llama, 12, 7, 8, 10);
		g->FillEllipse(gcnew SolidBrush(Color::FromArgb(255, 255, 244, 190)), 14, 9, 4, 6);
		delete brillo;
		delete metal;
		delete llama;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileAlfombra() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 74, 28, 34));
		unsigned int semilla = 31337u;
		for (int i = 0; i < 42; i++) {
			int x = (int)(SiguienteAleatorio(semilla) % 32);
			int y = (int)(SiguienteAleatorio(semilla) % 32);
			Color c = (SiguienteAleatorio(semilla) % 2 == 0)
				? Color::FromArgb(255, 68, 24, 30)
				: Color::FromArgb(255, 82, 32, 38);
			g->FillRectangle(gcnew SolidBrush(c), x, y, 1, 1);
		}
		SolidBrush^ oro = gcnew SolidBrush(Color::FromArgb(95, 202, 158, 78));
		g->FillPolygon(oro, gcnew array<Point>{ Point(16, 12), Point(20, 16), Point(16, 20), Point(12, 16) });
		SolidBrush^ centro = gcnew SolidBrush(Color::FromArgb(140, 226, 184, 96));
		g->FillRectangle(centro, 15, 15, 2, 2);
		delete oro;
		delete centro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileEscalera(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ escalon = gcnew SolidBrush(Color::FromArgb(255, 112, 74, 42));
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(255, 58, 38, 24));
		for (int i = 0; i < 4; i++) {
			g->FillRectangle(sombra, 4, 5 + i * 6, 24, 2);
			g->FillRectangle(escalon, 4, 7 + i * 6, 24, 4);
		}
		delete escalon;
		delete sombra;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileArbolBorde() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 14, 24, 19));
		SolidBrush^ tronco = gcnew SolidBrush(Color::FromArgb(255, 57, 39, 25));
		SolidBrush^ copa = gcnew SolidBrush(Color::FromArgb(255, 20, 50, 34));
		SolidBrush^ copa2 = gcnew SolidBrush(Color::FromArgb(255, 25, 62, 42));
		g->FillRectangle(tronco, 14, 20, 4, 12);
		g->FillPolygon(copa, gcnew array<Point>{ Point(16, 1), Point(2, 22), Point(30, 22) });
		g->FillPolygon(copa2, gcnew array<Point>{ Point(16, 8), Point(6, 26), Point(26, 26) });
		g->FillPolygon(copa, gcnew array<Point>{ Point(16, 15), Point(9, 30), Point(23, 30) });
		delete tronco;
		delete copa;
		delete copa2;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileFlor() {
		Bitmap^ bmp = CrearTileBosque();
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ tallo = gcnew SolidBrush(Color::FromArgb(255, 34, 62, 40));
		SolidBrush^ petalo = gcnew SolidBrush(Color::FromArgb(255, 150, 128, 190));
		SolidBrush^ centro = gcnew SolidBrush(Color::FromArgb(255, 232, 204, 120));
		g->FillRectangle(tallo, 8, 21, 1, 7);
		g->FillRectangle(tallo, 14, 23, 1, 5);
		g->FillRectangle(tallo, 23, 20, 1, 8);
		g->FillRectangle(petalo, 7, 18, 3, 3);
		g->FillRectangle(petalo, 13, 20, 3, 3);
		g->FillRectangle(petalo, 22, 17, 3, 3);
		g->FillRectangle(centro, 8, 19, 1, 1);
		g->FillRectangle(centro, 14, 21, 1, 1);
		g->FillRectangle(centro, 23, 18, 1, 1);
		delete tallo;
		delete petalo;
		delete centro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileMesa(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ tabla = gcnew SolidBrush(Color::FromArgb(255, 104, 68, 38));
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(255, 55, 35, 22));
		g->FillRectangle(sombra, 5, 8, 22, 13);
		g->FillRectangle(tabla, 6, 9, 20, 11);
		g->FillRectangle(sombra, 8, 20, 3, 8);
		g->FillRectangle(sombra, 21, 20, 3, 8);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 226, 214, 180)), 10, 11, 8, 6);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(255, 120, 60, 50)), 11, 12, 2, 4);
		delete tabla;
		delete sombra;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileAguaVariante(Bitmap^ agua, int desplazamiento, int alpha) {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->InterpolationMode = InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = PixelOffsetMode::Half;
		g->DrawImage(agua, 0, 0, 32, 32);
		SolidBrush^ tinte = gcnew SolidBrush(Color::FromArgb(alpha, 8, 24, 54));
		g->FillRectangle(tinte, 0, 0, 32, 32);
		SolidBrush^ luz = gcnew SolidBrush(Color::FromArgb(90, 96, 170, 220));
		for (int i = 0; i < 3; i++) {
			int y = (desplazamiento + i * 11) % 32;
			g->FillRectangle(luz, (i * 9) % 12, y, 16, 1);
			g->FillRectangle(luz, 18 + (i * 3) % 8, (y + 16) % 32, 10, 1);
		}
		delete tinte;
		delete luz;
		delete g;
		return bmp;
	}

	void ConstruirNivel1() {
		filas = Configuracion::FILAS_MAPA_NIVEL1;
		columnas = Configuracion::COLUMNAS_MAPA_NIVEL1;
		matriz.clear();
		matriz.reserve(filas);
		for (int r = 0; r < filas; r++) {
			matriz.push_back(std::string(columnas, ','));
		}

		for (int r = 8; r <= 40; r++) {
			for (int c = 8; c <= 52; c++) {
				matriz[r][c] = 'W';
			}
		}

		for (int c = 8; c <= 52; c++) {
			matriz[8][c] = '#';
			matriz[40][c] = '#';
		}
		for (int r = 8; r <= 40; r++) {
			matriz[r][8] = '#';
			matriz[r][52] = '#';
		}
		matriz[23][52] = 'D';
		matriz[24][52] = 'D';
		matriz[40][19] = 'd';
		matriz[40][20] = 'd';

		for (int c = 12; c <= 26; c++) {
			matriz[12][c] = '#';
			matriz[20][c] = '#';
			matriz[28][c] = '#';
			matriz[36][c] = '#';
		}
		for (int r = 12; r <= 20; r++) {
			matriz[r][12] = '#';
			matriz[r][26] = '#';
		}
		for (int r = 28; r <= 36; r++) {
			matriz[r][12] = '#';
			matriz[r][26] = '#';
		}
		for (int c = 34; c <= 48; c++) {
			matriz[12][c] = '#';
			matriz[20][c] = '#';
			matriz[28][c] = '#';
			matriz[36][c] = '#';
		}
		for (int r = 12; r <= 20; r++) {
			matriz[r][34] = '#';
			matriz[r][48] = '#';
		}
		for (int r = 28; r <= 36; r++) {
			matriz[r][34] = '#';
			matriz[r][48] = '#';
		}

		matriz[20][18] = 'W';
		matriz[20][19] = 'W';
		matriz[20][40] = 'W';
		matriz[20][41] = 'W';
		matriz[28][18] = 'W';
		matriz[28][19] = 'W';
		matriz[28][40] = 'W';
		matriz[28][41] = 'W';

		for (int c = 14; c <= 24; c++) {
			matriz[13][c] = 'B';
			matriz[35][c] = 'B';
		}
		for (int c = 36; c <= 46; c++) {
			matriz[13][c] = 'B';
			matriz[35][c] = 'B';
		}
		matriz[15][14] = 'M';
		matriz[15][15] = 'M';
		matriz[15][45] = 'M';
		matriz[15][46] = 'M';
		matriz[32][14] = 'M';
		matriz[32][15] = 'M';
		matriz[32][45] = 'M';
		matriz[32][46] = 'M';

		for (int r = 22; r <= 26; r++) {
			for (int c = 28; c <= 32; c++) {
				matriz[r][c] = '_';
			}
		}
		for (int r = 14; r <= 18; r++) {
			for (int c = 16; c <= 22; c++) {
				if (matriz[r][c] == 'W') matriz[r][c] = '_';
			}
		}
		for (int r = 30; r <= 34; r++) {
			for (int c = 16; c <= 22; c++) {
				if (matriz[r][c] == 'W') matriz[r][c] = '_';
			}
		}

		matriz[9][9] = 'L';
		matriz[9][51] = 'L';
		matriz[39][9] = 'L';
		matriz[39][51] = 'L';
		matriz[22][30] = 'P';
		matriz[22][31] = 'P';
		matriz[26][30] = 'P';
		matriz[26][31] = 'P';
		matriz[38][30] = 'S';
		matriz[38][31] = 'S';
		matriz[9][10] = 'B';
		matriz[9][11] = 'B';
		matriz[9][49] = 'B';
		matriz[9][50] = 'B';
		matriz[39][10] = 'B';
		matriz[39][11] = 'B';
		matriz[39][49] = 'B';
		matriz[39][50] = 'B';
		matriz[21][27] = 'M';
		matriz[27][33] = 'M';
		matriz[10][10] = 'C';
		matriz[10][50] = 'C';
		matriz[38][50] = 'C';
		matriz[30][10] = 'C';

		for (int r = 22; r <= 25; r++) {
			for (int c = 53; c <= 69; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 24; r <= 45; r++) {
			for (int c = 66; c <= 69; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 43; r <= 46; r++) {
			for (int c = 67; c <= 74; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 43; r <= 46; r++) {
			for (int c = 74; c <= 96; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 45; r <= 62; r++) {
			for (int c = 94; c <= 97; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 58; r <= 63; r++) {
			for (int c = 98; c <= 106; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 41; r <= 67; r++) {
			for (int c = 18; c <= 21; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 64; r <= 68; r++) {
			for (int c = 19; c <= 27; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 11; r <= 24; r++) {
			for (int c = 58; c <= 61; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 5; r <= 9; r++) {
			for (int c = 56; c <= 64; c++) {
				matriz[r][c] = '.';
			}
		}
		for (int c = 55; c <= 65; c++) {
			matriz[4][c] = 'O';
			matriz[10][c] = 'O';
		}
		for (int r = 5; r <= 10; r++) {
			matriz[r][55] = 'O';
			matriz[r][65] = 'O';
		}

		for (int r = 3; r <= 76; r++) {
			for (int c = 70; c <= 73; c++) {
				matriz[r][c] = '~';
			}
		}
		for (int r = 43; r <= 46; r++) {
			for (int c = 70; c <= 73; c++) {
				matriz[r][c] = '=';
			}
		}

		for (int r = 8; r <= 12; r++) {
			for (int c = 107; c <= 113; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 0; r < filas; r++) {
			for (int c = 0; c < columnas; c++) {
				if (r <= 2 || r >= filas - 3 || c <= 2 || c >= columnas - 3) {
					matriz[r][c] = 'T';
				}
			}
		}

		unsigned int semilla = 777u;
		for (int i = 0; i < 90; i++) {
			int r = 4 + (int)(SiguienteAleatorio(semilla) % (filas - 8));
			int c = 4 + (int)(SiguienteAleatorio(semilla) % (columnas - 8));
			if (matriz[r][c] != ',') continue;
			if (r >= 6 && r <= 42 && c >= 6 && c <= 54) continue;
			if (c >= 66 && c <= 76) continue;
			if (r >= 40 && r <= 48 && c >= 52 && c <= 98) continue;
			if (r >= 40 && r <= 70 && c >= 16 && c <= 30) continue;
			if (r >= 56 && r <= 66 && c >= 92 && c <= 110) continue;
			if (r >= 6 && r <= 14 && c >= 105 && c <= 115) continue;
			matriz[r][c] = 'O';
		}

		for (int i = 0; i < 70; i++) {
			int r = 4 + (int)(SiguienteAleatorio(semilla) % (filas - 8));
			int c = 4 + (int)(SiguienteAleatorio(semilla) % (columnas - 8));
			if (matriz[r][c] != ',') continue;
			if (r >= 6 && r <= 42 && c >= 6 && c <= 54) continue;
			if (r >= 40 && r <= 70 && c >= 16 && c <= 30) continue;
			if (r >= 56 && r <= 66 && c >= 92 && c <= 110) continue;
			matriz[r][c] = 'F';
		}
	}

	void ConstruirPozo() {
		const char* filasPozo[25] = {
			R"(+--------------------------------------------------------------------------------+)",
			R"(|                                                                                |)",
			R"(|                                                                                |)",
			R"(|     +---------+                                                                |)",
			R"(|     |  /===/  |                                                                |)",
			R"(|     |  /===/  |                            +-----------------------------+     |)",
			R"(|     |  /===/  |                            |                             |     |)",
			R"(|     |  /===/  |                            |                             |     |)",
			R"(|     +         +----------------------------+                             |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     |                                                                    |     |)",
			R"(|     +-------------------------------------+                              |     |)",
			R"(|                                           |                              |     |)",
			R"(|                                           |                              |     |)",
			R"(|                                           +------------------------------+     |)",
			R"(|                                                                                |)",
			R"(|                                                                                |)",
			R"(|                                                                                |)",
			R"(|                                                                                |)",
			R"(|                                                                                |)",
			R"(+--------------------------------------------------------------------------------+)"
		};
		filas = 25;
		columnas = 82;
		matriz.clear();
		matriz.reserve(filas);
		for (int r = 0; r < filas; r++) {
			std::string fila;
			for (const char* p = filasPozo[r]; *p != 0; p++) {
				char ch = *p;
				if (ch == '+' || ch == '-' || ch == '|') fila += '#';
				else fila += 'W';
			}
			while ((int)fila.size() < columnas) fila += 'W';
			if ((int)fila.size() > columnas) fila.resize(columnas);
			matriz.push_back(fila);
		}
		matriz[10][50] = 'C';
		matriz[10][52] = 'C';
		matriz[13][50] = 'C';
		matriz[13][60] = 'C';
		matriz[14][60] = 'C';
	}

public:
	MapaTileMatrix() {
		tileSize = Configuracion::TILE_SIZE;
		filas = 0;
		columnas = 0;
		nivelActual = 1;
		esPozo = false;

		bmpMadera = CargarBitmapSeguro("sprites/tile_madera.png");
		bmpMuro = CargarBitmapSeguro("sprites/tile_muro.png");
		bmpCamino = CargarBitmapSeguro("sprites/tile_camino.png");
		bmpRoca = CargarBitmapSeguro("sprites/tile_roca.png");
		bmpPuerta = CargarBitmapSeguro("sprites/tile_puerta.png");
		bmpAgua = CargarBitmapSeguro("sprites/tile_agua.png");
		bmpCaja = CargarBitmapSeguro("sprites/tile_caja.png");

		bmpBosque = CrearTileBosque();
		bmpSendero = CrearTileSendero();
		bmpPuente = CrearTilePuente();
		bmpEstante = CrearTileEstante(bmpMadera);
		bmpPilar = CrearTilePilar(bmpMadera);
		bmpLampara = CrearTileLampara(bmpMadera);
		bmpAlfombra = CrearTileAlfombra();
		bmpEscalera = CrearTileEscalera(bmpMadera);
		bmpArbolBorde = CrearTileArbolBorde();
		bmpFlor = CrearTileFlor();
		bmpMesa = CrearTileMesa(bmpMadera);
		bmpAgua2 = CrearTileAguaVariante(bmpAgua, 8, 70);

		ConstruirNivel1();
	}

	~MapaTileMatrix() {}

	int getFilas() { return filas; }
	int getColumnas() { return columnas; }
	int getTileSize() { return tileSize; }
	int getAnchoTotalPixels() { return columnas * tileSize; }
	int getAltoTotalPixels() { return filas * tileSize; }
	int getNivelActual() { return nivelActual; }

	void cargarNivel(int numNivel) {
		nivelActual = 1;
		esPozo = false;
		ConstruirNivel1();
	}

	void cargarPozo() {
		nivelActual = 1;
		esPozo = true;
		ConstruirPozo();
	}

	bool getEsPozo() { return esPozo; }
	int getPuntoInicioPozoX() { return 10 * tileSize; }
	int getPuntoInicioPozoY() { return 8 * tileSize; }
	int getPuntoSalidaPozoX() { return 10 * tileSize; }
	int getPuntoSalidaPozoY() { return 6 * tileSize; }

	Rectangle getParedRocaRect() {
		return Rectangle(58 * tileSize, 10 * tileSize, 4 * tileSize, tileSize);
	}

	void romperParedRoca() {
		for (int c = 58; c <= 61; c++) {
			matriz[10][c] = '.';
		}
	}

	bool estaCercaParedRoca(int px, int py, int ancho, int alto) {
		Rectangle pared = getParedRocaRect();
		Rectangle jugador(px, py, ancho, alto);
		Rectangle zona(pared.X - 28, pared.Y - 20, pared.Width + 56, pared.Height + 60);
		return zona.IntersectsWith(jugador);
	}

	int getPuntoInicioX() { return 30 * tileSize; }
	int getPuntoInicioY() { return 24 * tileSize; }
	int getPuntoMyrphonX() { return 102 * tileSize; }
	int getPuntoMyrphonY() { return 60 * tileSize; }
	int getPuntoGatoX() { return 23 * tileSize; }
	int getPuntoGatoY() { return 66 * tileSize; }
	int getPuntoTrollX() { return 110 * tileSize; }
	int getPuntoTrollY() { return 10 * tileSize; }

	char getCaracter(int col, int fila) {
		if (fila < 0 || fila >= filas || col < 0 || col >= columnas) return '#';
		return matriz[fila][col];
	}

	void setCaracter(int col, int fila, char valor) {
		if (fila < 0 || fila >= filas || col < 0 || col >= columnas) return;
		matriz[fila][col] = valor;
	}

	void abrirPuertaSalida() {
		for (int r = 0; r < filas; r++) {
			for (int c = 0; c < columnas; c++) {
				if (matriz[r][c] == 'X') matriz[r][c] = 'D';
			}
		}
	}

	bool esPuertaSellada(int pixelX, int pixelY) {
		int col = pixelX / tileSize;
		int fila = pixelY / tileSize;
		return getCaracter(col, fila) == 'X';
	}

	static bool esSolido(char ch) {
		return ch == '#' || ch == 'O' || ch == 'B' || ch == 'C' || ch == 'P' || ch == 'T' || ch == 'M' || ch == 'X' || ch == '~';
	}

	bool esColisionSolida(int pixelX, int pixelY, int ancho, int alto) {
		int puntosX[4] = { pixelX + 4, pixelX + ancho - 4, pixelX + 4, pixelX + ancho - 4 };
		int puntosY[4] = { pixelY + alto / 2, pixelY + alto / 2, pixelY + alto - 2, pixelY + alto - 2 };
		for (int i = 0; i < 4; i++) {
			int col = puntosX[i] / tileSize;
			int fila = puntosY[i] / tileSize;
			if (fila < 0 || fila >= filas || col < 0 || col >= columnas) return true;
			if (esSolido(matriz[fila][col])) return true;
		}
		return false;
	}

	bool esColisionSolidaRect(Rectangle rect) {
		return esColisionSolida(rect.X, rect.Y, rect.Width, rect.Height);
	}

	bool esSalidaNivel(int pixelX, int pixelY, int ancho, int alto) {
		int puntosX[4] = { pixelX + 4, pixelX + ancho - 4, pixelX + 4, pixelX + ancho - 4 };
		int puntosY[4] = { pixelY + 4, pixelY + 4, pixelY + alto - 4, pixelY + alto - 4 };
		for (int i = 0; i < 4; i++) {
			int col = puntosX[i] / tileSize;
			int fila = puntosY[i] / tileSize;
			if (fila >= 0 && fila < filas && col >= 0 && col < columnas) {
				if (matriz[fila][col] == 'D') return true;
			}
		}
		return false;
	}

	bool estaDentroAtelier(int pixelX, int pixelY) {
		return pixelX >= 8 * tileSize && pixelX <= 52 * tileSize && pixelY >= 8 * tileSize && pixelY <= 40 * tileSize;
	}

	String^ getNombreUbicacion(int pixelX, int pixelY) {
		if (esPozo) return "Sótano de Richeh";
		int col = pixelX / tileSize;
		int fila = pixelY / tileSize;
		if (col >= 55 && col <= 65 && fila >= 4 && fila <= 12) return "Cueva Misteriosa";
		char ch = getCaracter(col, fila);
		bool muro = (ch == '#' || ch == 'B' || ch == 'C' || ch == 'P' || ch == 'M' || ch == 'O');
		if (!muro) {
			if (col >= 13 && col <= 25 && fila >= 13 && fila <= 19) return "Despacho de Qifrey";
			if (col >= 35 && col <= 47 && fila >= 13 && fila <= 19) return "Torre de Agott";
			if (col >= 13 && col <= 25 && fila >= 29 && fila <= 35) return "Biblioteca";
			if (col >= 35 && col <= 47 && fila >= 29 && fila <= 35) return "Almacén";
			if (col >= 9 && col <= 51 && fila >= 9 && fila <= 39) return "Gran Salón del Atelier";
		}
		if (col >= 99 && col <= 108 && fila >= 56 && fila <= 65) return "Claro de Myrphon";
		if (col >= 18 && col <= 28 && fila >= 63 && fila <= 69) return "Rincón del Gato";
		if (col >= 105 && col <= 115 && fila >= 7 && fila <= 14) return "Claro Olvidado";
		if (col >= 66 && col <= 76) return "Orilla del Río";
		return "Bosque de Serpentback";
	}

	Bitmap^ obtenerBitmapPorCaracter(char ch, int tick) {
		switch (ch) {
		case '#': return bmpMuro;
		case 'W': return bmpMadera;
		case '.': return bmpCamino;
		case ',': return bmpBosque;
		case ';': return bmpSendero;
		case '~': return (((tick / 12) % 2) == 0) ? bmpAgua : bmpAgua2;
		case '=': return bmpPuente;
		case 'O': return bmpRoca;
		case 'B': return bmpEstante;
		case 'C': return bmpCaja;
		case 'P': return bmpPilar;
		case 'L': return bmpLampara;
		case '_': return bmpAlfombra;
		case 'D': return bmpPuerta;
		case 'X': return bmpPuerta;
		case 'S': return bmpEscalera;
		case 'T': return bmpArbolBorde;
		case 'F': return bmpFlor;
		case 'M': return bmpMesa;
		case 'd': return bmpPuerta;
		default: return bmpBosque;
		}
	}

	void dibujar(Graphics^ g, int camX, int camY, int viewW, int viewH, int zoom, int tick) {
		if (g == nullptr || filas <= 0 || columnas <= 0) return;
		g->InterpolationMode = InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = PixelOffsetMode::Half;

		int anchoMundo = viewW / zoom;
		int altoMundo = viewH / zoom;
		int colInicio = camX / tileSize;
		int colFin = (camX + anchoMundo) / tileSize + 1;
		int filaInicio = camY / tileSize;
		int filaFin = (camY + altoMundo) / tileSize + 1;
		if (colInicio < 0) colInicio = 0;
		if (filaInicio < 0) filaInicio = 0;
		if (colFin >= columnas) colFin = columnas - 1;
		if (filaFin >= filas) filaFin = filas - 1;

		for (int r = filaInicio; r <= filaFin; r++) {
			for (int c = colInicio; c <= colFin; c++) {
				char ch = matriz[r][c];
				Bitmap^ bmpTile = obtenerBitmapPorCaracter(ch, tick);
				if (bmpTile == nullptr) continue;
				int drawX = (c * tileSize - camX) * zoom;
				int drawY = (r * tileSize - camY) * zoom;
				Rectangle destino(drawX, drawY, tileSize * zoom, tileSize * zoom);
				g->DrawImage(bmpTile, destino, 0, 0, tileSize, tileSize, GraphicsUnit::Pixel);
				if (ch == 'X') {
					SolidBrush^ candado = gcnew SolidBrush(Color::FromArgb(235, 220, 170, 60));
					g->FillRectangle(candado, drawX + 12 * zoom, drawY + 14 * zoom, 8 * zoom, 10 * zoom);
					g->FillRectangle(candado, drawX + 14 * zoom, drawY + 10 * zoom, 4 * zoom, 6 * zoom);
					delete candado;
				}
			}
		}
	}
};
