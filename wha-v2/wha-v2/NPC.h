#pragma once

#include "Entidad.h"
#include "Configuracion.h"
#include <string>
#include <cmath>

class NPC : public Entidad {
private:
	int idPersonaje;
	std::string nombre;
	std::string rol;
	int radioInteraccion;
	int vecesHablado;

public:
	NPC(int px, int py, int id, std::string nom, std::string r)
		: Entidad(px, py, 24, 32) {
		idPersonaje = id;
		nombre = nom;
		rol = r;
		radioInteraccion = 52;
		vecesHablado = 0;
		idX = 0;
		idY = 0;
		escalaVisual = Configuracion::ESCALA_PERSONAJE;
	}

	virtual ~NPC() {}

	int getIdPersonaje() { return idPersonaje; }
	std::string getNombre() { return nombre; }
	std::string getRol() { return rol; }
	int getVecesHablado() { return vecesHablado; }
	void incrementarVecesHablado() { vecesHablado++; }
	void setMirada(int dir) { direccion = dir; idY = dir; }

	bool estaCerca(int targetX, int targetY) {
		int centroX = x + ancho / 2;
		int centroY = y + alto / 2;
		int dx = centroX - targetX;
		int dy = centroY - targetY;
		return (dx * dx + dy * dy) <= (radioInteraccion * radioInteraccion);
	}

	Rectangle getInteractionRect() {
		return Rectangle(x - 30, y - 30, ancho + 60, alto + 60);
	}

	void dibujarConGrid(Graphics^ g, Bitmap^ bmp, int camX, int camY, int zoom, int tick, int cols, int rows, bool animar) {
		if (!visible || bmp == nullptr || g == nullptr || cols <= 0 || rows <= 0) return;
		int cellW = bmp->Width / cols;
		int cellH = bmp->Height / rows;
		if (cellW <= 0 || cellH <= 0) return;

		int anchoBase = ancho * zoom;
		int altoBase = alto * zoom;
		int anchoDestino = (int)(anchoBase * escalaVisual);
		int altoDestino = (int)(altoBase * escalaVisual);
		int posX = (x - camX) * zoom - (anchoDestino - anchoBase) / 2;
		int posY = (y - camY) * zoom - (altoDestino - altoBase);

		if (posX + anchoDestino < 0 || posX > g->VisibleClipBounds.Width ||
			posY + altoDestino < 0 || posY > g->VisibleClipBounds.Height) {
			return;
		}

		int col = animar ? ((tick / 14) % cols) : (idX % cols);
		if (col < 0) col = 0;
		int fila = idY % rows;
		if (fila < 0) fila = 0;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmp, Rectangle(posX, posY, anchoDestino, altoDestino), Rectangle(col * cellW, fila * cellH, cellW, cellH), GraphicsUnit::Pixel);
	}
};
