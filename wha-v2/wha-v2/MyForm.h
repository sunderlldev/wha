#pragma once

#include "ControladorJuego.h"
#include "Configuracion.h"
#include "Utilidades.h"
#include "FormLore.h"
#include "FormControles.h"
#include "FormCreditos.h"
#include "FormGuardarCargar.h"
#include "FormInventario.h"
#include "FormPersonajes.h"
#include "FormPausa.h"
#include "FormFinNivel.h"
#include "FormTrollface.h"
#include "FormMinijuego.h"
#include "Nivel2.h"

namespace whav2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Drawing::Imaging;

	public enum class EstadoApp {
		MENU_PRINCIPAL,
		EN_JUEGO
	};

	public ref class MyForm : public System::Windows::Forms::Form {
	private:
		Nivel2* nivel2;
	private:
		EstadoApp estadoActual;
		ControladorJuego* controlador;
		Graphics^ gPantalla;
		BufferedGraphicsContext^ espacioBuffer;
		BufferedGraphics^ bufferGrafico;
		bool bufferListo;

		System::Windows::Forms::Timer^ timerJuego;
		System::ComponentModel::IContainer^ components;

		Panel^ pnlMenu;
		Bitmap^ fondoMenu;
		Label^ lblTituloMenu;
		Label^ lblSubtituloMenu;
		Label^ lblCitaMenu;
		Label^ lblPieMenu;
		Button^ btnJugar;
		Button^ btnControles;
		Button^ btnLore;
		Button^ btnCreditos;
		Button^ btnCargar;
		Button^ btnSalir;
		Button^ btnHudControles;
		Button^ btnHudInventario;

		bool teclaArriba;
		bool teclaAbajo;
		bool teclaIzquierda;
		bool teclaDerecha;
		bool teclaAccion;

		static GraphicsPath^ RutaRedondeada(Rectangle rect, int radio) {
			GraphicsPath^ ruta = gcnew GraphicsPath();
			int diametro = radio * 2;
			ruta->AddArc(rect.Left, rect.Top, diametro, diametro, 180, 90);
			ruta->AddArc(rect.Right - diametro, rect.Top, diametro, diametro, 270, 90);
			ruta->AddArc(rect.Right - diametro, rect.Bottom - diametro, diametro, diametro, 0, 90);
			ruta->AddArc(rect.Left, rect.Bottom - diametro, diametro, diametro, 90, 90);
			ruta->CloseFigure();
			return ruta;
		}

		void DibujarPino(Graphics^ g, int x, int baseY, int alto) {
			SolidBrush^ pincel = gcnew SolidBrush(Color::FromArgb(255, 10, 24, 18));
			g->FillPolygon(pincel, gcnew array<Point>{ Point(x, baseY - alto), Point(x - alto / 3, baseY), Point(x + alto / 3, baseY) });
			g->FillRectangle(pincel, x - 2, baseY - 6, 4, 12);
			delete pincel;
		}

		void GenerarFondoMenu() {
			Bitmap^ bmp = gcnew Bitmap(960, 600, PixelFormat::Format32bppArgb);
			Graphics^ g = Graphics::FromImage(bmp);
			g->SmoothingMode = SmoothingMode::AntiAlias;

			LinearGradientBrush^ cielo = gcnew LinearGradientBrush(Rectangle(0, 0, 960, 600), Color::FromArgb(6, 12, 21), Color::FromArgb(14, 38, 30), LinearGradientMode::Vertical);
			g->FillRectangle(cielo, 0, 0, 960, 600);
			delete cielo;

			Random^ rnd = gcnew Random(20261118);
			for (int i = 0; i < 150; i++) {
				int x = rnd->Next(0, 960);
				int y = rnd->Next(0, 390);
				int tam = (rnd->Next(0, 5) == 0) ? 2 : 1;
				int alpha = rnd->Next(50, 190);
				SolidBrush^ estrella = gcnew SolidBrush(Color::FromArgb(alpha, 230, 240, 255));
				g->FillRectangle(estrella, x, y, tam, tam);
				delete estrella;
			}

			SolidBrush^ halo1 = gcnew SolidBrush(Color::FromArgb(12, 246, 226, 164));
			SolidBrush^ halo2 = gcnew SolidBrush(Color::FromArgb(18, 246, 226, 164));
			SolidBrush^ halo3 = gcnew SolidBrush(Color::FromArgb(26, 246, 226, 164));
			g->FillEllipse(halo1, 660, 10, 220, 220);
			g->FillEllipse(halo2, 680, 30, 180, 180);
			g->FillEllipse(halo3, 700, 50, 140, 140);
			delete halo1;
			delete halo2;
			delete halo3;
			SolidBrush^ luna = gcnew SolidBrush(Color::FromArgb(235, 232, 220, 172));
			g->FillEllipse(luna, 722, 72, 96, 96);
			delete luna;
			SolidBrush^ crater = gcnew SolidBrush(Color::FromArgb(60, 150, 140, 110));
			g->FillEllipse(crater, 744, 96, 18, 14);
			g->FillEllipse(crater, 774, 124, 12, 10);
			g->FillEllipse(crater, 760, 82, 8, 8);
			delete crater;

			SolidBrush^ bosqueLejano = gcnew SolidBrush(Color::FromArgb(255, 9, 21, 16));
			g->FillPolygon(bosqueLejano, gcnew array<Point>{
				Point(0, 430), Point(110, 360), Point(240, 425), Point(380, 350), Point(530, 430),
				Point(680, 365), Point(820, 420), Point(960, 370), Point(960, 600), Point(0, 600)
			});
			delete bosqueLejano;

			SolidBrush^ bosqueCercano = gcnew SolidBrush(Color::FromArgb(255, 12, 30, 22));
			g->FillPolygon(bosqueCercano, gcnew array<Point>{
				Point(0, 480), Point(120, 425), Point(260, 480), Point(400, 430), Point(540, 490),
				Point(690, 440), Point(830, 490), Point(960, 450), Point(960, 600), Point(0, 600)
			});
			delete bosqueCercano;

			SolidBrush^ suelo = gcnew SolidBrush(Color::FromArgb(255, 8, 17, 13));
			g->FillRectangle(suelo, 0, 520, 960, 80);
			delete suelo;

			SolidBrush^ colina = gcnew SolidBrush(Color::FromArgb(255, 18, 38, 28));
			g->FillEllipse(colina, 40, 430, 460, 240);
			delete colina;

			SolidBrush^ pared = gcnew SolidBrush(Color::FromArgb(255, 35, 42, 51));
			SolidBrush^ paredOscura = gcnew SolidBrush(Color::FromArgb(255, 24, 29, 36));
			SolidBrush^ techo = gcnew SolidBrush(Color::FromArgb(255, 58, 42, 34));
			SolidBrush^ techoOscuro = gcnew SolidBrush(Color::FromArgb(255, 40, 28, 23));
			Pen^ contorno = gcnew Pen(Color::FromArgb(255, 8, 12, 16), 2.0f);

			g->FillRectangle(paredOscura, 126, 342, 206, 128);
			g->FillRectangle(pared, 120, 335, 210, 130);
			g->DrawRectangle(contorno, 120, 335, 210, 130);
			g->FillPolygon(techo, gcnew array<Point>{ Point(98, 342), Point(225, 232), Point(352, 342) });
			g->DrawPolygon(contorno, gcnew array<Point>{ Point(98, 342), Point(225, 232), Point(352, 342) });

			g->FillRectangle(paredOscura, 336, 267, 64, 200);
			g->FillRectangle(pared, 330, 260, 72, 205);
			g->DrawRectangle(contorno, 330, 260, 72, 205);
			g->FillPolygon(techo, gcnew array<Point>{ Point(314, 266), Point(366, 174), Point(418, 266) });
			g->DrawPolygon(contorno, gcnew array<Point>{ Point(314, 266), Point(366, 174), Point(418, 266) });

			g->FillRectangle(techoOscuro, 286, 250, 22, 78);
			g->DrawRectangle(contorno, 286, 250, 22, 78);

			SolidBrush^ ventana = gcnew SolidBrush(Color::FromArgb(240, 242, 200, 106));
			g->FillRectangle(ventana, 150, 370, 30, 40);
			g->FillRectangle(ventana, 205, 370, 30, 40);
			g->FillRectangle(ventana, 260, 370, 30, 40);
			g->FillRectangle(ventana, 350, 305, 30, 40);
			delete ventana;
			Pen^ marco = gcnew Pen(Color::FromArgb(255, 55, 36, 20), 2.0f);
			g->DrawLine(marco, 165, 370, 165, 410);
			g->DrawLine(marco, 150, 390, 180, 390);
			g->DrawLine(marco, 220, 370, 220, 410);
			g->DrawLine(marco, 205, 390, 235, 390);
			g->DrawLine(marco, 275, 370, 275, 410);
			g->DrawLine(marco, 260, 390, 290, 390);
			g->DrawLine(marco, 365, 305, 365, 345);
			g->DrawLine(marco, 350, 325, 380, 325);
			delete marco;

			SolidBrush^ puerta = gcnew SolidBrush(Color::FromArgb(255, 24, 19, 15));
			g->FillRectangle(puerta, 212, 415, 38, 50);
			g->DrawRectangle(contorno, 212, 415, 38, 50);
			SolidBrush^ pomo = gcnew SolidBrush(Color::FromArgb(255, 220, 180, 80));
			g->FillEllipse(pomo, 240, 440, 5, 5);
			delete puerta;
			delete pomo;

			SolidBrush^ humo = gcnew SolidBrush(Color::FromArgb(40, 200, 210, 220));
			g->FillEllipse(humo, 278, 218, 22, 18);
			g->FillEllipse(humo, 288, 198, 30, 24);
			g->FillEllipse(humo, 276, 176, 40, 32);
			delete humo;

			SolidBrush^ sendero = gcnew SolidBrush(Color::FromArgb(255, 42, 33, 24));
			g->FillPolygon(sendero, gcnew array<Point>{
				Point(212, 465), Point(250, 465), Point(360, 540), Point(520, 600), Point(360, 600), Point(250, 540)
			});
			delete sendero;

			DibujarPino(g, 65, 520, 150);
			DibujarPino(g, 100, 540, 110);
			DibujarPino(g, 425, 520, 160);
			DibujarPino(g, 455, 550, 120);
			DibujarPino(g, 30, 580, 100);
			DibujarPino(g, 900, 540, 150);
			DibujarPino(g, 930, 580, 110);

			for (int i = 0; i < 18; i++) {
				int x = rnd->Next(60, 520);
				int y = rnd->Next(380, 560);
				SolidBrush^ glow = gcnew SolidBrush(Color::FromArgb(35, 250, 230, 140));
				SolidBrush^ punto = gcnew SolidBrush(Color::FromArgb(190, 255, 240, 160));
				g->FillEllipse(glow, x - 5, y - 5, 10, 10);
				g->FillEllipse(punto, x, y, 3, 3);
				delete glow;
				delete punto;
			}

			delete pared;
			delete paredOscura;
			delete techo;
			delete techoOscuro;
			delete contorno;

			Rectangle rectCard(560, 70, 360, 460);
			GraphicsPath^ card = RutaRedondeada(rectCard, 10);
			SolidBrush^ cardFondo = gcnew SolidBrush(Color::FromArgb(255, 15, 24, 34));
			g->FillPath(cardFondo, card);
			delete card;
			delete cardFondo;
			Pen^ lineaCard = gcnew Pen(Color::FromArgb(70, 214, 198, 166), 1.0f);
			g->DrawLine(lineaCard, 560, 92, 560, 508);
			delete lineaCard;

			delete g;
			fondoMenu = bmp;
		}

		Bitmap^ CargarFondoMenuOriginal() {
			array<String^>^ rutas = gcnew array<String^>{
				"sprites/background.png",
				"background.png",
				"..\\..\\background.png",
				"..\\..\\..\\background.png",
				"..\\..\\..\\..\\background.png"
			};
			for each (String^ ruta in rutas) {
				if (System::IO::File::Exists(ruta)) return gcnew Bitmap(ruta);
			}
			String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
			array<String^>^ rutasBase = gcnew array<String^>{
				"sprites\\background.png",
				"..\\..\\background.png",
				"..\\..\\..\\background.png",
				"..\\..\\..\\..\\background.png"
			};
			for each (String^ ruta in rutasBase) {
				String^ completa = System::IO::Path::Combine(baseDir, ruta);
				if (System::IO::File::Exists(completa)) return gcnew Bitmap(completa);
			}
			return nullptr;
		}

		void GenerarFondoMenuDesdeImagen() {
			Bitmap^ original = CargarFondoMenuOriginal();
			if (original == nullptr) {
				GenerarFondoMenu();
				return;
			}
			Bitmap^ bmp = gcnew Bitmap(960, 600);
			Graphics^ g = Graphics::FromImage(bmp);
			g->InterpolationMode = InterpolationMode::HighQualityBicubic;
			g->PixelOffsetMode = PixelOffsetMode::HighQuality;
			g->SmoothingMode = SmoothingMode::AntiAlias;
			g->DrawImage(original, 0, 0, 960, 600);
			delete original;

			Rectangle rectMarco(620, 80, 324, 440);
			GraphicsPath^ marco = RutaRedondeada(rectMarco, 18);
			SolidBrush^ fondoMarco = gcnew SolidBrush(Color::FromArgb(188, 8, 14, 22));
			Pen^ bordeExterior = gcnew Pen(Color::FromArgb(235, 218, 190, 130), 3.0f);
			g->FillPath(fondoMarco, marco);
			g->DrawPath(bordeExterior, marco);
			delete fondoMarco;
			delete bordeExterior;

			GraphicsPath^ marcoInterior = RutaRedondeada(Rectangle(629, 89, 306, 422), 13);
			Pen^ bordeInterior = gcnew Pen(Color::FromArgb(210, 242, 234, 214), 1.0f);
			g->DrawPath(bordeInterior, marcoInterior);
			delete marcoInterior;
			delete bordeInterior;

			SolidBrush^ adorno = gcnew SolidBrush(Color::FromArgb(235, 218, 190, 130));
			int xs[4] = { 634, 930, 634, 930 };
			int ys[4] = { 94, 94, 506, 506 };
			for (int i = 0; i < 4; i++) {
				g->FillPolygon(adorno, gcnew array<Point>{
					Point(xs[i], ys[i] - 6),
					Point(xs[i] + 6, ys[i]),
					Point(xs[i], ys[i] + 6),
					Point(xs[i] - 6, ys[i])
				});
			}
			g->FillPolygon(adorno, gcnew array<Point>{
				Point(782, 86), Point(790, 94), Point(782, 102), Point(774, 94)
			});
			g->FillPolygon(adorno, gcnew array<Point>{
				Point(782, 498), Point(790, 506), Point(782, 514), Point(774, 506)
			});
			delete adorno;
			delete marco;
			delete g;
			fondoMenu = bmp;
		}

		void ConfigurarBoton(Button^ boton, Color normal, Color hover, Color presionado) {
			boton->FlatStyle = FlatStyle::Flat;
			boton->FlatAppearance->BorderSize = 0;
			boton->FlatAppearance->MouseOverBackColor = hover;
			boton->FlatAppearance->MouseDownBackColor = presionado;
			boton->BackColor = normal;
			boton->ForeColor = Color::FromArgb(238, 244, 250);
			boton->Font = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
			boton->Cursor = Cursors::Hand;
			boton->TextAlign = ContentAlignment::MiddleCenter;
		}

		void InicializarMenuGUI() {
			pnlMenu = gcnew Panel();
			pnlMenu->Dock = DockStyle::Fill;
			pnlMenu->BackgroundImage = fondoMenu;
			pnlMenu->BackgroundImageLayout = ImageLayout::None;
			this->Controls->Add(pnlMenu);

			Color crema = Color::FromArgb(234, 224, 200);
			Color cremaHover = Color::FromArgb(247, 240, 224);
			Color cremaPresionado = Color::FromArgb(210, 196, 170);
			Color textoOscuro = Color::FromArgb(48, 40, 34);

			btnJugar = gcnew Button();
			btnJugar->Text = "INICIAR AVENTURA";
			btnJugar->Location = Point(652, 161);
			btnJugar->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnJugar, Color::FromArgb(222, 196, 140), Color::FromArgb(238, 212, 154), Color::FromArgb(198, 172, 120));
			btnJugar->ForeColor = textoOscuro;
			btnJugar->Click += gcnew EventHandler(this, &MyForm::btnJugar_Click);
			pnlMenu->Controls->Add(btnJugar);

			btnControles = gcnew Button();
			btnControles->Text = "CÓMO JUGAR";
			btnControles->Location = Point(652, 209);
			btnControles->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnControles, crema, cremaHover, cremaPresionado);
			btnControles->ForeColor = textoOscuro;
			btnControles->Click += gcnew EventHandler(this, &MyForm::btnControles_Click);
			pnlMenu->Controls->Add(btnControles);

			btnLore = gcnew Button();
			btnLore->Text = "LORE DE LA OBRA";
			btnLore->Location = Point(652, 257);
			btnLore->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnLore, crema, cremaHover, cremaPresionado);
			btnLore->ForeColor = textoOscuro;
			btnLore->Click += gcnew EventHandler(this, &MyForm::btnLore_Click);
			pnlMenu->Controls->Add(btnLore);

			btnCreditos = gcnew Button();
			btnCreditos->Text = "CRÉDITOS DEL PROYECTO";
			btnCreditos->Location = Point(652, 305);
			btnCreditos->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnCreditos, crema, cremaHover, cremaPresionado);
			btnCreditos->ForeColor = textoOscuro;
			btnCreditos->Click += gcnew EventHandler(this, &MyForm::btnCreditos_Click);
			pnlMenu->Controls->Add(btnCreditos);

			btnCargar = gcnew Button();
			btnCargar->Text = "CARGAR PARTIDA";
			btnCargar->Location = Point(652, 353);
			btnCargar->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnCargar, crema, cremaHover, cremaPresionado);
			btnCargar->ForeColor = textoOscuro;
			btnCargar->Click += gcnew EventHandler(this, &MyForm::btnCargar_Click);
			pnlMenu->Controls->Add(btnCargar);

			btnSalir = gcnew Button();
			btnSalir->Text = "SALIR";
			btnSalir->Location = Point(652, 401);
			btnSalir->Size = System::Drawing::Size(260, 38);
			ConfigurarBoton(btnSalir, crema, cremaHover, cremaPresionado);
			btnSalir->ForeColor = textoOscuro;
			btnSalir->Click += gcnew EventHandler(this, &MyForm::btnSalir_Click);
			pnlMenu->Controls->Add(btnSalir);
		}

		Button^ CrearBotonHUD(String^ texto, int y) {
			Button^ boton = gcnew Button();
			boton->Text = texto;
			boton->Font = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
			boton->ForeColor = Color::FromArgb(226, 232, 240);
			boton->BackColor = Color::FromArgb(12, 20, 30);
			boton->FlatStyle = FlatStyle::Flat;
			boton->FlatAppearance->BorderSize = 0;
			boton->FlatAppearance->MouseOverBackColor = Color::FromArgb(24, 36, 48);
			boton->Cursor = Cursors::Hand;
			boton->Location = Point(760, y);
			boton->Size = System::Drawing::Size(184, 30);
			boton->Visible = false;
			this->Controls->Add(boton);
			boton->BringToFront();
			return boton;
		}

		void InicializarBotonesHUD() {
			btnHudControles = CrearBotonHUD("VER CONTROLES", 16);
			btnHudControles->Click += gcnew EventHandler(this, &MyForm::btnHudControles_Click);
			btnHudInventario = CrearBotonHUD("INVENTARIO", 52);
			btnHudInventario->Click += gcnew EventHandler(this, &MyForm::btnHudInventario_Click);
		}

		void btnHudControles_Click(Object^ sender, EventArgs^ e) {
			AbrirControlesRapido();
		}

		void btnHudInventario_Click(Object^ sender, EventArgs^ e) {
			AbrirInventarioRapido();
		}

		void ActualizarTextoBotonJugar() {
			if (controlador->getMisionCompletada()) {
				btnJugar->Text = "REJUGAR AVENTURA";
			}
			else if (controlador->getTiempoSegundos() > 0) {
				btnJugar->Text = "CONTINUAR AVENTURA";
			}
			else {
				btnJugar->Text = "INICIAR AVENTURA";
			}
		}

		void MostrarMenuPrincipal() {
			estadoActual = EstadoApp::MENU_PRINCIPAL;
			timerJuego->Stop();
			if (controlador != nullptr) controlador->pausarAudio();
			teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
			teclaAccion = false;
			if (btnHudControles != nullptr) btnHudControles->Visible = false;
			if (btnHudInventario != nullptr) btnHudInventario->Visible = false;
			pnlMenu->Visible = true;
			pnlMenu->BringToFront();
			ActualizarTextoBotonJugar();
			btnJugar->Focus();
			bufferListo = false;
			this->Invalidate();
		}

		void IniciarJuego(bool permitirReinicio) {
			if (permitirReinicio && controlador->getMisionCompletada()) {
				controlador->reiniciarNivel();
			}
			pnlMenu->Visible = false;
			estadoActual = EstadoApp::EN_JUEGO;
			teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
			teclaAccion = false;
			if (btnHudControles != nullptr) {
				btnHudControles->Visible = true;
				btnHudControles->BringToFront();
			}
			if (btnHudInventario != nullptr) {
				btnHudInventario->Visible = true;
				btnHudInventario->BringToFront();
			}
			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			bufferListo = false;
			if (!timerJuego->Enabled) timerJuego->Start();
			this->Invalidate();
		}

		void btnJugar_Click(Object^ sender, EventArgs^ e) {
			IniciarJuego(true);
		}

		void btnControles_Click(Object^ sender, EventArgs^ e) {
			FormControles^ frm = gcnew FormControles();
			frm->ShowDialog(this);
		}

		void btnLore_Click(Object^ sender, EventArgs^ e) {
			FormLore^ frm = gcnew FormLore();
			frm->ShowDialog(this);
		}

		void btnCreditos_Click(Object^ sender, EventArgs^ e) {
			FormCreditos^ frm = gcnew FormCreditos();
			frm->ShowDialog(this);
		}

		void btnCargar_Click(Object^ sender, EventArgs^ e) {
			FormGuardarCargar^ frm = gcnew FormGuardarCargar(controlador);
			if (frm->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
				IniciarJuego(false);
			}
		}

		void btnSalir_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}

		void ConfigurarPersonajesEnControles(FormControles^ frm) {
			std::vector<PersonajeInfo> personajes;
			controlador->obtenerPersonajes(personajes);
			int cantidad = (int)personajes.size();
			array<String^>^ nombres = gcnew array<String^>(cantidad);
			array<String^>^ roles = gcnew array<String^>(cantidad);
			array<String^>^ descripciones = gcnew array<String^>(cantidad);
			array<String^>^ confianzas = gcnew array<String^>(cantidad);
			for (int i = 0; i < cantidad; i++) {
				nombres[i] = ToManaged(personajes[i].nombre);
				roles[i] = ToManaged(personajes[i].rol);
				descripciones[i] = ToManaged(personajes[i].descripcion);
				confianzas[i] = ToManaged(personajes[i].confianza);
			}
			frm->ConfigurarPersonajes(nombres, roles, descripciones, confianzas);
		}

		void AbrirControlesRapido() {
			timerJuego->Stop();
			controlador->pausarAudio();
			FormControles^ frm = gcnew FormControles();
			ConfigurarPersonajesEnControles(frm);
			frm->ShowDialog(this);
			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			timerJuego->Start();
		}

		void AbrirInventarioModal() {
			int cantidad = controlador->getCantidadInventario();
			array<String^>^ nombres = gcnew array<String^>(cantidad);
			array<String^>^ descripciones = gcnew array<String^>(cantidad);
			for (int i = 0; i < cantidad; i++) {
				nombres[i] = ToManaged(controlador->getInventarioNombre(i));
				descripciones[i] = ToManaged(controlador->getInventarioDescripcion(i));
			}
			FormInventario^ frm = gcnew FormInventario(nombres, descripciones);
			frm->ShowDialog(this);
		}

		void AbrirGuardarCargarModal() {
			FormGuardarCargar^ frm = gcnew FormGuardarCargar(controlador);
			frm->ShowDialog(this);
		}

		void AbrirPersonajesModal() {
			std::vector<PersonajeInfo> personajes;
			controlador->obtenerPersonajes(personajes);
			int cantidad = (int)personajes.size();
			array<String^>^ nombres = gcnew array<String^>(cantidad);
			array<String^>^ roles = gcnew array<String^>(cantidad);
			array<String^>^ descripciones = gcnew array<String^>(cantidad);
			array<String^>^ confianzas = gcnew array<String^>(cantidad);
			for (int i = 0; i < cantidad; i++) {
				nombres[i] = ToManaged(personajes[i].nombre);
				roles[i] = ToManaged(personajes[i].rol);
				descripciones[i] = ToManaged(personajes[i].descripcion);
				confianzas[i] = ToManaged(personajes[i].confianza);
			}
			FormPersonajes^ frm = gcnew FormPersonajes(nombres, roles, descripciones, confianzas);
			frm->ShowDialog(this);
		}

		void AbrirInventarioRapido() {
			timerJuego->Stop();
			controlador->pausarAudio();

			int cantidad = nivel2->getCantidadInventario();
			array<String^>^ nombres = gcnew array<String^>(cantidad);
			array<String^>^ descripciones = gcnew array<String^>(cantidad);

			for (int i = 0; i < cantidad; i++) {
				nombres[i] = ToManaged(nivel2->getInventarioNombre(i));
				descripciones[i] = ToManaged(nivel2->getInventarioDescripcion(i));
			}

			FormInventario^ frm = gcnew FormInventario(nombres, descripciones);
			frm->ShowDialog(this);

			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			timerJuego->Start();
		}

		void AbrirPersonajesRapido() {
			timerJuego->Stop();
			controlador->pausarAudio();
			AbrirPersonajesModal();
			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			timerJuego->Start();
		}

		void AbrirPausa() {
			timerJuego->Stop();
			controlador->pausarAudio();
			teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
			teclaAccion = false;

			FormPausa^ frm = gcnew FormPausa(controlador->getResumenPartida());
			frm->ShowDialog(this);
			int accion = frm->getAccion();
			if (accion == 5) {
				MostrarMenuPrincipal();
				return;
			}
			if (accion == 1) {
				AbrirInventarioModal();
			}
			else if (accion == 2) {
				AbrirPersonajesModal();
			}
			else if (accion == 3) {
				AbrirGuardarCargarModal();
			}
			else if (accion == 4) {
				FormControles^ controles = gcnew FormControles();
				ConfigurarPersonajesEnControles(controles);
				controles->ShowDialog(this);
			}

			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			timerJuego->Start();
			this->Invalidate();
		}

		void AbrirMinijuego() {
			timerJuego->Stop();
			controlador->reproducirAudioMinijuego();
			FormMinijuego^ frm = gcnew FormMinijuego();
			System::Windows::Forms::DialogResult resultado = frm->ShowDialog(this);
			controlador->resolverMinijuego(resultado == System::Windows::Forms::DialogResult::OK);
			this->ActiveControl = nullptr;
			this->Focus();
			timerJuego->Start();
		}

		void AbrirTrollface() {
			timerJuego->Stop();
			controlador->pausarAudio();
			FormTrollface^ frm = gcnew FormTrollface();
			frm->ShowDialog(this);
			controlador->marcarTrollDescubierto();
			this->ActiveControl = nullptr;
			this->Focus();
			controlador->reanudarAudioNivel();
			timerJuego->Start();
		}

		void AbrirFinNivel() {
			timerJuego->Stop();
			controlador->pausarAudio();
			FormFinNivel^ frm = gcnew FormFinNivel(controlador->getTiempoSegundos(), controlador->getCantidadInventario(), controlador->getMyrphonRescatado());
			frm->ShowDialog(this);
			int accion = frm->getAccion();
			if (accion == 1) {
				controlador->reiniciarNivel();
				controlador->reanudarAudioNivel();
				this->ActiveControl = nullptr;
				this->Focus();
				bufferListo = false;
				timerJuego->Start();
			}
			else {
				MostrarMenuPrincipal();
			}
		}

		void timerJuego_Tick(Object^ sender, EventArgs^ e) {
			if (estadoActual != EstadoApp::EN_JUEGO) return;
			if (this->ClientSize.Width <= 0 || this->ClientSize.Height <= 0) return;

			int dx = (teclaDerecha ? 1 : 0) - (teclaIzquierda ? 1 : 0);
			int dy = (teclaAbajo ? 1 : 0) - (teclaArriba ? 1 : 0);

			if (dx != 0 || dy != 0) {
				nivel2->moverJugador(dx, dy);
			}
			else {
				nivel2->detenerJugador();
			}

			nivel2->procesarTick();

			if (bufferGrafico == nullptr) return;
			bufferGrafico->Graphics->Clear(Color::FromArgb(7, 11, 17));

			nivel2->dibujar(bufferGrafico->Graphics);

			bufferGrafico->Render(gPantalla);
			bufferListo = true;

			// Aquí está el resto del código de los botones HUD
			bool mostrarHud = true;
			if (btnHudControles != nullptr) btnHudControles->Visible = mostrarHud;
			if (btnHudInventario != nullptr) btnHudInventario->Visible = mostrarHud;
		}

		void MyForm_PreviewKeyDown(Object^ sender, PreviewKeyDownEventArgs^ e) {
			if (estadoActual == EstadoApp::EN_JUEGO) {
				e->IsInputKey = true;
				return;
			}
			switch (e->KeyCode) {
			case Keys::Up:
			case Keys::Down:
			case Keys::Left:
			case Keys::Right:
			case Keys::W:
			case Keys::A:
			case Keys::S:
			case Keys::D:
			case Keys::Space:
			case Keys::Z:
			case Keys::I:
			case Keys::Escape:
			case Keys::D1:
			case Keys::D2:
			case Keys::D3:
			case Keys::D4:
				e->IsInputKey = true;
				break;
			}
		}

		void MyForm_KeyDown(Object^ sender, KeyEventArgs^ e) {
			if (estadoActual == EstadoApp::MENU_PRINCIPAL) {
				if (e->KeyCode == Keys::Escape) this->Close();
				return;
			}

			e->Handled = true;
			e->SuppressKeyPress = true;

			if (controlador->getDialogoActivo()) {
				if (e->KeyCode == Keys::Space || e->KeyCode == Keys::Z) {
					if (!teclaAccion) {
						teclaAccion = true;
						controlador->interactuar();
					}
				}
				else if (e->KeyCode == Keys::Up || e->KeyCode == Keys::W) {
					controlador->moverSeleccion(-1);
				}
				else if (e->KeyCode == Keys::Down || e->KeyCode == Keys::S) {
					controlador->moverSeleccion(1);
				}
				else if (e->KeyCode == Keys::D1) controlador->elegirOpcionDialogo(0);
				else if (e->KeyCode == Keys::D2) controlador->elegirOpcionDialogo(1);
				else if (e->KeyCode == Keys::D3) controlador->elegirOpcionDialogo(2);
				else if (e->KeyCode == Keys::D4) controlador->elegirOpcionDialogo(3);
				return;
			}

			switch (e->KeyCode) {
			case Keys::W:
			case Keys::Up:
				teclaArriba = true; break;
			case Keys::S:
			case Keys::Down:
				teclaAbajo = true; break;
			case Keys::A:
			case Keys::Left:
				teclaIzquierda = true; break;
			case Keys::D:
			case Keys::Right:
				teclaDerecha = true; break;
			case Keys::Space:
			case Keys::Z:
				if (!teclaAccion) {
					teclaAccion = true;
					nivel2->interactuar(); // <--- SE LLAMA AQUÍ, NO EN EL DIÁLOGO
				}
				break;
			case Keys::I:
				AbrirInventarioRapido();
				break;
			case Keys::P:
				AbrirPersonajesRapido();
				break;
			case Keys::M:
				controlador->alternarAudio();
				break;
			case Keys::Escape:
				AbrirPausa();
				break;
			}
		}

		void MyForm_KeyUp(Object^ sender, KeyEventArgs^ e) {
			if (estadoActual != EstadoApp::EN_JUEGO) return;
			switch (e->KeyCode) {
			case Keys::W:
			case Keys::Up:
				teclaArriba = false; break;
			case Keys::S:
			case Keys::Down:
				teclaAbajo = false; break;
			case Keys::A:
			case Keys::Left:
				teclaIzquierda = false; break;
			case Keys::D:
			case Keys::Right:
				teclaDerecha = false; break;
			case Keys::Space:
			case Keys::Z:
				teclaAccion = false; break;
			}
		}

		void MyForm_Deactivate(Object^ sender, EventArgs^ e) {
			teclaArriba = false;
			teclaAbajo = false;
			teclaIzquierda = false;
			teclaDerecha = false;
			teclaAccion = false;
			if (controlador != nullptr) controlador->detenerJugador();
		}

	protected:
		virtual bool ProcessDialogKey(Keys keyData) override {
			if (estadoActual == EstadoApp::EN_JUEGO) return false;
			return Form::ProcessDialogKey(keyData);
		}

		virtual void OnPaintBackground(PaintEventArgs^ e) override {
			if (estadoActual != EstadoApp::EN_JUEGO) {
				Form::OnPaintBackground(e);
			}
		}

		virtual void OnPaint(PaintEventArgs^ e) override {
			if (estadoActual == EstadoApp::EN_JUEGO) {
				if (bufferListo && bufferGrafico != nullptr) {
					bufferGrafico->Render(e->Graphics);
				}
				else {
					e->Graphics->Clear(Color::FromArgb(7, 11, 17));
				}
				return;
			}
			Form::OnPaint(e);
		}

	public:
		MyForm(void) {
			InitializeComponent();
			nivel2 = new Nivel2();

			this->Text = "Witch Hat Atelier · El Árbol de Plata";
			this->ClientSize = System::Drawing::Size(960, 600);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->StartPosition = FormStartPosition::CenterScreen;
			this->BackColor = Color::FromArgb(7, 11, 17);
			this->KeyPreview = true;
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;

			this->SetStyle(ControlStyles::AllPaintingInWmPaint | ControlStyles::UserPaint | ControlStyles::OptimizedDoubleBuffer, true);
			this->UpdateStyles();

			teclaArriba = false;
			teclaAbajo = false;
			teclaIzquierda = false;
			teclaDerecha = false;
			teclaAccion = false;
			bufferListo = false;

			gPantalla = this->CreateGraphics();
			espacioBuffer = BufferedGraphicsManager::Current;
			bufferGrafico = espacioBuffer->Allocate(gPantalla, this->ClientRectangle);

			controlador = new ControladorJuego();
			estadoActual = EstadoApp::MENU_PRINCIPAL;

			GenerarFondoMenuDesdeImagen();
			InicializarMenuGUI();
			InicializarBotonesHUD();
			ActualizarTextoBotonJugar();

			timerJuego = gcnew System::Windows::Forms::Timer();
			timerJuego->Interval = 25;
			timerJuego->Tick += gcnew EventHandler(this, &MyForm::timerJuego_Tick);

			this->KeyDown += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->KeyUp += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyUp);
			this->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &MyForm::MyForm_PreviewKeyDown);
			this->Deactivate += gcnew EventHandler(this, &MyForm::MyForm_Deactivate);

			btnJugar->Focus();
		}

	protected:
		~MyForm() {
			if (timerJuego != nullptr) {
				timerJuego->Stop();
				delete timerJuego;
				timerJuego = nullptr;
			}
			if (controlador != nullptr) {
				delete controlador;
				controlador = nullptr;
			}
			if (bufferGrafico != nullptr) {
				delete bufferGrafico;
				bufferGrafico = nullptr;
			}
			if (gPantalla != nullptr) {
				delete gPantalla;
				gPantalla = nullptr;
			}
			if (fondoMenu != nullptr) {
				delete fondoMenu;
				fondoMenu = nullptr;
			}
			if (components) {
				delete components;
			}
			if (nivel2 != nullptr) {
				delete nivel2;
				nivel2 = nullptr;
			}
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void) {
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(960, 600);
			this->Name = L"MyForm";
			this->Text = L"Witch Hat Atelier · El Árbol de Plata";
			this->ResumeLayout(false);
		}
#pragma endregion
	};
}
