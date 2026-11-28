#pragma once

#include "Utilidades.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;

	public ref class FormTrollface : public Form {
	private:
		Button^ btnCerrar;
		Bitmap^ bmpTroll;

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

		void FormTrollface_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Enter || e->KeyCode == Keys::Escape || e->KeyCode == Keys::Space || e->KeyCode == Keys::E || e->KeyCode == Keys::Z) {
				e->Handled = true;
				e->SuppressKeyPress = true;
				this->Close();
			}
		}

	protected:
		virtual void OnKeyPress(KeyPressEventArgs^ e) override {
			e->Handled = true;
		}
		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);
			Graphics^ g = e->Graphics;
			g->SmoothingMode = SmoothingMode::AntiAlias;
			g->InterpolationMode = InterpolationMode::HighQualityBicubic;
			g->Clear(Color::White);

			int cx = this->ClientSize.Width / 2;
			if (bmpTroll != nullptr) {
				int w = 310;
				int h = 288;
				g->DrawImage(bmpTroll, Rectangle(cx - w / 2, 26, w, h));
			}

			System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
			String^ msg = "... ¿Esperabas un secreto mágico ancestral aquí?";
			SizeF sMsg = g->MeasureString(msg, fTitulo);
			g->DrawString(msg, fTitulo, Brushes::Black, PointF(cx - sMsg.Width / 2.0f, 336.0f));
			delete fTitulo;
		}

	public:
		FormTrollface() {
			this->Text = "Secreto Oculto";
			this->ClientSize = System::Drawing::Size(640, 460);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::White;
			this->DoubleBuffered = true;
			this->KeyPreview = true;

			bmpTroll = CargarBitmapSeguroComun("sprites/trollface.png", false);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Volver al taller";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCerrar->ForeColor = Color::White;
			btnCerrar->BackColor = Color::FromArgb(24, 24, 28);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(52, 52, 60);
			btnCerrar->Cursor = Cursors::Hand;
			btnCerrar->Location = Point(220, 390);
			btnCerrar->Size = System::Drawing::Size(200, 40);
			btnCerrar->Click += gcnew EventHandler(this, &FormTrollface::btnCerrar_Click);
			this->Controls->Add(btnCerrar);

			this->KeyDown += gcnew KeyEventHandler(this, &FormTrollface::FormTrollface_KeyDown);
			this->ActiveControl = btnCerrar;
		}

		~FormTrollface() {
			if (bmpTroll != nullptr) {
				delete bmpTroll;
				bmpTroll = nullptr;
			}
		}
	};
}
