#pragma once

#include <string>
#include <vector>
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
	ObjetoMontonCajas = 10,
	ObjetoSalidaEscaleras = 11
};

class BloqueMapa : public Entidad {
private:
	int tipo;
	bool recolectado;
	std::string nombre;
	std::string descripcion;
	gcroot<Bitmap^> icono;
	gcroot<Bitmap^> iconoRunA;
	gcroot<Bitmap^> iconoRunB;

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		if (System::IO::File::Exists(nombreArchivo)) {
			return gcnew Bitmap(nombreArchivo);
		}
		String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
		array<String^>^ prefijos = gcnew array<String^>{
			"",
			"..\\..\\wha-v2\\",
			"..\\..\\..\\wha-v2\\wha-v2\\",
			"..\\..\\..\\..\\wha-v2\\wha-v2\\"
		};
		for each (String^ pref in prefijos) {
			String^ ruta = System::IO::Path::Combine(baseDir, pref + nombreArchivo);
			if (System::IO::File::Exists(ruta)) {
				return gcnew Bitmap(ruta);
			}
		}
		return gcnew Bitmap(32, 32);
	}

	static Bitmap^ CrearIconoVial(Color liquido) {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, liquido));
		SolidBrush^ vidrio = gcnew SolidBrush(Color::FromArgb(235, 210, 238, 250));
		SolidBrush^ corcho = gcnew SolidBrush(Color::FromArgb(235, 118, 82, 48));
		SolidBrush^ contenido = gcnew SolidBrush(liquido);
		SolidBrush^ reflejo = gcnew SolidBrush(Color::FromArgb(210, 250, 250, 255));
		g->FillEllipse(brillo, 2, 3, 20, 20);
		g->FillRectangle(vidrio, 8, 7, 8, 13);
		g->FillRectangle(contenido, 9, 11, 6, 8);
		g->FillRectangle(corcho, 9, 4, 6, 4);
		g->FillRectangle(reflejo, 10, 8, 2, 4);
		delete brillo;
		delete vidrio;
		delete corcho;
		delete contenido;
		delete reflejo;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPluma() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(70, 255, 214, 110));
		SolidBrush^ oro = gcnew SolidBrush(Color::FromArgb(245, 226, 174, 72));
		SolidBrush^ claro = gcnew SolidBrush(Color::FromArgb(245, 252, 232, 160));
		Pen^ trazo = gcnew Pen(Color::FromArgb(255, 130, 90, 40), 1.0f);
		g->FillEllipse(brillo, 2, 2, 20, 20);
		g->FillPolygon(oro, gcnew array<Point>{ Point(6, 19), Point(12, 4), Point(18, 19), Point(12, 15) });
		g->FillPolygon(claro, gcnew array<Point>{ Point(11, 16), Point(12, 6), Point(15, 16), Point(12, 14) });
		g->DrawLine(trazo, 6, 20, 18, 20);
		delete brillo;
		delete oro;
		delete claro;
		delete trazo;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoHoja() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(60, 120, 220, 130));
		SolidBrush^ hoja = gcnew SolidBrush(Color::FromArgb(235, 74, 150, 84));
		SolidBrush^ nervio = gcnew SolidBrush(Color::FromArgb(235, 40, 94, 54));
		Pen^ pNervio = gcnew Pen(nervio, 1.0f);
		g->FillEllipse(brillo, 2, 2, 20, 20);
		g->FillPolygon(hoja, gcnew array<Point>{ Point(5, 18), Point(10, 4), Point(20, 8), Point(13, 20) });
		g->DrawLine(pNervio, 7, 17, 17, 9);
		delete brillo;
		delete hoja;
		delete nervio;
		delete pNervio;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPergamino() {
		Bitmap^ bmp = gcnew Bitmap(24, 24, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(60, 230, 210, 170));
		SolidBrush^ papel = gcnew SolidBrush(Color::FromArgb(240, 226, 208, 164));
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(240, 148, 116, 70));
		Pen^ linea = gcnew Pen(Color::FromArgb(180, 110, 84, 50), 1.0f);
		g->FillEllipse(brillo, 2, 2, 20, 20);
		g->FillRectangle(papel, 5, 4, 14, 16);
		g->FillRectangle(borde, 4, 3, 16, 3);
		g->FillRectangle(borde, 4, 18, 16, 3);
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
		delete gato;
		delete madera;
		return bmp;
	}

	static Bitmap^ CrearIconoTroll() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		SolidBrush^ portal = gcnew SolidBrush(Color::FromArgb(200, 38, 28, 48));
		SolidBrush^ grieta = gcnew SolidBrush(Color::FromArgb(230, 120, 90, 170));
		SolidBrush^ marco = gcnew SolidBrush(Color::FromArgb(255, 70, 68, 76));
		g->FillRectangle(marco, 5, 4, 22, 24);
		g->FillRectangle(portal, 8, 6, 16, 20);
		g->FillPolygon(grieta, gcnew array<Point>{ Point(15, 7), Point(18, 12), Point(13, 17), Point(17, 24), Point(14, 24), Point(10, 17), Point(15, 12) });
		delete portal;
		delete grieta;
		delete marco;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoPozo() {
		Bitmap^ bmp = gcnew Bitmap(64, 64, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(180, 18, 14, 12));
		SolidBrush^ piedra = gcnew SolidBrush(Color::FromArgb(245, 118, 116, 108));
		SolidBrush^ piedraClara = gcnew SolidBrush(Color::FromArgb(245, 148, 146, 138));
		SolidBrush^ piedraOscura = gcnew SolidBrush(Color::FromArgb(245, 72, 72, 68));
		SolidBrush^ hueco = gcnew SolidBrush(Color::FromArgb(255, 8, 10, 14));
		SolidBrush^ madera = gcnew SolidBrush(Color::FromArgb(245, 104, 68, 38));
		SolidBrush^ maderaOscura = gcnew SolidBrush(Color::FromArgb(245, 58, 38, 24));
		SolidBrush^ soga = gcnew SolidBrush(Color::FromArgb(245, 198, 176, 116));

		g->FillEllipse(sombra, 4, 14, 56, 46);
		g->FillEllipse(piedraOscura, 6, 12, 52, 44);
		g->FillEllipse(piedra, 8, 10, 48, 40);
		g->FillEllipse(piedraClara, 14, 12, 36, 14);
		g->FillEllipse(hueco, 16, 18, 32, 24);
		g->FillRectangle(maderaOscura, 8, 4, 7, 46);
		g->FillRectangle(maderaOscura, 49, 4, 7, 46);
		g->FillRectangle(madera, 9, 4, 5, 44);
		g->FillRectangle(madera, 50, 4, 5, 44);
		g->FillRectangle(maderaOscura, 6, 2, 52, 7);
		g->FillRectangle(madera, 8, 3, 48, 4);
		g->FillRectangle(soga, 30, 9, 4, 24);
		g->FillEllipse(madera, 25, 30, 14, 11);

		delete sombra;
		delete piedra;
		delete piedraClara;
		delete piedraOscura;
		delete hueco;
		delete madera;
		delete maderaOscura;
		delete soga;
		delete g;
		return bmp;
	}

	static Bitmap^ CrearIconoEscalerasSubida() {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;

		SolidBrush^ bordePiedra = gcnew SolidBrush(Color::FromArgb(255, 22, 26, 34));
		SolidBrush^ marcoPiedra = gcnew SolidBrush(Color::FromArgb(255, 68, 76, 92));
		SolidBrush^ luzPiedra = gcnew SolidBrush(Color::FromArgb(255, 96, 106, 126));
		SolidBrush^ pozoOscuro = gcnew SolidBrush(Color::FromArgb(255, 16, 18, 26));
		SolidBrush^ luzArriba = gcnew SolidBrush(Color::FromArgb(255, 226, 184, 104));
		SolidBrush^ luzSuave = gcnew SolidBrush(Color::FromArgb(255, 152, 114, 62));
		SolidBrush^ maderaBorde = gcnew SolidBrush(Color::FromArgb(255, 58, 36, 20));
		SolidBrush^ maderaOscura = gcnew SolidBrush(Color::FromArgb(255, 112, 74, 42));
		SolidBrush^ maderaClara = gcnew SolidBrush(Color::FromArgb(255, 168, 118, 68));

		g->FillRectangle(bordePiedra, 3, 2, 26, 28);
		g->FillRectangle(marcoPiedra, 4, 3, 24, 26);
		g->FillRectangle(luzPiedra, 4, 3, 24, 2);
		g->FillRectangle(pozoOscuro, 6, 5, 20, 23);
		g->FillRectangle(luzArriba, 6, 5, 20, 6);
		g->FillRectangle(luzSuave, 6, 11, 20, 4);

		g->FillRectangle(maderaBorde, 9, 4, 3, 25);
		g->FillRectangle(maderaBorde, 20, 4, 3, 25);
		g->FillRectangle(maderaOscura, 10, 4, 2, 25);
		g->FillRectangle(maderaOscura, 20, 4, 2, 25);

		int pasos[5] = { 7, 12, 17, 22, 26 };
		for (int i = 0; i < 5; i++) {
			int py = pasos[i];
			g->FillRectangle(maderaBorde, 10, py + 1, 12, 2);
			g->FillRectangle(maderaClara, 10, py, 12, 2);
		}

		delete bordePiedra;
		delete marcoPiedra;
		delete luzPiedra;
		delete pozoOscuro;
		delete luzArriba;
		delete luzSuave;
		delete maderaBorde;
		delete maderaOscura;
		delete maderaClara;
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
		delete caja;
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

	static Color ColorPixelMyrphon(char ch) {
		switch (ch) {
		case 'B': return Color::FromArgb(255, 86, 168, 238);
		case 'L': return Color::FromArgb(255, 142, 206, 252);
		case 'I': return Color::FromArgb(255, 214, 152, 222);
		case 'K': return Color::FromArgb(255, 30, 32, 40);
		case 'D': return Color::FromArgb(255, 58, 66, 86);
		case 'W': return Color::FromArgb(255, 248, 250, 255);
		case 'E': return Color::FromArgb(255, 14, 16, 22);
		case 'F': return Color::FromArgb(255, 246, 244, 236);
		case 'S': return Color::FromArgb(255, 204, 210, 222);
		case 'P': return Color::FromArgb(255, 224, 138, 190);
		default: return Color::Transparent;
		}
	}

	static Bitmap^ RenderizarPixelArt16(const char* filas[16]) {
		Bitmap^ bmp = gcnew Bitmap(32, 32, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->Clear(Color::Transparent);
		g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;
		for (int r = 0; r < 16; r++) {
			for (int c = 0; c < 16; c++) {
				char ch = filas[r][c];
				if (ch == '.') continue;
				SolidBrush^ pincel = gcnew SolidBrush(ColorPixelMyrphon(ch));
				g->FillRectangle(pincel, c * 2, r * 2, 2, 2);
				delete pincel;
			}
		}
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
			icono = CrearSpriteMyrphonFrame(0);
			iconoRunA = CrearSpriteMyrphonFrame(1);
			iconoRunB = CrearSpriteMyrphonFrame(2);
			ancho = 32;
			alto = 32;
		}
		else if (tipo == ObjetoPozo) {
			icono = CrearIconoPozo();
			ancho = 64;
			alto = 64;
		}
		else if (tipo == ObjetoSalidaEscaleras) {
			icono = CrearIconoEscalerasSubida();
			ancho = 64;
			alto = 64;
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
	static Bitmap^ CrearSpriteMyrphonFrame(int frame) {
		static const char* spriteIdle[16] = {
			"....BL...BL.....",
			"....BI...BI.....",
			"....BIKKKIB.....",
			"....KKKKKKKD....",
			"...KKWWKKWWKDD..",
			"...KKWEKKWEKKD..",
			"....KKKKKKKK....",
			"...LFFFFFFFF....",
			"..BBFFFFFFFFLL..",
			".BB..FFPPFF..LB.",
			"....FFFFFFFF....",
			"...KKFFFFFFSS...",
			"...KKFFFFFFSS...",
			"....KSSSSSSS....",
			"....PP....PP....",
			"................"
		};
		static const char* spriteRun1[16] = {
			".....BL...BL....",
			".....BI...BI....",
			"....BBIKKKIB....",
			"....KKKKKKKKD...",
			"...KKWWKKWWKKDD.",
			"...KKWEKKWEKKD..",
			".LL.KKKKKKKK.LL.",
			"BB..FFFFFFFF..BB",
			"....FFFPPFFF....",
			"....FFFFFFFF....",
			"...KKFFFFFFSS...",
			"...KKSSSSSS.....",
			"..PP........PP..",
			"................",
			"................",
			"................"
		};
		static const char* spriteRun2[16] = {
			"................",
			"....BL...BL.....",
			"....BI...BI.....",
			"....BIKKKIB.....",
			"....KKKKKKKD....",
			"...KKWWKKWWKDD..",
			"...KKWEKKWEKKD..",
			"....KKKKKKKK....",
			"...LFFFFFFFF....",
			"..BBFFFPPFFFBB..",
			"....FFFFFFFF....",
			"...KKFFFFFFSS...",
			"....KSSSSSSS....",
			".....PP..PP.....",
			"................",
			"................"
		};
		if (frame == 1) return RenderizarPixelArt16(spriteRun1);
		if (frame == 2) return RenderizarPixelArt16(spriteRun2);
		return RenderizarPixelArt16(spriteIdle);
	}

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

	Bitmap^ getIconoAnimado(int tick, bool corriendo) {
		if (tipo != ObjetoMyrphon) return icono;
		if (corriendo) {
			return ((tick / 5) % 2 == 0) ? iconoRunA : iconoRunB;
		}
		return ((tick / 18) % 4 == 3) ? iconoRunB : icono;
	}

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
		if (!visible || recolectado || g == nullptr || tipo == ObjetoTroll) return;
		Bitmap^ bmpIcono = (tipo == ObjetoMyrphon) ? getIconoAnimado(tick, false) : icono;
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
