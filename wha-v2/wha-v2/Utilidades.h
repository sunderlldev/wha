#pragma once

#include <string>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Imaging;

struct MisionInfo {
	std::string titulo;
	std::string detalle;
	bool completada;
};

struct PersonajeInfo {
	std::string nombre;
	std::string rol;
	std::string descripcion;
	std::string confianza;
	std::string spriteClave;
};

static String^ ToManaged(const std::string& texto) {
	if (texto.empty()) return "";
	array<Byte>^ bytes = gcnew array<Byte>((int)texto.size());
	for (int i = 0; i < (int)texto.size(); i++) {
		bytes[i] = (Byte)(unsigned char)texto[i];
	}
	return System::Text::Encoding::UTF8->GetString(bytes);
}

static String^ ToManaged(const char* texto) {
	if (texto == nullptr) return "";
	return ToManaged(std::string(texto));
}

static int ClampEntero(int valor, int minimo, int maximo) {
	if (valor < minimo) return minimo;
	if (valor > maximo) return maximo;
	return valor;
}

static float ClampFlotante(float valor, float minimo, float maximo) {
	if (valor < minimo) return minimo;
	if (valor > maximo) return maximo;
	return valor;
}

static String^ ObtenerRutaGuardadoComun(String^ nombreArchivo) {
	String^ carpeta = ".wha_datos";
	try {
		if (!System::IO::Directory::Exists(carpeta)) {
			System::IO::Directory::CreateDirectory(carpeta);
		}
		System::IO::FileAttributes attrs = System::IO::File::GetAttributes(carpeta);
		if ((attrs & System::IO::FileAttributes::Hidden) != System::IO::FileAttributes::Hidden) {
			System::IO::File::SetAttributes(carpeta, attrs | System::IO::FileAttributes::Hidden);
		}
		String^ destino = System::IO::Path::Combine(carpeta, nombreArchivo);
		if (!System::IO::File::Exists(destino) && System::IO::File::Exists(nombreArchivo)) {
			System::IO::File::Move(nombreArchivo, destino);
		}
		return destino;
	}
	catch (...) {
		return nombreArchivo;
	}
}

static std::string ObtenerRutaGuardadoStdComun(const char* nombreArchivo) {
	ObtenerRutaGuardadoComun(ToManaged(nombreArchivo));
	if (System::IO::Directory::Exists(".wha_datos")) {
		return std::string(".wha_datos/") + nombreArchivo;
	}
	return std::string(nombreArchivo);
}

static Bitmap^ CargarBitmapSeguroComun(String^ nombreArchivo, bool crearVacioSiFalta) {
	if (System::IO::File::Exists(nombreArchivo)) return gcnew Bitmap(nombreArchivo);
	String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
	array<String^>^ prefijos = gcnew array<String^>{
		"",
		"..\\..\\wha-v2\\",
		"..\\..\\..\\wha-v2\\wha-v2\\",
		"..\\..\\..\\..\\wha-v2\\wha-v2\\"
	};
	for each (String^ pref in prefijos) {
		String^ ruta = System::IO::Path::Combine(baseDir, pref + nombreArchivo);
		if (System::IO::File::Exists(ruta)) return gcnew Bitmap(ruta);
	}
	if (crearVacioSiFalta) return gcnew Bitmap(32, 32);
	return nullptr;
}

static Bitmap^ CrearVarianteSpriteComun(Bitmap^ base, Color ropaOscura, Color ropaClara, Color peloOscuro, Color peloClaro) {
	if (base == nullptr) return gcnew Bitmap(240, 256);
	Bitmap^ copia = gcnew Bitmap(base->Width, base->Height, PixelFormat::Format32bppArgb);
	for (int y = 0; y < base->Height; y++) {
		for (int x = 0; x < base->Width; x++) {
			Color p = base->GetPixel(x, y);
			int alpha = (int)p.A;
			if (alpha < 20) {
				copia->SetPixel(x, y, Color::Transparent);
				continue;
			}
			Color out = p;
			if (p.G > p.R + 18 && p.B >= p.R + 10) {
				int br = ((int)p.R + (int)p.G + (int)p.B) / 3;
				out = (br > 145) ? Color::FromArgb(alpha, ropaClara) : Color::FromArgb(alpha, ropaOscura);
			}
			else if (p.G > p.B + 25 && p.R >= p.B + 15) {
				int br = ((int)p.R + (int)p.G + (int)p.B) / 3;
				out = (br > 145) ? Color::FromArgb(alpha, peloClaro) : Color::FromArgb(alpha, peloOscuro);
			}
			copia->SetPixel(x, y, out);
		}
	}
	return copia;
}

