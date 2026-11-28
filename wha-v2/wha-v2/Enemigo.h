#pragma once

#include "Entidad.h"
#include <string>

using namespace System::Drawing;

class Enemigo : public Entidad {
private:
	std::string nombre;
	int danio;
	int color;
	bool activo;
	int idCuartoOrigen;
	int vidas;
	int ticksAnimacion;
	int frameActual;
	int direccionPatrulla;
	int minCoord;
	int maxCoord;
	int velocidad;

public:
	Enemigo(int px, int py, std::string nom = "Sanguijuela", int vida = 2, int d = 1, int col = 4, int cuarto = 0)
		: Entidad(px, py, 16, 16) {
		nombre = nom;
		vidas = vida;
		danio = d;
		color = col;
		activo = true;
		idCuartoOrigen = cuarto;
		ticksAnimacion = 0;
		frameActual = 0;
		direccionPatrulla = 0;
		minCoord = px;
		maxCoord = px;
		velocidad = 2;
		escalaVisual = 1.6f;
	}

	Enemigo(int px, int py, int dirPatrulla, int minC, int maxC, std::string nom = "Sombrero de Ala Ancha", int vel = 2)
		: Entidad(px, py, 32, 32) {
		nombre = nom;
		vidas = 3;
		danio = 1;
		color = 4;
		activo = true;
		idCuartoOrigen = 0;
		ticksAnimacion = 0;
		frameActual = 0;
		direccionPatrulla = dirPatrulla;
		minCoord = minC;
		maxCoord = maxC;
		velocidad = vel;
		dx = vel;
		dy = vel;
		direccion = (dirPatrulla == 0) ? 3 : 0;
		escalaVisual = 1.6f;
	}

	virtual ~Enemigo() {}

	std::string getNombre() const { return nombre; }
	int getDanio() const { return danio; }
	int getColor() const { return color; }
	bool getActivo() const { return activo && visible; }
	int getIdCuartoOrigen() const { return idCuartoOrigen; }
	int getVidas() const { return vidas; }

	void setDanio(int d) { danio = d; }
	void setColor(int col) { color = col; }
	void setActivo(bool act) { activo = act; visible = act; }
	void setIdCuartoOrigen(int c) { idCuartoOrigen = c; }
	void setVidas(int v) { vidas = v; }

	int getIdX() const { return idX; }
	int getIdY() const { return idY; }
	int getVelocidad() const { return velocidad; }
	void setIdY(int y) { idY = y; }
	void setVelocidad(int v) {
		velocidad = v;
		dx = v;
		dy = v;
	}

	void recibirDanio() {
		if (vidas > 0) vidas--;
		if (vidas <= 0) {
			activo = false;
			visible = false;
		}
	}

	void animar() {
		ticksAnimacion++;
		if (nombre == "Sanguijuela") {
			if (ticksAnimacion % 10 == 0) {
				idX = (idX + 1) % 8;
			}
			return;
		}
		if (ticksAnimacion % 6 == 0) {
			frameActual = (frameActual + 1) % 6;
			idX = frameActual;
		}
		idY = direccion;
	}

	void moverPatrulla() {
		if (!getActivo()) return;
		if (direccionPatrulla == 0) {
			x += dx;
			if (x >= maxCoord) {
				x = maxCoord;
				dx = -velocidad;
				direccion = 1;
			}
			else if (x <= minCoord) {
				x = minCoord;
				dx = velocidad;
				direccion = 3;
			}
			else {
				direccion = (dx >= 0) ? 3 : 1;
			}
		}
		else {
			y += dy;
			if (y >= maxCoord) {
				y = maxCoord;
				dy = -velocidad;
				direccion = 2;
			}
			else if (y <= minCoord) {
				y = minCoord;
				dy = velocidad;
				direccion = 0;
			}
			else {
				direccion = (dy >= 0) ? 0 : 2;
			}
		}
		animar();
	}

