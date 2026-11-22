#pragma once

#include <gcroot.h>

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;

	public ref class FormMinijuego : public Form {
	private:
		static const int TILE = 32;
		static const int COLUMNAS = 24;
		static const int FILAS = 15;
		static const int OFFSET_X = 26;
		static const int OFFSET_Y = 96;

		List<String^>^ mapa;
		Timer^ timerJuego;
		Random^ aleatorio;

		Bitmap^ bmpCoco;
		Bitmap^ bmpMuro;
		Bitmap^ bmpPiso;

		int jugadorX;
		int jugadorY;
		int myrphonTileX;
		int myrphonTileY;
		array<int>^ enemigoTileX;
		array<int>^ enemigoTileY;
		array<int>^ enemigoDirX;
		array<int>^ enemigoDirY;
		array<int>^ enemigoTicks;

		bool arriba;
		bool abajo;
		bool izquierda;
		bool derecha;

		int vidas;
		int ticksRestantes;
		int ticksAnimacion;
		int invulnerableTicks;
		int ticksMyrphon;
		int frameCoco;
		bool terminado;

		String^ LeerArchivoTexto(String^ nombre) {
			if (System::IO::File::Exists(nombre)) return nombre;
			String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
			String^ ruta1 = System::IO::Path::Combine(baseDir, nombre);
			if (System::IO::File::Exists(ruta1)) return ruta1;
			return "";
		}

		Bitmap^ CargarBitmap(String^ nombre) {
			String^ ruta = LeerArchivoTexto(nombre);
			if (ruta != "") return gcnew Bitmap(ruta);
			return gcnew Bitmap(32, 32);
		}

		void ConstruirMapa() {
			mapa = gcnew List<String^>();
			for (int f = 0; f < FILAS; f++) {
				array<Char>^ fila = gcnew array<Char>(COLUMNAS);
				for (int c = 0; c < COLUMNAS; c++) fila[c] = '.';
				fila[0] = '#';
				fila[COLUMNAS - 1] = '#';
				if (f == 0 || f == FILAS - 1) {
					for (int c = 0; c < COLUMNAS; c++) fila[c] = '#';
				}
				mapa->Add(gcnew String(fila));
			}
			int bloques[8][4] = {
				{ 4, 2, 4, 2 }, { 15, 2, 4, 2 },
				{ 4, 6, 4, 2 }, { 15, 6, 4, 2 },
				{ 4, 10, 4, 2 }, { 15, 10, 4, 2 },
				{ 10, 4, 3, 2 }, { 10, 9, 3, 2 }
			};
			for (int b = 0; b < 8; b++) {
				for (int y = bloques[b][1]; y < bloques[b][1] + bloques[b][3]; y++) {
					for (int x = bloques[b][0]; x < bloques[b][0] + bloques[b][2]; x++) {
						if (y > 0 && y < FILAS - 1 && x > 0 && x < COLUMNAS - 1) {
							String^ fila = mapa[y];
							array<Char>^ chars = fila->ToCharArray();
							chars[x] = '#';
							mapa[y] = gcnew String(chars);
						}
					}
				}
			}
		}

		Char TileEn(int px, int py) {
			int col = px / TILE;
			int fil = py / TILE;
			if (fil < 0 || fil >= FILAS || col < 0 || col >= COLUMNAS) return '#';
			return mapa[fil][col];
		}

		bool EsPared(int px, int py, int ancho, int alto) {
			int xs[4] = { px + 4, px + ancho - 4, px + 4, px + ancho - 4 };
			int ys[4] = { py + 14, py + 14, py + alto - 3, py + alto - 3 };
			for (int i = 0; i < 4; i++) {
				if (TileEn(xs[i], ys[i]) == '#') return true;
			}
			return false;
		}

		void ReiniciarJugador() {
			jugadorX = 2 * TILE + 4;
			jugadorY = 1 * TILE;
		}

		void ReiniciarMinijuego() {
			vidas = 3;
			ticksRestantes = 30 * 40;
			ticksAnimacion = 0;
			invulnerableTicks = 0;
			ticksMyrphon = 0;
			frameCoco = 0;
			terminado = false;
			ReiniciarJugador();

			myrphonTileX = 21;
			myrphonTileY = 13;

			enemigoTileX[0] = 20; enemigoTileY[0] = 2; enemigoDirX[0] = -1; enemigoDirY[0] = 0; enemigoTicks[0] = 0;
			enemigoTileX[1] = 2; enemigoTileY[1] = 12; enemigoDirX[1] = 1; enemigoDirY[1] = 0; enemigoTicks[1] = 20;
			enemigoTileX[2] = 12; enemigoTileY[2] = 7; enemigoDirX[2] = 0; enemigoDirY[2] = 1; enemigoTicks[2] = 10;
		}

		void MoverJugador(int dx, int dy) {
			int pasoX = 5;
			int pasoY = 5;
			if (dx != 0 && dy != 0) {
				pasoX = 4;
				pasoY = 4;
			}
			if (dx != 0) {
				int nx = jugadorX + dx * pasoX;
				if (!EsPared(nx, jugadorY, 24, 32)) jugadorX = nx;
			}
			if (dy != 0) {
				int ny = jugadorY + dy * pasoY;
				if (!EsPared(jugadorX, ny, 24, 32)) jugadorY = ny;
			}
			ticksAnimacion++;
			if (ticksAnimacion % 2 == 0) {
				frameCoco++;
				if (frameCoco >= 10) frameCoco = 0;
			}
		}

		void MoverMyrphon() {
			ticksMyrphon++;
			if (ticksMyrphon % 7 != 0) return;
			int direccion = aleatorio->Next(0, 4);
			int dx = 0;
			int dy = 0;
			if (direccion == 0) dx = 1;
			else if (direccion == 1) dx = -1;
			else if (direccion == 2) dy = 1;
			else dy = -1;
			int nx = myrphonTileX + dx;
			int ny = myrphonTileY + dy;
			if (nx > 0 && nx < COLUMNAS - 1 && ny > 0 && ny < FILAS - 1 && mapa[ny][nx] == '.') {
				myrphonTileX = nx;
				myrphonTileY = ny;
			}
		}

		void MoverEnemigos() {
			for (int i = 0; i < 3; i++) {
				enemigoTicks[i]++;
				if (enemigoTicks[i] % 9 != 0) continue;
				int nx = enemigoTileX[i] + enemigoDirX[i];
				int ny = enemigoTileY[i] + enemigoDirY[i];
				if (nx > 0 && nx < COLUMNAS - 1 && ny > 0 && ny < FILAS - 1 && mapa[ny][nx] == '.') {
					enemigoTileX[i] = nx;
					enemigoTileY[i] = ny;
				}
				else {
					enemigoDirX[i] = -enemigoDirX[i];
					enemigoDirY[i] = -enemigoDirY[i];
				}
			}
		}

		void Fallar(String^ mensaje) {
			if (terminado) return;
			terminado = true;
			timerJuego->Stop();
			MessageBox::Show(mensaje, "Laberinto Serpentback", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

		void Ganar() {
			if (terminado) return;
			terminado = true;
			timerJuego->Stop();
			MessageBox::Show("¡Atrapaste a Myrphon! El pequeño grifo ronronea asustado pero a salvo.", "¡Rescate exitoso!", MessageBoxButtons::OK, MessageBoxIcon::Information);
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void timerJuego_Tick(Object^ sender, EventArgs^ e) {
			if (terminado) return;
			ticksRestantes--;
			if (ticksRestantes <= 0) {
				Fallar("Se acabó el tiempo. Myrphon volvió a escabullirse entre las sombras.");
				return;
			}
			if (invulnerableTicks > 0) invulnerableTicks--;

			int dx = 0;
			int dy = 0;
			if (izquierda) dx = -1;
			else if (derecha) dx = 1;
			if (arriba) dy = -1;
			else if (abajo) dy = 1;
			if (dx != 0 || dy != 0) MoverJugador(dx, dy);

			MoverMyrphon();
			MoverEnemigos();

			int centroTileX = (jugadorX + 12) / TILE;
			int centroTileY = (jugadorY + 20) / TILE;
			if (centroTileX == myrphonTileX && centroTileY == myrphonTileY) {
				Ganar();
				return;
			}

			Rectangle rectJugador(jugadorX + 4, jugadorY + 14, 16, 18);
			for (int i = 0; i < 3; i++) {
				Rectangle rectEnemigo(enemigoTileX[i] * TILE + 4, enemigoTileY[i] * TILE + 8, 24, 24);
				if (rectJugador.IntersectsWith(rectEnemigo) && invulnerableTicks == 0) {
					vidas--;
					invulnerableTicks = 40;
					ReiniciarJugador();
					if (vidas <= 0) {
						Fallar("Las sombras atraparon a Coco. Myrphon se perdió en el laberinto.");
						return;
					}
					break;
				}
			}

			this->Invalidate();
		}

		void DibujarCorazon(Graphics^ g, int x, int y, Color color) {
			SolidBrush^ pincel = gcnew SolidBrush(color);
			g->FillEllipse(pincel, x, y, 9, 9);
			g->FillEllipse(pincel, x + 7, y, 9, 9);
			g->FillPolygon(pincel, gcnew array<Point>{ Point(x, y + 7), Point(x + 16, y + 7), Point(x + 8, y + 17) });
			delete pincel;
		}

		void DibujarMyrphon(Graphics^ g, int px, int py) {
			int flotacion = (int)(Math::Sin(ticksAnimacion * 0.2) * 2.0);
			py += flotacion;
			SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(65, 240, 140, 210));
			SolidBrush^ cuerpo = gcnew SolidBrush(Color::FromArgb(240, 226, 150, 210));
			SolidBrush^ panza = gcnew SolidBrush(Color::FromArgb(240, 250, 220, 240));
			SolidBrush^ ojos = gcnew SolidBrush(Color::FromArgb(255, 40, 30, 60));
			SolidBrush^ pico = gcnew SolidBrush(Color::FromArgb(255, 242, 196, 80));
			g->FillEllipse(aura, px - 2, py - 2, 36, 36);
			g->FillEllipse(cuerpo, px + 4, py + 10, 24, 20);
			g->FillEllipse(panza, px + 9, py + 17, 14, 11);
			g->FillPolygon(cuerpo, gcnew array<Point>{ Point(px + 6, py + 11), Point(px + 8, py + 1), Point(px + 13, py + 10) });
			g->FillPolygon(cuerpo, gcnew array<Point>{ Point(px + 26, py + 11), Point(px + 24, py + 1), Point(px + 19, py + 10) });
			g->FillEllipse(ojos, px + 10, py + 15, 3, 4);
			g->FillEllipse(ojos, px + 20, py + 15, 3, 4);
			g->FillPolygon(pico, gcnew array<Point>{ Point(px + 16, py + 19), Point(px + 19, py + 21), Point(px + 16, py + 22) });
			delete aura;
			delete cuerpo;
			delete panza;
			delete ojos;
			delete pico;
		}

		void DibujarEnemigo(Graphics^ g, int px, int py) {
			SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(225, 28, 22, 38));
			SolidBrush^ ojos = gcnew SolidBrush(Color::FromArgb(255, 230, 60, 70));
			g->FillEllipse(sombra, px + 2, py + 6, 28, 26);
			g->FillEllipse(sombra, px + 6, py, 20, 18);
			g->FillEllipse(ojos, px + 10, py + 8, 4, 4);
			g->FillEllipse(ojos, px + 18, py + 8, 4, 4);
			delete sombra;
			delete ojos;
		}

	protected:
		virtual bool ProcessDialogKey(Keys keyData) override {
			return false;
		}

		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);
			Graphics^ g = e->Graphics;
			g->SmoothingMode = SmoothingMode::None;
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;

			SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(255, 8, 12, 18));
			g->FillRectangle(fondo, this->ClientRectangle);
			delete fondo;

			SolidBrush^ panel = gcnew SolidBrush(Color::FromArgb(235, 14, 20, 30));
			Pen^ borde = gcnew Pen(Color::FromArgb(220, 200, 158, 78), 1.5f);
			g->FillRectangle(panel, 18, 12, this->ClientSize.Width - 36, 70);
			g->DrawRectangle(borde, 18, 12, this->ClientSize.Width - 36, 70);

			System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
			System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			System::Drawing::Font^ fTiempo = gcnew System::Drawing::Font("Consolas", 16.0f, FontStyle::Bold);
			g->DrawString("L A B E R I N T O   S E R P E N T B A C K", fTitulo, Brushes::Gold, PointF(34, 22));
			g->DrawString("Atrapa a Myrphon antes de que las sombras te alcancen.", fTexto, Brushes::Gainsboro, PointF(34, 50));
			int segundos = ticksRestantes / 40;
			Brush^ colorTiempo = (segundos <= 8) ? Brushes::Tomato : Brushes::LightGoldenrodYellow;
			g->DrawString("TIEMPO  " + segundos.ToString("00") + "s", fTiempo, colorTiempo, PointF((float)(this->ClientSize.Width - 230), 30.0f));
			for (int i = 0; i < 3; i++) {
				Color colorCorazon = (i < vidas) ? Color::FromArgb(255, 214, 56, 76) : Color::FromArgb(255, 58, 48, 54);
				DibujarCorazon(g, 230 + i * 24, 46, colorCorazon);
			}

			for (int f = 0; f < FILAS; f++) {
				for (int c = 0; c < COLUMNAS; c++) {
					Bitmap^ tile = (mapa[f][c] == '#') ? bmpMuro : bmpPiso;
					g->DrawImage(tile, OFFSET_X + c * TILE, OFFSET_Y + f * TILE, TILE, TILE);
				}
			}

			DibujarEnemigo(g, OFFSET_X + enemigoTileX[0] * TILE, OFFSET_Y + enemigoTileY[0] * TILE);
			DibujarEnemigo(g, OFFSET_X + enemigoTileX[1] * TILE, OFFSET_Y + enemigoTileY[1] * TILE);
			DibujarEnemigo(g, OFFSET_X + enemigoTileX[2] * TILE, OFFSET_Y + enemigoTileY[2] * TILE);
			DibujarMyrphon(g, OFFSET_X + myrphonTileX * TILE, OFFSET_Y + myrphonTileY * TILE);

			if (invulnerableTicks == 0 || (invulnerableTicks % 2 == 0)) {
				int frameX = frameCoco * 24;
				int frameY = 4 * 32;
				Rectangle destino(OFFSET_X + jugadorX - 12, OFFSET_Y + jugadorY - 32, 48, 64);
				Rectangle origen(frameX, frameY, 24, 32);
				g->DrawImage(bmpCoco, destino, origen, GraphicsUnit::Pixel);
			}

			System::Drawing::Font^ fAyuda = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold);
			g->DrawString("WASD / Flechas: moverte    |    Evita las sombras    |    ESC: abandonar", fAyuda, Brushes::LightSteelBlue, PointF(26.0f, (float)(this->ClientSize.Height - 34)));

			delete panel;
			delete borde;
			delete fTitulo;
			delete fTexto;
			delete fTiempo;
			delete fAyuda;
		}

	public:
		FormMinijuego() {
			this->Text = "Laberinto Serpentback - Rescate de Myrphon";
			this->ClientSize = System::Drawing::Size(820, 640);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(8, 12, 18);
			this->DoubleBuffered = true;
			this->KeyPreview = true;

			bmpCoco = CargarBitmap("sprites/Coco_Spritesheet.png");
			bmpMuro = CargarBitmap("sprites/tile_muro.png");
			bmpPiso = CargarBitmap("sprites/tile_camino.png");
			aleatorio = gcnew Random();
			enemigoTileX = gcnew array<int>(3);
			enemigoTileY = gcnew array<int>(3);
			enemigoDirX = gcnew array<int>(3);
			enemigoDirY = gcnew array<int>(3);
			enemigoTicks = gcnew array<int>(3);
			arriba = false;
			abajo = false;
			izquierda = false;
			derecha = false;

			ConstruirMapa();
			ReiniciarMinijuego();

			timerJuego = gcnew Timer();
			timerJuego->Interval = 25;
			timerJuego->Tick += gcnew EventHandler(this, &FormMinijuego::timerJuego_Tick);

			this->KeyDown += gcnew KeyEventHandler(this, &FormMinijuego::FormMinijuego_KeyDown);
			this->KeyUp += gcnew KeyEventHandler(this, &FormMinijuego::FormMinijuego_KeyUp);
			this->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &FormMinijuego::FormMinijuego_PreviewKeyDown);
			this->Deactivate += gcnew EventHandler(this, &FormMinijuego::FormMinijuego_Deactivate);

			timerJuego->Start();
		}

		~FormMinijuego() {
			if (timerJuego != nullptr) {
				timerJuego->Stop();
				delete timerJuego;
			}
			if (bmpCoco != nullptr) delete bmpCoco;
			if (bmpMuro != nullptr) delete bmpMuro;
			if (bmpPiso != nullptr) delete bmpPiso;
		}

	private:
		void FormMinijuego_PreviewKeyDown(Object^ sender, PreviewKeyDownEventArgs^ e) {
			if (e->KeyCode == Keys::Up || e->KeyCode == Keys::Down || e->KeyCode == Keys::Left || e->KeyCode == Keys::Right ||
				e->KeyCode == Keys::W || e->KeyCode == Keys::A || e->KeyCode == Keys::S || e->KeyCode == Keys::D ||
				e->KeyCode == Keys::Escape) {
				e->IsInputKey = true;
			}
		}

		void FormMinijuego_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::W || e->KeyCode == Keys::Up) arriba = true;
			else if (e->KeyCode == Keys::S || e->KeyCode == Keys::Down) abajo = true;
			else if (e->KeyCode == Keys::A || e->KeyCode == Keys::Left) izquierda = true;
			else if (e->KeyCode == Keys::D || e->KeyCode == Keys::Right) derecha = true;
			else if (e->KeyCode == Keys::Escape) {
				timerJuego->Stop();
				System::Windows::Forms::DialogResult respuesta = MessageBox::Show("¿Abandonar el minijuego? Myrphon seguirá perdido.", "Laberinto Serpentback", MessageBoxButtons::YesNo, MessageBoxIcon::Question);
				if (respuesta == System::Windows::Forms::DialogResult::Yes) {
					this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
					this->Close();
				}
				else {
					timerJuego->Start();
				}
			}
		}

		void FormMinijuego_KeyUp(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::W || e->KeyCode == Keys::Up) arriba = false;
			else if (e->KeyCode == Keys::S || e->KeyCode == Keys::Down) abajo = false;
			else if (e->KeyCode == Keys::A || e->KeyCode == Keys::Left) izquierda = false;
			else if (e->KeyCode == Keys::D || e->KeyCode == Keys::Right) derecha = false;
		}

		void FormMinijuego_Deactivate(Object^ sender, EventArgs^ e) {
			arriba = false;
			abajo = false;
			izquierda = false;
			derecha = false;
		}
	};
}
