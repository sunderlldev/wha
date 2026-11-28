#include "MyForm.h"
#include "Configuracion.h"
#include "Utilidades.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"
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
#include "FormMisiones.h"
#include "FormSelectorNivel.h"
#include "FormReflexion.h"
#include "FormEscenaCuarto.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Drawing::Imaging;
using namespace System::Windows::Forms;

namespace whav2 {

	static inline Nivel1* N1(System::IntPtr p) { return static_cast<Nivel1*>(p.ToPointer()); }
	static inline Nivel2* N2(System::IntPtr p) { return static_cast<Nivel2*>(p.ToPointer()); }
	static inline Nivel3* N3(System::IntPtr p) { return static_cast<Nivel3*>(p.ToPointer()); }

	static GraphicsPath^ RutaRedondeadaMenu(Rectangle rect, int radio) {
		GraphicsPath^ ruta = gcnew GraphicsPath();
		int diametro = radio * 2;
		ruta->AddArc(rect.Left, rect.Top, diametro, diametro, 180, 90);
		ruta->AddArc(rect.Right - diametro, rect.Top, diametro, diametro, 270, 90);
		ruta->AddArc(rect.Right - diametro, rect.Bottom - diametro, diametro, diametro, 0, 90);
		ruta->AddArc(rect.Left, rect.Bottom - diametro, diametro, diametro, 90, 90);
		ruta->CloseFigure();
		return ruta;
	}

