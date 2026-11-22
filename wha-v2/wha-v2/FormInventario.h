#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormInventario : public Form {
	private:
		ListBox^ lstItems;
		Label^ lblDescripcion;
		Button^ btnCerrar;
		array<String^>^ descripciones;

		void lstItems_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			int indice = lstItems->SelectedIndex;
			if (indice >= 0 && indice < descripciones->Length) {
				lblDescripcion->Text = descripciones[indice];
			}
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormInventario(array<String^>^ nombres, array<String^>^ descs) {
			descripciones = descs;

			this->Text = "Inventario Arcano - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(620, 440);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			Panel^ panel = gcnew Panel();
			panel->Location = Point(15, 15);
			panel->Size = System::Drawing::Size(590, 410);
			panel->BackColor = Color::FromArgb(21, 29, 44);
			panel->BorderStyle = BorderStyle::None;
			this->Controls->Add(panel);

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "I N V E N T A R I O   A R C A N O";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 15.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 14);
			lblTitulo->Size = System::Drawing::Size(550, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			panel->Controls->Add(lblTitulo);

			lstItems = gcnew ListBox();
			lstItems->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			lstItems->BackColor = Color::FromArgb(12, 18, 27);
			lstItems->ForeColor = Color::FromArgb(236, 228, 194);
			lstItems->BorderStyle = BorderStyle::None;
			lstItems->Location = Point(28, 58);
			lstItems->Size = System::Drawing::Size(250, 280);
			lstItems->SelectedIndexChanged += gcnew EventHandler(this, &FormInventario::lstItems_SelectedIndexChanged);
			panel->Controls->Add(lstItems);

			lblDescripcion = gcnew Label();
			lblDescripcion->Text = "Selecciona un objeto para leer su descripción.";
			lblDescripcion->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			lblDescripcion->ForeColor = Color::FromArgb(206, 220, 236);
			lblDescripcion->BackColor = Color::FromArgb(12, 18, 27);
			lblDescripcion->BorderStyle = BorderStyle::None;
			lblDescripcion->Location = Point(294, 58);
			lblDescripcion->Size = System::Drawing::Size(270, 280);
			lblDescripcion->Padding = System::Windows::Forms::Padding(12);
			panel->Controls->Add(lblDescripcion);

			if (nombres != nullptr && nombres->Length > 0) {
				for each (String^ nombre in nombres) {
					lstItems->Items->Add(nombre);
				}
				lstItems->SelectedIndex = 0;
			}
			else {
				lstItems->Items->Add("(Sin objetos todavía)");
				lblDescripcion->Text = "Aún no has recogido ningún objeto. Explora el atelier y el bosque.";
			}

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar inventario";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(230, 238, 248);
			btnCerrar->BackColor = Color::FromArgb(18, 27, 38);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCerrar->Location = Point(220, 352);
			btnCerrar->Size = System::Drawing::Size(160, 38);
			btnCerrar->Click += gcnew EventHandler(this, &FormInventario::btnCerrar_Click);
			panel->Controls->Add(btnCerrar);
		}

		~FormInventario() {}
	};
}
