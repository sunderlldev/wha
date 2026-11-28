#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormMisiones : public Form {
	private:
		Panel^ pnlContenedor;
		Button^ btnCerrar;

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormMisiones(String^ subtituloNivel, array<String^>^ titulos, array<String^>^ detalles, array<bool>^ completadas, int indiceActual) {
			this->Text = "Misiones - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(640, 500);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(610, 470);
			pnlContenedor->BackColor = Color::FromArgb(10, 16, 24);
			this->Controls->Add(pnlContenedor);

			Label^ lblSub = gcnew Label();
			lblSub->Text = subtituloNivel;
			lblSub->Font = gcnew System::Drawing::Font("Segoe UI", 12.5f, FontStyle::Bold);
			lblSub->ForeColor = Color::FromArgb(214, 198, 166);
			lblSub->Location = Point(28, 20);
			lblSub->Size = System::Drawing::Size(554, 28);
			lblSub->TextAlign = ContentAlignment::MiddleLeft;
			pnlContenedor->Controls->Add(lblSub);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(28, 54);
			linea->Size = System::Drawing::Size(554, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			pnlContenedor->Controls->Add(linea);

			int total = (titulos != nullptr) ? titulos->Length : 0;
			int yActual = 68;
			for (int i = 0; i < total; i++) {
				bool hecha = (completadas != nullptr && i < completadas->Length) ? completadas[i] : false;
				bool esActual = (!hecha && i == indiceActual);

				String^ marca = hecha ? "[X] " : (esActual ? "[O] " : "[ ] ");
				Color colTitulo = hecha
					? Color::FromArgb(148, 206, 164)
					: (esActual ? Color::FromArgb(236, 214, 148) : Color::FromArgb(172, 184, 198));
				Color colDetalle = esActual ? Color::FromArgb(230, 236, 244) : Color::FromArgb(146, 158, 174);

				Label^ lblTit = gcnew Label();
				lblTit->Text = marca + titulos[i];
				lblTit->Font = gcnew System::Drawing::Font("Segoe UI", 9.8f, esActual ? FontStyle::Bold : FontStyle::Regular);
				lblTit->ForeColor = colTitulo;
				lblTit->Location = Point(28, yActual);
				lblTit->Size = System::Drawing::Size(554, 20);
				pnlContenedor->Controls->Add(lblTit);

				Label^ lblDet = gcnew Label();
				lblDet->Text = (detalles != nullptr && i < detalles->Length) ? detalles[i] : "";
				lblDet->Font = gcnew System::Drawing::Font("Segoe UI", 8.8f, FontStyle::Regular);
				lblDet->ForeColor = colDetalle;
				lblDet->Location = Point(54, yActual + 20);
				lblDet->Size = System::Drawing::Size(528, 20);
				pnlContenedor->Controls->Add(lblDet);

				yActual += 44;
			}

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(230, 238, 248);
			btnCerrar->BackColor = Color::FromArgb(18, 27, 38);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCerrar->Cursor = Cursors::Hand;
			btnCerrar->Location = Point(235, 418);
			btnCerrar->Size = System::Drawing::Size(140, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormMisiones::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);
			this->ActiveControl = btnCerrar;
		}

		~FormMisiones() {}
	};
}
