#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::IO;

	public ref class FormReflexion : public Form {
	private:
		Label^ lblTitulo;
		Label^ lblPregunta;
		TextBox^ txtRespuesta;
		Button^ btnGuardar;
		Panel^ pnlContenedor;

		void btnGuardar_Click(Object^ sender, EventArgs^ e) {
			if (txtRespuesta->Text->Trim()->Length < 10) {
				MessageBox::Show("Por favor escribe tu reflexión sobre los tres personajes antes de continuar.", "Witch Hat Atelier", MessageBoxButtons::OK, MessageBoxIcon::Information);
				return;
			}
			StreamWriter^ sw = nullptr;
			try {
				sw = gcnew StreamWriter("reflexion_final.txt", false, System::Text::Encoding::UTF8);
				sw->WriteLine("=== PREGUNTA REFLEXIVA - WITCH HAT ATELIER (HITO 3) ===");
				sw->WriteLine("Fecha: " + DateTime::Now.ToString("dd/MM/yyyy HH:mm:ss"));
				sw->WriteLine();
				sw->WriteLine("Pregunta: ¿Con cuál de las tres perspectivas te sentiste más identificado y cómo cambió tu interpretación de los hechos?");
				sw->WriteLine();
				sw->WriteLine("Respuesta del Jugador:");
				sw->WriteLine(txtRespuesta->Text);
				sw->Close();
				sw = nullptr;
				MessageBox::Show("¡Tu reflexión ha sido guardada exitosamente en 'reflexion_final.txt'!\n\nHas completado los tres niveles de Witch Hat Atelier.", "Fin de la Historia", MessageBoxButtons::OK, MessageBoxIcon::Information);
				this->Close();
			}
			catch (Exception^ ex) {
				if (sw != nullptr) sw->Close();
				MessageBox::Show("Error al guardar: " + ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}

	public:
		FormReflexion() {
			this->Text = "Pregunta Reflexiva Final - Hito 3";
			this->ClientSize = System::Drawing::Size(720, 540);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(690, 510);
			pnlContenedor->BackColor = Color::FromArgb(21, 29, 44);
			pnlContenedor->BorderStyle = BorderStyle::FixedSingle;
			this->Controls->Add(pnlContenedor);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "R E F L E X I Ó N   F I N A L   D E   L A   H I S T O R I A";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 14, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 16);
			lblTitulo->Size = System::Drawing::Size(650, 28);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlContenedor->Controls->Add(lblTitulo);

			lblPregunta = gcnew Label();
			lblPregunta->Text =
				"Has recorrido los tres niveles y experimentado la historia a través de tres miradas distintas:\n"
				"1. Coco: La inocencia, el asombro y el peso de un secreto mágico no elegido.\n"
				"2. Tartah: La perseverancia del habitante común que lucha contra sus propias limitaciones físicas.\n"
				"3. Coustas: El dolor desgarrador ante la pérdida y la tentación de cruzar líneas morales por esperanza.\n\n"
				"¿Con cuál de estos tres personajes te sentiste más identificado y de qué manera cambió tu lectura de los hechos al pasar de una perspectiva a otra?";
			lblPregunta->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
			lblPregunta->ForeColor = Color::FromArgb(200, 215, 235);
			lblPregunta->Location = Point(25, 50);
			lblPregunta->Size = System::Drawing::Size(640, 146);
			pnlContenedor->Controls->Add(lblPregunta);

			txtRespuesta = gcnew TextBox();
			txtRespuesta->Multiline = true;
			txtRespuesta->ScrollBars = ScrollBars::Vertical;
			txtRespuesta->BackColor = Color::FromArgb(14, 20, 31);
			txtRespuesta->ForeColor = Color::FromArgb(240, 246, 252);
			txtRespuesta->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			txtRespuesta->Location = Point(25, 204);
			txtRespuesta->Size = System::Drawing::Size(640, 224);
			txtRespuesta->BorderStyle = BorderStyle::FixedSingle;
			pnlContenedor->Controls->Add(txtRespuesta);

			btnGuardar = gcnew Button();
			btnGuardar->Text = "Guardar Reflexión y Concluir";
			btnGuardar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnGuardar->ForeColor = Color::FromArgb(240, 246, 252);
			btnGuardar->BackColor = Color::FromArgb(38, 70, 52);
			btnGuardar->FlatStyle = FlatStyle::Flat;
			btnGuardar->Location = Point(230, 446);
			btnGuardar->Size = System::Drawing::Size(230, 40);
			btnGuardar->Click += gcnew EventHandler(this, &FormReflexion::btnGuardar_Click);
			pnlContenedor->Controls->Add(btnGuardar);
		}

		~FormReflexion() {}
	};
}
