#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Drawing::Imaging;
	using namespace System::Windows::Forms;

	public ref class FormFinNivel : public Form {
	private:
		int accion;
		int nivel;
		int tiempo;
		int objetos;
		bool detalleExtra;
		int puntajeNivel;
		int cuartosVisitados;
		int totalCuartos;
		Bitmap^ bmpEstrellaPixel;
		Button^ btnSiguiente;
		Button^ btnRejugar;
		Button^ btnMenu;

		static Bitmap^ CrearEstrellaPixel() {
			static const char* patron[16] = {
				".......OO.......",
				"......OYYO......",
				"......OYYO......",
				".....OYYYYO.....",
				".OOOOYLLLLYYOOO.",
				"OYYYYLLLLLLYYYYO",
				".OYYLLLLLLLLYYO.",
				"..OYYLLLLLLYYO..",
				"...OYYYYYYYYO...",
				"...OYYGGGGYYO...",
				"..OYYGG..GGYYO..",
				"..OYG......GYO..",
				".OYG........GYO.",
				".OO..........OO.",
				"................",
				"................"
			};
			Bitmap^ bmp = gcnew Bitmap(16, 16, PixelFormat::Format32bppArgb);
			for (int y = 0; y < 16; y++) {
				for (int x = 0; x < 16; x++) {
					char c = patron[y][x];
					Color col = Color::Transparent;
					if (c == 'O') col = Color::FromArgb(255, 74, 52, 24);
					else if (c == 'G') col = Color::FromArgb(255, 168, 126, 48);
					else if (c == 'Y') col = Color::FromArgb(255, 224, 182, 86);
					else if (c == 'L') col = Color::FromArgb(255, 250, 228, 146);
					bmp->SetPixel(x, y, col);
				}
			}
			return bmp;
		}

		void btnSiguiente_Click(Object^ sender, EventArgs^ e) {
			accion = 3;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnRejugar_Click(Object^ sender, EventArgs^ e) {
			accion = 1;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnMenu_Click(Object^ sender, EventArgs^ e) {
			accion = 2;
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);
			Graphics^ g = e->Graphics;
			g->SmoothingMode = SmoothingMode::None;
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;

			SolidBrush^ bg = gcnew SolidBrush(Color::FromArgb(255, 12, 17, 25));
			g->FillRectangle(bg, this->ClientRectangle);
			delete bg;

			SolidBrush^ caja = gcnew SolidBrush(Color::FromArgb(255, 18, 25, 36));
			g->FillRectangle(caja, 20, 20, 440, 270);
			delete caja;

			Pen^ bordeExt = gcnew Pen(Color::FromArgb(255, 214, 198, 166), 2.0f);
			Pen^ bordeInt = gcnew Pen(Color::FromArgb(255, 58, 72, 92), 2.0f);
			g->DrawRectangle(bordeExt, 20, 20, 440, 270);
			g->DrawRectangle(bordeInt, 26, 26, 428, 258);
			delete bordeExt;
			delete bordeInt;

			SolidBrush^ pxEsquina = gcnew SolidBrush(Color::FromArgb(255, 214, 198, 166));
			g->FillRectangle(pxEsquina, 18, 18, 6, 6);
			g->FillRectangle(pxEsquina, 456, 18, 6, 6);
			g->FillRectangle(pxEsquina, 18, 286, 6, 6);
			g->FillRectangle(pxEsquina, 456, 286, 6, 6);
			delete pxEsquina;

			if (bmpEstrellaPixel != nullptr) {
				g->DrawImage(bmpEstrellaPixel, Rectangle(216, 40, 48, 48), 0, 0, 16, 16, GraphicsUnit::Pixel);
			}
		}

		void InicializarModal(int tiempoSegundos, int objetosRecogidos, bool extra, int numNivel, int punt, int cVis, int cTot) {
			tiempo = tiempoSegundos;
			objetos = objetosRecogidos;
			detalleExtra = extra;
			nivel = numNivel;
			puntajeNivel = punt;
			cuartosVisitados = cVis;
			totalCuartos = cTot;
			accion = 0;
			bmpEstrellaPixel = CrearEstrellaPixel();

			this->Text = "Nivel " + nivel.ToString() + " completado";
			this->ClientSize = System::Drawing::Size(480, 310);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(12, 17, 25);
			this->DoubleBuffered = true;

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "NIVEL " + nivel.ToString() + " COMPLETADO";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 14.5f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(236, 224, 196);
			lblTitulo->BackColor = Color::FromArgb(18, 25, 36);
			lblTitulo->Location = Point(40, 102);
			lblTitulo->Size = System::Drawing::Size(400, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			this->Controls->Add(lblTitulo);

			Label^ lblSub = gcnew Label();
			if (nivel == 2) lblSub->Text = "Tartah · Laberinto de Kaln";
			else if (nivel == 3) lblSub->Text = detalleExtra ? "Coustas · Pacto de la Semilla Prohibida" : "Coustas · Voluntad del Árbol de Plata";
			else lblSub->Text = "Coco · Atelier de Qifrey";
			lblSub->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			lblSub->ForeColor = Color::FromArgb(170, 184, 202);
			lblSub->BackColor = Color::FromArgb(18, 25, 36);
			lblSub->Location = Point(40, 134);
			lblSub->Size = System::Drawing::Size(400, 22);
			lblSub->TextAlign = ContentAlignment::MiddleCenter;
			this->Controls->Add(lblSub);

			Label^ lblStats = gcnew Label();
			if (nivel == 2 && puntajeNivel >= 0) {
				lblStats->Text = "Tiempo: " + tiempo.ToString() + " s  ·  Cuartos: " + cuartosVisitados.ToString() + "/" + totalCuartos.ToString() + "  ·  Puntaje: " + puntajeNivel.ToString();
			}
			else {
				lblStats->Text = "Tiempo: " + tiempo.ToString() + " s   ·   Objetos: " + objetos.ToString();
			}
			lblStats->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
			lblStats->ForeColor = Color::FromArgb(228, 234, 242);
			lblStats->BackColor = Color::FromArgb(18, 25, 36);
			lblStats->Location = Point(40, 168);
			lblStats->Size = System::Drawing::Size(400, 26);
			lblStats->TextAlign = ContentAlignment::MiddleCenter;
			this->Controls->Add(lblStats);

			btnSiguiente = gcnew Button();
			btnSiguiente->Text = (nivel >= 3) ? "FINALIZAR" : "SIGUIENTE";
			btnSiguiente->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
			btnSiguiente->ForeColor = Color::FromArgb(236, 224, 196);
			btnSiguiente->BackColor = Color::FromArgb(26, 36, 52);
			btnSiguiente->FlatStyle = FlatStyle::Flat;
			btnSiguiente->FlatAppearance->BorderSize = 1;
			btnSiguiente->FlatAppearance->BorderColor = Color::FromArgb(140, 128, 104);
			btnSiguiente->FlatAppearance->MouseOverBackColor = Color::FromArgb(36, 50, 70);
			btnSiguiente->Cursor = Cursors::Hand;
			btnSiguiente->Location = Point(48, 222);
			btnSiguiente->Size = System::Drawing::Size(124, 38);
			btnSiguiente->Click += gcnew EventHandler(this, &FormFinNivel::btnSiguiente_Click);
			this->Controls->Add(btnSiguiente);

			btnRejugar = gcnew Button();
			btnRejugar->Text = "REJUGAR";
			btnRejugar->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
			btnRejugar->ForeColor = Color::FromArgb(228, 234, 242);
			btnRejugar->BackColor = Color::FromArgb(24, 33, 46);
			btnRejugar->FlatStyle = FlatStyle::Flat;
			btnRejugar->FlatAppearance->BorderSize = 1;
			btnRejugar->FlatAppearance->BorderColor = Color::FromArgb(64, 78, 98);
			btnRejugar->FlatAppearance->MouseOverBackColor = Color::FromArgb(34, 46, 64);
			btnRejugar->Cursor = Cursors::Hand;
			btnRejugar->Location = Point(182, 222);
			btnRejugar->Size = System::Drawing::Size(116, 38);
			btnRejugar->Click += gcnew EventHandler(this, &FormFinNivel::btnRejugar_Click);
			this->Controls->Add(btnRejugar);

			btnMenu = gcnew Button();
			btnMenu->Text = "MENÚ";
			btnMenu->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			btnMenu->ForeColor = Color::FromArgb(204, 214, 226);
			btnMenu->BackColor = Color::FromArgb(20, 28, 40);
			btnMenu->FlatStyle = FlatStyle::Flat;
			btnMenu->FlatAppearance->BorderSize = 1;
			btnMenu->FlatAppearance->BorderColor = Color::FromArgb(54, 66, 84);
			btnMenu->FlatAppearance->MouseOverBackColor = Color::FromArgb(30, 42, 58);
			btnMenu->Cursor = Cursors::Hand;
			btnMenu->Location = Point(308, 222);
			btnMenu->Size = System::Drawing::Size(124, 38);
			btnMenu->Click += gcnew EventHandler(this, &FormFinNivel::btnMenu_Click);
			this->Controls->Add(btnMenu);
		}

	public:
		FormFinNivel(int tiempoSegundos, int objetosRecogidos, bool extra, int numNivel) {
			InicializarModal(tiempoSegundos, objetosRecogidos, extra, numNivel, -1, 0, 9);
		}

		FormFinNivel(int tiempoSegundos, int objetosRecogidos, bool extra, int numNivel, int punt, int cVis, int cTot) {
			InicializarModal(tiempoSegundos, objetosRecogidos, extra, numNivel, punt, cVis, cTot);
		}

		int getAccion() { return accion; }

		~FormFinNivel() {
			if (bmpEstrellaPixel != nullptr) delete bmpEstrellaPixel;
		}
	};
}
