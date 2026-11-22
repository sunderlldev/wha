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
		Button^ btnInventario;
		Button^ btnPersonajes;
		Button^ btnGuardar;
		Button^ btnControles;
		Button^ btnMenu;

		void btnContinuar_Click(Object^ sender, EventArgs^ e) {
			accion = 0;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
		void btnInventario_Click(Object^ sender, EventArgs^ e) {
			accion = 1;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
		void btnPersonajes_Click(Object^ sender, EventArgs^ e) {
			accion = 2;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
		void btnGuardar_Click(Object^ sender, EventArgs^ e) {
			accion = 3;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
		void btnControles_Click(Object^ sender, EventArgs^ e) {
			accion = 4;
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
		void btnMenu_Click(Object^ sender, EventArgs^ e) {
			accion = 5;
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

		Button^ CrearBoton(String^ texto, int y, bool primario) {
			Button^ boton = gcnew Button();
			boton->Text = texto;
			boton->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, primario ? FontStyle::Bold : FontStyle::Regular);
			boton->ForeColor = primario ? Color::FromArgb(214, 198, 166) : Color::FromArgb(226, 232, 240);
			boton->BackColor = Color::FromArgb(18, 27, 38);
			boton->FlatStyle = FlatStyle::Flat;
			boton->FlatAppearance->BorderSize = 0;
			boton->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			boton->Cursor = Cursors::Hand;
			boton->Location = Point(155, y);
			boton->Size = System::Drawing::Size(220, 36);
			panelContenedor->Controls->Add(boton);
			return boton;
		}

	public:
		FormPausa(String^ resumen) {
			accion = 0;
			this->Text = "Pausa - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(560, 600);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			panelContenedor = gcnew Panel();
			panelContenedor->Dock = DockStyle::Fill;
			panelContenedor->BackColor = Color::FromArgb(10, 16, 24);
			this->Controls->Add(panelContenedor);

			Label^ lblTitulo = gcnew Label();
			lblTitulo->Text = "PAUSA";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 18.0f, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(214, 198, 166);
			lblTitulo->Location = Point(30, 22);
			lblTitulo->Size = System::Drawing::Size(500, 32);
			lblTitulo->TextAlign = ContentAlignment::MiddleLeft;
			panelContenedor->Controls->Add(lblTitulo);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(30, 60);
			linea->Size = System::Drawing::Size(500, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			panelContenedor->Controls->Add(linea);

			Label^ lblResumen = gcnew Label();
			lblResumen->Text = resumen;
			lblResumen->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			lblResumen->ForeColor = Color::FromArgb(196, 210, 224);
			lblResumen->Location = Point(30, 76);
			lblResumen->Size = System::Drawing::Size(500, 96);
			lblResumen->Padding = System::Windows::Forms::Padding(0, 4, 0, 0);
			panelContenedor->Controls->Add(lblResumen);

			btnContinuar = CrearBoton("CONTINUAR AVENTURA", 190, true);
			btnContinuar->Click += gcnew EventHandler(this, &FormPausa::btnContinuar_Click);
			btnInventario = CrearBoton("INVENTARIO", 236, false);
			btnInventario->Click += gcnew EventHandler(this, &FormPausa::btnInventario_Click);
			btnPersonajes = CrearBoton("PERSONAJES", 282, false);
			btnPersonajes->Click += gcnew EventHandler(this, &FormPausa::btnPersonajes_Click);
			btnGuardar = CrearBoton("GUARDAR / CARGAR", 328, false);
			btnGuardar->Click += gcnew EventHandler(this, &FormPausa::btnGuardar_Click);
			btnControles = CrearBoton("CONTROLES", 374, false);
			btnControles->Click += gcnew EventHandler(this, &FormPausa::btnControles_Click);
			btnMenu = CrearBoton("VOLVER AL MENÚ", 420, false);
			btnMenu->Click += gcnew EventHandler(this, &FormPausa::btnMenu_Click);
			this->ActiveControl = btnContinuar;
		}

		int getAccion() { return accion; }

		~FormPausa() {}
	};
}
