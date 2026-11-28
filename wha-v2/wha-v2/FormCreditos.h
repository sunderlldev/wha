#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormCreditos : public Form {
	private:
		Button^ btnCerrar;
		Panel^ pnlContenedor;

		Label^ CrearLabel(String^ texto, int x, int y, int ancho, int alto, float tam, FontStyle estilo, Color color) {
			Label^ lbl = gcnew Label();
			lbl->Text = texto;
			lbl->Font = gcnew System::Drawing::Font("Segoe UI", tam, estilo);
			lbl->ForeColor = color;
			lbl->BackColor = Color::Transparent;
			lbl->Location = Point(x, y);
			lbl->Size = System::Drawing::Size(ancho, alto);
			lbl->TextAlign = ContentAlignment::MiddleLeft;
			pnlContenedor->Controls->Add(lbl);
			return lbl;
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormCreditos() {
			this->Text = "Créditos - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(580, 420);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(550, 390);
			pnlContenedor->BackColor = Color::FromArgb(10, 16, 24);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			CrearLabel("CRÉDITOS", 30, 18, 490, 34, 17.0f, FontStyle::Bold, Color::FromArgb(214, 198, 166));

			Panel^ linea = gcnew Panel();
			linea->Location = Point(30, 58);
			linea->Size = System::Drawing::Size(490, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			pnlContenedor->Controls->Add(linea);

			Color colorSeccion = Color::FromArgb(214, 198, 166);
			Color colorTexto = Color::FromArgb(242, 245, 248);

			CrearLabel("CREADORES:", 30, 76, 490, 24, 10.5f, FontStyle::Bold, colorSeccion);
			CrearLabel("•  Juan José Blas Cáceres (U20261B980)", 44, 102, 476, 24, 10.5f, FontStyle::Regular, colorTexto);
			CrearLabel("•  Kiana Valentina Arancibia Romero (U202619105)", 44, 128, 476, 24, 10.5f, FontStyle::Regular, colorTexto);

			CrearLabel("OBRA ORIGINAL:", 30, 170, 490, 24, 10.5f, FontStyle::Bold, colorSeccion);
			CrearLabel("•  'Witch Hat Atelier' (Tongari Bōshi no Atorie)", 44, 196, 476, 24, 10.5f, FontStyle::Regular, colorTexto);

			CrearLabel("PROFESOR DE CURSO:", 30, 238, 490, 24, 10.5f, FontStyle::Bold, colorSeccion);
			CrearLabel("•  Luis Alberto Raymundo Chacaltana", 44, 264, 476, 24, 10.5f, FontStyle::Regular, colorTexto);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(230, 238, 248);
			btnCerrar->BackColor = Color::FromArgb(18, 27, 38);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCerrar->Cursor = Cursors::Hand;
			btnCerrar->Location = Point(205, 326);
			btnCerrar->Size = System::Drawing::Size(140, 38);
			btnCerrar->Click += gcnew EventHandler(this, &FormCreditos::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);
			this->ActiveControl = btnCerrar;
		}

		~FormCreditos() {}
	};
}
