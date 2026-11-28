#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormLore : public Form {
	private:
		int paginaActual;
		Label^ lblCapitulo;
		Label^ lblContenido;
		Label^ lblNumeroPagina;
		Button^ btnAnterior;
		Button^ btnSiguiente;
		Button^ btnCerrar;
		Panel^ pnlFondo;

		void ActualizarPagina() {
			lblNumeroPagina->Text = "Página " + (paginaActual + 1).ToString() + " de 4";
			btnAnterior->Enabled = (paginaActual > 0);
			btnSiguiente->Enabled = (paginaActual < 3);

			switch (paginaActual) {
			case 0:
				lblCapitulo->Text = "CAPÍTULO I: EL SECRETO DEL TRAZO MÁGICO";
				lblContenido->Text =
					"En el mundo de Witch Hat Atelier, la magia no es un don divino ni un privilegio de sangre: "
					"es un arte que se dibuja con tinta especial sobre pergamino mediante sellos mágicos.\n\n"
					"Sin embargo, este conocimiento es celosamente resguardado por los magos con el fin de proteger "
					"al mundo del uso de la magia prohibida, aquella que altera directamente los cuerpos humanos y puede "
					"causar catástrofes irreversibles.\n\n"
					"Coco, una humilde niña sin linaje mágico que ayudaba en la sastrería de su madre, descubre accidentalmente "
					"este secreto al observar al maestro Qifrey mientras dibujaba un conjuro protector.";
				break;
			case 1:
				lblCapitulo->Text = "CAPÍTULO II: EL ATELIER DE QIFREY";
				lblContenido->Text =
					"Tras un trágico incidente donde la madre de Coco queda petrificada por un libro de magia prohibida "
					"obsequiado por un enigmático Sombrero de Ala Ancha, el maestro Qifrey decide acogerla como su aprendiz "
					"en su apartado atelier.\n\n"
					"Allí, Coco convive con Agott, una aprendiz talentosa y estricta; Tetia, entusiasta y compasiva; "
					"y Richeh, rebelde e introspectiva.\n\n"
					"En este nivel, Coco explora los cuartos del atelier, recopila ingredientes de tinta y se esfuerza "
					"por dominar el pulso de su plumilla para algún día deshacer la petrificación de su madre.";
				break;
			case 2:
				lblCapitulo->Text = "CAPÍTULO III: EL LABERINTO DE KALN Y TARTAH";
				lblContenido->Text =
					"Tartah es un joven habitante del pueblo de Kaln que padece de una afección en la vista llamada "
					"'ojos de plata', lo que le dificulta reconocer ciertos colores y trazos.\n\n"
					"A pesar de no ser considerado un mago prodigio, Tartah posee un profundo ingenio práctico y un corazón "
					"solidario, asistiendo en la botica y la plaza del pueblo.\n\n"
					"Su mirada revela las calles de Kaln desde la perspectiva de la gente común, los rumores sobre la Asamblea "
					"de Magos y la inquietud provocada por misteriosas sombras que acechan en los callejones.";
				break;
			case 3:
				lblCapitulo->Text = "CAPÍTULO IV: COUSTAS Y LA PRUEBA DEL ÁRBOL DE PLATA";
				lblContenido->Text =
					"Coustas, un enérgico bardo ambulante cuya vida cambia drásticamente tras perder la movilidad en sus piernas "
					"durante un derrumbe, busca desesperadamente recuperar su libertad de viajar y componer música.\n\n"
					"En su camino hacia el Santuario Ancestral del Gran Árbol de Plata, Coustas se enfrenta a dilemas morales "
					"sobre qué límites cruzar para sanar su cuerpo, tentado por las promesas de los Sombreros de Ala Ancha.\n\n"
					"En la obra original, este arco conecta las tres perspectivas: la inocencia y dedicación de Coco, la empatía "
					"perseverante de Tartah, y la búsqueda de esperanza de Coustas ante las sombras del destino.";
				break;
			}
		}

		void btnAnterior_Click(Object^ sender, EventArgs^ e) {
			if (paginaActual > 0) {
				paginaActual--;
				ActualizarPagina();
			}
		}

		void btnSiguiente_Click(Object^ sender, EventArgs^ e) {
			if (paginaActual < 3) {
				paginaActual++;
				ActualizarPagina();
			}
		}

		void btnCerrar_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

	public:
		FormLore() {
			paginaActual = 0;

			this->Text = "Lore de la Obra - Witch Hat Atelier";
			this->ClientSize = System::Drawing::Size(740, 490);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(9, 14, 21);

			pnlFondo = gcnew Panel();
			pnlFondo->Location = Point(15, 15);
			pnlFondo->Size = System::Drawing::Size(710, 460);
			pnlFondo->BackColor = Color::FromArgb(10, 16, 24);
			pnlFondo->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlFondo);

			lblCapitulo = gcnew Label();
			lblCapitulo->Font = gcnew System::Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
			lblCapitulo->ForeColor = Color::FromArgb(245, 245, 245);
			lblCapitulo->Location = Point(30, 22);
			lblCapitulo->Size = System::Drawing::Size(650, 28);
			lblCapitulo->TextAlign = ContentAlignment::MiddleLeft;
			pnlFondo->Controls->Add(lblCapitulo);

			Panel^ linea = gcnew Panel();
			linea->Location = Point(30, 56);
			linea->Size = System::Drawing::Size(650, 1);
			linea->BackColor = Color::FromArgb(70, 214, 198, 166);
			pnlFondo->Controls->Add(linea);

			lblContenido = gcnew Label();
			lblContenido->ForeColor = Color::FromArgb(242, 244, 247);
			lblContenido->BackColor = Color::Transparent;
			lblContenido->Font = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
			lblContenido->Location = Point(30, 74);
			lblContenido->Size = System::Drawing::Size(650, 305);
			lblContenido->TextAlign = ContentAlignment::TopLeft;
			pnlFondo->Controls->Add(lblContenido);

			lblNumeroPagina = gcnew Label();
			lblNumeroPagina->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			lblNumeroPagina->ForeColor = Color::FromArgb(180, 190, 205);
			lblNumeroPagina->Location = Point(235, 396);
			lblNumeroPagina->Size = System::Drawing::Size(160, 36);
			lblNumeroPagina->TextAlign = ContentAlignment::MiddleCenter;
			pnlFondo->Controls->Add(lblNumeroPagina);

			btnAnterior = gcnew Button();
			btnAnterior->Text = "Anterior";
			btnAnterior->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnAnterior->ForeColor = Color::FromArgb(230, 238, 248);
			btnAnterior->BackColor = Color::FromArgb(18, 27, 38);
			btnAnterior->FlatStyle = FlatStyle::Flat;
			btnAnterior->FlatAppearance->BorderSize = 0;
			btnAnterior->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnAnterior->Cursor = Cursors::Hand;
			btnAnterior->Location = Point(30, 396);
			btnAnterior->Size = System::Drawing::Size(120, 36);
			btnAnterior->Click += gcnew EventHandler(this, &FormLore::btnAnterior_Click);
			pnlFondo->Controls->Add(btnAnterior);

			btnSiguiente = gcnew Button();
			btnSiguiente->Text = "Siguiente";
			btnSiguiente->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnSiguiente->ForeColor = Color::FromArgb(230, 238, 248);
			btnSiguiente->BackColor = Color::FromArgb(18, 27, 38);
			btnSiguiente->FlatStyle = FlatStyle::Flat;
			btnSiguiente->FlatAppearance->BorderSize = 0;
			btnSiguiente->FlatAppearance->MouseOverBackColor = Color::FromArgb(28, 40, 54);
			btnSiguiente->Cursor = Cursors::Hand;
			btnSiguiente->Location = Point(560, 396);
			btnSiguiente->Size = System::Drawing::Size(120, 36);
			btnSiguiente->Click += gcnew EventHandler(this, &FormLore::btnSiguiente_Click);
			pnlFondo->Controls->Add(btnSiguiente);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(210, 215, 225);
			btnCerrar->BackColor = Color::FromArgb(14, 22, 30);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->FlatAppearance->BorderSize = 0;
			btnCerrar->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 34, 46);
			btnCerrar->Cursor = Cursors::Hand;
			btnCerrar->Location = Point(440, 396);
			btnCerrar->Size = System::Drawing::Size(105, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormLore::btnCerrar_Click);
			pnlFondo->Controls->Add(btnCerrar);

			ActualizarPagina();
			this->ActiveControl = btnSiguiente;
		}

		~FormLore() {}
	};
}
