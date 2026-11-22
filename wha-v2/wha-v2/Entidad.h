#pragma once

using namespace System::Drawing;

class Entidad {
protected:
	int x;
	int y;
	int dx;
	int dy;
	int ancho;
	int alto;
	int idX;
	int idY;
	int direccion;
	bool visible;
	float escalaVisual;

public:
	Entidad(int px, int py, int w, int h) {
		x = px;
		y = py;
		ancho = w;
		alto = h;
		dx = 4;
		dy = 4;
		idX = 0;
		idY = 0;
		direccion = 0;
		visible = true;
		escalaVisual = 1.0f;
	}

	virtual ~Entidad() {}

	int getX() { return x; }
	int getY() { return y; }
	int getAncho() { return ancho; }
	int getAlto() { return alto; }
	int getIdX() { return idX; }
	int getIdY() { return idY; }
	int getDireccion() { return direccion; }
	bool getVisible() { return visible; }

	void setX(int val) { x = val; }
	void setY(int val) { y = val; }
	void setDx(int val) { dx = val; }
	void setDy(int val) { dy = val; }
	void setVisible(bool val) { visible = val; }
	void setDireccion(int val) { direccion = val; }
	void setEscalaVisual(float val) { escalaVisual = val; }

	Rectangle getRectangle() {
		return Rectangle(x, y, ancho, alto);
	}

	Rectangle getCollisionRect() {
		return Rectangle(x + 3, y + 12, ancho - 6, alto - 12);
	}

	int getBaseY() {
		return y + alto;
	}

	virtual void dibujar(Graphics^ g, Bitmap^ bmp, int camX, int camY, int zoom, int tick) {
		if (!visible || bmp == nullptr || g == nullptr) return;

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

		int srcX = idX * ancho;
		int srcY = idY * alto;
		if (srcX < 0) srcX = 0;
		if (srcY < 0) srcY = 0;
		if (srcX >= bmp->Width || srcY >= bmp->Height) return;

		int srcW = ancho;
		int srcH = alto;
		if (srcX + srcW > bmp->Width) srcW = bmp->Width - srcX;
		if (srcY + srcH > bmp->Height) srcH = bmp->Height - srcY;
		if (srcW <= 0 || srcH <= 0) return;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		Rectangle destino(posX, posY, anchoDestino, altoDestino);
		Rectangle origen(srcX, srcY, srcW, srcH);
		g->DrawImage(bmp, destino, origen, GraphicsUnit::Pixel);
	}
};
