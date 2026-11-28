#pragma once

#include "Utilidades.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;

	public ref class FormPersonajes : public Form {
	private:
		ListBox^ lstPersonajes;
		Panel^ pnlRetrato;
		Label^ lblNombre;
		Label^ lblRol;
		Label^ lblDetalle;
		Label^ lblConfianza;
		Button^ btnCerrar;
		array<String^>^ nombres;
		array<String^>^ roles;
		array<String^>^ descripciones;
		array<String^>^ confianzas;
		array<String^>^ spriteClaves;
		Bitmap^ bmpRetratoActual;

		void pnlRetrato_Paint(Object^ sender, PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			int w = pnlRetrato->Width;
			int h = pnlRetrato->Height;
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;
			g->SmoothingMode = SmoothingMode::None;

			SolidBrush^ bFondo = gcnew SolidBrush(Color::FromArgb(14, 22, 33));
			g->FillRectangle(bFondo, 0, 0, w, h);
			delete bFondo;

			Pen^ pExt = gcnew Pen(Color::FromArgb(214, 198, 166), 2.0f);
			Pen^ pInt = gcnew Pen(Color::FromArgb(42, 58, 78), 1.0f);
			g->DrawRectangle(pExt, 1, 1, w - 2, h - 2);
			g->DrawRectangle(pInt, 4, 4, w - 9, h - 9);
			delete pExt;
			delete pInt;

			if (bmpRetratoActual != nullptr) {
				int bw = bmpRetratoActual->Width;
				int bh = bmpRetratoActual->Height;
				int esc = Math::Max(1, Math::Min(108 / bw, 96 / bh));
				int dw = bw * esc;
				int dh = bh * esc;
				int dx = (w - dw) / 2;
				int dy = (h - dh) / 2 + 2;
				g->DrawImage(bmpRetratoActual, Rectangle(dx, dy, dw, dh), Rectangle(0, 0, bw, bh), GraphicsUnit::Pixel);
			}
		}

		void lstPersonajes_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			int i = lstPersonajes->SelectedIndex;
			if (i >= 0 && i < nombres->Length) {
				lblNombre->Text = nombres[i];
				lblRol->Text = roles[i];
				String^ conf = confianzas[i];
				if (conf == "Protagonista") lblConfianza->Text = "Estado: Protagonista";
				else if (conf == "Hostil") lblConfianza->Text = "Actitud: Hostil";
				else lblConfianza->Text = "Vínculo: " + conf;
				lblDetalle->Text = descripciones[i];
				if (bmpRetratoActual != nullptr) {
					delete bmpRetratoActual;
					bmpRetratoActual = nullptr;
				}
				String^ clave = (spriteClaves != nullptr && i < spriteClaves->Length && !String::IsNullOrEmpty(spriteClaves[i])) ? spriteClaves[i] : nombres[i];
				bmpRetratoActual = CargarRetratoPersonajeComun(clave);
				pnlRetrato->Invalidate();
			}
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

		void FormPersonajes_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Escape || e->KeyCode == Keys::P) {
				e->Handled = true;
				e->SuppressKeyPress = true;
				this->Close();
			}
		}

		void InicializarUI(array<String^>^ n, array<String^>^ r, array<String^>^ d, array<String^>^ c, array<String^>^ s) {
			nombres = n;
			roles = r;
			descripciones = d;
			confianzas = c;
			spriteClaves = s;
			bmpRetratoActual = nullptr;

			this->Text = "Personajes - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(660, 460);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(10, 16, 24);
			this->KeyPreview = true;
			this->KeyDown += gcnew KeyEventHandler(this, &FormPersonajes::FormPersonajes_KeyDown);

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "DICCIONARIO DE PERSONAJES";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(214, 198, 166);
			lblTitulo->Location = Point(28, 18);
			lblTitulo->Size = System::Drawing::Size(604, 28);
			lblTitulo->TextAlign = ContentAlignment::MiddleLeft;
			this->Controls->Add(lblTitulo);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(28, 52);
			linea->Size = System::Drawing::Size(604, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			this->Controls->Add(linea);

			lstPersonajes = gcnew ListBox();
			lstPersonajes->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			lstPersonajes->BackColor = Color::FromArgb(13, 20, 30);
			lstPersonajes->ForeColor = Color::FromArgb(226, 232, 240);
			lstPersonajes->BorderStyle = BorderStyle::FixedSingle;
			lstPersonajes->ItemHeight = 26;
			lstPersonajes->Location = Point(28, 70);
			lstPersonajes->Size = System::Drawing::Size(212, 304);
			lstPersonajes->SelectedIndexChanged += gcnew EventHandler(this, &FormPersonajes::lstPersonajes_SelectedIndexChanged);
			this->Controls->Add(lstPersonajes);

			pnlRetrato = gcnew Panel();
			pnlRetrato->Location = Point(260, 70);
			pnlRetrato->Size = System::Drawing::Size(120, 112);
			pnlRetrato->BackColor = Color::FromArgb(14, 22, 33);
			pnlRetrato->Paint += gcnew PaintEventHandler(this, &FormPersonajes::pnlRetrato_Paint);
			this->Controls->Add(pnlRetrato);

			lblNombre = gcnew Label();
			lblNombre->Font = gcnew System::Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
			lblNombre->ForeColor = Color::FromArgb(245, 232, 196);
			lblNombre->Location = Point(394, 72);
			lblNombre->Size = System::Drawing::Size(238, 30);
			this->Controls->Add(lblNombre);

			lblRol = gcnew Label();
			lblRol->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Italic);
			lblRol->ForeColor = Color::FromArgb(152, 178, 202);
			lblRol->Location = Point(394, 106);
			lblRol->Size = System::Drawing::Size(238, 36);
			this->Controls->Add(lblRol);

			lblConfianza = gcnew Label();
			lblConfianza->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			lblConfianza->ForeColor = Color::FromArgb(214, 192, 134);
			lblConfianza->Location = Point(394, 150);
			lblConfianza->Size = System::Drawing::Size(238, 28);
			this->Controls->Add(lblConfianza);

			Panel^ lineaDesc = gcnew Panel();
			lineaDesc->Location = Point(260, 194);
			lineaDesc->Size = System::Drawing::Size(372, 1);
			lineaDesc->BackColor = Color::FromArgb(40, 214, 198, 166);
			this->Controls->Add(lineaDesc);

			lblDetalle = gcnew Label();
			lblDetalle->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			lblDetalle->ForeColor = Color::FromArgb(206, 218, 230);
			lblDetalle->Location = Point(260, 208);
			lblDetalle->Size = System::Drawing::Size(372, 166);
			this->Controls->Add(lblDetalle);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar [ESC]";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(226, 232, 240);
			btnCerrar->BackColor = Color::FromArgb(20, 30, 42);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 1;
			btnCerrar->FlatAppearance->BorderColor = Color::FromArgb(62, 82, 104);
			btnCerrar->Location = Point(256, 396);
			btnCerrar->Size = System::Drawing::Size(148, 38);
			btnCerrar->Click += gcnew EventHandler(this, &FormPersonajes::btnCerrar_Click);
			this->Controls->Add(btnCerrar);

			if (nombres != nullptr && nombres->Length > 0) {
				for each (String^ nombre in nombres) lstPersonajes->Items->Add(nombre);
				lstPersonajes->SelectedIndex = 0;
			}
			this->ActiveControl = lstPersonajes;
		}

	public:
		FormPersonajes(array<String^>^ n, array<String^>^ r, array<String^>^ d, array<String^>^ c) {
			InicializarUI(n, r, d, c, n);
		}

		FormPersonajes(array<String^>^ n, array<String^>^ r, array<String^>^ d, array<String^>^ c, array<String^>^ s) {
			InicializarUI(n, r, d, c, s);
		}

		~FormPersonajes() {
			if (bmpRetratoActual != nullptr) {
				delete bmpRetratoActual;
				bmpRetratoActual = nullptr;
			}
		}
	};
}
