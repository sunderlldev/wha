#pragma once

namespace whav2 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class FormLore : public Form {
	private:
		int paginaActual;
		Label^ lblTitulo;
		Label^ lblSubtitulo;
		TextBox^ txtContenido;
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
				lblSubtitulo->Text = "Capítulo I: El Secreto del Trazo Mágico";
				txtContenido->Text =
					"En el mundo de Witch Hat Atelier, la magia no es un don divino ni un privilegio de sangre: "
					"es un arte que se dibuja con tinta especial sobre pergamino mediante sellos mágicos.\r\n\r\n"
					"Sin embargo, este conocimiento es celosamente resguardado por los magos con el fin de proteger "
					"al mundo del uso de la magia prohibida, aquella que altera directamente los cuerpos humanos y puede "
					"causar catástrofes irreversibles.\r\n\r\n"
					"Coco, una humilde niña sin linaje mágico que ayudaba en la sastrería de su madre, descubre accidentalmente "
					"este secreto al observar en secreto al maestro Qifrey mientras dibujaba un conjuro protector.";
				break;
			case 1:
				lblSubtitulo->Text = "Capítulo II: El Atelier de Qifrey";
				txtContenido->Text =
					"Tras un trágico incidente donde la madre de Coco queda petrificada por un libro de magia prohibida "
					"obsequiado por un enigmático Sombrero de Ala Ancha, el maestro Qifrey decide acogerla como su aprendiz "
					"en su apartado atelier.\r\n\r\n"
					"Allí, Coco convive con Agott, una aprendiz talentosa y estricta; Tetia, entusiasta y compasiva; "
					"y Richeh, rebelde e introspectiva.\r\n\r\n"
					"En este nivel, Coco explora los cuartos del atelier, recopila ingredientes de tinta y se esfuerza "
					"por dominar el pulso de su plumilla para algún día deshacer la petrificación de su madre.";
				break;
			case 2:
				lblSubtitulo->Text = "Capítulo III: El Laberinto de Kaln y Tartah";
				txtContenido->Text =
					"Tartah es un joven habitante del pueblo de Kaln que padece de una afección en la vista llamada "
					"'ojos de plata', lo que le dificulta reconocer ciertos colores y trazos.\r\n\r\n"
					"A pesar de no ser considerado un mago prodigio, Tartah posee un profundo ingenio práctico y un corazón "
					"solidario, asistiendo en la botica y la plaza del pueblo.\r\n\r\n"
					"Su mirada revela las calles de Kaln desde la perspectiva de la gente común, los rumores sobre la Asamblea "
					"de Magos y la inquietud provocada por misteriosas sombras que acechan en los callejones.";
				break;
			case 3:
				lblSubtitulo->Text = "Capítulo IV: Coustas y la Prueba del Árbol de Plata";
				txtContenido->Text =
					"Coustas, un enérgico bardo ambulante cuya vida cambia drásticamente tras perder la movilidad en sus piernas "
					"durante un derrumbe, busca desesperadamente recuperar su libertad de viajar y componer música.\r\n\r\n"
					"En su camino hacia el Santuario Ancestral del Gran Árbol de Plata, Coustas se enfrenta a dilemas morales "
					"sobre qué límites cruzar para sanar su cuerpo, tentado por las promesas de los Sombreros de Ala Ancha.\r\n\r\n"
					"En la obra original, este arco conecta las tres perspectivas: la inocencia y dedicación de Coco, la empatía "
					"perseverante de Tartah, y la desgarradora búsqueda de esperanza de Coustas ante las sombras del destino.";
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

			this->Text = "Witch Hat Atelier - Lore de la Obra";
			this->ClientSize = System::Drawing::Size(740, 520);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->StartPosition = FormStartPosition::CenterParent;
			this->BackColor = Color::FromArgb(14, 20, 31);

			pnlFondo = gcnew Panel();
			pnlFondo->Location = Point(15, 15);
			pnlFondo->Size = System::Drawing::Size(710, 490);
			pnlFondo->BackColor = Color::FromArgb(21, 29, 44);
			pnlFondo->BorderStyle = BorderStyle::None;
			this->Controls->Add(pnlFondo);

