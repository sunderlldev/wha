#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormSelectorNivel : public Form {
	private:
		int nivelElegido;
		Label^ lblTitulo;
		Label^ lblSubtitulo;
		Button^ btnNivel1;
		Button^ btnNivel2;
		Button^ btnNivel3;
		Button^ btnCancelar;
		Panel^ pnlContenedor;

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

	public:
		FormSelectorNivel() {
			nivelElegido = 1;

			this->Text = "Seleccionar Perspectiva y Nivel";
			this->ClientSize = System::Drawing::Size(620, 420);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(590, 390);
			pnlContenedor->BackColor = Color::FromArgb(21, 29, 44);
			pnlContenedor->BorderStyle = BorderStyle::FixedSingle;
			this->Controls->Add(pnlContenedor);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "S E L E C T O R   D E   N I V E L";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 15, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 16);
			lblTitulo->Size = System::Drawing::Size(550, 28);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlContenedor->Controls->Add(lblTitulo);

			lblSubtitulo = gcnew Label();
			lblSubtitulo->Text = "Elige la perspectiva narrativa que deseas jugar o evaluar:";
			lblSubtitulo->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			lblSubtitulo->ForeColor = Color::FromArgb(180, 190, 205);
			lblSubtitulo->Location = Point(20, 48);
			lblSubtitulo->Size = System::Drawing::Size(550, 24);
			lblSubtitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlContenedor->Controls->Add(lblSubtitulo);

			btnNivel1 = gcnew Button();
			btnNivel1->Text = "NIVEL 1: COCO — El Atelier del Maestro Qifrey\n(Perspectiva: La Aprendiz que Descubre la Magia)";
			btnNivel1->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnNivel1->ForeColor = Color::FromArgb(240, 246, 252);
			btnNivel1->BackColor = Color::FromArgb(34, 68, 48);
			btnNivel1->FlatStyle = FlatStyle::Flat;
			btnNivel1->Location = Point(35, 86);
			btnNivel1->Size = System::Drawing::Size(520, 56);
			btnNivel1->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel1_Click);
			pnlContenedor->Controls->Add(btnNivel1);

			btnNivel2 = gcnew Button();
			btnNivel2->Text = "NIVEL 2: TARTAH — El Laberinto del Pueblo de Kaln\n(Perspectiva: El Habitante con Ojos de Plata)";
			btnNivel2->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnNivel2->ForeColor = Color::FromArgb(240, 246, 252);
			btnNivel2->BackColor = Color::FromArgb(65, 52, 38);
			btnNivel2->FlatStyle = FlatStyle::Flat;
			btnNivel2->Location = Point(35, 154);
			btnNivel2->Size = System::Drawing::Size(520, 56);
			btnNivel2->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel2_Click);
			pnlContenedor->Controls->Add(btnNivel2);

			btnNivel3 = gcnew Button();
			btnNivel3->Text = "NIVEL 3: COUSTAS — La Emboscada y el Árbol de Plata\n(Perspectiva: El Bardo y la Desesperación por Sanar)";
			btnNivel3->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnNivel3->ForeColor = Color::FromArgb(240, 246, 252);
			btnNivel3->BackColor = Color::FromArgb(38, 54, 78);
			btnNivel3->FlatStyle = FlatStyle::Flat;
			btnNivel3->Location = Point(35, 222);
			btnNivel3->Size = System::Drawing::Size(520, 56);
			btnNivel3->Click += gcnew EventHandler(this, &FormSelectorNivel::btnNivel3_Click);
			pnlContenedor->Controls->Add(btnNivel3);

			btnCancelar = gcnew Button();
			btnCancelar->Text = "Volver al Menú Principal";
			btnCancelar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			btnCancelar->ForeColor = Color::FromArgb(210, 215, 225);
			btnCancelar->BackColor = Color::FromArgb(28, 36, 50);
			btnCancelar->FlatStyle = FlatStyle::Flat;
			btnCancelar->Location = Point(195, 305);
			btnCancelar->Size = System::Drawing::Size(200, 38);
			btnCancelar->Click += gcnew EventHandler(this, &FormSelectorNivel::btnCancelar_Click);
			pnlContenedor->Controls->Add(btnCancelar);
		}

		int getNivelElegido() {
			return nivelElegido;
		}

		~FormSelectorNivel() {}
	};
}
