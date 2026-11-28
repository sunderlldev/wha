#pragma once

#include "Utilidades.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;

	public ref class FormEscenaCuarto : public Form {
	private:
		int indiceActual;
		String^ tituloEscena;
		array<String^>^ rutas;
		array<String^>^ textos;
		Bitmap^ bmpActual;
		Panel^ pnlEscena;
		Label^ lblTitulo;
		Label^ lblProgreso;
		Label^ lblTexto;
		Button^ btnSiguiente;

		void CargarSlideActual() {
			if (bmpActual != nullptr) {
				delete bmpActual;
				bmpActual = nullptr;
			}
			if (rutas != nullptr && indiceActual >= 0 && indiceActual < rutas->Length) {
				bmpActual = CargarBitmapSeguroComun(rutas[indiceActual], false);
				lblProgreso->Text = "Escena " + (indiceActual + 1).ToString() + " / " + rutas->Length.ToString();
				if (textos != nullptr && indiceActual < textos->Length) {
					lblTexto->Text = textos[indiceActual];
				}
				btnSiguiente->Text = (indiceActual + 1 < rutas->Length) ? "Siguiente [Espacio]" : "Continuar [Espacio]";
			}
			pnlEscena->Invalidate();
		}

		void pnlEscena_Paint(Object^ sender, PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			int w = pnlEscena->Width;
			int h = pnlEscena->Height;
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;

			SolidBrush^ bg = gcnew SolidBrush(Color::FromArgb(14, 21, 32));
			g->FillRectangle(bg, 0, 0, w, h);
			delete bg;

			Pen^ borde = gcnew Pen(Color::FromArgb(214, 198, 166), 2.0f);
			g->DrawRectangle(borde, 1, 1, w - 2, h - 2);
			delete borde;

			if (bmpActual != nullptr) {
				float rx = (float)(w - 16) / (float)Math::Max(1, bmpActual->Width);
				float ry = (float)(h - 16) / (float)Math::Max(1, bmpActual->Height);
				float esc = Math::Min(rx, ry);
				int dw = Math::Max(1, (int)(bmpActual->Width * esc));
				int dh = Math::Max(1, (int)(bmpActual->Height * esc));
				int dx = (w - dw) / 2;
				int dy = (h - dh) / 2;
				g->DrawImage(bmpActual, Rectangle(dx, dy, dw, dh), Rectangle(0, 0, bmpActual->Width, bmpActual->Height), GraphicsUnit::Pixel);
			}
			else {
				System::Drawing::Font^ fInfo = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
				System::Drawing::Font^ fRuta = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Regular);
				String^ slot = (rutas != nullptr && indiceActual < rutas->Length) ? rutas[indiceActual] : "";
				StringFormat^ sf = gcnew StringFormat();
				sf->Alignment = StringAlignment::Center;
				sf->LineAlignment = StringAlignment::Center;
				g->DrawString(tituloEscena + " (" + (indiceActual + 1).ToString() + ")", fInfo, Brushes::Khaki, RectangleF(16.0f, 70.0f, (float)w - 32.0f, 36.0f), sf);
				g->DrawString("Imagen pendiente: " + slot, fRuta, Brushes::LightSteelBlue, RectangleF(16.0f, 118.0f, (float)w - 32.0f, 32.0f), sf);
				delete sf;
				delete fInfo;
				delete fRuta;
			}
		}

		void Avanzar() {
			if (rutas == nullptr || indiceActual + 1 >= rutas->Length) {
				this->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->Close();
				return;
			}
			indiceActual++;
			CargarSlideActual();
		}

		void btnSiguiente_Click(Object^ sender, EventArgs^ e) {
			Avanzar();
		}

		void FormEscenaCuarto_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Space || e->KeyCode == Keys::Enter || e->KeyCode == Keys::Z) {
				e->Handled = true;
				e->SuppressKeyPress = true;
				Avanzar();
			}
			else if (e->KeyCode == Keys::Escape) {
				e->Handled = true;
				e->SuppressKeyPress = true;
				this->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->Close();
			}
		}

	public:
		FormEscenaCuarto(int cuarto, String^ nombreCuarto, array<String^>^ imgs, array<String^>^ desc) {
			indiceActual = 0;
			tituloEscena = String::IsNullOrEmpty(nombreCuarto) ? "Pueblo de Kaln" : nombreCuarto;
			rutas = imgs;
			textos = desc;
			bmpActual = nullptr;

			this->Text = "Kaln - " + tituloEscena;
			this->ClientSize = System::Drawing::Size(680, 480);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(10, 16, 24);
			this->KeyPreview = true;
			this->KeyDown += gcnew KeyEventHandler(this, &FormEscenaCuarto::FormEscenaCuarto_KeyDown);

			lblTitulo = gcnew Label();
			lblTitulo->Text = tituloEscena;
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(214, 198, 166);
			lblTitulo->Location = Point(28, 16);
			lblTitulo->Size = System::Drawing::Size(460, 28);
			lblTitulo->TextAlign = ContentAlignment::MiddleLeft;
			this->Controls->Add(lblTitulo);

			lblProgreso = gcnew Label();
			lblProgreso->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
			lblProgreso->ForeColor = Color::FromArgb(156, 184, 210);
			lblProgreso->Location = Point(500, 16);
			lblProgreso->Size = System::Drawing::Size(152, 28);
			lblProgreso->TextAlign = ContentAlignment::MiddleRight;
			this->Controls->Add(lblProgreso);

			pnlEscena = gcnew Panel();
			pnlEscena->Location = Point(28, 54);
			pnlEscena->Size = System::Drawing::Size(624, 278);
			pnlEscena->Paint += gcnew PaintEventHandler(this, &FormEscenaCuarto::pnlEscena_Paint);
			this->Controls->Add(pnlEscena);

			lblTexto = gcnew Label();
			lblTexto->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
			lblTexto->ForeColor = Color::FromArgb(218, 228, 238);
			lblTexto->Location = Point(28, 344);
			lblTexto->Size = System::Drawing::Size(624, 68);
			lblTexto->TextAlign = ContentAlignment::MiddleCenter;
			this->Controls->Add(lblTexto);

			btnSiguiente = gcnew Button();
			btnSiguiente->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnSiguiente->ForeColor = Color::FromArgb(236, 224, 196);
			btnSiguiente->BackColor = Color::FromArgb(26, 36, 52);
			btnSiguiente->FlatStyle = FlatStyle::Flat;
			btnSiguiente->FlatAppearance->BorderSize = 1;
			btnSiguiente->FlatAppearance->BorderColor = Color::FromArgb(140, 128, 104);
			btnSiguiente->Cursor = Cursors::Hand;
			btnSiguiente->Location = Point(240, 422);
			btnSiguiente->Size = System::Drawing::Size(200, 38);
			btnSiguiente->Click += gcnew EventHandler(this, &FormEscenaCuarto::btnSiguiente_Click);
			this->Controls->Add(btnSiguiente);

			CargarSlideActual();
			this->ActiveControl = btnSiguiente;
		}

		~FormEscenaCuarto() {
			if (bmpActual != nullptr) {
				delete bmpActual;
				bmpActual = nullptr;
			}
		}
	};
}
