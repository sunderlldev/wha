#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormSelectorNivel : public Form {
	private:
		int nivelElegido;
		Label^ lblTitulo;
		Button^ btnNivel1;
		Button^ btnNivel2;
		Button^ btnNivel3;
		Button^ btnCancelar;
		Panel^ pnlContenedor;

		Button^ CrearBotonNivel(String^ texto, int y, bool completado, bool inicial) {
			Button^ boton = gcnew Button();
			boton->Text = completado ? (texto + "   [COMPLETADO]") : texto;
			boton->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			if (completado) {
				boton->ForeColor = Color::FromArgb(212, 248, 220);
				boton->BackColor = Color::FromArgb(24, 62, 42);
				boton->FlatStyle = FlatStyle::Flat;
				boton->FlatAppearance->BorderSize = 1;
				boton->FlatAppearance->BorderColor = Color::FromArgb(88, 174, 118);
				boton->FlatAppearance->MouseOverBackColor = Color::FromArgb(32, 78, 52);
			}
			else {
				boton->ForeColor = inicial ? Color::FromArgb(214, 198, 166) : Color::FromArgb(230, 236, 244);
				boton->BackColor = Color::FromArgb(18, 27, 38);
				boton->FlatStyle = FlatStyle::Flat;
				boton->FlatAppearance->BorderSize = 0;
				boton->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			}
			boton->Cursor = Cursors::Hand;
			boton->Location = Point(36, y);
			boton->Size = System::Drawing::Size(448, 44);
			pnlContenedor->Controls->Add(boton);
			return boton;
		}

		void btnNivel1_Click(Object^ sender, EventArgs^ e) {
			nivelElegido = 1;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnNivel2_Click(Object^ sender, EventArgs^ e) {
			nivelElegido = 2;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnNivel3_Click(Object^ sender, EventArgs^ e) {
			nivelElegido = 3;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnCancelar_Click(Object^ sender, EventArgs^ e) {
			nivelElegido = 0;
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

	private:
		void InicializarSelector(bool c1, bool c2, bool c3) {
			nivelElegido = 1;

			this->Text = "Seleccionar Nivel - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(550, 340);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(520, 310);
			pnlContenedor->BackColor = Color::FromArgb(10, 16, 24);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "SELECCIONAR NIVEL";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 16.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(214, 198, 166);
			lblTitulo->Location = Point(36, 18);
			lblTitulo->Size = System::Drawing::Size(448, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleLeft;
			pnlContenedor->Controls->Add(lblTitulo);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(36, 56);
			linea->Size = System::Drawing::Size(448, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			pnlContenedor->Controls->Add(linea);

			btnNivel1 = CrearBotonNivel("Nivel 1 - Coco: El Atelier de Qifrey", 76, c1, true);
			btnNivel1->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel1_Click);

			btnNivel2 = CrearBotonNivel("Nivel 2 - Tartah: El Laberinto de Kaln", 132, c2, false);
			btnNivel2->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel2_Click);

			btnNivel3 = CrearBotonNivel("Nivel 3 - Coustas: El Árbol de Plata", 188, c3, false);
			btnNivel3->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel3_Click);

			btnCancelar = gcnew Button();
			btnCancelar->Text = "Volver";
			btnCancelar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			btnCancelar->ForeColor = Color::FromArgb(210, 218, 228);
			btnCancelar->BackColor = Color::FromArgb(14, 22, 30);
			btnCancelar->FlatStyle = FlatStyle::Flat;
			btnCancelar->FlatAppearance->BorderSize = 0;
			btnCancelar->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 34, 46);
			btnCancelar->Cursor = Cursors::Hand;
			btnCancelar->Location = Point(190, 252);
			btnCancelar->Size = System::Drawing::Size(140, 36);
			btnCancelar->Click += gcnew EventHandler(this, &FormSelectorNivel::btnCancelar_Click);
			pnlContenedor->Controls->Add(btnCancelar);

			this->ActiveControl = btnNivel1;
		}

	public:
		FormSelectorNivel() {
			InicializarSelector(false, false, false);
		}

		FormSelectorNivel(bool c1, bool c2, bool c3) {
			InicializarSelector(c1, c2, c3);
		}

		int getNivelElegido() {
			return nivelElegido;
		}

		~FormSelectorNivel() {}
	};
}
