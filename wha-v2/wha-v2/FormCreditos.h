#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormCreditos : public Form {
	private:
		Label^ lblTitulo;
		TextBox^ txtCreditos;
		Button^ btnCerrar;
		Panel^ pnlContenedor;

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormCreditos() {
			this->Text = "Witch Hat Atelier - Créditos del Proyecto";
			this->ClientSize = System::Drawing::Size(640, 480);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(610, 450);
			pnlContenedor->BackColor = Color::FromArgb(21, 29, 44);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "C R É D I T O S";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 16, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 18);
			lblTitulo->Size = System::Drawing::Size(570, 30);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlContenedor->Controls->Add(lblTitulo);

			txtCreditos = gcnew TextBox();
			txtCreditos->Multiline = true;
			txtCreditos->ReadOnly = true;
			txtCreditos->TabStop = false;
			txtCreditos->ScrollBars = ScrollBars::Vertical;
			txtCreditos->BackColor = Color::FromArgb(14, 20, 31);
			txtCreditos->ForeColor = Color::FromArgb(230, 238, 248);
			txtCreditos->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			txtCreditos->Location = Point(25, 60);
			txtCreditos->Size = System::Drawing::Size(560, 315);
			txtCreditos->BorderStyle = BorderStyle::None;
			txtCreditos->Text =
				"UNIVERSIDAD PERUANA DE CIENCIAS APLICADAS (UPC)\r\n"
				"Facultad de Ingeniería - Ciencias de la Computación / Ingeniería de Software\r\n"
				"Curso: Algoritmos (1ACC0265) - Ciclo Académico 2026-20\r\n\r\n"
				"EQUIPO DE DESARROLLO:\r\n"
				"  • Juan José Blas Cáceres (U20261B980)\r\n"
				"  • Kiana Valentina Arancibia Romero (U202619105)\r\n\r\n"
				"DOCENTES DEL CURSO:\r\n"
				"  • Johann Cristian Berny Pérez Rojas\r\n"
				"  • Henry Antonio Mendoza Puerta\r\n"
				"  • Joel Hernán Flores Martínez\r\n"
				"  • Edson Mendiolaza Cornejo\r\n"
				"  • Flor Cagniy Cárdenas Mariño\r\n"
				"  • Jorge Eduardo Díaz Suárez\r\n\r\n"
				"OBRA ORIGINAL ADAPTADA:\r\n"
				"  • 'Witch Hat Atelier' (Tongari Bōshi no Atorie)\r\n"
				"  • Creadora y dibujante: Kamome Shirahama\r\n"
				"  • Publicado por Kōdansha (Morning Two)\r\n\r\n"
				"MOTOR Y TECNOLOGÍAS UTILIZADAS:\r\n"
				"  • C++/CLI con Windows Forms y GDI+\r\n"
				"  • Doble Búfer (BufferedGraphicsManager) a 40 FPS\r\n"
				"  • Animaciones con Spritesheets de 24x32 px\r\n"
				"  • Motor de Tilemap dinámico con matrices de caracteres\r\n"
				"  • Arquitectura pura Orientada a Objetos (POO)";
			pnlContenedor->Controls->Add(txtCreditos);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(230, 238, 248);
			btnCerrar->BackColor = Color::FromArgb(18, 27, 38);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCerrar->Location = Point(235, 392);
			btnCerrar->Size = System::Drawing::Size(140, 38);
			btnCerrar->Click += gcnew EventHandler(this, &FormCreditos::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);
			this->ActiveControl = btnCerrar;
		}

		~FormCreditos() {}
	};
}
