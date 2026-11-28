#pragma once

#include <gcroot.h>
#include "BloqueMapa.h"
#include "GestorAudio.h"
#include "Utilidades.h"

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
		Bitmap^ bmpEnemigo;
		Bitmap^ bmpDisparo;
		Bitmap^ bmpMyrphon;

		int jugadorX;
		int jugadorY;
		int jugadorDir;
		int myrphonTileX;
		int myrphonTileY;
		int myrphonDir;
		int myrphonMovTicks;
		array<int>^ enemigoTileX;
		array<int>^ enemigoTileY;
		array<int>^ enemigoDirX;
		array<int>^ enemigoDirY;
		array<int>^ enemigoTicks;

		static const int MAX_DISPAROS = 12;
		array<int>^ dispX;
		array<int>^ dispY;
		array<int>^ dispDx;
		array<int>^ dispDy;
		array<int>^ dispFila;
		array<bool>^ dispActivo;
		array<bool>^ dispImpacto;
		array<int>^ dispCol;
		array<int>^ dispTick;

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

		String^ BuscarArchivoSprite(String^ nombre) {
			if (System::IO::File::Exists(nombre)) return nombre;
			String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
			array<String^>^ prefijos = gcnew array<String^>{
				"",
				"..\\..\\wha-v2\\",
				"..\\..\\..\\wha-v2\\wha-v2\\",
				"..\\..\\..\\..\\wha-v2\\wha-v2\\"
			};
			for each (String^ pref in prefijos) {
				String^ ruta = System::IO::Path::Combine(baseDir, pref + nombre);
				if (System::IO::File::Exists(ruta)) return ruta;
			}
			return "";
		}

		Bitmap^ CargarBitmap(String^ nombre) {
			String^ ruta = BuscarArchivoSprite(nombre);
			if (ruta != "") return gcnew Bitmap(ruta);
			return nullptr;
		}

		void ConstruirMapa() {
			array<String^>^ filasFijas = gcnew array<String^>{
				"########################",
				"#......#........#......#",
				"#.####.#.######.#.####.#",
				"#.#..#...#....#...#..#.#",
				"#.#..#####.##.#####..#.#",
				"#..........##..........#",
				"###.####.#....#.####.###",
				"#...#....######....#...#",
				"#.###.##........##.###.#",
				"#.....#..######..#.....#",
				"#.#####..#....#..#####.#",
				"#........#.##.#........#",
				"#.######...##...######.#",
				"#......................#",
				"########################"
			};
			mapa = gcnew List<String^>();
			for (int f = 0; f < FILAS; f++) {
				mapa->Add(filasFijas[f]);
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
			jugadorX = 1 * TILE + 4;
			jugadorY = 1 * TILE;
			jugadorDir = 0;
		}

		void ReiniciarMinijuego() {
			vidas = 3;
			ticksRestantes = 45 * 40;
			ticksAnimacion = 0;
			invulnerableTicks = 0;
			ticksMyrphon = 0;
			frameCoco = 0;
			terminado = false;
			ReiniciarJugador();

			myrphonTileX = 21;
			myrphonTileY = 13;
			myrphonDir = 0;
			myrphonMovTicks = 0;

			enemigoTileX[0] = 19; enemigoTileY[0] = 5; enemigoDirX[0] = -1; enemigoDirY[0] = 0; enemigoTicks[0] = 0;
			enemigoTileX[1] = 8; enemigoTileY[1] = 8; enemigoDirX[1] = 1; enemigoDirY[1] = 0; enemigoTicks[1] = 15;
			enemigoTileX[2] = 12; enemigoTileY[2] = 13; enemigoDirX[2] = 1; enemigoDirY[2] = 0; enemigoTicks[2] = 8;

			for (int i = 0; i < MAX_DISPAROS; i++) {
				dispActivo[i] = false;
				dispImpacto[i] = false;
				dispCol[i] = 0;
				dispTick[i] = 0;
			}
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
			if (dx < 0) jugadorDir = 1;
			else if (dx > 0) jugadorDir = 3;
			else if (dy < 0) jugadorDir = 2;
			else if (dy > 0) jugadorDir = 0;

			ticksAnimacion++;
			if (ticksAnimacion % 2 == 0) {
				frameCoco++;
				if (frameCoco >= 10) frameCoco = 0;
			}
		}

		void MoverMyrphon() {
			ticksMyrphon++;
			if (ticksMyrphon % 9 != 0) return;
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
				if (dx < 0) myrphonDir = 1;
				else if (dx > 0) myrphonDir = 3;
				else if (dy < 0) myrphonDir = 2;
				else if (dy > 0) myrphonDir = 0;
				myrphonMovTicks = 9;
			}
		}

		void LanzarDisparoEnemigo(int idx) {
			for (int p = 0; p < MAX_DISPAROS; p++) {
				if (dispActivo[p]) continue;
				int vel = 5;
				dispX[p] = enemigoTileX[idx] * TILE + 4;
				dispY[p] = enemigoTileY[idx] * TILE + 4;
				dispDx[p] = enemigoDirX[idx] * vel;
				dispDy[p] = enemigoDirY[idx] * vel;
				if (enemigoDirY[idx] < 0) dispFila[p] = 0;
				else if (enemigoDirY[idx] > 0) dispFila[p] = 1;
				else if (enemigoDirX[idx] < 0) dispFila[p] = 2;
				else dispFila[p] = 3;
				dispActivo[p] = true;
				dispImpacto[p] = false;
				dispCol[p] = 0;
				dispTick[p] = 0;
				break;
			}
		}

		void MoverEnemigos() {
			for (int i = 0; i < 3; i++) {
				enemigoTicks[i]++;
				if (enemigoTicks[i] % 48 == 24) {
					LanzarDisparoEnemigo(i);
				}
				if (enemigoTicks[i] % 8 != 0) continue;
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

		bool AplicarDanioAJugador(String^ mensajeDerrota) {
			GestorAudio::ReproducirSfxAccion();
			vidas--;
			invulnerableTicks = 45;
			ReiniciarJugador();
			if (vidas <= 0) {
				Fallar(mensajeDerrota);
				return true;
			}
			return false;
		}

		bool ActualizarDisparos(Rectangle rectJugador) {
			for (int p = 0; p < MAX_DISPAROS; p++) {
				if (!dispActivo[p]) continue;
				if (dispImpacto[p]) {
					dispTick[p]++;
					if (dispTick[p] % 3 == 0) {
						dispCol[p]++;
						if (dispCol[p] > 3) dispActivo[p] = false;
					}
					continue;
				}
				dispX[p] += dispDx[p];
				dispY[p] += dispDy[p];
				if (TileEn(dispX[p] + 12, dispY[p] + 12) == '#') {
					dispImpacto[p] = true;
					dispDx[p] = 0;
					dispDy[p] = 0;
					dispCol[p] = 1;
					dispTick[p] = 0;
					continue;
				}
				Rectangle rectDisp(dispX[p] + 5, dispY[p] + 5, 14, 14);
				if (rectJugador.IntersectsWith(rectDisp)) {
					dispImpacto[p] = true;
					dispDx[p] = 0;
					dispDy[p] = 0;
					dispCol[p] = 1;
					dispTick[p] = 0;
					if (invulnerableTicks == 0) {
						if (AplicarDanioAJugador("Un disparo mágico te alcanzó. Myrphon se perdió en el laberinto.")) return true;
					}
				}
			}
			return false;
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
			if (myrphonMovTicks > 0) myrphonMovTicks--;

			int dx = 0;
			int dy = 0;
			if (izquierda) dx = -1;
			else if (derecha) dx = 1;
			if (arriba) dy = -1;
			else if (abajo) dy = 1;
			if (dx != 0 || dy != 0) {
				MoverJugador(dx, dy);
				if (ticksRestantes % 9 == 0) GestorAudio::ReproducirSfxPaso();
			}

			MoverMyrphon();
			MoverEnemigos();

			Rectangle rectJugador(jugadorX + 3, jugadorY + 10, 18, 20);
			Rectangle rectMyrphon(myrphonTileX * TILE + 2, myrphonTileY * TILE + 2, TILE - 4, TILE - 4);
			if (rectJugador.IntersectsWith(rectMyrphon)) {
				GestorAudio::ReproducirSfxAccion();
				Ganar();
				return;
			}

			for (int i = 0; i < 3; i++) {
				Rectangle rectEnemigo(enemigoTileX[i] * TILE + 5, enemigoTileY[i] * TILE + 6, 22, 22);
				if (rectJugador.IntersectsWith(rectEnemigo) && invulnerableTicks == 0) {
					if (AplicarDanioAJugador("Las sombras atraparon a Coco. Myrphon se perdió en el laberinto.")) return;
					break;
				}
			}

			if (ActualizarDisparos(rectJugador)) return;

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
			if (bmpMyrphon != nullptr) {
				DibujarMyrphonSpritesheetComun(g, bmpMyrphon, px, py, myrphonDir, myrphonMovTicks > 0, ticksMyrphon, 0, 0, 1, TILE, TILE);
			}
		}

		void DibujarEnemigo(Graphics^ g, int idx, int px, int py) {
			if (bmpEnemigo != nullptr && bmpEnemigo->Width >= 64 && bmpEnemigo->Height >= 96) {
				int cellW = bmpEnemigo->Width / 4;
				int cellH = bmpEnemigo->Height / 4;
				int frame = (enemigoTicks[idx] / 6) % 4;
				int fila = 0;
				if (enemigoDirY[idx] > 0) fila = 0;
				else if (enemigoDirY[idx] < 0) fila = 1;
				else if (enemigoDirX[idx] < 0) fila = 2;
				else if (enemigoDirX[idx] > 0) fila = 3;
				g->DrawImage(bmpEnemigo, Rectangle(px + 2, py - 8, 28, 40), frame * cellW, fila * cellH, cellW, cellH, GraphicsUnit::Pixel);
				return;
			}
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

		virtual void OnKeyPress(KeyPressEventArgs^ e) override {
			e->Handled = true;
		}

		virtual void WndProc(Message% m) override {
			if (m.Msg == 0x0120) {
				m.Result = IntPtr(1 << 16);
				return;
			}
			Form::WndProc(m);
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
				DibujarCorazon(g, 385 + i * 24, 46, colorCorazon);
			}

			for (int f = 0; f < FILAS; f++) {
				for (int c = 0; c < COLUMNAS; c++) {
					Bitmap^ tile = (mapa[f][c] == '#') ? bmpMuro : bmpPiso;
					if (tile != nullptr) {
						g->DrawImage(tile, OFFSET_X + c * TILE, OFFSET_Y + f * TILE, TILE, TILE);
					}
					else {
						SolidBrush^ b = gcnew SolidBrush((mapa[f][c] == '#') ? Color::FromArgb(45, 52, 66) : Color::FromArgb(22, 26, 34));
						g->FillRectangle(b, OFFSET_X + c * TILE, OFFSET_Y + f * TILE, TILE, TILE);
						delete b;
					}
				}
			}

			for (int i = 0; i < 3; i++) {
				DibujarEnemigo(g, i, OFFSET_X + enemigoTileX[i] * TILE, OFFSET_Y + enemigoTileY[i] * TILE);
			}
			if (bmpDisparo != nullptr && bmpDisparo->Width >= 4 && bmpDisparo->Height >= 4) {
				int dCellW = bmpDisparo->Width / 4;
				int dCellH = bmpDisparo->Height / 4;
				for (int p = 0; p < MAX_DISPAROS; p++) {
					if (!dispActivo[p]) continue;
					int col = dispImpacto[p] ? dispCol[p] : 0;
					if (col < 0) col = 0;
					if (col > 3) col = 3;
					g->DrawImage(bmpDisparo, Rectangle(OFFSET_X + dispX[p], OFFSET_Y + dispY[p], 24, 24),
						col * dCellW, dispFila[p] * dCellH, dCellW, dCellH, GraphicsUnit::Pixel);
				}
			}
			DibujarMyrphon(g, OFFSET_X + myrphonTileX * TILE, OFFSET_Y + myrphonTileY * TILE);

			if ((invulnerableTicks == 0 || (invulnerableTicks % 2 == 0)) && bmpCoco != nullptr) {
				int frameX = frameCoco * 24;
				int frameY = (4 + jugadorDir) * 32;
				Rectangle destino(OFFSET_X + jugadorX - 8, OFFSET_Y + jugadorY - 18, 40, 52);
				Rectangle origen(frameX, frameY, 24, 32);
				g->DrawImage(bmpCoco, destino, origen, GraphicsUnit::Pixel);
			}

			System::Drawing::Font^ fAyuda = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold);
			g->DrawString("WASD / Flechas: moverte    |    Esquiva magos y disparos    |    ESC: abandonar", fAyuda, Brushes::LightSteelBlue, PointF(26.0f, (float)(this->ClientSize.Height - 34)));

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
			bmpEnemigo = CargarBitmap("sprites/Mago_Cueva_Spritesheet.png");
			bmpDisparo = CargarBitmap("sprites/Disparo_Cueva_Spritesheet.png");
			bmpMyrphon = CargarBitmap("sprites/Myrphon_Spritesheet.png");

			aleatorio = gcnew Random();
			enemigoTileX = gcnew array<int>(3);
			enemigoTileY = gcnew array<int>(3);
			enemigoDirX = gcnew array<int>(3);
			enemigoDirY = gcnew array<int>(3);
			enemigoTicks = gcnew array<int>(3);
			dispX = gcnew array<int>(MAX_DISPAROS);
			dispY = gcnew array<int>(MAX_DISPAROS);
			dispDx = gcnew array<int>(MAX_DISPAROS);
			dispDy = gcnew array<int>(MAX_DISPAROS);
			dispFila = gcnew array<int>(MAX_DISPAROS);
			dispActivo = gcnew array<bool>(MAX_DISPAROS);
			dispImpacto = gcnew array<bool>(MAX_DISPAROS);
			dispCol = gcnew array<int>(MAX_DISPAROS);
			dispTick = gcnew array<int>(MAX_DISPAROS);
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
			if (bmpEnemigo != nullptr) delete bmpEnemigo;
			if (bmpDisparo != nullptr) delete bmpDisparo;
			if (bmpMyrphon != nullptr) delete bmpMyrphon;
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
			e->Handled = true;
			e->SuppressKeyPress = true;
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
