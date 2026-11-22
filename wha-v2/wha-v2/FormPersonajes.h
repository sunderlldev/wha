#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormPersonajes : public Form {
	private:
		ListBox^ lstPersonajes;
		Label^ lblDetalle;
		Label^ lblConfianza;
		Button^ btnCerrar;
		array<String^>^ roles;
		array<String^>^ descripciones;
		array<String^>^ confianzas;
		array<String^>^ nombres;

		void lstPersonajes_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			int i = lstPersonajes->SelectedIndex;
			if (i >= 0 && i < nombres->Length) {
				lblDetalle->Text = roles[i] + "\n\n" + descripciones[i];
				lblConfianza->Text = confianzas[i];
			}
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormPersonajes(array<String^>^ n, array<String^>^ r, array<String^>^ d, array<String^>^ c) {
			nombres = n;
			roles = r;
			descripciones = d;
			confianzas = c;

			this->Text = "Personajes - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(640, 460);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(10, 16, 24);

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "PERSONAJES";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 15.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(214, 198, 166);
			lblTitulo->Location = Point(28, 18);
			lblTitulo->Size = System::Drawing::Size(584, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleLeft;
			this->Controls->Add(lblTitulo);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(30, 52);
			linea->Size = System::Drawing::Size(580, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			this->Controls->Add(linea);

			lstPersonajes = gcnew ListBox();
			lstPersonajes->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			lstPersonajes->BackColor = Color::FromArgb(9, 14, 21);
			lstPersonajes->ForeColor = Color::FromArgb(226, 232, 240);
			lstPersonajes->BorderStyle = BorderStyle::None;
			lstPersonajes->Location = Point(30, 70);
			lstPersonajes->Size = System::Drawing::Size(220, 300);
			lstPersonajes->SelectedIndexChanged += gcnew EventHandler(this, &FormPersonajes::lstPersonajes_SelectedIndexChanged);
			this->Controls->Add(lstPersonajes);

			lblDetalle = gcnew Label();
			lblDetalle->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			lblDetalle->ForeColor = Color::FromArgb(206, 218, 230);
			lblDetalle->Location = Point(274, 70);
			lblDetalle->Size = System::Drawing::Size(336, 230);
			this->Controls->Add(lblDetalle);

			lblConfianza = gcnew Label();
			lblConfianza->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			lblConfianza->ForeColor = Color::FromArgb(206, 184, 128);
			lblConfianza->Location = Point(274, 306);
			lblConfianza->Size = System::Drawing::Size(336, 26);
			this->Controls->Add(lblConfianza);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(226, 232, 240);
			btnCerrar->BackColor = Color::FromArgb(20, 30, 42);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->Location = Point(250, 386);
			btnCerrar->Size = System::Drawing::Size(140, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormPersonajes::btnCerrar_Click);
			this->Controls->Add(btnCerrar);

			if (nombres != nullptr && nombres->Length > 0) {
				for each (String^ nombre in nombres) lstPersonajes->Items->Add(nombre);
				lstPersonajes->SelectedIndex = 0;
			}
			this->ActiveControl = btnCerrar;
		}

		~FormPersonajes() {}
	};
}
