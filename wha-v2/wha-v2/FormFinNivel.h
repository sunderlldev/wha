#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;

	public ref class FormFinNivel : public Form {
	private:
		int accion;
		int tiempo;
		int objetos;
		bool myrphonRescatado;
		Button^ btnSiguiente;
		Button^ btnRejugar;
		Button^ btnMenu;
		Panel^ panel;

		void btnSiguiente_Click(Object^ sender, EventArgs^ e) {
			MessageBox::Show("El Nivel 2 estará disponible en la próxima entrega.\n\nPor ahora, el Atelier de Qifrey y el bosque de Serpentback quedan a tu disposición para explorar sus secretos.", "Continuará...", MessageBoxButtons::OK, MessageBoxIcon::Information);
			accion = 0;
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
			g->SmoothingMode = SmoothingMode::AntiAlias;

			LinearGradientBrush^ fondo = gcnew LinearGradientBrush(this->ClientRectangle, Color::FromArgb(9, 14, 22), Color::FromArgb(20, 35, 28), LinearGradientMode::Vertical);
			g->FillRectangle(fondo, this->ClientRectangle);
			delete fondo;

			SolidBrush^ halo = gcnew SolidBrush(Color::FromArgb(28, 220, 180, 90));
			g->FillEllipse(halo, this->ClientSize.Width / 2 - 110, 18, 220, 220);
			delete halo;

			Pen^ rayo = gcnew Pen(Color::FromArgb(180, 226, 184, 82), 2.0f);
			for (int i = 0; i < 12; i++) {
				double angulo = i * Math::PI / 6.0;
				int x1 = this->ClientSize.Width / 2 + (int)(Math::Cos(angulo) * 74);
				int y1 = 128 + (int)(Math::Sin(angulo) * 74);
				int x2 = this->ClientSize.Width / 2 + (int)(Math::Cos(angulo) * 96);
				int y2 = 128 + (int)(Math::Sin(angulo) * 96);
				g->DrawLine(rayo, x1, y1, x2, y2);
			}
			delete rayo;

			SolidBrush^ oro = gcnew SolidBrush(Color::FromArgb(235, 200, 158, 78));
			int centro = this->ClientSize.Width / 2;
			g->FillPolygon(oro, gcnew array<Point>{
				Point(centro, 82),
				Point(centro + 14, 116),
				Point(centro + 50, 116),
				Point(centro + 21, 137),
				Point(centro + 32, 172),
				Point(centro, 150),
				Point(centro - 32, 172),
				Point(centro - 21, 137),
				Point(centro - 50, 116),
				Point(centro - 14, 116)
			});
			delete oro;
		}

	public:
		FormFinNivel(int tiempoSegundos, int objetosRecogidos, bool rescate) {
			tiempo = tiempoSegundos;
			objetos = objetosRecogidos;
			myrphonRescatado = rescate;
			accion = 0;

			this->Text = "Nivel 1 completado - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(720, 600);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 22);
			this->DoubleBuffered = true;

			panel = gcnew Panel();
			panel->Location = Point(40, 210);
			panel->Size = System::Drawing::Size(640, 330);
			panel->BackColor = Color::FromArgb(225, 12, 19, 28);
			panel->BorderStyle = BorderStyle::None;
			this->Controls->Add(panel);

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "N I V E L   1   C O M P L E T A D O";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 17.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 200, 158, 78);
			lblTitulo->Location = Point(20, 18);
			lblTitulo->Size = System::Drawing::Size(600, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			panel->Controls->Add(lblTitulo);

			Label^ lblSub = gcnew Label();
			lblSub->Text = "El Atelier de Qifrey · La misión de Richeh";
			lblSub->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Italic);
			lblSub->ForeColor = Color::FromArgb(140, 200, 190);
			lblSub->Location = Point(20, 50);
			lblSub->Size = System::Drawing::Size(600, 24);
			lblSub->TextAlign = ContentAlignment::MiddleCenter;
			panel->Controls->Add(lblSub);

			Label^ lblStats = gcnew Label();
			lblStats->Text =
				"Tiempo de aventura:  " + tiempo.ToString() + " s\n"
				"Objetos encontrados:  " + objetos.ToString() + " / 8\n"
				"Myrphon:  " + (myrphonRescatado ? "rescatado" : "perdido") + "\n"
				"Capa Mágica:  confeccionada";
			lblStats->Font = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Regular);
			lblStats->ForeColor = Color::FromArgb(226, 234, 244);
			lblStats->Location = Point(150, 92);
			lblStats->Size = System::Drawing::Size(340, 110);
			panel->Controls->Add(lblStats);

			Label^ lblCita = gcnew Label();
			lblCita->Text = "\"La verdadera magia no reside en el linaje,\nsino en el trazo del corazón.\"";
			lblCita->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Italic);
			lblCita->ForeColor = Color::FromArgb(160, 210, 170);
			lblCita->Location = Point(60, 206);
			lblCita->Size = System::Drawing::Size(520, 44);
			lblCita->TextAlign = ContentAlignment::MiddleCenter;
			panel->Controls->Add(lblCita);

			btnSiguiente = gcnew Button();
			btnSiguiente->Text = "SIGUIENTE NIVEL";
			btnSiguiente->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			btnSiguiente->ForeColor = Color::FromArgb(214, 198, 166);
			btnSiguiente->BackColor = Color::FromArgb(18, 27, 38);
			btnSiguiente->FlatStyle = FlatStyle::Flat;
			btnSiguiente->FlatAppearance->BorderSize = 0;
			btnSiguiente->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnSiguiente->Location = Point(60, 264);
			btnSiguiente->Size = System::Drawing::Size(180, 42);
			btnSiguiente->Click += gcnew EventHandler(this, &FormFinNivel::btnSiguiente_Click);
			panel->Controls->Add(btnSiguiente);

			btnRejugar = gcnew Button();
			btnRejugar->Text = "REJUGAR NIVEL";
			btnRejugar->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			btnRejugar->ForeColor = Color::FromArgb(226, 232, 240);
			btnRejugar->BackColor = Color::FromArgb(18, 27, 38);
			btnRejugar->FlatStyle = FlatStyle::Flat;
			btnRejugar->FlatAppearance->BorderSize = 0;
			btnRejugar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnRejugar->Location = Point(250, 264);
			btnRejugar->Size = System::Drawing::Size(180, 42);
			btnRejugar->Click += gcnew EventHandler(this, &FormFinNivel::btnRejugar_Click);
			panel->Controls->Add(btnRejugar);

			btnMenu = gcnew Button();
			btnMenu->Text = "VOLVER AL MENÚ";
			btnMenu->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
			btnMenu->ForeColor = Color::FromArgb(220, 228, 238);
			btnMenu->BackColor = Color::FromArgb(14, 22, 30);
			btnMenu->FlatStyle = FlatStyle::Flat;
			btnMenu->FlatAppearance->BorderSize = 0;
			btnMenu->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 34, 46);
			btnMenu->Location = Point(440, 264);
			btnMenu->Size = System::Drawing::Size(140, 42);
			btnMenu->Click += gcnew EventHandler(this, &FormFinNivel::btnMenu_Click);
			panel->Controls->Add(btnMenu);
		}

		int getAccion() { return accion; }

		~FormFinNivel() {}
	};
}
