#pragma once

#include <gcroot.h>
#include "Configuracion.h"

using namespace System;
using namespace System::Drawing;

enum ContenidoCaja {
	CajaVacia,
	CajaGrimorio,
	CajaPozo
};

class CajaMovible {
private:
	int tileX;
	int tileY;
	int origTileX;
	int origTileY;
	int ancho;
	int alto;
	ContenidoCaja contenido;
	bool movida;
	gcroot<Bitmap^> icono;

	static Bitmap^ CargarIcono() {
		String^ nombre = "sprites/tile_caja.png";
		if (System::IO::File::Exists(nombre)) return gcnew Bitmap(nombre);
		String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
		array<String^>^ prefijos = gcnew array<String^>{
			"",
			"..\\..\\wha-v2\\",
			"..\\..\\..\\wha-v2\\wha-v2\\",
			"..\\..\\..\\..\\wha-v2\\wha-v2\\"
		};
		for each (String^ pref in prefijos) {
			String^ ruta = System::IO::Path::Combine(baseDir, pref + nombre);
			if (System::IO::File::Exists(ruta)) return gcnew Bitmap(ruta);
		}
		return gcnew Bitmap(32, 32);
	}

public:
	CajaMovible(int tx, int ty) {
		tileX = tx;
		tileY = ty;
		origTileX = tx;
		origTileY = ty;
		ancho = Configuracion::TILE_SIZE * 2;
		alto = Configuracion::TILE_SIZE * 2;
		contenido = CajaVacia;
		movida = false;
		icono = CargarIcono();
	}

	~CajaMovible() {}

	int getTileX() { return tileX; }
	int getTileY() { return tileY; }
	int getOrigTileX() { return origTileX; }
	int getOrigTileY() { return origTileY; }
	int getAncho() { return ancho; }
	int getAlto() { return alto; }
	ContenidoCaja getContenido() { return contenido; }
	bool getMovida() { return movida; }
	void setContenido(ContenidoCaja c) { contenido = c; }
	void setMovida(bool m) { movida = m; }
	void setTilePos(int tx, int ty) { tileX = tx; tileY = ty; }

	void restaurarOrigen() {
		tileX = origTileX;
		tileY = origTileY;
	}

	Rectangle getRectangle() {
		return Rectangle(tileX * Configuracion::TILE_SIZE, tileY * Configuracion::TILE_SIZE, ancho, alto);
	}

	void mover(int dx, int dy) {
		tileX += dx;
		tileY += dy;
	}

	void dibujar(Graphics^ g, int camX, int camY, int zoom) {
		Bitmap^ bmp = icono;
		if (g == nullptr || bmp == nullptr) return;
		int posX = (tileX * Configuracion::TILE_SIZE - camX) * zoom;
		int posY = (tileY * Configuracion::TILE_SIZE - camY) * zoom;
		int tamW = ancho * zoom;
		int tamH = alto * zoom;
		if (posX + tamW < 0 || posX > g->VisibleClipBounds.Width || posY + tamH < 0 || posY > g->VisibleClipBounds.Height) return;
		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmp, Rectangle(posX, posY, tamW, tamH), 0, 0, bmp->Width, bmp->Height, GraphicsUnit::Pixel);
	}
};