static Bitmap^ CrearVarianteDagdaComun(Bitmap^ baseQifrey) {
	if (baseQifrey == nullptr) return gcnew Bitmap(96, 128);
	Bitmap^ copia = gcnew Bitmap(baseQifrey->Width, baseQifrey->Height, PixelFormat::Format32bppArgb);
	for (int y = 0; y < baseQifrey->Height; y++) {
		for (int x = 0; x < baseQifrey->Width; x++) {
			Color p = baseQifrey->GetPixel(x, y);
			int alpha = (int)p.A;
			if (alpha < 20) {
				copia->SetPixel(x, y, Color::Transparent);
				continue;
			}
			Color out = p;
			if ((int)p.B >= (int)p.R + 6 && (int)p.B > 110 && !((int)p.R > 220 && (int)p.G > 195)) {
				int br = ((int)p.R + (int)p.G + (int)p.B) / 3;
				if (br > 205) out = Color::FromArgb(alpha, 122, 138, 96);
				else if (br > 170) out = Color::FromArgb(alpha, 88, 108, 72);
				else out = Color::FromArgb(alpha, 56, 72, 48);
			}
			copia->SetPixel(x, y, out);
		}
	}
	return copia;
}

static Bitmap^ CrearVarianteIniniaComun(Bitmap^ baseMago) {
	if (baseMago == nullptr) return gcnew Bitmap(64, 96);
	Bitmap^ copia = gcnew Bitmap(baseMago->Width, baseMago->Height, PixelFormat::Format32bppArgb);
	for (int y = 0; y < baseMago->Height; y++) {
		for (int x = 0; x < baseMago->Width; x++) {
			Color p = baseMago->GetPixel(x, y);
			int alpha = (int)p.A;
			if (alpha < 20) {
				copia->SetPixel(x, y, Color::Transparent);
				continue;
			}
			Color out = p;
			if ((int)p.R >= (int)p.G + 14 && (int)p.B <= (int)p.R - 10 && (int)p.R > 95) {
				out = Color::FromArgb(alpha, Math::Min(255, (int)p.R + 28), Math::Min(255, (int)p.G + 8), Math::Min(255, (int)p.B + 86));
			}
			else if ((int)p.B >= (int)p.G + 8 && (int)p.R >= 30 && (int)p.B >= 45) {
				out = Color::FromArgb(alpha, Math::Min(255, (int)p.R + 22), (int)p.G, Math::Min(255, (int)p.B + 42));
			}
			copia->SetPixel(x, y, out);
		}
	}
	return copia;
}

static Bitmap^ CrearSpritePolvoFrameComun(int frame) {
	Bitmap^ bmp = gcnew Bitmap(16, 16, PixelFormat::Format32bppArgb);
	Graphics^ g = Graphics::FromImage(bmp);
	g->Clear(Color::Transparent);
	SolidBrush^ cBorde = gcnew SolidBrush(Color::FromArgb(145, 92, 84, 72));
	SolidBrush^ cSombra = gcnew SolidBrush(Color::FromArgb(175, 168, 158, 140));
	SolidBrush^ cMedio = gcnew SolidBrush(Color::FromArgb(205, 212, 204, 186));
	SolidBrush^ cLuz = gcnew SolidBrush(Color::FromArgb(220, 240, 236, 224));

	if (frame % 3 == 0) {
		g->FillRectangle(cBorde, 5, 10, 6, 4);
		g->FillRectangle(cSombra, 4, 9, 7, 4);
		g->FillRectangle(cMedio, 5, 8, 5, 4);
		g->FillRectangle(cLuz, 6, 8, 3, 2);
		g->FillRectangle(cMedio, 11, 11, 2, 2);
	}
	else if (frame % 3 == 1) {
		g->FillRectangle(cBorde, 3, 8, 8, 5);
		g->FillRectangle(cSombra, 4, 7, 8, 5);
		g->FillRectangle(cMedio, 5, 6, 6, 4);
		g->FillRectangle(cLuz, 6, 6, 4, 2);
		g->FillRectangle(cSombra, 1, 10, 3, 3);
		g->FillRectangle(cMedio, 1, 9, 2, 2);
	}
	else {
		g->FillRectangle(cBorde, 2, 9, 6, 4);
		g->FillRectangle(cSombra, 2, 8, 5, 4);
		g->FillRectangle(cMedio, 3, 7, 4, 3);
		g->FillRectangle(cSombra, 9, 6, 4, 3);
		g->FillRectangle(cLuz, 10, 6, 2, 2);
		g->FillRectangle(cMedio, 0, 11, 2, 2);
	}

	delete cBorde;
	delete cSombra;
	delete cMedio;
	delete cLuz;
	delete g;
	return bmp;
}