	MyForm::MyForm(void) {
		InitializeComponent();

		this->SetStyle(ControlStyles::AllPaintingInWmPaint | ControlStyles::UserPaint | ControlStyles::OptimizedDoubleBuffer, true);
		this->UpdateStyles();

		teclaArriba = false;
		teclaAbajo = false;
		teclaIzquierda = false;
		teclaDerecha = false;
		teclaAccion = false;
		teclaShift = false;
		bufferListo = false;
		nivel1Completado = false;
		nivel2Completado = false;
		nivel3Completado = false;
		CargarProgresoNiveles();

		gPantalla = this->CreateGraphics();
		espacioBuffer = BufferedGraphicsManager::Current;
		bufferGrafico = espacioBuffer->Allocate(gPantalla, this->ClientRectangle);

		ptrNivel1 = System::IntPtr(new Nivel1());
		ptrNivel2 = System::IntPtr(new Nivel2());
		ptrNivel3 = System::IntPtr(new Nivel3());
		estadoActual = Configuracion::MENU_PRINCIPAL;
		nivelJugando = 1;

		GenerarFondoMenuDesdeImagen();
		if (pnlMenu != nullptr && fondoMenu != nullptr) {
			pnlMenu->BackgroundImage = fondoMenu;
			pnlMenu->BringToFront();
		}
		ActualizarTextoBotonJugar();

		this->KeyDown += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyDown);
		this->KeyUp += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyUp);
		this->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &MyForm::MyForm_PreviewKeyDown);
		this->Deactivate += gcnew EventHandler(this, &MyForm::MyForm_Deactivate);

		if (btnJugar != nullptr) btnJugar->Focus();
	}

	MyForm::~MyForm() {
		if (timerJuego != nullptr) {
			timerJuego->Stop();
		}
		if (ptrNivel1 != System::IntPtr::Zero) {
			delete N1(ptrNivel1);
			ptrNivel1 = System::IntPtr::Zero;
		}
		if (ptrNivel2 != System::IntPtr::Zero) {
			delete N2(ptrNivel2);
			ptrNivel2 = System::IntPtr::Zero;
		}
		if (ptrNivel3 != System::IntPtr::Zero) {
			delete N3(ptrNivel3);
			ptrNivel3 = System::IntPtr::Zero;
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
	}

	bool MyForm::ProcessDialogKey(Keys keyData) {
		if (estadoActual == Configuracion::EN_JUEGO) return false;
		return Form::ProcessDialogKey(keyData);
	}

	void MyForm::OnKeyPress(KeyPressEventArgs^ e) {
		if (estadoActual == Configuracion::EN_JUEGO) {
			e->Handled = true;
			return;
		}
		Form::OnKeyPress(e);
	}

	void MyForm::WndProc(Message% m) {
		if (m.Msg == 0x0120) {
			m.Result = IntPtr(1 << 16);
			return;
		}
		Form::WndProc(m);
	}

	void MyForm::OnPaintBackground(PaintEventArgs^ e) {
		if (estadoActual != Configuracion::EN_JUEGO) {
			Form::OnPaintBackground(e);
		}
	}

	void MyForm::OnPaint(PaintEventArgs^ e) {
		if (estadoActual == Configuracion::EN_JUEGO) {
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

	void MyForm::DibujarPino(Graphics^ g, int x, int baseY, int alto) {
		SolidBrush^ pincel = gcnew SolidBrush(Color::FromArgb(255, 10, 24, 18));
		g->FillPolygon(pincel, gcnew array<Point>{ Point(x, baseY - alto), Point(x - alto / 3, baseY), Point(x + alto / 3, baseY) });
		g->FillRectangle(pincel, x - 2, baseY - 6, 4, 12);
		delete pincel;
	}

	void MyForm::GenerarFondoMenu() {
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

		SolidBrush^ bosqueLejano = gcnew SolidBrush(Color::FromArgb(255, 9, 21, 16));
		g->FillPolygon(bosqueLejano, gcnew array<Point>{
			Point(0, 430), Point(110, 360), Point(240, 425), Point(380, 350), Point(530, 430),
			Point(680, 365), Point(820, 420), Point(960, 370), Point(960, 600), Point(0, 600)
		});
		delete bosqueLejano;

		SolidBrush^ suelo = gcnew SolidBrush(Color::FromArgb(255, 8, 17, 13));
		g->FillRectangle(suelo, 0, 520, 960, 80);
		delete suelo;

		DibujarPino(g, 65, 520, 150);
		DibujarPino(g, 425, 520, 160);
		DibujarPino(g, 900, 540, 150);

		Rectangle rectCard(560, 70, 360, 460);
		GraphicsPath^ card = RutaRedondeadaMenu(rectCard, 10);
		SolidBrush^ cardFondo = gcnew SolidBrush(Color::FromArgb(255, 15, 24, 34));
		g->FillPath(cardFondo, card);
		delete card;
		delete cardFondo;

		delete g;
		fondoMenu = bmp;
	}

	Bitmap^ MyForm::CargarFondoMenuOriginal() {
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
			"..\\..\\wha-v2\\sprites\\background.png",
			"..\\..\\..\\wha-v2\\wha-v2\\sprites\\background.png",
			"..\\..\\..\\..\\wha-v2\\wha-v2\\sprites\\background.png"
		};
		for each (String^ ruta in rutasBase) {
			String^ completa = System::IO::Path::Combine(baseDir, ruta);
			if (System::IO::File::Exists(completa)) return gcnew Bitmap(completa);
		}
		return nullptr;
	}

	void MyForm::GenerarFondoMenuDesdeImagen() {
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

		Rectangle rectMarco(709, 260, 234, 321);
		GraphicsPath^ marco = RutaRedondeadaMenu(rectMarco, 16);
		SolidBrush^ fondoMarco = gcnew SolidBrush(Color::FromArgb(192, 8, 14, 22));
		Pen^ bordeExterior = gcnew Pen(Color::FromArgb(235, 218, 190, 130), 3.0f);
		g->FillPath(fondoMarco, marco);
		g->DrawPath(bordeExterior, marco);
		delete fondoMarco;
		delete bordeExterior;

		GraphicsPath^ marcoInterior = RutaRedondeadaMenu(Rectangle(716, 267, 220, 307), 12);
		Pen^ bordeInterior = gcnew Pen(Color::FromArgb(210, 242, 234, 214), 1.0f);
		g->DrawPath(bordeInterior, marcoInterior);
		delete marcoInterior;
		delete bordeInterior;

		SolidBrush^ adorno = gcnew SolidBrush(Color::FromArgb(235, 218, 190, 130));
		int xs[4] = { 721, 931, 721, 931 };
		int ys[4] = { 271, 271, 570, 570 };
		for (int i = 0; i < 4; i++) {
			g->FillPolygon(adorno, gcnew array<Point>{
				Point(xs[i], ys[i] - 5),
				Point(xs[i] + 5, ys[i]),
				Point(xs[i], ys[i] + 5),
				Point(xs[i] - 5, ys[i])
			});
		}
		g->FillPolygon(adorno, gcnew array<Point>{
			Point(826, 266), Point(833, 271), Point(826, 276), Point(819, 271)
		});
		g->FillPolygon(adorno, gcnew array<Point>{
			Point(826, 565), Point(833, 570), Point(826, 575), Point(819, 570)
		});
		delete adorno;
		delete marco;
		delete g;
		fondoMenu = bmp;
	}

	void MyForm::ActualizarTextoBotonJugar() {
		btnJugar->Text = "SELECCIONAR NIVEL";
	}

	void MyForm::MostrarMenuPrincipal() {
		estadoActual = Configuracion::MENU_PRINCIPAL;
		timerJuego->Stop();
		if (ptrNivel1 != System::IntPtr::Zero) N1(ptrNivel1)->detenerAudio();
		teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
		teclaAccion = false;
		teclaShift = false;
		if (btnHudControles != nullptr) btnHudControles->Visible = false;
		if (btnHudInventario != nullptr) btnHudInventario->Visible = false;
		if (btnHudMisiones != nullptr) btnHudMisiones->Visible = false;
		if (btnHudPersonajes != nullptr) btnHudPersonajes->Visible = false;
		if (btnHudAyuda != nullptr) btnHudAyuda->Visible = false;
		pnlMenu->Visible = true;
		pnlMenu->BringToFront();
		ActualizarTextoBotonJugar();
		btnJugar->Focus();
		bufferListo = false;
		this->Invalidate();
	}

	void MyForm::IniciarNivelSeleccionado(int numNivel, bool reiniciar) {
		nivelJugando = numNivel;
		if (reiniciar) {
			if (nivelJugando == 1) N1(ptrNivel1)->iniciarNivel(1);
			else if (nivelJugando == 2) N2(ptrNivel2)->reiniciarNivel();
			else if (nivelJugando == 3) N3(ptrNivel3)->reiniciarNivel();
		}
		pnlMenu->Visible = false;
		estadoActual = Configuracion::EN_JUEGO;
		teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
		teclaAccion = false;
		teclaShift = false;
		if (btnHudControles != nullptr) {
			btnHudControles->Visible = false;
			btnHudControles->BringToFront();
		}
		if (btnHudInventario != nullptr) {
			btnHudInventario->Visible = false;
			btnHudInventario->BringToFront();
		}
		if (btnHudMisiones != nullptr) {
			btnHudMisiones->Visible = false;
			btnHudMisiones->BringToFront();
		}
		if (btnHudPersonajes != nullptr) {
			btnHudPersonajes->Visible = false;
			btnHudPersonajes->BringToFront();
		}
		if (btnHudAyuda != nullptr) {
			btnHudAyuda->Visible = false;
			btnHudAyuda->BringToFront();
		}
		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reproducirAudioNivel(nivelJugando);
		bufferListo = false;
		if (!timerJuego->Enabled) timerJuego->Start();
		this->Invalidate();
	}

	void MyForm::CargarProgresoNiveles() {
		std::ifstream arch(ObtenerRutaGuardadoStdComun("progreso_niveles.txt"));
		if (!arch.is_open()) {
			GuardarProgresoNiveles();
			return;
		}
		std::string linea;
		while (std::getline(arch, linea)) {
			if (linea == "n1=1") nivel1Completado = true;
			else if (linea == "n2=1") nivel2Completado = true;
			else if (linea == "n3=1") nivel3Completado = true;
		}
		arch.close();
	}

	void MyForm::GuardarProgresoNiveles() {
		std::ofstream arch(ObtenerRutaGuardadoStdComun("progreso_niveles.txt"), std::ios::out | std::ios::trunc);
		if (!arch.is_open()) return;
		arch << "n1=" << (nivel1Completado ? 1 : 0) << "\n";
		arch << "n2=" << (nivel2Completado ? 1 : 0) << "\n";
		arch << "n3=" << (nivel3Completado ? 1 : 0) << "\n";
		arch.close();
	}

	void MyForm::btnJugar_Click(Object^ sender, EventArgs^ e) {
		FormSelectorNivel^ frm = gcnew FormSelectorNivel(nivel1Completado, nivel2Completado, nivel3Completado);
		if (frm->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
			int elegido = frm->getNivelElegido();
			if (elegido >= 1 && elegido <= 3) {
				IniciarNivelSeleccionado(elegido, true);
			}
		}
	}

	void MyForm::btnControles_Click(Object^ sender, EventArgs^ e) {
		FormControles^ frm = gcnew FormControles();
		frm->ShowDialog(this);
	}

	void MyForm::btnLore_Click(Object^ sender, EventArgs^ e) {
		FormLore^ frm = gcnew FormLore();
		frm->ShowDialog(this);
	}

	void MyForm::btnCreditos_Click(Object^ sender, EventArgs^ e) {
		FormCreditos^ frm = gcnew FormCreditos();
		frm->ShowDialog(this);
	}

	void MyForm::btnCargar_Click(Object^ sender, EventArgs^ e) {
		FormGuardarCargar^ frm = gcnew FormGuardarCargar(N1(ptrNivel1));
		if (frm->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
			IniciarNivelSeleccionado(1, false);
		}
	}

	void MyForm::btnSalir_Click(Object^ sender, EventArgs^ e) {
		this->Close();
	}

	void MyForm::btnHudControles_Click(Object^ sender, EventArgs^ e) {
		AbrirControlesRapido();
	}

	void MyForm::btnHudInventario_Click(Object^ sender, EventArgs^ e) {
		AbrirInventarioRapido();
	}

	void MyForm::btnHudMisiones_Click(Object^ sender, EventArgs^ e) {
		AbrirMisionesRapido();
	}

	void MyForm::btnHudPersonajes_Click(Object^ sender, EventArgs^ e) {
		AbrirPersonajesRapido();
	}

	void MyForm::btnHudAyuda_Click(Object^ sender, EventArgs^ e) {
		AlternarAyudaHud();
	}

	void MyForm::ConfigurarPersonajesEnControles(Form^ frmBase) {
		FormControles^ frm = safe_cast<FormControles^>(frmBase);
		std::vector<PersonajeInfo> personajes;
		if (nivelJugando == 1) N1(ptrNivel1)->obtenerPersonajes(personajes);
		else if (nivelJugando == 2) N2(ptrNivel2)->obtenerPersonajes(personajes);
		else N3(ptrNivel3)->obtenerPersonajes(personajes);
		int cantidad = (int)personajes.size();
		array<String^>^ nombres = gcnew array<String^>(cantidad);
		array<String^>^ roles = gcnew array<String^>(cantidad);
		array<String^>^ descripciones = gcnew array<String^>(cantidad);
		array<String^>^ confianzas = gcnew array<String^>(cantidad);
		array<String^>^ sprites = gcnew array<String^>(cantidad);
		for (int i = 0; i < cantidad; i++) {
			nombres[i] = ToManaged(personajes[i].nombre);
			roles[i] = ToManaged(personajes[i].rol);
			descripciones[i] = ToManaged(personajes[i].descripcion);
			confianzas[i] = ToManaged(personajes[i].confianza);
			sprites[i] = ToManaged(personajes[i].spriteClave);
		}
		frm->ConfigurarPersonajes(nombres, roles, descripciones, confianzas, sprites);
	}

	void MyForm::AbrirControlesRapido() {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();
		FormControles^ frm = gcnew FormControles();
		ConfigurarPersonajesEnControles(frm);
		frm->ShowDialog(this);
		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel(nivelJugando);
		timerJuego->Start();
	}

	void MyForm::AbrirInventarioModal() {
		int cantidad = 0;
		if (nivelJugando == 1) cantidad = N1(ptrNivel1)->getCantidadInventario();
		else if (nivelJugando == 2) cantidad = N2(ptrNivel2)->getCantidadInventario();
		else cantidad = N3(ptrNivel3)->getCantidadInventario();

		array<String^>^ nombres = gcnew array<String^>(cantidad);
		array<String^>^ descripciones = gcnew array<String^>(cantidad);
		for (int i = 0; i < cantidad; i++) {
			if (nivelJugando == 1) {
				nombres[i] = ToManaged(N1(ptrNivel1)->getInventarioNombre(i));
				descripciones[i] = ToManaged(N1(ptrNivel1)->getInventarioDescripcion(i));
			}
			else if (nivelJugando == 2) {
				nombres[i] = ToManaged(N2(ptrNivel2)->getInventarioNombre(i));
				descripciones[i] = ToManaged(N2(ptrNivel2)->getInventarioDescripcion(i));
			}
			else {
				nombres[i] = ToManaged(N3(ptrNivel3)->getInventarioNombre(i));
				descripciones[i] = ToManaged(N3(ptrNivel3)->getInventarioDescripcion(i));
			}
		}
		FormInventario^ frm = gcnew FormInventario(nombres, descripciones);
		frm->ShowDialog(this);
	}

	void MyForm::AbrirGuardarCargarModal() {
		FormGuardarCargar^ frm = gcnew FormGuardarCargar(N1(ptrNivel1));
		frm->ShowDialog(this);
	}

	void MyForm::AbrirPersonajesModal() {
		std::vector<PersonajeInfo> personajes;
		if (nivelJugando == 1) N1(ptrNivel1)->obtenerPersonajes(personajes);
		else if (nivelJugando == 2) N2(ptrNivel2)->obtenerPersonajes(personajes);
		else N3(ptrNivel3)->obtenerPersonajes(personajes);
		int cantidad = (int)personajes.size();
		array<String^>^ nombres = gcnew array<String^>(cantidad);
		array<String^>^ roles = gcnew array<String^>(cantidad);
		array<String^>^ descripciones = gcnew array<String^>(cantidad);
		array<String^>^ confianzas = gcnew array<String^>(cantidad);
		array<String^>^ sprites = gcnew array<String^>(cantidad);
		for (int i = 0; i < cantidad; i++) {
			nombres[i] = ToManaged(personajes[i].nombre);
			roles[i] = ToManaged(personajes[i].rol);
			descripciones[i] = ToManaged(personajes[i].descripcion);
			confianzas[i] = ToManaged(personajes[i].confianza);
			sprites[i] = ToManaged(personajes[i].spriteClave);
		}
		FormPersonajes^ frm = gcnew FormPersonajes(nombres, roles, descripciones, confianzas, sprites);
		frm->ShowDialog(this);
	}

	void MyForm::AbrirInventarioRapido() {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();
		AbrirInventarioModal();
		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel(nivelJugando);
		timerJuego->Start();
	}

	void MyForm::AbrirPersonajesRapido() {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();
		AbrirPersonajesModal();
		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel(nivelJugando);
		timerJuego->Start();
	}

	void MyForm::AlternarAyudaHud() {
		if (estadoActual != Configuracion::EN_JUEGO) return;
		if (nivelJugando == 1) N1(ptrNivel1)->alternarGuia();
		else if (nivelJugando == 2) N2(ptrNivel2)->alternarGuia();
		else N3(ptrNivel3)->alternarGuia();
		this->ActiveControl = nullptr;
		this->Focus();
	}

	void MyForm::AbrirMisionesRapido() {
		if (estadoActual != Configuracion::EN_JUEGO) return;
		timerJuego->Stop();
		if (nivelJugando == 1) N1(ptrNivel1)->pausarAudio();

		std::vector<MisionInfo> lista;
		int indiceActual = 0;
		String^ subtitulo = "";
		if (nivelJugando == 1) {
			subtitulo = "Nivel 1 - El Atelier de Qifrey";
			N1(ptrNivel1)->obtenerMisiones(lista, indiceActual);
		}
		else if (nivelJugando == 2) {
			subtitulo = "Nivel 2 - El Laberinto del Pueblo de Kaln";
			N2(ptrNivel2)->obtenerMisiones(lista, indiceActual);
		}
		else {
			subtitulo = "Nivel 3 - El Árbol de Plata";
			N3(ptrNivel3)->obtenerMisiones(lista, indiceActual);
		}

		int n = (int)lista.size();
		array<String^>^ titulos = gcnew array<String^>(n);
		array<String^>^ detalles = gcnew array<String^>(n);
		array<bool>^ completadas = gcnew array<bool>(n);
		for (int i = 0; i < n; i++) {
			titulos[i] = ToManaged(lista[i].titulo);
			detalles[i] = ToManaged(lista[i].detalle);
			completadas[i] = lista[i].completada;
		}
		FormMisiones^ frm = gcnew FormMisiones(subtitulo, titulos, detalles, completadas, indiceActual);
		frm->ShowDialog(this);

		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel(nivelJugando);
		timerJuego->Start();
	}

	void MyForm::AbrirPausa() {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();
		teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
		teclaAccion = false;
		teclaShift = false;

		String^ textoProgreso = "";
		if (nivelJugando == 1) {
			N1(ptrNivel1)->guardarPartida();
			textoProgreso = "Tu progreso se guardó hasta:\n" + ToManaged(N1(ptrNivel1)->getObjetivo()) + "\nTiempo: " + N1(ptrNivel1)->getTiempoSegundos().ToString() + " s";
		}
		else if (nivelJugando == 2) {
			textoProgreso = "Progreso de sesión en Nivel 2:\nCruzar el Laberinto del Pueblo de Kaln\nTiempo: " + N2(ptrNivel2)->getTiempoSegundos().ToString() + " s";
		}
		else {
			textoProgreso = "Progreso de sesión en Nivel 3:\nEl Árbol de Plata\nTiempo: " + N3(ptrNivel3)->getTiempoSegundos().ToString() + " s";
		}

		FormPausa^ frm = gcnew FormPausa(textoProgreso);
		frm->ShowDialog(this);
		if (frm->getAccion() == 5) {
			MostrarMenuPrincipal();
			return;
		}

		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel(nivelJugando);
		timerJuego->Start();
		this->Invalidate();
	}

	void MyForm::AbrirMinijuego() {
		timerJuego->Stop();
		N1(ptrNivel1)->reproducirAudioMinijuego();
		FormMinijuego^ frm = gcnew FormMinijuego();
		System::Windows::Forms::DialogResult resultado = frm->ShowDialog(this);
		N1(ptrNivel1)->resolverMinijuego(resultado == System::Windows::Forms::DialogResult::OK);
		this->ActiveControl = nullptr;
		this->Focus();
		timerJuego->Start();
	}

	void MyForm::AbrirTrollface() {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();
		FormTrollface^ frm = gcnew FormTrollface();
		frm->ShowDialog(this);
		N1(ptrNivel1)->marcarTrollDescubierto();
		this->ActiveControl = nullptr;
		this->Focus();
		N1(ptrNivel1)->reanudarAudioNivel();
		timerJuego->Start();
	}

	void MyForm::AbrirFinNivel(int numNivel) {
		timerJuego->Stop();
		N1(ptrNivel1)->pausarAudio();

		if (numNivel == 1) nivel1Completado = true;
		else if (numNivel == 2) nivel2Completado = true;
		else if (numNivel == 3) nivel3Completado = true;
		GuardarProgresoNiveles();

		int t = 0;
		int objs = 0;
		bool extra = false;
		if (numNivel == 1) {
			t = N1(ptrNivel1)->getTiempoSegundos();
			objs = N1(ptrNivel1)->getCantidadInventario();
			extra = N1(ptrNivel1)->getMyrphonRescatado();
		}
		else if (numNivel == 2) {
			t = N2(ptrNivel2)->getTiempoSegundos();
			objs = N2(ptrNivel2)->getCantidadInventario();
			extra = true;
		}
		else {
			t = N3(ptrNivel3)->getTiempoSegundos();
			objs = N3(ptrNivel3)->getCantidadInventario();
			extra = N3(ptrNivel3)->getAceptoSemilla();
		}

		FormFinNivel^ frm = (numNivel == 2)
			? gcnew FormFinNivel(t, objs, extra, numNivel, N2(ptrNivel2)->getPuntaje(), N2(ptrNivel2)->getCuartosExplorados(), N2(ptrNivel2)->getTotalCuartos())
			: gcnew FormFinNivel(t, objs, extra, numNivel);
		frm->ShowDialog(this);
		int accion = frm->getAccion();
		if (accion == 1) {
			IniciarNivelSeleccionado(numNivel, true);
		}
		else if (accion == 3) {
			if (numNivel == 1) IniciarNivelSeleccionado(2, true);
			else if (numNivel == 2) IniciarNivelSeleccionado(3, true);
			else {
				FormReflexion^ frmRef = gcnew FormReflexion();
				frmRef->ShowDialog(this);
				MostrarMenuPrincipal();
			}
		}
		else {
			MostrarMenuPrincipal();
		}
	}

	void MyForm::timerJuego_Tick(Object^ sender, EventArgs^ e) {
		if (estadoActual != Configuracion::EN_JUEGO) return;
		if (this->ClientSize.Width <= 0 || this->ClientSize.Height <= 0) return;
		if (bufferGrafico == nullptr) return;

		int dx = (teclaDerecha ? 1 : 0) - (teclaIzquierda ? 1 : 0);
		int dy = (teclaAbajo ? 1 : 0) - (teclaArriba ? 1 : 0);
		bool corriendo = teclaShift || ((Control::ModifierKeys & Keys::Shift) == Keys::Shift);

		bufferGrafico->Graphics->Clear(Color::FromArgb(7, 11, 17));

		bool mostrarHud = true;
		bool guiaActiva = true;

		if (nivelJugando == 1) {
			if (dx != 0 || dy != 0) N1(ptrNivel1)->moverJugador(dx, dy, corriendo);
			else N1(ptrNivel1)->detenerJugador();
			N1(ptrNivel1)->procesarTick(bufferGrafico->Graphics, this->ClientSize.Width, this->ClientSize.Height, Configuracion::ZOOM, corriendo);
			mostrarHud = !N1(ptrNivel1)->getEnTransicionPozo() && !N1(ptrNivel1)->getEnIntro();
			guiaActiva = N1(ptrNivel1)->getGuiaActiva();
		}
		else if (nivelJugando == 2) {
			N1(ptrNivel1)->actualizarAudio();
			if (dx != 0 || dy != 0) N2(ptrNivel2)->moverJugador(dx, dy, corriendo);
			else N2(ptrNivel2)->detenerJugador();
			N2(ptrNivel2)->procesarTick(corriendo);
			N2(ptrNivel2)->dibujar(bufferGrafico->Graphics);
			mostrarHud = !N2(ptrNivel2)->getEnIntro();
			guiaActiva = N2(ptrNivel2)->getGuiaActiva();
		}
		else {
			N1(ptrNivel1)->actualizarAudio();
			if (dx != 0 || dy != 0) N3(ptrNivel3)->moverJugador(dx, dy, corriendo);
			else N3(ptrNivel3)->detenerJugador();
			N3(ptrNivel3)->procesarTick(bufferGrafico->Graphics, this->ClientSize.Width, this->ClientSize.Height, corriendo);
			mostrarHud = !N3(ptrNivel3)->getEnIntro();
			guiaActiva = N3(ptrNivel3)->getGuiaActiva();
		}

		bufferGrafico->Render(gPantalla);
		bufferListo = true;

		if (btnHudControles != nullptr) btnHudControles->Visible = mostrarHud;
		if (btnHudInventario != nullptr) btnHudInventario->Visible = mostrarHud;
		if (btnHudMisiones != nullptr) btnHudMisiones->Visible = mostrarHud;
		if (btnHudPersonajes != nullptr) btnHudPersonajes->Visible = mostrarHud;
		if (btnHudAyuda != nullptr) {
			btnHudAyuda->Visible = mostrarHud;
			btnHudAyuda->Text = guiaActiva ? L"QUITAR AYUDA [H]" : L"VER AYUDA [H]";
		}

		if (nivelJugando == 1) {
			if (N1(ptrNivel1)->consumirSolicitudMinijuego()) {
				AbrirMinijuego();
				return;
			}
			if (N1(ptrNivel1)->consumirSolicitudTrollface()) {
				AbrirTrollface();
				return;
			}
			if (N1(ptrNivel1)->consumirSolicitudFinNivel()) {
				AbrirFinNivel(1);
				return;
			}
		}
		else if (nivelJugando == 2) {
			int cuartoEsc = 0;
			if (N2(ptrNivel2)->consumirSolicitudEscenaCuarto(cuartoEsc)) {
				timerJuego->Stop();
				teclaArriba = teclaAbajo = teclaIzquierda = teclaDerecha = false;
				teclaAccion = false;
				teclaShift = false;
				N2(ptrNivel2)->detenerJugador();
				array<String^>^ nombres = gcnew array<String^>{
					"Pueblo de Kaln",
					"Hogar Abandonado",
					"Casa del Panadero",
					"Casa Sospechosa",
					"Refugio de Mago",
					"Baños del Pueblo",
					"Cuartel Moralis",
					"Choza del Pueblo",
					"Taberna de Kaln",
					"Botica del Pueblo"
				};
				String^ nom = (cuartoEsc >= 1 && cuartoEsc <= 9) ? nombres[cuartoEsc] : "Pueblo de Kaln";
				array<String^>^ imgs = nullptr;
				array<String^>^ desc = nullptr;
				if (cuartoEsc == 2) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_Panaderia01.png",
						"sprites/Nivel2_Panaderia02.png",
						"sprites/Nivel2_Panaderia03.png",
						"sprites/Nivel2_Panaderia04.png",
						"sprites/Nivel2_Panaderia05.png"
					};
					desc = gcnew array<String^>{
						"¡Un mago de sombrero puntiagudo!",
						"¡El gato fue atrapado por los escombros!",
						"¡Magia prohibida!",
						"¿Está ayudándolo?",
						"¿La magia se puede utilizar para curar?"
					};
				}
				else if (cuartoEsc == 3) {
					imgs = gcnew array<String^>{ "sprites/Nivel2_Abuelita01.png" };
					desc = gcnew array<String^>{ "Casa de la viejita nada sospechosa..." };
				}
				else if (cuartoEsc == 4) {
					imgs = gcnew array<String^>{ "sprites/Nivel2_Inicio04.png" };
					desc = gcnew array<String^>{ "Casa alquilada usada por un mago." };
				}
				else if (cuartoEsc == 5) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_Inicio01.png",
						"sprites/Nivel2_Inicio02.png",
						"sprites/Nivel2_Inicio03.png",
						"sprites/Nivel2_Inicio04.png"
					};
					desc = gcnew array<String^>{
						"Coco avanza entre la niebla nocturna de Kaln.",
						"Tartah observa los callejones antes de separarse.",
						"Las puertas del pueblo permanecen cerradas ante el peligro.",
						"Las sanguijuelas sombrías comienzan a rodear las casas."
					};
				}
				else if (cuartoEsc == 6) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_Moralis01.png",
						"sprites/Nivel2_Moralis02.png"
					};
					desc = gcnew array<String^>{
						"Hagan aviso.",
						"El pueblo está siendo invadido por una sanguijuela mágica."
					};
				}
				else if (cuartoEsc == 7) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_ChozaPueblo01.png",
						"sprites/Nivel2_ChozaPueblo02.png",
						"sprites/Nivel2_ChozaPueblo03.png"
					};
					desc = gcnew array<String^>{
						"Verás, Tartah...",
						"La razón por la que Coustas puede caminar...",
						"...es por la semilla del árbol de plata."
					};
				}
				else if (cuartoEsc == 8) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_Taberna01.png",
						"sprites/Nivel2_Taberna02.png",
						"sprites/Nivel2_Taberna03.png",
						"sprites/Nivel2_Taberna03.png"
					};
					desc = gcnew array<String^>{
						"Tipo1: Sí, como te decía...",
						"Tipo2: ¡Corran!",
						"Tipo2: No sé qué sea esa cosa pero nos va a matar",
						"Tipo1: ah"
					};
				}
				else if (cuartoEsc == 9) {
					imgs = gcnew array<String^>{
						"sprites/Nivel2_Botica03.png",
						"sprites/Nivel2_Botica03.png",
						"sprites/Nivel2_Botica03.png"
					};
					desc = gcnew array<String^>{
						"Tipo1: ¡Ayuda! Mi amigo está herido",
						"Medico1: Hay muchos heridos",
						"Medico2: No hay suficientes médicos y siguen llegando más personas"
					};
				}
				else {
					imgs = gcnew array<String^>{ "sprites/Nivel2_Inicio04.png" };
					desc = gcnew array<String^>{ "Tartah recorre " + nom + " en busca de los habitantes." };
				}
				FormEscenaCuarto^ frmEsc = gcnew FormEscenaCuarto(cuartoEsc, nom, imgs, desc);
				frmEsc->ShowDialog(this);
				this->ActiveControl = nullptr;
				this->Focus();
				timerJuego->Start();
				return;
			}
			if (N2(ptrNivel2)->consumirSolicitudFinNivel()) {
				AbrirFinNivel(2);
				return;
			}
		}
		else {
			if (N3(ptrNivel3)->consumirSolicitudFinNivel()) {
				AbrirFinNivel(3);
				return;
			}
		}
	}

	void MyForm::MyForm_PreviewKeyDown(Object^ sender, PreviewKeyDownEventArgs^ e) {
		if (estadoActual == Configuracion::EN_JUEGO) {
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
		case Keys::Enter:
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

	void MyForm::MyForm_KeyDown(Object^ sender, KeyEventArgs^ e) {
		if (estadoActual == Configuracion::MENU_PRINCIPAL) {
			if (e->KeyCode == Keys::Escape) this->Close();
			return;
		}

		e->Handled = true;
		e->SuppressKeyPress = true;

		bool enDialogo = (nivelJugando == 1 && N1(ptrNivel1)->getDialogoActivo()) || (nivelJugando == 3 && N3(ptrNivel3)->getDialogoActivo());
		if (enDialogo) {
			if (e->KeyCode == Keys::Space || e->KeyCode == Keys::Z || e->KeyCode == Keys::Enter) {
				if (!teclaAccion) {
					teclaAccion = true;
					if (nivelJugando == 1) N1(ptrNivel1)->interactuar();
					else N3(ptrNivel3)->interactuar();
				}
			}
			else if (e->KeyCode == Keys::Up || e->KeyCode == Keys::W) {
				if (nivelJugando == 1) N1(ptrNivel1)->moverSeleccion(-1);
				else N3(ptrNivel3)->moverSeleccion(-1);
			}
			else if (e->KeyCode == Keys::Down || e->KeyCode == Keys::S) {
				if (nivelJugando == 1) N1(ptrNivel1)->moverSeleccion(1);
				else N3(ptrNivel3)->moverSeleccion(1);
			}
			else if (e->KeyCode == Keys::D1) {
				if (nivelJugando == 1) N1(ptrNivel1)->elegirOpcionDialogo(0);
				else N3(ptrNivel3)->elegirOpcionDialogo(0);
			}
			else if (e->KeyCode == Keys::D2) {
				if (nivelJugando == 1) N1(ptrNivel1)->elegirOpcionDialogo(1);
				else N3(ptrNivel3)->elegirOpcionDialogo(1);
			}
			else if (e->KeyCode == Keys::D3 && nivelJugando == 1) N1(ptrNivel1)->elegirOpcionDialogo(2);
			else if (e->KeyCode == Keys::D4 && nivelJugando == 1) N1(ptrNivel1)->elegirOpcionDialogo(3);
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
		case Keys::Enter:
			if (!teclaAccion) {
				teclaAccion = true;
				if (nivelJugando == 1) N1(ptrNivel1)->interactuar();
				else if (nivelJugando == 2) N2(ptrNivel2)->interactuar();
				else N3(ptrNivel3)->interactuar();
			}
			break;
		case Keys::ShiftKey:
		case Keys::LShiftKey:
		case Keys::RShiftKey:
			teclaShift = true;
			break;
		case Keys::Q:
			AbrirMisionesRapido();
			break;
		case Keys::H:
			AlternarAyudaHud();
			break;
		case Keys::F:
			if (nivelJugando == 3) {
				int dx = (teclaDerecha ? 1 : 0) - (teclaIzquierda ? 1 : 0);
				int dy = (teclaAbajo ? 1 : 0) - (teclaArriba ? 1 : 0);
				N3(ptrNivel3)->activarCapaVuelo(dx, dy);
			}
			break;
		case Keys::I:
			AbrirInventarioRapido();
			break;
		case Keys::P:
			AbrirPersonajesRapido();
			break;
		case Keys::M:
			N1(ptrNivel1)->alternarAudio(nivelJugando);
			break;
		case Keys::Escape:
			AbrirPausa();
			break;
		}
	}

	void MyForm::MyForm_KeyUp(Object^ sender, KeyEventArgs^ e) {
		if (estadoActual != Configuracion::EN_JUEGO) return;
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
		case Keys::Enter:
			teclaAccion = false; break;
		case Keys::ShiftKey:
		case Keys::LShiftKey:
		case Keys::RShiftKey:
			teclaShift = false; break;
		}
	}

	void MyForm::MyForm_Deactivate(Object^ sender, EventArgs^ e) {
		teclaArriba = false;
		teclaAbajo = false;
		teclaIzquierda = false;
		teclaDerecha = false;
		teclaAccion = false;
		teclaShift = false;
		if (ptrNivel1 != System::IntPtr::Zero) N1(ptrNivel1)->detenerJugador();
		if (ptrNivel2 != System::IntPtr::Zero) N2(ptrNivel2)->detenerJugador();
		if (ptrNivel3 != System::IntPtr::Zero) N3(ptrNivel3)->detenerJugador();
	}
}

[STAThread]
void main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew whav2::MyForm());
}