			lblTitulo = gcnew Label();
			lblTitulo->Text = "W I T C H   H A T   A T E L I E R";
			lblTitulo->Font = gcnew System::Drawing::Font("Segoe UI", 16, FontStyle::Bold);
			lblTitulo->ForeColor = Color::FromArgb(235, 195, 105);
			lblTitulo->Location = Point(20, 16);
			lblTitulo->Size = System::Drawing::Size(670, 32);
			lblTitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlFondo->Controls->Add(lblTitulo);

			lblSubtitulo = gcnew Label();
			lblSubtitulo->Font = gcnew System::Drawing::Font("Segoe UI", 11, FontStyle::Italic);
			lblSubtitulo->ForeColor = Color::FromArgb(120, 190, 255);
			lblSubtitulo->Location = Point(20, 52);
			lblSubtitulo->Size = System::Drawing::Size(670, 24);
			lblSubtitulo->TextAlign = ContentAlignment::MiddleCenter;
			pnlFondo->Controls->Add(lblSubtitulo);

			txtContenido = gcnew TextBox();
			txtContenido->Multiline = true;
			txtContenido->ReadOnly = true;
			txtContenido->TabStop = false;
			txtContenido->ScrollBars = ScrollBars::Vertical;
			txtContenido->BackColor = Color::FromArgb(14, 20, 31);
			txtContenido->ForeColor = Color::FromArgb(230, 238, 248);
			txtContenido->Font = gcnew System::Drawing::Font("Segoe UI", 11, FontStyle::Regular);
			txtContenido->Location = Point(30, 88);
			txtContenido->Size = System::Drawing::Size(650, 315);
			txtContenido->BorderStyle = BorderStyle::None;
			pnlFondo->Controls->Add(txtContenido);

			lblNumeroPagina = gcnew Label();
			lblNumeroPagina->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			lblNumeroPagina->ForeColor = Color::FromArgb(180, 190, 205);
			lblNumeroPagina->Location = Point(280, 422);
			lblNumeroPagina->Size = System::Drawing::Size(150, 30);
			lblNumeroPagina->TextAlign = ContentAlignment::MiddleCenter;
			pnlFondo->Controls->Add(lblNumeroPagina);

			btnAnterior = gcnew Button();
			btnAnterior->Text = "◀ Anterior";
			btnAnterior->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnAnterior->ForeColor = Color::FromArgb(230, 238, 248);
			btnAnterior->BackColor = Color::FromArgb(18, 27, 38);
			btnAnterior->FlatStyle = FlatStyle::Flat;
			btnAnterior->Location = Point(30, 420);
			btnAnterior->Size = System::Drawing::Size(120, 36);
			btnAnterior->Click += gcnew EventHandler(this, &FormLore::btnAnterior_Click);
			pnlFondo->Controls->Add(btnAnterior);

			btnSiguiente = gcnew Button();
			btnSiguiente->Text = "Siguiente ▶";
			btnSiguiente->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);
			btnSiguiente->ForeColor = Color::FromArgb(230, 238, 248);
			btnSiguiente->BackColor = Color::FromArgb(18, 27, 38);
			btnSiguiente->FlatStyle = FlatStyle::Flat;
			btnSiguiente->Location = Point(560, 420);
			btnSiguiente->Size = System::Drawing::Size(120, 36);
			btnSiguiente->Click += gcnew EventHandler(this, &FormLore::btnSiguiente_Click);
			pnlFondo->Controls->Add(btnSiguiente);

			btnCerrar = gcnew Button();
			btnCerrar->Text = "Cerrar";
			btnCerrar->Font = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Regular);
			btnCerrar->ForeColor = Color::FromArgb(210, 215, 225);
			btnCerrar->BackColor = Color::FromArgb(14, 22, 30);
			btnCerrar->FlatStyle = FlatStyle::Flat;
			btnCerrar->Location = Point(440, 420);
			btnCerrar->Size = System::Drawing::Size(100, 36);
			btnCerrar->Click += gcnew EventHandler(this, &FormLore::btnCerrar_Click);
			pnlFondo->Controls->Add(btnCerrar);

			ActualizarPagina();
			this->ActiveControl = btnSiguiente;
		}

		~FormLore() {}
	};
}
