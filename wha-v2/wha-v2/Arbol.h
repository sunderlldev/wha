#pragma once

#include <vector>
#include <string>
#include <gcroot.h>
#include "Entidad.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

enum TipoArbol {
	ArbolPino,
	ArbolFrondoso,
	ArbolGigante
};

class Arbol : public Entidad {
private:
	TipoArbol tipo;
	gcroot<Bitmap^> imagen;

	static const std::vector<std::string>& obtenerArte(TipoArbol tipoArbol) {
		static const std::vector<std::string> pino = {
			"   /\\  ",
			"  //\\\\ ",
			" ///\\\\\\",
			"   ][  "
		};
		static const std::vector<std::string> frondoso = {
			"    #o#      ",
			"  ####o#     ",
			" #o# \\#|_#,# ",
			"###\\ |/   #o#",
			" # {}{      #",
			"    }{{      ",
			"   ,'  `     "
		};
		static const std::vector<std::string> gigante = {
			"        &&                ",
			"      &&&&&               ",
			"    &&&\\/& &&&            ",
			"   &&|,/  |/& &&          ",
			"    &&/   /  /_&  &&      ",
			"      \\  {  |_____/_&     ",
			"      {  / /          &&& ",
			"      `, \\{___________/_&&",
			"       } }{       \\       ",
			"       }{{         \\____& ",
			"      {}{           `&\\&& ",
			"      {{}             &&  ",
			", -=-~{ .-^- _            ",
			"      `}                  ",
			"       {                  "
		};
		if (tipoArbol == ArbolFrondoso) return frondoso;
		if (tipoArbol == ArbolGigante) return gigante;
		return pino;
	}

	static bool esTronco(char ch) {
		return ch == '[' || ch == ']' || ch == '|' || ch == '{' || ch == '}' ||
			ch == ',' || ch == '\'' || ch == '`' || ch == '_';
	}

	void generarImagen() {
		const std::vector<std::string>& arte = obtenerArte(tipo);
		int anchoLetra = 6;
		int altoLetra = 8;

		int maxColumnas = 0;
		for (size_t r = 0; r < arte.size(); r++) {
			if ((int)arte[r].size() > maxColumnas) maxColumnas = (int)arte[r].size();
		}

		ancho = maxColumnas * anchoLetra;
		alto = (int)arte.size() * altoLetra;
		Bitmap^ bmp = gcnew Bitmap(ancho, alto, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;

		Color colorCopa = Color::FromArgb(255, 36, 88, 54);
		Color colorCopaOscura = Color::FromArgb(255, 20, 54, 34);
		Color colorCopaClara = Color::FromArgb(255, 54, 112, 68);
		if (tipo == ArbolFrondoso) {
			colorCopa = Color::FromArgb(255, 30, 78, 46);
			colorCopaOscura = Color::FromArgb(255, 17, 47, 29);
			colorCopaClara = Color::FromArgb(255, 48, 102, 60);
		}
		else if (tipo == ArbolGigante) {
			colorCopa = Color::FromArgb(255, 24, 64, 40);
			colorCopaOscura = Color::FromArgb(255, 13, 39, 24);
			colorCopaClara = Color::FromArgb(255, 38, 84, 50);
		}
		SolidBrush^ pincelCopa = gcnew SolidBrush(colorCopa);
		SolidBrush^ pincelCopaOscura = gcnew SolidBrush(colorCopaOscura);
		SolidBrush^ pincelCopaClara = gcnew SolidBrush(colorCopaClara);
		SolidBrush^ pincelTronco = gcnew SolidBrush(Color::FromArgb(255, 82, 56, 34));
		SolidBrush^ pincelTroncoOscuro = gcnew SolidBrush(Color::FromArgb(255, 48, 32, 20));

		for (size_t r = 0; r < arte.size(); r++) {
			for (size_t c = 0; c < arte[r].size(); c++) {
				char ch = arte[r][c];
				if (ch == ' ') continue;
				int px = (int)c * anchoLetra;
				int py = (int)r * altoLetra;
				if (esTronco(ch)) {
					g->FillRectangle(pincelTroncoOscuro, px, py, anchoLetra, altoLetra);
					g->FillRectangle(pincelTronco, px, py, anchoLetra - 1, altoLetra - 1);
				}
				else {
					SolidBrush^ pincel = pincelCopa;
					if (ch == 'o' || ch == '*') pincel = pincelCopaClara;
					else if (ch == '&' || ch == '#') pincel = pincelCopaOscura;
					g->FillRectangle(pincel, px, py, anchoLetra, altoLetra);
				}
			}
		}

		delete pincelCopa;
		delete pincelCopaOscura;
		delete pincelCopaClara;
		delete pincelTronco;
		delete pincelTroncoOscuro;
		delete g;
		imagen = bmp;
	}

public:
	Arbol(int px, int py, TipoArbol tipoArbol) : Entidad(px, py, 32, 32) {
		tipo = tipoArbol;
		dx = 0;
		dy = 0;
		generarImagen();
	}

	virtual ~Arbol() {}

	TipoArbol getTipo() { return tipo; }

	Rectangle getTrunkRect() {
		int anchoTronco = ancho / 10;
		int altoTronco = alto / 6;
		if (anchoTronco < 12) anchoTronco = 12;
		if (altoTronco < 12) altoTronco = 12;
		return Rectangle(x + ancho / 2 - anchoTronco / 2, y + alto - altoTronco, anchoTronco, altoTronco);
	}

	virtual void dibujar(Graphics^ g, Bitmap^ bmp, int camX, int camY, int zoom, int tick) override {
		if (!visible || g == nullptr) return;
		Bitmap^ bmpArbol = imagen;
		if (bmpArbol == nullptr) return;

		int posX = (x - camX) * zoom;
		int posY = (y - camY) * zoom;
		int anchoDestino = ancho * zoom;
		int altoDestino = alto * zoom;
		if (posX + anchoDestino < 0 || posX > g->VisibleClipBounds.Width ||
			posY + altoDestino < 0 || posY > g->VisibleClipBounds.Height) {
			return;
		}

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(bmpArbol, Rectangle(posX, posY, anchoDestino, altoDestino), 0, 0, ancho, alto, GraphicsUnit::Pixel);
	}
};
