#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Windows::Forms;

	public ref class FormTrollface : public Form {
	private:
		Button^ btnCerrar;

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			Form::OnPaint(e);
			Graphics^ g = e->Graphics;
			g->SmoothingMode = SmoothingMode::AntiAlias;

			LinearGradientBrush^ fondo = gcnew LinearGradientBrush(this->ClientRectangle, Color::FromArgb(10, 8, 16), Color::FromArgb(30, 16, 40), LinearGradientMode::Vertical);
			g->FillRectangle(fondo, this->ClientRectangle);
			delete fondo;

			int cx = this->ClientSize.Width / 2;
			SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(50, 170, 90, 220));
			g->FillEllipse(aura, cx - 150, 30, 300, 300);
			delete aura;

			SolidBrush^ blanco = gcnew SolidBrush(Color::FromArgb(245, 240, 236, 226));
			SolidBrush^ negro = gcnew SolidBrush(Color::FromArgb(255, 24, 20, 28));
			Pen^ contorno = gcnew Pen(Color::FromArgb(255, 24, 20, 28), 4.0f);
			g->FillEllipse(blanco, cx - 110, 55, 220, 220);
			g->DrawEllipse(contorno, cx - 110, 55, 220, 220);

			g->FillEllipse(negro, cx - 68, 118, 34, 44);
			g->FillEllipse(negro, cx + 34, 118, 34, 44);
			g->FillEllipse(blanco, cx - 60, 128, 10, 12);
			g->FillEllipse(blanco, cx + 42, 128, 10, 12);

			Pen^ ceja = gcnew Pen(Color::FromArgb(255, 24, 20, 28), 5.0f);
			g->DrawLine(ceja, cx - 80, 104, cx - 30, 114);
			g->DrawLine(ceja, cx + 80, 104, cx + 30, 114);
			delete ceja;

			SolidBrush^ boca = gcnew SolidBrush(Color::FromArgb(255, 24, 20, 28));
			g->FillEllipse(boca, cx - 80, 172, 160, 62);
			g->FillRectangle(boca, cx - 72, 186, 144, 20);
			SolidBrush^ dientes = gcnew SolidBrush(Color::FromArgb(255, 244, 240, 230));
			for (int i = 0; i < 6; i++) {
				g->FillRectangle(dientes, cx - 66 + i * 23, 186, 16, 18);
			}
			g->FillRectangle(blanco, cx - 14, 180, 28, 34);
			delete boca;
			delete dientes;

			Pen^ arruga = gcnew Pen(Color::FromArgb(160, 120, 100, 90), 2.0f);
			g->DrawArc(arruga, cx - 90, 148, 60, 26, 20, 120);
			g->DrawArc(arruga, cx + 30, 148, 60, 26, 40, 120);
			delete arruga;

			System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 15.0f, FontStyle::Bold);
			System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			g->DrawString("... ¿Esperabas un secreto magico ancestral aqui? ...", fTitulo, Brushes::Gold, PointF((float)(cx - 230), 330.0f));
			g->DrawString("[ Presiona ENTER o E para volver al taller ]", fTexto, Brushes::Gainsboro, RectangleF((float)(cx - 230), 374.0f, 460.0f, 30.0f));

			delete blanco;
			delete negro;
			delete contorno;
			delete fTitulo;
			delete fTexto;
		}

	public:
		FormTrollface() {
			this->Text = "Easter egg - Trollface";
			this->ClientSize = System::Drawing::Size(640, 520);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(10, 8, 16);
			this->DoubleBuffered = true;

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Volver al taller";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(240, 246, 252);
			btnCerrar->BackColor = Color::FromArgb(48, 38, 62);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->Location = Point(220, 452);
			btnCerrar->Size = System::Drawing::Size(200, 40);
			btnCerrar->Click += gcnew EventHandler(this, &FormTrollface::btnCerrar_Click);
			this->Controls->Add(btnCerrar);
		}

		~FormTrollface() {}
	};
}
