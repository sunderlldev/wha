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
	int ancho;
	int alto;
	ContenidoCaja contenido;
	bool movida;
	gcroot<Bitmap^> icono;

	static Bitmap^ CargarIcono() {
		String^ nombre = "sprites/tile_caja.png";
		if (System::IO::File::Exists(nombre)) return gcnew Bitmap(nombre);
		String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
		String^ ruta1 = System::IO::Path::Combine(baseDir, nombre);
		if (System::IO::File::Exists(ruta1)) return gcnew Bitmap(ruta1);
		String^ ruta2 = System::IO::Path::Combine(baseDir, "..\\..\\wha-v2\\" + nombre);
		if (System::IO::File::Exists(ruta2)) return gcnew Bitmap(ruta2);
		return gcnew Bitmap(32, 32);
	}

public:
	CajaMovible(int tx, int ty) {
		tileX = tx;
		tileY = ty;
		ancho = Configuracion::TILE_SIZE;
		alto = Configuracion::TILE_SIZE;
		contenido = CajaVacia;
		movida = false;
		icono = CargarIcono();
	}

	~CajaMovible() {}

	int getTileX() { return tileX; }
	int getTileY() { return tileY; }
	int getAncho() { return ancho; }
	int getAlto() { return alto; }
	ContenidoCaja getContenido() { return contenido; }
	bool getMovida() { return movida; }
	void setContenido(ContenidoCaja c) { contenido = c; }
	void setMovida(bool m) { movida = m; }

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
		int tam = Configuracion::TILE_SIZE * zoom;
		if (posX + tam < 0 || posX > g->VisibleClipBounds.Width || posY + tam < 0 || posY > g->VisibleClipBounds.Height) return;
		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmp, Rectangle(posX, posY, tam, tam), 0, 0, 32, 32, GraphicsUnit::Pixel);
	}
};
