#pragma once

#include "ControladorJuego.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::IO;

	public ref class FormGuardarCargar : public Form {
	private:
		ControladorJuego* controlador;
		Label^ lblTitulo;
		Label^ lblInfo;
		Label^ lblArchivo;
		Button^ btnGuardar;
		Button^ btnCargar;
		Button^ btnCerrar;
		Panel^ pnlContenedor;

		void ActualizarInfo() {
			lblInfo->Text = controlador->getResumenPartida();
			if (File::Exists("partida_guardada.txt")) {
				DateTime fecha = File::GetLastWriteTime("partida_guardada.txt");
				lblArchivo->Text = "Archivo encontrado: partida_guardada.txt\nÚltima modificación: " + fecha.ToString("dd/MM/yyyy HH:mm");
				lblArchivo->ForeColor = Color::FromArgb(170, 214, 180);
			}
			else {
				lblArchivo->Text = "No existe ninguna partida guardada todavía.";
				lblArchivo->ForeColor = Color::FromArgb(190, 180, 150);
			}
		}

		void btnGuardar_Click(Object^ sender, EventArgs^ e) {
			if (controlador->guardarPartida()) {
				MessageBox::Show("Partida guardada en 'partida_guardada.txt'.\n\n" + controlador->getResumenPartida(), "Guardar partida", MessageBoxButtons::OK, MessageBoxIcon::Information);
				ActualizarInfo();
			}
			else {
				MessageBox::Show("No se pudo escribir el archivo de partida.", "Guardar partida", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}

		void btnCargar_Click(Object^ sender, EventArgs^ e) {
			if (!File::Exists("partida_guardada.txt")) {
				MessageBox::Show("No se encontró ninguna partida guardada.", "Cargar partida", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			bool cargada = false;
			try {
				cargada = controlador->cargarPartida();
			}
			catch (...) {
				cargada = false;
			}
			if (cargada) {
				MessageBox::Show("Partida cargada correctamente.\n\n" + controlador->getResumenPartida(), "Cargar partida", MessageBoxButtons::OK, MessageBoxIcon::Information);
				this->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->Close();
			}
			else {
				MessageBox::Show("El archivo de partida está incompleto o dañado.", "Cargar partida", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

	public:
		FormGuardarCargar(ControladorJuego* ctrl) {
			controlador = ctrl;

			this->Text = "Guardar y cargar partida - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(620, 460);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(590, 430);
			pnlContenedor->BackColor = Color::FromArgb(21, 29, 44);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "G U A R D A R   Y   C A R G A R";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 15.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 16);
			lblTitulo->Size = System::Drawing::Size(550, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlContenedor->Controls->Add(lblTitulo);

			lblInfo = gcnew Label();
			lblInfo->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			lblInfo->ForeColor = Color::FromArgb(206, 220, 236);
			lblInfo->BackColor = Color::FromArgb(12, 18, 27);
			lblInfo->BorderStyle = BorderStyle::None;
			lblInfo->Location = Point(28, 58);
			lblInfo->Size = System::Drawing::Size(534, 136);
			lblInfo->Padding = System::Windows::Forms::Padding(12);
			pnlContenedor->Controls->Add(lblInfo);

			lblArchivo = gcnew Label();
			lblArchivo->Font = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Italic);
			lblArchivo->ForeColor = Color::FromArgb(170, 214, 180);
			lblArchivo->Location = Point(30, 202);
			lblArchivo->Size = System::Drawing::Size(530, 44);
			pnlContenedor->Controls->Add(lblArchivo);

			btnGuardar = gcnew Button();
			btnGuardar->Text = "GUARDAR PARTIDA";
			btnGuardar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnGuardar->ForeColor = Color::FromArgb(214, 198, 166);
			btnGuardar->BackColor = Color::FromArgb(18, 27, 38);
			btnGuardar->FlatStyle = FlatStyle::Flat;
			btnGuardar->FlatAppearance->BorderSize = 0;
			btnGuardar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnGuardar->Location = Point(50, 258);
			btnGuardar->Size = System::Drawing::Size(220, 46);
			btnGuardar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnGuardar_Click);
			pnlContenedor->Controls->Add(btnGuardar);

			btnCargar = gcnew Button();
			btnCargar->Text = "CARGAR PARTIDA";
			btnCargar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCargar->ForeColor = Color::FromArgb(226, 232, 240);
			btnCargar->BackColor = Color::FromArgb(18, 27, 38);
			btnCargar->FlatStyle = FlatStyle::Flat;
			btnCargar->FlatAppearance->BorderSize = 0;
			btnCargar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCargar->Location = Point(320, 258);
			btnCargar->Size = System::Drawing::Size(220, 46);
			btnCargar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnCargar_Click);
			pnlContenedor->Controls->Add(btnCargar);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "VOLVER AL JUEGO";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(220, 228, 238);
			btnCerrar->BackColor = Color::FromArgb(14, 22, 30);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 34, 46);
			btnCerrar->Location = Point(210, 340);
			btnCerrar->Size = System::Drawing::Size(170, 40);
			btnCerrar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);

			ActualizarInfo();
		}

		~FormGuardarCargar() {}
	};
}