static void DibujarPolvoCorrerComun(Graphics^ g, int x, int y, int ancho, int alto, int direccion, int ticks, int camX, int camY, int zoom) {
	if (g == nullptr) return;
	int frame = (ticks / 3) % 3;
	Bitmap^ sprite = CrearSpritePolvoFrameComun(frame);
	int mundoX = x;
	int mundoY = y + alto - 13;
	if (direccion == 3) {
		mundoX = x - 11;
		mundoY = y + alto - 12;
	}
	else if (direccion == 1) {
		mundoX = x + ancho - 3;
		mundoY = y + alto - 12;
	}
	else if (direccion == 0) {
		mundoX = x + ancho / 2 - 7;
		mundoY = y + alto - 18;
	}
	else {
		mundoX = x + ancho / 2 - 7;
		mundoY = y + alto - 4;
	}

	int px = (mundoX - camX) * zoom;
	int py = (mundoY - camY) * zoom;
	int tam = 14 * zoom;
	g->InterpolationMode = InterpolationMode::NearestNeighbor;
	g->PixelOffsetMode = PixelOffsetMode::Half;
	g->DrawImage(sprite, Rectangle(px, py, tam, tam), 0, 0, 16, 16, GraphicsUnit::Pixel);

	Bitmap^ spriteSec = CrearSpritePolvoFrameComun((frame + 2) % 3);
	int secX = px;
	int secY = py + 2 * zoom;
	if (direccion == 3) secX -= 8 * zoom;
	else if (direccion == 1) secX += 8 * zoom;
	else if (direccion == 0) secY -= 5 * zoom;
	else secY += 5 * zoom;
	g->DrawImage(spriteSec, Rectangle(secX, secY, 10 * zoom, 10 * zoom), 0, 0, 16, 16, GraphicsUnit::Pixel);
	delete sprite;
	delete spriteSec;
}

static void DibujarMyrphonSpritesheetComun(Graphics^ g, Bitmap^ bmp, int x, int y, int dirEntidad, bool moviendo, int ticks, int camX, int camY, int zoom, int destW, int destH) {
	if (g == nullptr || bmp == nullptr || bmp->Width < 6 || bmp->Height < 4) return;
	if (moviendo) {
		DibujarPolvoCorrerComun(g, x, y, destW, destH, dirEntidad, ticks, camX, camY, zoom);
	}
	int cellW = bmp->Width / 6;
	int cellH = bmp->Height / 4;
	int col = 0;
	int fila = 0;
	if (moviendo) {
		col = (ticks / 4) % 6;
		if (dirEntidad == 2) fila = 1;
		else if (dirEntidad == 1) fila = 2;
		else if (dirEntidad == 3) fila = 3;
		else fila = 0;
	}
	int px = (x - camX) * zoom;
	int py = (y - camY) * zoom;
	g->InterpolationMode = InterpolationMode::NearestNeighbor;
	g->PixelOffsetMode = PixelOffsetMode::Half;
	g->DrawImage(bmp, Rectangle(px, py, destW * zoom, destH * zoom), col * cellW, fila * cellH, cellW, cellH, GraphicsUnit::Pixel);
}

