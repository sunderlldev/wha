#pragma once

#include <string>
#include <gcroot.h>
#include "Entidad.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

enum TipoObjetoMapa {
	ObjetoVial = 1,
	ObjetoPluma = 2,
	ObjetoGato = 3,
	ObjetoTroll = 4,
	ObjetoLetrero = 5,
	ObjetoMyrphon = 6,
	ObjetoHoja = 7,
	ObjetoPergamino = 8,
	ObjetoPozo = 9,
	ObjetoMontonCajas = 10
};

class BloqueMapa : public Entidad {
private:
	int tipo;
	bool recolectado;
	std::string nombre;
	std::string descripcion;
	gcroot<Bitmap^> icono;

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
		return gcnew Bitmap(32, 32);
	}

	static Bitmap^ CrearIconoVial(Color liquido) {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, liquido));
		g->FillEllipse(brillo, 2, 3, 20, 20);
		SolidBrush^ vidrio = gcnew SolidBrush(Color::FromArgb(235, 210, 238, 250));
		SolidBrush^ corcho = gcnew SolidBrush(Color::FromArgb(235, 118, 82, 48));
		SolidBrush^ contenido = gcnew SolidBrush(liquido);
		g->FillRectangle(vidrio, 8, 7, 8, 13);
		g->FillRectangle(contenido, 9, 11, 6, 8);
		g->FillRectangle(corcho, 9, 4, 6, 4);
		g->FillRectangle(gcnew SolidBrush(Color::FromArgb(210, 250, 250, 255)), 10, 8, 2, 4);
		delete brillo;
		delete vidrio;
		delete corcho;
		delete contenido;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPluma() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, 255, 214, 110));
		g->FillEllipse(brillo, 2, 2, 20, 20);
		SolidBrush^ oro = gcnew SolidBrush(Color::FromArgb(245, 226, 174, 72));
		SolidBrush^ claro = gcnew SolidBrush(Color::FromArgb(245, 252, 232, 160));
		g->FillPolygon(oro, gcnew array<Point>{ Point(6, 19), Point(12, 4), Point(18, 19), Point(12, 15) });
		g->FillPolygon(claro, gcnew array<Point>{ Point(11, 16), Point(12, 6), Point(15, 16), Point(12, 14) });
		g->DrawLine(gcnew Pen(Color::FromArgb(255, 130, 90, 40), 1.0f), 6, 20, 18, 20);
		delete brillo;
		delete oro;
		delete claro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoHoja() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(60, 120, 220, 130));
		g->FillEllipse(brillo, 2, 2, 20, 20);
		SolidBrush^ hoja = gcnew SolidBrush(Color::FromArgb(235, 74, 150, 84));
		SolidBrush^ nervio = gcnew SolidBrush(Color::FromArgb(235, 40, 94, 54));
		g->FillPolygon(hoja, gcnew array<Point>{ Point(5, 18), Point(10, 4), Point(20, 8), Point(13, 20) });
		g->DrawLine(gcnew Pen(nervio, 1.0f), 7, 17, 17, 9);
		delete brillo;
		delete hoja;
		delete nervio;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPergamino() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(60, 230, 210, 170));
		g->FillEllipse(brillo, 2, 2, 20, 20);
		SolidBrush^ papel = gcnew SolidBrush(Color::FromArgb(240, 226, 208, 164));
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(240, 148, 116, 70));
		g->FillRectangle(papel, 5, 4, 14, 16);
		g->FillRectangle(borde, 4, 3, 16, 3);
		g->FillRectangle(borde, 4, 18, 16, 3);
		Pen^ linea = gcnew Pen(Color::FromArgb(180, 110, 84, 50), 1.0f);
		for (int i = 0; i < 4; i++) {
			g->DrawLine(linea, 8, 8 + i * 3, 16, 8 + i * 3);
		}
		delete brillo;
		delete papel;
		delete borde;
		delete linea;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoGato() {
		Bitmap^ gato = CargarBitmapSeguro("sprites/tile_gato.png");
		Bitmap^ madera = CargarBitmapSeguro("sprites/tile_madera.png");
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->DrawImage(gato, 0, 0, 32, 32);
		delete g;
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 32; x++) {
				Color a = bmp->GetPixel(x, y);
				Color b = madera->GetPixel(x % madera->Width, y % madera->Height);
				int dr = a.R - b.R;
				int dg = a.G - b.G;
				int db = a.B - b.B;
				if (dr * dr + dg * dg + db * db < 400) {
					bmp->SetPixel(x, y, Color::FromArgb(0, 0, 0, 0));
				}
			}
		}
		return bmp;
	}

	static Bitmap^ CrearIconoTroll() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ portal = gcnew SolidBrush(Color::FromArgb(190, 50, 24, 70));
		SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(70, 150, 80, 200));
		SolidBrush^ cara = gcnew SolidBrush(Color::FromArgb(235, 238, 236, 228));
		SolidBrush^ negro = gcnew SolidBrush(Color::FromArgb(255, 24, 20, 28));
		g->FillEllipse(aura, 0, 0, 32, 32);
		g->FillEllipse(portal, 3, 3, 26, 26);
		g->FillEllipse(cara, 8, 8, 16, 14);
		g->FillRectangle(negro, 11, 12, 3, 2);
		g->FillRectangle(negro, 18, 12, 3, 2);
		g->FillRectangle(negro, 10, 17, 12, 2);
		g->FillRectangle(cara, 12, 19, 8, 2);
		delete portal;
		delete aura;
		delete cara;
		delete negro;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPozo() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(180, 30, 22, 18));
		SolidBrush^ piedra = gcnew SolidBrush(Color::FromArgb(235, 118, 116, 108));
		SolidBrush^ piedraOscura = gcnew SolidBrush(Color::FromArgb(235, 78, 78, 74));
		SolidBrush^ hueco = gcnew SolidBrush(Color::FromArgb(255, 10, 12, 16));
		SolidBrush^ madera = gcnew SolidBrush(Color::FromArgb(235, 96, 62, 34));
		SolidBrush^ maderaOscura = gcnew SolidBrush(Color::FromArgb(235, 58, 38, 24));
		SolidBrush^ cuerda = gcnew SolidBrush(Color::FromArgb(235, 190, 168, 110));
		g->FillEllipse(sombra, 3, 7, 26, 23);
		g->FillEllipse(piedra, 4, 6, 24, 22);
		g->FillEllipse(piedraOscura, 7, 9, 18, 16);
		g->FillEllipse(hueco, 10, 11, 12, 11);
		g->FillRectangle(maderaOscura, 5, 3, 4, 24);
		g->FillRectangle(maderaOscura, 23, 3, 4, 24);
		g->FillRectangle(madera, 5, 3, 3, 23);
		g->FillRectangle(madera, 23, 3, 3, 23);
		g->FillRectangle(maderaOscura, 4, 2, 25, 4);
		g->FillRectangle(cuerda, 15, 6, 2, 12);
		g->FillEllipse(madera, 12, 16, 8, 7);
		delete sombra;
		delete piedra;
		delete piedraOscura;
		delete hueco;
		delete madera;
		delete maderaOscura;
		delete cuerda;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoMontonCajas() {
		Bitmap^ caja = CargarBitmapSeguro("sprites/tile_caja.png");
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->DrawImage(caja, 0, 0, 32, 32);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(50, 240, 210, 140));
		g->FillEllipse(brillo, 8, 4, 16, 12);
		delete brillo;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoLetrero() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ madera = gcnew SolidBrush(Color::FromArgb(235, 102, 70, 40));
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(235, 58, 38, 22));
		SolidBrush^ texto = gcnew SolidBrush(Color::FromArgb(235, 236, 208, 140));
		g->FillRectangle(borde, 8, 6, 16, 14);
		g->FillRectangle(madera, 9, 7, 14, 12);
		g->FillRectangle(borde, 14, 19, 4, 10);
		g->FillRectangle(texto, 13, 10, 2, 6);
		g->FillRectangle(texto, 17, 10, 2, 2);
		g->FillRectangle(texto, 17, 14, 2, 2);
		delete madera;
		delete borde;
		delete texto;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoMyrphon() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(60, 240, 140, 210));
		SolidBrush^ cuerpo = gcnew SolidBrush(Color::FromArgb(240, 226, 150, 210));
		SolidBrush^ panza = gcnew SolidBrush(Color::FromArgb(240, 250, 220, 240));
		SolidBrush^ ojos = gcnew SolidBrush(Color::FromArgb(255, 40, 30, 60));
		SolidBrush^ pico = gcnew SolidBrush(Color::FromArgb(255, 242, 196, 80));
		g->FillEllipse(aura, 1, 1, 30, 30);
		g->FillEllipse(cuerpo, 6, 9, 20, 18);
		g->FillEllipse(panza, 10, 15, 12, 10);
		g->FillPolygon(cuerpo, gcnew array<Point>{ Point(8, 10), Point(10, 1), Point(14, 9) });
		g->FillPolygon(cuerpo, gcnew array<Point>{ Point(24, 10), Point(22, 1), Point(18, 9) });
		g->FillEllipse(ojos, 11, 13, 3, 4);
		g->FillEllipse(ojos, 19, 13, 3, 4);
		g->FillPolygon(pico, gcnew array<Point>{ Point(15, 17), Point(18, 19), Point(15, 20) });
		g->FillEllipse(pico, 3, 20, 6, 4);
		delete aura;
		delete cuerpo;
		delete panza;
		delete ojos;
		delete pico;
		delete g;
		return bmp;
	}

	void generarIcono() {
		if (tipo == ObjetoVial) {
			icono = CrearIconoVial(Color::FromArgb(240, 70, 150, 240));
			ancho = 24;
			alto = 24;
		}
		else if (tipo == ObjetoPluma) {
			icono = CrearIconoPluma();
			ancho = 24;
			alto = 24;
		}
		else if (tipo == ObjetoHoja) {
			icono = CrearIconoHoja();
			ancho = 24;
			alto = 24;
		}
		else if (tipo == ObjetoPergamino) {
			icono = CrearIconoPergamino();
			ancho = 24;
			alto = 24;
		}
		else if (tipo == ObjetoGato) {
			icono = CrearIconoGato();
			ancho = 32;
			alto = 32;
		}
		else if (tipo == ObjetoTroll) {
			icono = CrearIconoTroll();
			ancho = 32;
			alto = 32;
		}
		else if (tipo == ObjetoMyrphon) {
			icono = CrearIconoMyrphon();
			ancho = 32;
			alto = 32;
		}
		else if (tipo == ObjetoPozo) {
			icono = CrearIconoPozo();
			ancho = 32;
			alto = 32;
		}
		else if (tipo == ObjetoMontonCajas) {
			icono = CrearIconoMontonCajas();
			ancho = 32;
			alto = 32;
		}
		else {
			icono = CrearIconoLetrero();
			ancho = 32;
			alto = 32;
		}
	}

