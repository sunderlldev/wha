#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormPausa : public Form {
	private:
		int accion;
		Panel^ panelContenedor;
		Button^ btnContinuar;
		Button^ btnSalirMenu;

		void btnContinuar_Click(Object^ sender, EventArgs^ e) {
			accion = 0;
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

		void btnSalirMenu_Click(Object^ sender, EventArgs^ e) {
			accion = 5;
			this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			this->Close();
		}

		void FormPausa_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::Escape) {
				e->Handled = true;
				e->SuppressKeyPress = true;
				accion = 0;
				this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->Close();
			}
		}

	protected:
		virtual void OnKeyPress(KeyPressEventArgs^ e) override {
			e->Handled = true;
		}

	public:
		FormPausa(String^ textoProgreso) {
			accion = 0;
			this->Text = "Salir de la partida";
			this->ClientSize = System::Drawing::Size(520, 290);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);
			this->KeyPreview = true;

			panelContenedor = gcnew Panel();
			panelContenedor->Location = Point(15, 15);
			panelContenedor->Size = System::Drawing::Size(490, 260);
			panelContenedor->BackColor = Color::FromArgb(10, 16, 24);
			this->Controls->Add(panelContenedor);

			Label^ lblPregunta = gcnew Label();
			lblPregunta->Text = "¿Estás seguro de que quieres salir de la partida?";
			lblPregunta->Font = gcnew System::Drawing::Font("Segoe UI", 12.5f, FontStyle::Bold);
			lblPregunta->ForeColor = Color::FromArgb(240, 244, 248);
			lblPregunta->Location = Point(24, 26);
			lblPregunta->Size = System::Drawing::Size(442, 34);
			lblPregunta->TextAlign = ContentAlignment::MiddleCenter;
			panelContenedor->Controls->Add(lblPregunta);

			Label^ lblResumen = gcnew Label();
			lblResumen->Text = textoProgreso;
			lblResumen->Font = gcnew System::Drawing::Font("Segoe UI", 9.8f, FontStyle::Regular);
			lblResumen->ForeColor = Color::FromArgb(202, 214, 228);
			lblResumen->BackColor = Color::FromArgb(14, 22, 32);
			lblResumen->Location = Point(28, 74);
			lblResumen->Size = System::Drawing::Size(434, 102);
			lblResumen->Padding = System::Windows::Forms::Padding(12);
			lblResumen->TextAlign = ContentAlignment::MiddleCenter;
			panelContenedor->Controls->Add(lblResumen);

			btnSalirMenu = gcnew Button();
			btnSalirMenu->Text = "Sí, salir al menú";
			btnSalirMenu->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnSalirMenu->ForeColor = Color::FromArgb(236, 174, 174);
			btnSalirMenu->BackColor = Color::FromArgb(38, 22, 26);
			btnSalirMenu->FlatStyle = FlatStyle::Flat;
			btnSalirMenu->FlatAppearance->BorderSize = 0;
			btnSalirMenu->FlatAppearance->MouseOverBackColor = Color::FromArgb(58, 30, 36);
			btnSalirMenu->Cursor = Cursors::Hand;
			btnSalirMenu->Location = Point(42, 198);
			btnSalirMenu->Size = System::Drawing::Size(190, 40);
			btnSalirMenu->Click += gcnew EventHandler(this, &FormPausa::btnSalirMenu_Click);
			panelContenedor->Controls->Add(btnSalirMenu);

			btnContinuar = gcnew Button();
			btnContinuar->Text = "No, seguir jugando";
			btnContinuar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnContinuar->ForeColor = Color::FromArgb(214, 198, 166);
			btnContinuar->BackColor = Color::FromArgb(18, 27, 38);
			btnContinuar->FlatStyle = FlatStyle::Flat;
			btnContinuar->FlatAppearance->BorderSize = 0;
			btnContinuar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnContinuar->Cursor = Cursors::Hand;
			btnContinuar->Location = Point(258, 198);
			btnContinuar->Size = System::Drawing::Size(190, 40);
			btnContinuar->Click += gcnew EventHandler(this, &FormPausa::btnContinuar_Click);
			panelContenedor->Controls->Add(btnContinuar);

			this->KeyDown += gcnew KeyEventHandler(this, &FormPausa::FormPausa_KeyDown);
			this->ActiveControl = btnContinuar;
		}

		~FormPausa() {}

		int getAccion() {
			return accion;
		}
	};
}