static void DibujarFlechaGuiaComun(Graphics^ g, int jugadorSX, int jugadorSY, int targetSX, int targetSY, int viewW, int viewH, int ticks) {
	if (g == nullptr) return;
	float dx = (float)(targetSX - jugadorSX);
	float dy = (float)(targetSY - jugadorSY);
	float dist = (float)Math::Sqrt(dx * dx + dy * dy);
	if (dist < 1.0f) return;

	float ux = dx / dist;
	float uy = dy / dist;
	float cx = (float)jugadorSX + ux * 74.0f;
	float cy = (float)jugadorSY + uy * 74.0f;

	if (dist <= 96.0f && targetSX >= 30 && targetSX <= viewW - 30 && targetSY >= 30 && targetSY <= viewH - 30) {
		float bob = (float)(Math::Sin(ticks * 0.22) * 4.0);
		cx = (float)targetSX;
		cy = (float)targetSY - 32.0f + bob;
		ux = 0.0f;
		uy = 1.0f;
	}
	else {
		if (cx < 32.0f) cx = 32.0f;
		if (cx > (float)(viewW - 32)) cx = (float)(viewW - 32);
		if (cy < 92.0f) cy = 92.0f;
		if (cy > (float)(viewH - 42)) cy = (float)(viewH - 42);
	}

	float px = -uy;
	float py = ux;
	PointF pPunta(cx + ux * 13.0f, cy + uy * 13.0f);
	PointF pIzq(cx - ux * 8.0f + px * 9.0f, cy - uy * 8.0f + py * 9.0f);
	PointF pCentro(cx - ux * 3.0f, cy - uy * 3.0f);
	PointF pDer(cx - ux * 8.0f - px * 9.0f, cy - uy * 8.0f - py * 9.0f);

	g->SmoothingMode = SmoothingMode::AntiAlias;
	SolidBrush^ halo = gcnew SolidBrush(Color::FromArgb(90, 12, 18, 28));
	g->FillEllipse(halo, cx - 15.0f, cy - 15.0f, 30.0f, 30.0f);
	SolidBrush^ relleno = gcnew SolidBrush(Color::FromArgb(245, 232, 202, 126));
	Pen^ borde = gcnew Pen(Color::FromArgb(255, 24, 28, 38), 1.6f);
	array<PointF>^ pts = gcnew array<PointF>{ pPunta, pIzq, pCentro, pDer };
	g->FillPolygon(relleno, pts);
	g->DrawPolygon(borde, pts);
	delete halo;
	delete relleno;
	delete borde;
}

static void DibujarIntroActoComun(Graphics^ g, int viewW, int viewH, String^ actoTag, String^ titulo, String^ cuerpo, String^ pie) {
	if (g == nullptr) return;
	SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(255, 9, 14, 21));
	g->FillRectangle(fondo, 0, 0, viewW, viewH);
	delete fondo;

	int cardW = Math::Min(680, viewW - 96);
	int cardH = Math::Min(410, viewH - 96);
	int cardX = (viewW - cardW) / 2;
	int cardY = (viewH - cardH) / 2;

	SolidBrush^ cardBrush = gcnew SolidBrush(Color::FromArgb(255, 11, 17, 26));
	Pen^ borde = gcnew Pen(Color::FromArgb(75, 214, 198, 166), 1.0f);
	g->FillRectangle(cardBrush, cardX, cardY, cardW, cardH);
	g->DrawRectangle(borde, cardX, cardY, cardW, cardH);
	delete cardBrush;
	delete borde;

	System::Drawing::Font^ fActo = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
	System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 17.5f, FontStyle::Bold);
	System::Drawing::Font^ fCuerpo = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
	System::Drawing::Font^ fPie = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);

	SolidBrush^ brushActo = gcnew SolidBrush(Color::FromArgb(255, 214, 198, 166));
	SolidBrush^ brushTitulo = gcnew SolidBrush(Color::FromArgb(255, 244, 247, 250));
	SolidBrush^ brushCuerpo = gcnew SolidBrush(Color::FromArgb(255, 206, 216, 228));
	SolidBrush^ brushPie = gcnew SolidBrush(Color::FromArgb(255, 224, 206, 152));
	SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(70, 214, 198, 166));

	StringFormat^ sf = gcnew StringFormat();
	sf->Alignment = StringAlignment::Center;
	sf->LineAlignment = StringAlignment::Near;

	g->DrawString(actoTag, fActo, brushActo, RectangleF((float)cardX + 36.0f, (float)cardY + 34.0f, (float)cardW - 72.0f, 24.0f), sf);
	g->DrawString(titulo, fTitulo, brushTitulo, RectangleF((float)cardX + 36.0f, (float)cardY + 62.0f, (float)cardW - 72.0f, 38.0f), sf);
	g->FillRectangle(linea, cardX + 90, cardY + 110, cardW - 180, 1);
	g->DrawString(cuerpo, fCuerpo, brushCuerpo, RectangleF((float)cardX + 48.0f, (float)cardY + 132.0f, (float)cardW - 96.0f, (float)cardH - 204.0f), sf);
	g->DrawString(pie, fPie, brushPie, RectangleF((float)cardX + 36.0f, (float)(cardY + cardH - 52), (float)cardW - 72.0f, 28.0f), sf);

	delete sf;
	delete brushActo;
	delete brushTitulo;
	delete brushCuerpo;
	delete brushPie;
	delete linea;
	delete fActo;
	delete fTitulo;
	delete fCuerpo;
	delete fPie;
}

