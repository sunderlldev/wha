#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <gcroot.h>
#include "Configuracion.h"
#include "Utilidades.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Imaging;

class MapaNivel1 {
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
	gcroot<Bitmap^> bmpAgua3;
	gcroot<Bitmap^> bmpAgua4;
	gcroot<Bitmap^> bmpBosque;
	gcroot<Bitmap^> bmpBosque2;
	gcroot<Bitmap^> bmpBosque3;
	gcroot<Bitmap^> bmpSendero;
	gcroot<Bitmap^> bmpSendero2;
	gcroot<Bitmap^> bmpSendero3;
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
	gcroot<Bitmap^> bmpSueloSubterraneo;
	gcroot<Bitmap^> bmpRocaSubterranea;

	static unsigned int SiguienteAleatorio(unsigned int& semilla) {
		semilla = semilla * 1664525u + 1013904223u;
		return semilla;
	}

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		return CargarBitmapSeguroComun(nombreArchivo, true);
	}

	static Bitmap^ CrearLienzo32() {
		return gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
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

	static Bitmap^ CrearTileBosqueVariante(unsigned int semillaBase) {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 24, 42, 30));
		unsigned int semilla = semillaBase;
		SolidBrush^ b1 = gcnew SolidBrush(Color::FromArgb(255, 30, 52, 36));
		SolidBrush^ b2 = gcnew SolidBrush(Color::FromArgb(255, 19, 34, 24));
		SolidBrush^ b3 = gcnew SolidBrush(Color::FromArgb(255, 38, 62, 43));
		SolidBrush^ musgo = gcnew SolidBrush(Color::FromArgb(255, 28, 48, 33));
		SolidBrush^ bHierba = gcnew SolidBrush(Color::FromArgb(255, 15, 28, 20));
		SolidBrush^ bPunta = gcnew SolidBrush(Color::FromArgb(255, 48, 76, 52));

		for (int i = 0; i < 4; i++) {
			int px = (int)(SiguienteAleatorio(semilla) % 24);
			int py = (int)(SiguienteAleatorio(semilla) % 24);
			g->FillEllipse(musgo, px, py, 8, 6);
		}
		for (int i = 0; i < 180; i++) {
			int x = (int)(SiguienteAleatorio(semilla) % 32);
			int y = (int)(SiguienteAleatorio(semilla) % 32);
			int tono = (int)(SiguienteAleatorio(semilla) % 3);
			SolidBrush^ pincel = (tono == 0) ? b1 : ((tono == 1) ? b2 : b3);
			g->FillRectangle(pincel, x, y, 1, 1);
		}
		for (int i = 0; i < 10; i++) {
			int x = 2 + (int)(SiguienteAleatorio(semilla) % 26);
			int y = 4 + (int)(SiguienteAleatorio(semilla) % 24);
			g->FillRectangle(bHierba, x, y, 1, 4);
			g->FillRectangle(bPunta, x + 1, y - 1, 1, 2);
			g->FillRectangle(bHierba, x + 2, y + 1, 1, 3);
		}
		delete b1;
		delete b2;
		delete b3;
		delete musgo;
		delete bHierba;
		delete bPunta;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileSenderoVariante(unsigned int semillaBase) {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 62, 47, 34));
		unsigned int semilla = semillaBase;
		SolidBrush^ parcheA = gcnew SolidBrush(Color::FromArgb(255, 56, 42, 30));
		SolidBrush^ parcheB = gcnew SolidBrush(Color::FromArgb(255, 70, 54, 39));
		SolidBrush^ granoOsc = gcnew SolidBrush(Color::FromArgb(255, 46, 35, 25));
		SolidBrush^ granoCla = gcnew SolidBrush(Color::FromArgb(255, 78, 61, 45));
		SolidBrush^ piedra = gcnew SolidBrush(Color::FromArgb(255, 92, 78, 62));
		SolidBrush^ piedraOsc = gcnew SolidBrush(Color::FromArgb(255, 40, 31, 23));

		for (int i = 0; i < 4; i++) {
			int px = (int)(SiguienteAleatorio(semilla) % 24);
			int py = (int)(SiguienteAleatorio(semilla) % 24);
			g->FillEllipse((i % 2 == 0) ? parcheA : parcheB, px, py, 9, 6);
		}
		for (int i = 0; i < 160; i++) {
			int x = (int)(SiguienteAleatorio(semilla) % 32);
			int y = (int)(SiguienteAleatorio(semilla) % 32);
			g->FillRectangle((SiguienteAleatorio(semilla) % 2 == 0) ? granoOsc : granoCla, x, y, 1, 1);
		}
		for (int i = 0; i < 7; i++) {
			int x = 3 + (int)(SiguienteAleatorio(semilla) % 24);
			int y = 4 + (int)(SiguienteAleatorio(semilla) % 23);
			g->FillRectangle(piedraOsc, x, y + 1, 3, 2);
			g->FillRectangle(piedra, x, y, 3, 2);
		}
		delete parcheA;
		delete parcheB;
		delete granoOsc;
		delete granoCla;
		delete piedra;
		delete piedraOsc;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTilePuente() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 96, 65, 38));
		SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(255, 61, 41, 26));
		for (int x = 0; x < 32; x += 8) {
			g->FillRectangle(linea, x, 0, 2, 32);
		}
		g->FillRectangle(linea, 0, 4, 32, 2);
		g->FillRectangle(linea, 0, 26, 32, 2);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(255, 126, 88, 50));
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
		SolidBrush^ tabla = gcnew SolidBrush(Color::FromArgb(255, 78, 50, 30));
		SolidBrush^ libroA = gcnew SolidBrush(Color::FromArgb(255, 150, 52, 48));
		SolidBrush^ libroB = gcnew SolidBrush(Color::FromArgb(255, 62, 108, 76));
		SolidBrush^ libroC = gcnew SolidBrush(Color::FromArgb(255, 64, 84, 138));
		SolidBrush^ libroD = gcnew SolidBrush(Color::FromArgb(255, 176, 142, 74));
		g->FillRectangle(oscuro, 3, 5, 26, 22);
		g->FillRectangle(tabla, 4, 6, 24, 6);
		g->FillRectangle(tabla, 4, 14, 24, 6);
		g->FillRectangle(tabla, 4, 22, 24, 6);
		array<SolidBrush^>^ pinceles = gcnew array<SolidBrush^>{ libroA, libroB, libroC, libroD };
		for (int i = 0; i < 7; i++) {
			int x = 5 + i * 3;
			g->FillRectangle(pinceles[i % 4], x, 7, 2, 5);
			g->FillRectangle(pinceles[(i + 2) % 4], x, 15, 2, 5);
			g->FillRectangle(pinceles[(i + 1) % 4], x, 23, 2, 5);
		}
		g->FillRectangle(oscuro, 3, 5, 26, 1);
		g->FillRectangle(oscuro, 3, 12, 26, 1);
		g->FillRectangle(oscuro, 3, 20, 26, 1);
		g->FillRectangle(oscuro, 3, 28, 26, 1);
		delete oscuro;
		delete tabla;
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

	static Bitmap^ CrearTileLampara(Bitmap^ base) {
		Bitmap^ bmp = ClonarTile(base);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, 255, 214, 120));
		SolidBrush^ metal = gcnew SolidBrush(Color::FromArgb(255, 70, 58, 44));
		SolidBrush^ llama = gcnew SolidBrush(Color::FromArgb(255, 255, 210, 110));
		SolidBrush^ centro = gcnew SolidBrush(Color::FromArgb(255, 255, 244, 190));
		g->FillEllipse(brillo, 3, 3, 26, 26);
		g->FillRectangle(metal, 14, 14, 4, 12);
		g->FillEllipse(metal, 10, 24, 12, 4);
		g->FillEllipse(llama, 12, 7, 8, 10);
		g->FillEllipse(centro, 14, 9, 4, 6);
		delete brillo;
		delete metal;
		delete llama;
		delete centro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileAlfombra() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 108, 40, 46));
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(255, 180, 138, 72));
		SolidBrush^ interior = gcnew SolidBrush(Color::FromArgb(255, 130, 50, 56));
		g->FillRectangle(borde, 2, 2, 28, 1);
		g->FillRectangle(borde, 2, 29, 28, 1);
		g->FillRectangle(borde, 2, 2, 1, 28);
		g->FillRectangle(borde, 29, 2, 1, 28);
		g->FillRectangle(interior, 6, 6, 20, 20);
		delete borde;
		delete interior;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileEscalera(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ palo = gcnew SolidBrush(Color::FromArgb(255, 136, 94, 52));
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(255, 72, 48, 26));
		g->FillRectangle(sombra, 7, 3, 3, 26);
		g->FillRectangle(sombra, 22, 3, 3, 26);
		g->FillRectangle(palo, 8, 4, 2, 24);
		g->FillRectangle(palo, 22, 4, 2, 24);
		for (int y = 6; y <= 24; y += 5) {
			g->FillRectangle(sombra, 8, y + 1, 16, 2);
			g->FillRectangle(palo, 8, y, 16, 2);
		}
		delete palo;
		delete sombra;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileArbolBorde() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 14, 24, 18));
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(255, 18, 36, 24));
		SolidBrush^ tronco = gcnew SolidBrush(Color::FromArgb(255, 62, 42, 28));
		SolidBrush^ copaA = gcnew SolidBrush(Color::FromArgb(255, 24, 62, 38));
		SolidBrush^ copaB = gcnew SolidBrush(Color::FromArgb(255, 34, 82, 48));
		g->FillEllipse(sombra, 3, 18, 26, 10);
		g->FillRectangle(tronco, 13, 18, 6, 11);
		g->FillEllipse(copaA, 4, 4, 24, 20);
		g->FillEllipse(copaB, 7, 2, 18, 16);
		delete sombra;
		delete tronco;
		delete copaA;
		delete copaB;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileFlor() {
		Bitmap^ bmp = CrearTileBosqueVariante(31415u);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ tallo = gcnew SolidBrush(Color::FromArgb(255, 42, 92, 48));
		SolidBrush^ petalo = gcnew SolidBrush(Color::FromArgb(255, 210, 174, 128));
		SolidBrush^ centro = gcnew SolidBrush(Color::FromArgb(255, 240, 216, 110));
		g->FillRectangle(tallo, 10, 18, 1, 5);
		g->FillEllipse(petalo, 8, 15, 5, 5);
		g->FillRectangle(centro, 10, 17, 1, 1);
		g->FillRectangle(tallo, 22, 10, 1, 5);
		g->FillEllipse(petalo, 20, 7, 5, 5);
		g->FillRectangle(centro, 22, 9, 1, 1);
		delete tallo;
		delete petalo;
		delete centro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileMesa(Bitmap^ madera) {
		Bitmap^ bmp = ClonarTile(madera);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ pata = gcnew SolidBrush(Color::FromArgb(255, 64, 42, 26));
		SolidBrush^ tablero = gcnew SolidBrush(Color::FromArgb(255, 138, 94, 54));
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(255, 92, 60, 34));
		g->FillRectangle(pata, 5, 20, 4, 9);
		g->FillRectangle(pata, 23, 20, 4, 9);
		g->FillRectangle(borde, 3, 8, 26, 15);
		g->FillRectangle(tablero, 4, 9, 24, 12);
		delete pata;
		delete tablero;
		delete borde;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileAguaAnimada(Bitmap^ base, int fase) {
		Bitmap^ bmp = ClonarTile(base);
		Graphics^ g = Graphics::FromImage(bmp);
		SolidBrush^ ondaOscura = gcnew SolidBrush(Color::FromArgb(65, 22, 58, 98));
		SolidBrush^ ondaClara = gcnew SolidBrush(Color::FromArgb(115, 170, 224, 250));
		SolidBrush^ espuma = gcnew SolidBrush(Color::FromArgb(135, 220, 246, 255));
		int despA = (fase * 7) % 24;
		int despB = (fase * 7 + 12) % 24;
		int yA = 5 + ((fase * 3) % 8);
		int yB = 18 + ((fase * 2) % 8);
		g->FillRectangle(ondaOscura, 0, yA + 2, 32, 3);
		g->FillRectangle(ondaOscura, 0, yB + 2, 32, 3);
		g->FillRectangle(ondaClara, despA, yA, 11, 2);
		g->FillRectangle(ondaClara, despB, yB, 11, 2);
		g->FillRectangle(espuma, (despA + 3) % 28, yA, 4, 1);
		g->FillRectangle(espuma, (despB + 4) % 28, yB, 4, 1);
		delete ondaOscura;
		delete ondaClara;
		delete espuma;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileSueloSubterraneo() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 42, 47, 58));
		SolidBrush^ junta = gcnew SolidBrush(Color::FromArgb(255, 30, 34, 43));
		SolidBrush^ losa = gcnew SolidBrush(Color::FromArgb(255, 50, 56, 68));
		g->FillRectangle(losa, 1, 1, 14, 14);
		g->FillRectangle(losa, 17, 1, 14, 14);
		g->FillRectangle(losa, 1, 17, 14, 14);
		g->FillRectangle(losa, 17, 17, 14, 14);
		g->FillRectangle(junta, 0, 15, 32, 2);
		g->FillRectangle(junta, 15, 0, 2, 32);
		delete junta;
		delete losa;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearTileRocaSubterranea() {
		Bitmap^ bmp = CrearLienzo32();
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::FromArgb(255, 20, 22, 28));
		unsigned int sem = 654321u;
		SolidBrush^ rocaA = gcnew SolidBrush(Color::FromArgb(255, 29, 32, 40));
		SolidBrush^ rocaB = gcnew SolidBrush(Color::FromArgb(255, 16, 18, 23));
		SolidBrush^ beta = gcnew SolidBrush(Color::FromArgb(255, 38, 42, 52));
		for (int i = 0; i < 6; i++) {
			int px = (int)(SiguienteAleatorio(sem) % 22);
			int py = (int)(SiguienteAleatorio(sem) % 22);
			g->FillPolygon((i % 2 == 0) ? rocaA : rocaB, gcnew array<Point>{ Point(px, py + 8), Point(px + 6, py), Point(px + 12, py + 6), Point(px + 8, py + 12) });
		}
		g->FillRectangle(beta, 6, 8, 5, 2);
		g->FillRectangle(beta, 19, 21, 6, 2);
		delete rocaA;
		delete rocaB;
		delete beta;
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

		for (int r = 6; r <= 18; r++) {
			for (int c = 10; c <= 28; c++) {
				matriz[r][c] = 'W';
			}
		}
		for (int c = 10; c <= 28; c++) {
			matriz[6][c] = '#';
			matriz[18][c] = '#';
		}
		for (int r = 6; r <= 18; r++) {
			matriz[r][10] = '#';
			matriz[r][28] = '#';
		}
		matriz[18][18] = 'D';
		matriz[18][19] = 'D';
		matriz[18][20] = 'D';
		for (int c = 12; c <= 17; c++) matriz[7][c] = 'B';
		for (int c = 21; c <= 26; c++) matriz[7][c] = 'B';
		matriz[10][12] = 'M';
		matriz[10][13] = 'M';
		matriz[10][25] = 'M';
		matriz[10][26] = 'M';
		for (int r = 11; r <= 14; r++) {
			for (int c = 16; c <= 22; c++) {
				matriz[r][c] = '_';
			}
		}
		matriz[7][11] = 'L';
		matriz[7][27] = 'L';

		for (int r = 19; r <= 51; r++) {
			for (int c = 17; c <= 21; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 52; r <= 66; r++) {
			for (int c = 10; c <= 28; c++) {
				matriz[r][c] = 'W';
			}
		}
		for (int c = 10; c <= 28; c++) {
			matriz[52][c] = '#';
			matriz[66][c] = '#';
		}
		for (int r = 52; r <= 66; r++) {
			matriz[r][10] = '#';
			matriz[r][28] = '#';
		}
		matriz[52][18] = 'D';
		matriz[52][19] = 'D';
		matriz[52][20] = 'D';
		for (int c = 12; c <= 26; c++) matriz[65][c] = 'B';
		for (int r = 56; r <= 62; r++) {
			for (int c = 15; c <= 23; c++) {
				matriz[r][c] = '_';
			}
		}
		matriz[55][13] = 'P';
		matriz[55][25] = 'P';
		matriz[63][13] = 'P';
		matriz[63][25] = 'P';
		matriz[53][11] = 'L';
		matriz[53][27] = 'L';
		matriz[54][12] = 'M';
		matriz[54][13] = 'M';

		for (int r = 35; r <= 38; r++) {
			for (int c = 22; c <= 51; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 20; r <= 35; r++) {
			for (int c = 45; c <= 50; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 3; r <= 76; r++) {
			for (int c = 52; c <= 57; c++) {
				matriz[r][c] = '~';
			}
		}
		for (int r = 35; r <= 38; r++) {
			for (int c = 52; c <= 57; c++) {
				matriz[r][c] = '=';
			}
		}

		for (int r = 35; r <= 38; r++) {
			for (int c = 58; c <= 91; c++) {
				matriz[r][c] = ';';
			}
		}

		for (int r = 14; r <= 34; r++) {
			for (int c = 87; c <= 90; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 14; r <= 17; r++) {
			for (int c = 91; c <= 105; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 10; r <= 21; r++) {
			for (int c = 106; c <= 126; c++) {
				matriz[r][c] = '.';
			}
		}
		for (int c = 106; c <= 126; c++) {
			matriz[10][c] = 'O';
			matriz[21][c] = 'O';
		}
		for (int r = 10; r <= 21; r++) {
			matriz[r][106] = 'O';
			matriz[r][107] = 'O';
			matriz[r][126] = 'O';
		}

		for (int r = 35; r <= 38; r++) {
			for (int c = 92; c <= 111; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 28; r <= 45; r++) {
			for (int c = 112; c <= 134; c++) {
				matriz[r][c] = 'W';
			}
		}
		for (int c = 112; c <= 134; c++) {
			matriz[28][c] = '#';
			matriz[45][c] = '#';
		}
		for (int r = 28; r <= 45; r++) {
			matriz[r][112] = '#';
			matriz[r][134] = '#';
		}
		matriz[36][112] = 'D';
		matriz[37][112] = 'D';
		for (int c = 114; c <= 122; c++) {
			matriz[29][c] = 'B';
			matriz[44][c] = 'B';
		}
		for (int c = 125; c <= 132; c++) {
			matriz[29][c] = 'B';
		}
		for (int r = 33; r <= 40; r++) {
			for (int c = 117; c <= 128; c++) {
				matriz[r][c] = '_';
			}
		}
		matriz[30][113] = 'L';
		matriz[30][133] = 'L';
		matriz[43][133] = 'L';
		matriz[43][129] = 'M';
		matriz[43][130] = 'M';

		for (int r = 39; r <= 62; r++) {
			for (int c = 87; c <= 90; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 59; r <= 62; r++) {
			for (int c = 91; c <= 124; c++) {
				matriz[r][c] = ';';
			}
		}
		for (int r = 55; r <= 66; r++) {
			for (int c = 116; c <= 132; c++) {
				matriz[r][c] = '.';
			}
		}
		for (int c = 116; c <= 133; c++) {
			matriz[55][c] = 'O';
			matriz[66][c] = 'O';
		}
		for (int r = 55; r <= 66; r++) {
			matriz[r][133] = 'O';
		}
		matriz[60][133] = 'X';
		matriz[61][133] = 'X';

		for (int r = 63; r <= 69; r++) {
			for (int c = 100; c <= 108; c++) {
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
		for (int i = 0; i < 85; i++) {
			int r = 4 + (int)(SiguienteAleatorio(semilla) % (filas - 8));
			int c = 4 + (int)(SiguienteAleatorio(semilla) % (columnas - 8));
			if (matriz[r][c] != ',') continue;
			if (r >= 4 && r <= 70 && c >= 8 && c <= 31) continue;
			if (c >= 43 && c <= 60) continue;
			if (r >= 8 && r <= 24 && c >= 85 && c <= 130) continue;
			if (r >= 26 && r <= 47 && c >= 85 && c <= 138) continue;
			if (r >= 52 && r <= 72 && c >= 85 && c <= 136) continue;
			matriz[r][c] = 'O';
		}

		for (int i = 0; i < 110; i++) {
			int r = 4 + (int)(SiguienteAleatorio(semilla) % (filas - 8));
			int c = 4 + (int)(SiguienteAleatorio(semilla) % (columnas - 8));
			if (matriz[r][c] != ',') continue;
			matriz[r][c] = 'F';
		}
	}

	void ConstruirPozo() {
		filas = 20;
		columnas = 28;
		matriz.clear();
		matriz.reserve(filas);
		for (int r = 0; r < filas; r++) {
			matriz.push_back(std::string(columnas, 'K'));
		}
		for (int r = 3; r <= 16; r++) {
			for (int c = 3; c <= 24; c++) {
				matriz[r][c] = 'U';
			}
		}
		for (int c = 3; c <= 24; c++) {
			matriz[3][c] = '#';
			matriz[16][c] = '#';
		}
		for (int r = 3; r <= 16; r++) {
			matriz[r][3] = '#';
			matriz[r][24] = '#';
		}
		for (int r = 7; r <= 11; r++) {
			for (int c = 9; c <= 15; c++) {
				matriz[r][c] = '_';
			}
		}
		matriz[4][4] = 'L';
		matriz[4][23] = 'L';
		matriz[4][19] = 'B';
		matriz[4][20] = 'B';
		matriz[4][21] = 'B';
		matriz[4][22] = 'B';
		matriz[14][21] = 'C';
		matriz[14][22] = 'C';
		matriz[15][22] = 'C';
		matriz[15][5] = 'C';
		matriz[15][6] = 'C';
	}

public:
	MapaNivel1() {
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

		bmpBosque = CrearTileBosqueVariante(481516u);
		bmpBosque2 = CrearTileBosqueVariante(991122u);
		bmpBosque3 = CrearTileBosqueVariante(774411u);
		bmpSendero = CrearTileSenderoVariante(90210u);
		bmpSendero2 = CrearTileSenderoVariante(55123u);
		bmpSendero3 = CrearTileSenderoVariante(18273u);
		bmpPuente = CrearTilePuente();
		bmpEstante = CrearTileEstante(bmpMadera);
		bmpPilar = CrearTilePilar(bmpMadera);
		bmpLampara = CrearTileLampara(bmpMadera);
		bmpAlfombra = CrearTileAlfombra();
		bmpEscalera = CrearTileEscalera(bmpMadera);
		bmpArbolBorde = CrearTileArbolBorde();
		bmpFlor = CrearTileFlor();
		bmpMesa = CrearTileMesa(bmpMadera);
		bmpAgua2 = CrearTileAguaAnimada(bmpAgua, 1);
		bmpAgua3 = CrearTileAguaAnimada(bmpAgua, 2);
		bmpAgua4 = CrearTileAguaAnimada(bmpAgua, 3);
		bmpSueloSubterraneo = CrearTileSueloSubterraneo();
		bmpRocaSubterranea = CrearTileRocaSubterranea();

		ConstruirNivel1();
	}

	~MapaNivel1() {}

	int getFilas() { return filas; }
	int getColumnas() { return columnas; }
	int getTileSize() { return tileSize; }
	int getAnchoTotalPixels() { return columnas * tileSize; }
	int getAltoTotalPixels() { return filas * tileSize; }
	int getNivelActual() { return nivelActual; }

	void cargarNivel(int numNivel) {
		nivelActual = numNivel;
		esPozo = false;
		ConstruirNivel1();
	}

	void cargarPozo() {
		nivelActual = 1;
		esPozo = true;
		ConstruirPozo();
	}

	bool getEsPozo() { return esPozo; }
	int getPuntoInicioPozoX() { return 6 * tileSize; }
	int getPuntoInicioPozoY() { return 7 * tileSize; }
	int getPuntoSalidaPozoX() { return 5 * tileSize; }
	int getPuntoSalidaPozoY() { return 4 * tileSize; }

	Rectangle getParedRocaRect() {
		return Rectangle(106 * tileSize, 14 * tileSize, 2 * tileSize, 4 * tileSize);
	}

	void romperParedRoca() {
		for (int r = 14; r <= 17; r++) {
			matriz[r][106] = '.';
			matriz[r][107] = '.';
		}
	}

	bool estaCercaParedRoca(int px, int py, int ancho, int alto) {
		Rectangle pared = getParedRocaRect();
		Rectangle jugador(px, py, ancho, alto);
		Rectangle zona(pared.X - 32, pared.Y - 16, pared.Width + 64, pared.Height + 32);
		return zona.IntersectsWith(jugador);
	}

	int getPuntoInicioX() { return 19 * tileSize; }
	int getPuntoInicioY() { return 12 * tileSize; }
	int getPuntoQifreyX() { return 19 * tileSize; }
	int getPuntoQifreyY() { return 59 * tileSize; }
	int getPuntoAgottX() { return 130 * tileSize; }
	int getPuntoAgottY() { return 36 * tileSize; }
	int getPuntoMyrphonX() { return 120 * tileSize; }
	int getPuntoMyrphonY() { return 60 * tileSize; }
	int getPuntoGatoX() { return 104 * tileSize; }
	int getPuntoGatoY() { return 68 * tileSize; }
	int getPuntoTrollX() { return 3 * tileSize; }
	int getPuntoTrollY() { return 36 * tileSize; }

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

	static bool esSolido(char ch) {
		return ch == '#' || ch == 'O' || ch == 'B' || ch == 'C' || ch == 'P' || ch == 'T' || ch == 'M' || ch == 'X' || ch == '~' || ch == 'K';
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

	int determinarCuarto(int pixelX, int pixelY) {
		if (esPozo) return 0;
		int col = pixelX / tileSize;
		int fila = pixelY / tileSize;
		if (col >= 11 && col <= 27 && fila >= 7 && fila <= 17) return 1;
		if (col >= 11 && col <= 27 && fila >= 53 && fila <= 65) return 2;
		if (col >= 113 && col <= 133 && fila >= 29 && fila <= 44) return 3;
		if (col >= 108 && col <= 125 && fila >= 11 && fila <= 20) return 4;
		return 0;
	}

	bool esExteriorParaLluvia(int col, int fila) {
		if (esPozo) return false;
		if (col >= 10 && col <= 28 && fila >= 6 && fila <= 18) return false;
		if (col >= 10 && col <= 28 && fila >= 52 && fila <= 66) return false;
		if (col >= 112 && col <= 134 && fila >= 28 && fila <= 45) return false;
		if (col >= 106 && col <= 126 && fila >= 10 && fila <= 21) return false;
		return true;
	}

	String^ getNombreUbicacion(int pixelX, int pixelY) {
		if (esPozo) return "Sótano de Richeh";
		int cuarto = determinarCuarto(pixelX, pixelY);
		if (cuarto == 1) return "Choza de Hechizos";
		if (cuarto == 2) return "Despacho de Qifrey";
		if (cuarto == 3) return "Torre de Agott";
		if (cuarto == 4) return "Cueva Misteriosa";

		int col = pixelX / tileSize;
		int fila = pixelY / tileSize;
		if (col >= 84 && col <= 94 && fila >= 33 && fila <= 40) return "Cruce de Tres Caminos";
		if (col >= 50 && col <= 59) return "Puente del Gran Río";
		if (col >= 114 && col <= 134 && fila >= 54 && fila <= 67) return "Laberinto Serpentback";
		if (col >= 98 && col <= 112 && fila >= 62 && fila <= 71) return "Rincón del Gato";
		return "Sendero del Atelier";
	}

	Bitmap^ obtenerBitmapPorCaracter(char ch, int r, int c, int tick) {
		switch (ch) {
		case '#': return bmpMuro;
		case 'W': return bmpMadera;
		case '.': return bmpCamino;
		case 'U': return bmpSueloSubterraneo;
		case 'K': return bmpRocaSubterranea;
		case ',': {
			int v = (r * 17 + c * 31) % 3;
			if (v == 1) return bmpBosque2;
			if (v == 2) return bmpBosque3;
			return bmpBosque;
		}
		case ';': {
			int v = (r * 13 + c * 7) % 3;
			if (v == 1) return bmpSendero2;
			if (v == 2) return bmpSendero3;
			return bmpSendero;
		}
		case '~': {
			int fase = ((tick / 7) + r + c * 2) % 4;
			if (fase == 1) return bmpAgua2;
			if (fase == 2) return bmpAgua3;
			if (fase == 3) return bmpAgua4;
			return bmpAgua;
		}
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
				Bitmap^ bmpTile = obtenerBitmapPorCaracter(ch, r, c, tick);
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
