#pragma once

#include "Nivel1.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::IO;

	public ref class FormGuardarCargar : public Form {
	private:
		Nivel1* controlador;
		Label^ lblInfo;
		Label^ lblArchivo;
		Button^ btnGuardar;
		Button^ btnCargar;
		Button^ btnCerrar;
		Panel^ pnlContenedor;

		void ActualizarInfo() {
			if (controlador != nullptr) controlador->asegurarPartidaRegenerativa();
			lblInfo->Text = controlador->getResumenPartida();
			String^ rutaGuardado = ObtenerRutaGuardadoComun("partida_guardada.txt");
			if (File::Exists(rutaGuardado)) {
				DateTime fecha = File::GetLastWriteTime(rutaGuardado);
				lblArchivo->Text = "Archivo disponible: " + rutaGuardado + "  (" + fecha.ToString("dd/MM/yyyy HH:mm") + ")";
				lblArchivo->ForeColor = Color::FromArgb(182, 214, 188);
			}
			else {
				lblArchivo->Text = "No hay ninguna partida guardada en este equipo.";
				lblArchivo->ForeColor = Color::FromArgb(182, 188, 198);
			}
		}

		void btnGuardar_Click(Object^ sender, EventArgs^ e) {
			if (controlador->guardarPartida()) {
				MessageBox::Show("Partida guardada correctamente.\n\n" + controlador->getResumenPartida(), "Guardar partida", MessageBoxButtons::OK, MessageBoxIcon::Information);
				ActualizarInfo();
			}
			else {
				MessageBox::Show("No se pudo escribir el archivo de partida.", "Guardar partida", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}

		void btnCargar_Click(Object^ sender, EventArgs^ e) {
			if (controlador != nullptr) controlador->asegurarPartidaRegenerativa();
			if (!File::Exists(ObtenerRutaGuardadoComun("partida_guardada.txt"))) {
				MessageBox::Show("No se pudo regenerar el archivo de partida.", "Cargar partida", MessageBoxButtons::OK, MessageBoxIcon::Warning);
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
		FormGuardarCargar(Nivel1* ctrl) {
			controlador = ctrl;

			this->Text = "Cargar o Guardar Partida";
			this->ClientSize = System::Drawing::Size(540, 360);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(510, 330);
			pnlContenedor->BackColor = Color::FromArgb(10, 16, 24);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			lblInfo = gcnew Label();
			lblInfo->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			lblInfo->ForeColor = Color::FromArgb(234, 240, 246);
			lblInfo->BackColor = Color::FromArgb(14, 22, 32);
			lblInfo->BorderStyle = BorderStyle::None;
			lblInfo->Location = Point(26, 26);
			lblInfo->Size = System::Drawing::Size(458, 128);
			lblInfo->Padding = System::Windows::Forms::Padding(14);
			pnlContenedor->Controls->Add(lblInfo);

			lblArchivo = gcnew Label();
			lblArchivo->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			lblArchivo->ForeColor = Color::FromArgb(182, 214, 188);
			lblArchivo->Location = Point(26, 166);
			lblArchivo->Size = System::Drawing::Size(458, 32);
			lblArchivo->TextAlign = ContentAlignment::MiddleLeft;
			pnlContenedor->Controls->Add(lblArchivo);

			btnGuardar = gcnew Button();
			btnGuardar->Text = "Guardar partida";
			btnGuardar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnGuardar->ForeColor = Color::FromArgb(214, 198, 166);
			btnGuardar->BackColor = Color::FromArgb(18, 27, 38);
			btnGuardar->FlatStyle = FlatStyle::Flat;
			btnGuardar->FlatAppearance->BorderSize = 0;
			btnGuardar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnGuardar->Cursor = Cursors::Hand;
			btnGuardar->Location = Point(26, 212);
			btnGuardar->Size = System::Drawing::Size(218, 42);
			btnGuardar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnGuardar_Click);
			pnlContenedor->Controls->Add(btnGuardar);

			btnCargar = gcnew Button();
			btnCargar->Text = "Cargar partida";
			btnCargar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			btnCargar->ForeColor = Color::FromArgb(236, 240, 245);
			btnCargar->BackColor = Color::FromArgb(18, 27, 38);
			btnCargar->FlatStyle = FlatStyle::Flat;
			btnCargar->FlatAppearance->BorderSize = 0;
			btnCargar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCargar->Cursor = Cursors::Hand;
			btnCargar->Location = Point(266, 212);
			btnCargar->Size = System::Drawing::Size(218, 42);
			btnCargar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnCargar_Click);
			pnlContenedor->Controls->Add(btnCargar);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Volver";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(210, 218, 228);
			btnCerrar->BackColor = Color::FromArgb(14, 22, 30);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 34, 46);
			btnCerrar->Cursor = Cursors::Hand;
			btnCerrar->Location = Point(185, 272);
			btnCerrar->Size = System::Drawing::Size(140, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormGuardarCargar::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);

			ActualizarInfo();
		}

		~FormGuardarCargar() {}
	};
}