	void dibujarEnemigo(Graphics^ g, Bitmap^ bmp, int camX, int camY, int zoom) {
		if (!getActivo() || g == nullptr) return;
		if (bmp != nullptr && bmp->Width >= 192 && bmp->Height >= 128) {
			dibujar(g, bmp, camX, camY, zoom, ticksAnimacion);
			return;
		}
		if (bmp != nullptr && bmp->Width >= 64 && bmp->Height >= 96) {
			int cellW = bmp->Width / 4;
			int cellH = bmp->Height / 4;
			int col = (ticksAnimacion / 6) % 4;
			int fila = 0;
			if (direccion == 2) fila = 1;
			else if (direccion == 1) fila = 2;
			else if (direccion == 3) fila = 3;

			int anchoDestino = (int)(24 * zoom * escalaVisual);
			int altoDestino = (int)(36 * zoom * escalaVisual);
			int posX = (x - camX) * zoom + (ancho * zoom - anchoDestino) / 2;
			int posY = (y - camY) * zoom + (alto * zoom - altoDestino);

			if (posX + anchoDestino >= 0 && posX <= g->VisibleClipBounds.Width &&
				posY + altoDestino >= 0 && posY <= g->VisibleClipBounds.Height) {
				g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
				g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
				g->DrawImage(bmp, Rectangle(posX, posY, anchoDestino, altoDestino),
					Rectangle(col * cellW, fila * cellH, cellW, cellH), GraphicsUnit::Pixel);
			}
			return;
		}
		int px = (x - camX) * zoom;
		int py = (y - camY) * zoom;
		int w = (int)(ancho * zoom * 1.4f);
		int h = (int)(alto * zoom * 1.4f);
		SolidBrush^ capa = gcnew SolidBrush(Color::FromArgb(230, 38, 26, 54));
		SolidBrush^ sombrero = gcnew SolidBrush(Color::FromArgb(245, 22, 16, 32));
		SolidBrush^ ojo = gcnew SolidBrush(Color::FromArgb(255, 236, 70, 80));
		g->FillEllipse(capa, px + 4, py + h / 3, w - 8, (h * 2) / 3);
		g->FillEllipse(sombrero, px, py + h / 4, w, h / 5);
		g->FillPolygon(sombrero, gcnew array<Point>{ Point(px + w / 2, py), Point(px + w / 4, py + h / 3), Point(px + (w * 3) / 4, py + h / 3) });
		g->FillEllipse(ojo, px + w / 3, py + h / 2, 4, 4);
		g->FillEllipse(ojo, px + (w * 2) / 3 - 4, py + h / 2, 4, 4);
		delete capa;
		delete sombrero;
		delete ojo;
	}
};

struct ProyectilMago {
	int x;
	int y;
	int dx;
	int dy;
	int fila;
	bool activo;
	bool impactando;
	int colImpacto;
	int tickImpacto;
	int distanciaRecorrida;

	ProyectilMago() {
		x = 0;
		y = 0;
		dx = 0;
		dy = 0;
		fila = 1;
		activo = false;
		impactando = false;
		colImpacto = 0;
		tickImpacto = 0;
		distanciaRecorrida = 0;
	}

	static ProyectilMago CrearPorDireccion(int px, int py, int dirEntidad, int velocidad) {
		ProyectilMago p;
		p.x = px;
		p.y = py;
		p.activo = true;
		p.impactando = false;
		p.colImpacto = 0;
		p.tickImpacto = 0;
		p.distanciaRecorrida = 0;
		if (dirEntidad == 2) {
			p.dx = 0;
			p.dy = -velocidad;
			p.fila = 0;
		}
		else if (dirEntidad == 0) {
			p.dx = 0;
			p.dy = velocidad;
			p.fila = 1;
		}
		else if (dirEntidad == 1) {
			p.dx = -velocidad;
			p.dy = 0;
			p.fila = 2;
		}
		else {
			p.dx = velocidad;
			p.dy = 0;
			p.fila = 3;
		}
		return p;
	}

	void iniciarImpacto() {
		if (impactando) return;
		impactando = true;
		dx = 0;
		dy = 0;
		colImpacto = 1;
		tickImpacto = 0;
	}

	void avanzarImpacto() {
		tickImpacto++;
		if (tickImpacto % 3 == 0) {
			colImpacto++;
			if (colImpacto > 3) activo = false;
		}
	}

	Rectangle getCollisionRect() const {
		return Rectangle(x + 5, y + 5, 14, 14);
	}

	void dibujar(Graphics^ g, Bitmap^ bmpDisparo, int camX, int camY, int zoom, int tamano) {
		if (!activo || g == nullptr || bmpDisparo == nullptr) return;
		if (bmpDisparo->Width < 4 || bmpDisparo->Height < 4) return;
		int cellW = bmpDisparo->Width / 4;
		int cellH = bmpDisparo->Height / 4;
		int col = impactando ? colImpacto : 0;
		if (col < 0) col = 0;
		if (col > 3) col = 3;
		int f = (fila >= 0 && fila < 4) ? fila : 1;
		int posX = (x - camX) * zoom;
		int posY = (y - camY) * zoom;
		int destSize = tamano * zoom;
		if (posX + destSize < 0 || posX > g->VisibleClipBounds.Width ||
			posY + destSize < 0 || posY > g->VisibleClipBounds.Height) {
			return;
		}
		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmpDisparo, Rectangle(posX, posY, destSize, destSize),
			Rectangle(col * cellW, f * cellH, cellW, cellH), GraphicsUnit::Pixel);
	}
};