public:
	BloqueMapa(int px, int py, int t, std::string nom, std::string desc)
		: Entidad(px, py, 32, 32) {
		tipo = t;
		recolectado = false;
		nombre = nom;
		descripcion = desc;
		dx = 0;
		dy = 0;
		generarIcono();
	}

	virtual ~BloqueMapa() {}

	int getTipo() { return tipo; }
	bool getRecolectado() { return recolectado; }
	std::string getNombre() { return nombre; }
	std::string getDescripcion() { return descripcion; }
	Bitmap^ getIcono() { return icono; }

	bool esRecolectable() {
		return tipo == ObjetoVial || tipo == ObjetoHoja || tipo == ObjetoPergamino;
	}

	void recolectar() {
		recolectado = true;
		visible = false;
	}

	Rectangle getInteractionRect() {
		return Rectangle(x - 18, y - 18, ancho + 36, alto + 36);
	}

	virtual void dibujar(Graphics^ g, Bitmap^ bmp, int camX, int camY, int zoom, int tick) override {
		if (!visible || recolectado || g == nullptr) return;
		Bitmap^ bmpIcono = icono;
		if (bmpIcono == nullptr) return;

		int flotacion = 0;
		if (esRecolectable()) {
			flotacion = (int)(Math::Sin((tick + x) * 0.14) * 3.0);
		}
		int posX = (x - camX) * zoom;
		int posY = (y - camY + flotacion) * zoom;
		int anchoDestino = ancho * zoom;
		int altoDestino = alto * zoom;
		if (posX + anchoDestino < 0 || posX > g->VisibleClipBounds.Width ||
			posY + altoDestino < 0 || posY > g->VisibleClipBounds.Height) {
			return;
		}

		if (esRecolectable()) {
			SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(45, 255, 230, 150));
			g->FillEllipse(brillo, posX - 6 * zoom, posY - 6 * zoom, (ancho + 12) * zoom, (alto + 12) * zoom);
			delete brillo;
		}

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmpIcono, Rectangle(posX, posY, anchoDestino, altoDestino), 0, 0, bmpIcono->Width, bmpIcono->Height, GraphicsUnit::Pixel);
	}
};