static Bitmap^ CargarRetratoPersonajeComun(String^ clave) {
	if (String::IsNullOrEmpty(clave)) return nullptr;
	String^ k = clave->Trim()->ToLowerInvariant();
	String^ archivo = nullptr;
	int cols = 10;
	int rows = 8;
	int modoVariante = 0;

	if (k->Contains("coco")) {
		archivo = "sprites/Coco_Spritesheet.png";
		cols = 10; rows = 8;
	}
	else if (k->Contains("qifrey")) {
		archivo = "sprites/Qifrey_Spritesheet.png";
		cols = 4; rows = 4;
	}
	else if (k->Contains("agott")) {
		archivo = "sprites/Agott_Spritesheet.png";
		cols = 10; rows = 8;
	}
	else if (k->Contains("richeh")) {
		archivo = "sprites/Richeh_Spritesheet.png";
		cols = 4; rows = 4;
	}
	else if (k->Contains("myrphon")) {
		archivo = "sprites/Myrphon_Spritesheet.png";
		cols = 6; rows = 4;
	}
	else if (k->Contains("sombrero") || k->Contains("mago")) {
		archivo = "sprites/Mago_Cueva_Spritesheet.png";
		cols = 4; rows = 4;
	}
	else if (k->Contains("tartah")) {
		archivo = "sprites/Tartah_Spritesheet.png";
		cols = 10; rows = 8;
	}
	else if (k->Contains("sanguijuela")) {
		archivo = "sprites/sanguijuela.png";
		cols = 8; rows = 4;
	}
	else if (k->Contains("coustas")) {
		archivo = "sprites/Coustas_Spritesheet.png";
		cols = 10; rows = 8;
	}
	else if (k->Contains("dagda")) {
		archivo = "sprites/Qifrey_Spritesheet.png";
		cols = 4; rows = 4;
		modoVariante = 1;
	}
	else if (k->Contains("ininia")) {
		archivo = "sprites/Mago_Cueva_Spritesheet.png";
		cols = 4; rows = 4;
		modoVariante = 2;
	}
	else {
		return nullptr;
	}

	Bitmap^ hoja = CargarBitmapSeguroComun(archivo, false);
	if (hoja == nullptr) return nullptr;
	Bitmap^ origen = hoja;
	if (modoVariante == 1) {
		origen = CrearVarianteDagdaComun(hoja);
		delete hoja;
	}
	else if (modoVariante == 2) {
		origen = CrearVarianteIniniaComun(hoja);
		delete hoja;
	}

	int fw = Math::Max(1, origen->Width / cols);
	int fh = Math::Max(1, origen->Height / rows);
	int filaQuieto = (k->Contains("sanguijuela")) ? 1 : 0;
	Bitmap^ quieto = gcnew Bitmap(fw, fh, PixelFormat::Format32bppArgb);
	Graphics^ gr = Graphics::FromImage(quieto);
	gr->Clear(Color::Transparent);
	gr->InterpolationMode = InterpolationMode::NearestNeighbor;
	gr->PixelOffsetMode = PixelOffsetMode::Half;
	gr->DrawImage(origen, Rectangle(0, 0, fw, fh), Rectangle(0, filaQuieto * fh, fw, fh), GraphicsUnit::Pixel);
	delete gr;
	delete origen;
	return quieto;
}
