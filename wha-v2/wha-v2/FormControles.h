#pragma once

#include "FormPersonajes.h"

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormControles : public Form {
	private:
		Button^ btnCerrar;
		Button^ btnPersonajes;
		Panel^ pnlContenedor;
		array<String^>^ nombresPersonajes;
		array<String^>^ rolesPersonajes;
		array<String^>^ descripcionesPersonajes;
		array<String^>^ confianzasPersonajes;

		Label^ CrearLabel(String^ texto, int x, int y, int ancho, int alto, float tam, FontStyle estilo, Color color) {
			Label^ lbl = gcnew Label();
			lbl->Text = texto;
			lbl->Font = gcnew System::Drawing::Font("Segoe UI", tam, estilo);
			lbl->ForeColor = color;
			lbl->BackColor = Color::Transparent;
			lbl->Location = Point(x, y);
			lbl->Size = System::Drawing::Size(ancho, alto);
			lbl->TextAlign = ContentAlignment::MiddleLeft;
			pnlContenedor->Controls->Add(lbl);
			return lbl;
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

		void btnPersonajes_Click(Object^ sender, EventArgs^ e) {
			if (nombresPersonajes == nullptr) return;
			FormPersonajes^ frm = gcnew FormPersonajes(nombresPersonajes, rolesPersonajes, descripcionesPersonajes, confianzasPersonajes);
			frm->ShowDialog(this);
		}

	public:
		void ConfigurarPersonajes(array<String^>^ nombres, array<String^>^ roles, array<String^>^ descripciones, array<String^>^ confianzas) {
			nombresPersonajes = nombres;
			rolesPersonajes = roles;
			descripcionesPersonajes = descripciones;
			confianzasPersonajes = confianzas;
			btnPersonajes->Visible = true;
		}

		FormControles() {
			this->Text = "Controles e Instrucciones";
			this->ClientSize = System::Drawing::Size(780, 560);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlContenedor = gcnew Panel();
			pnlContenedor->Location = Point(15, 15);
			pnlContenedor->Size = System::Drawing::Size(750, 530);
			pnlContenedor->BackColor = Color::FromArgb(10, 16, 24);
			pnlContenedor->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlContenedor);

			Label^ titulo = CrearLabel("CONTROLES", 28, 16, 694, 34, 17.0f, FontStyle::Bold, Color::FromArgb(214, 198, 166));
			Panel^ linea = gcnew Panel();
			linea->Location = Point(30, 56);
			linea->Size = System::Drawing::Size(690, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			pnlContenedor->Controls->Add(linea);

			Color colorClave = Color::FromArgb(214, 198, 166);
			Color colorTexto = Color::FromArgb(206, 218, 230);
			Color colorSeccion = Color::FromArgb(226, 232, 240);
			Color colorTenue = Color::FromArgb(156, 174, 190);

			CrearLabel("MOVIMIENTO", 30, 74, 340, 24, 11.0f, FontStyle::Bold, colorSeccion);
			array<String^>^ teclasMov = gcnew array<String^>{ "W  /  ↑", "S  /  ↓", "A  /  ←", "D  /  →" };
			array<String^>^ descMov = gcnew array<String^>{ "Mover arriba", "Mover abajo", "Mover izquierda", "Mover derecha" };
			for (int i = 0; i < 4; i++) {
				CrearLabel(teclasMov[i], 34, 104 + i * 27, 110, 22, 10.0f, FontStyle::Bold, colorClave);
				CrearLabel(descMov[i], 150, 104 + i * 27, 220, 22, 10.0f, FontStyle::Regular, colorTexto);
			}

			CrearLabel("ACCIONES", 30, 222, 340, 24, 11.0f, FontStyle::Bold, colorSeccion);
			array<String^>^ teclasAcc = gcnew array<String^>{ "Espacio / Z", "1 · 2 · 3 · 4", "W / S", "I", "P", "M", "ESC" };
			array<String^>^ descAcc = gcnew array<String^>{
				"Interactuar, leer, recoger y avanzar diálogos",
				"Elegir una respuesta",
				"Mover la selección de respuesta",
				"Abrir el inventario",
				"Personajes interactuados y confianza",
				"Música on/off (inicia apagada)",
				"Pausa"
			};
			for (int i = 0; i < 7; i++) {
				CrearLabel(teclasAcc[i], 34, 252 + i * 27, 118, 22, 10.0f, FontStyle::Bold, colorClave);
				CrearLabel(descAcc[i], 158, 252 + i * 27, 230, 22, 9.5f, FontStyle::Regular, colorTexto);
			}

			CrearLabel("FLUJO DEL NIVEL 1", 410, 74, 330, 24, 11.0f, FontStyle::Bold, colorSeccion);
			array<String^>^ pasos = gcnew array<String^>{
				"1. Habla con Qifrey y pide el Manto de Aprendiz.",
				"2. Mueve las cajas de la torre de Agott hasta encontrar el Grimorio de Trazos.",
				"3. Encuentra el pozo escondido entre las cajas y baja al sótano.",
				"4. Habla con Richeh y acepta rescatar a Myrphon.",
				"5. Cruza el puente al este y atrapa a Myrphon en el laberinto.",
				"6. Vuelve con Richeh para recibir la Pluma Térmica.",
				"7. Rompe la pared rocosa de la cueva y recoge la Fibra de Árbol.",
				"8. Reúne la Tinta mágica y confecciona la Capa con Qifrey."
			};
			for (int i = 0; i < 8; i++) {
				CrearLabel(pasos[i], 410, 104 + i * 42, 330, 40, 9.5f, FontStyle::Regular, colorTenue);
			}

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Entendido";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnCerrar->ForeColor = Color::FromArgb(230, 238, 248);
			btnCerrar->BackColor = Color::FromArgb(18, 27, 38);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnCerrar->Location = Point(270, 472);
			btnCerrar->Size = System::Drawing::Size(140, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormControles::btnCerrar_Click);
			pnlContenedor->Controls->Add(btnCerrar);

			btnPersonajes = gcnew Button();
			btnPersonajes->Text = "PERSONAJES";
			btnPersonajes->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			btnPersonajes->ForeColor = Color::FromArgb(214, 198, 166);
			btnPersonajes->BackColor = Color::FromArgb(18, 27, 38);
			btnPersonajes->FlatStyle = FlatStyle::Flat;
			btnPersonajes->FlatAppearance->BorderSize = 0;
			btnPersonajes->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnPersonajes->Location = Point(425, 472);
			btnPersonajes->Size = System::Drawing::Size(140, 36);
			btnPersonajes->Visible = false;
			btnPersonajes->Click += gcnew EventHandler(this, &FormControles::btnPersonajes_Click);
			pnlContenedor->Controls->Add(btnPersonajes);

			this->ActiveControl = btnCerrar;
		}

		~FormControles() {}
	};
}
