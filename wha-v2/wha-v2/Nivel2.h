#ifndef Nivel2_h
#define Nivel2_h

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <gcroot.h>
#include "MapaNivel2.h"
#include "Enemigo.h"
#include "Protagonista.h"
#include "Arbol.h"
#include "Utilidades.h"
#include "Inventario.h"
#include "Configuracion.h"
#include "GestorAudio.h"

using namespace System;
using namespace System::Drawing;

struct LibretaRecargaN2 {
	int x;
	int y;
	bool recogida;
	bool esInicial;
};

struct AldeanoN2 {
	int x;
	int y;
	int dir;
	int colorIdx;
	int ticksAnim;
	bool huyendo;
};

struct ProyectilN2 {
	int x;
	int y;
	char dir;
	bool explotando;
	int ticks;
};

class Nivel2 {
private:
	std::vector<std::string> mapa;
	Protagonista* protagonista;
	Inventario* inventario;
	std::vector<Enemigo*> enemigos;
	std::vector<Arbol*> arboles;
	std::vector<LibretaRecargaN2> libretas;
	std::vector<AldeanoN2> aldeanos;
	std::vector<ProyectilN2> proyectiles;
	std::vector<std::vector<int>> mapaPasto;

	bool cuartosVisitados[10];
	int cuartosExplorados;
	int puntaje;
	int ticksJuego;
	int ticksOleada;
	int ticksCooldownDisparo;
	int ticksCooldownDanio;

	int cargasHechizoAire;
	bool tieneLibretaInicial;

	int idCuartoActual;
	bool solicitaEscenaCuarto;
	int cuartoEscenaPendiente;

	int camX;
	int camY;
	int ticksMensaje;
	std::string mensajeTemporal;
	bool enIntro;
	bool guiaActiva;
	bool correrActivo;
	bool completado;
	bool solicitaFinNivel;

	gcroot<Bitmap^> bmpProtagonista;
	gcroot<Bitmap^> bmpTileset;
	gcroot<Bitmap^> bmpEnemigo;
	gcroot<Bitmap^> bmpHechizo;
	gcroot<Bitmap^> bmpLibreta;
	gcroot<array<Bitmap^>^> bmpsAldeanos;

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		return CargarBitmapSeguroComun(nombreArchivo, false);
	}

	void limpiarEntidades() {
		for (size_t i = 0; i < enemigos.size(); i++) {
			if (enemigos[i] != nullptr) delete enemigos[i];
		}
		enemigos.clear();
		for (size_t i = 0; i < arboles.size(); i++) {
			if (arboles[i] != nullptr) delete arboles[i];
		}
		arboles.clear();
		libretas.clear();
		aldeanos.clear();
		proyectiles.clear();
	}

	static const char* obtenerNombreCuarto(int id) {
		static const char* nombres[10] = {
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
		if (id < 0 || id > 9) return nombres[0];
		return nombres[id];
	}

	bool esCeldaTransitable(int col, int fila) {
		if (fila < 0 || fila >= (int)mapa.size() || col < 0 || col >= (int)mapa[fila].length()) return false;
		char ch = mapa[fila][col];
		return ch == '.' || ch == ' ';
	}

	bool hayColision(int x, int y, int w, int h) {
		int minC = x / 32;
		int maxC = (x + w - 1) / 32;
		int minF = y / 32;
		int maxF = (y + h - 1) / 32;
		for (int f = minF; f <= maxF; f++) {
			for (int c = minC; c <= maxC; c++) {
				if (!esCeldaTransitable(c, f)) return true;
			}
		}
		Rectangle rect(x, y, w, h);
		for (size_t i = 0; i < arboles.size(); i++) {
			if (arboles[i] != nullptr && rect.IntersectsWith(arboles[i]->getTrunkRect())) return true;
		}
		return false;
	}

	void inicializarLibretas() {
		libretas.clear();
		libretas.push_back(LibretaRecargaN2{ 13 * 32, 32 * 32, false, true });
		libretas.push_back(LibretaRecargaN2{ 10 * 32, 8 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 50 * 32, 13 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 82 * 32, 14 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 107 * 32, 22 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 50 * 32, 32 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 80 * 32, 33 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 12 * 32, 49 * 32, false, false });
		libretas.push_back(LibretaRecargaN2{ 60 * 32, 50 * 32, false, false });
	}

	void inicializarAldeanos() {
		aldeanos.clear();
		int pos[][3] = {
			{ 18, 9, 0 },
			{ 46, 16, 1 },
			{ 78, 14, 2 },
			{ 104, 24, 0 },
			{ 46, 30, 1 },
			{ 56, 34, 2 },
			{ 76, 31, 0 },
			{ 22, 48, 1 },
			{ 52, 48, 2 },
			{ 68, 51, 0 }
		};
		for (int i = 0; i < 10; i++) {
			AldeanoN2 a;
			a.x = pos[i][0] * 32;
			a.y = pos[i][1] * 32;
			a.dir = 0;
			a.colorIdx = pos[i][2];
			a.ticksAnim = i * 5;
			a.huyendo = false;
			aldeanos.push_back(a);
		}
	}

	void generarArbolesAleatorios() {
		System::Random^ rng = gcnew System::Random((int)(System::DateTime::Now.Ticks & 0x7FFFFFFF));
		int maxCols = (int)mapa[0].length();
		int maxFilas = (int)mapa.size();
		int intentos = 0;
		while ((int)arboles.size() < 55 && intentos < 3000) {
			intentos++;
			int col = rng->Next(3, maxCols - 5);
			int fila = rng->Next(3, maxFilas - 4);

			bool librePasto = true;
			for (int dr = -1; dr <= 2 && librePasto; dr++) {
				for (int dc = -1; dc <= 3; dc++) {
					int rf = fila + dr;
					int rc = col + dc;
					if (rf < 0 || rf >= maxFilas || rc < 0 || rc >= maxCols ||
						mapa[rf][rc] != ' ' || determinarCuarto(rc * 32, rf * 32) > 0) {
						librePasto = false;
						break;
					}
				}
			}
			if (!librePasto) continue;

			int px = col * 32;
			int py = fila * 32;
			int dxP = px - protagonista->getX();
			int dyP = py - protagonista->getY();
			if (dxP * dxP + dyP * dyP < 120 * 120) continue;

			int dxSal = px - 107 * 32;
			int dySal = py - 8 * 32;
			if (dxSal * dxSal + dySal * dySal < 150 * 150) continue;

			bool cercaAld = false;
			for (size_t i = 0; i < aldeanos.size(); i++) {
				int dxA = (px + 39) - aldeanos[i].x;
				int dyA = (py + 44) - aldeanos[i].y;
				if (dxA * dxA + dyA * dyA < 64 * 64) {
					cercaAld = true;
					break;
				}
			}
			if (cercaAld) continue;

			bool muyCerca = false;
			for (size_t i = 0; i < arboles.size(); i++) {
				int dx = px - arboles[i]->getX();
				int dy = py - arboles[i]->getY();
				if (dx * dx + dy * dy < 84 * 84) {
					muyCerca = true;
					break;
				}
			}
			if (muyCerca) continue;

			TipoArbol tipo = (rng->Next(0, 2) == 0) ? ArbolPino : ArbolFrondoso;
			arboles.push_back(new Arbol(px, py, tipo));
		}
	}

	void inicializarNivel() {
		MapaNivel2::cargarMatriz(mapa);
		limpiarEntidades();

		mapaPasto.clear();
		mapaPasto.resize(mapa.size());
		System::Random^ rnd = gcnew System::Random(2026);
		for (int i = 0; i < (int)mapa.size(); i++) {
			mapaPasto[i].resize(mapa[i].length());
			for (int j = 0; j < (int)mapa[i].length(); j++) {
				mapaPasto[i][j] = rnd->Next(0, 8);
			}
		}

		if (protagonista == nullptr) protagonista = new Protagonista(11 * 32, 29 * 32, 2);
		else {
			protagonista->setX(11 * 32);
			protagonista->setY(29 * 32);
			protagonista->setVidas(3);
			protagonista->setDireccion(0);
			protagonista->detener();
		}
		if (inventario == nullptr) inventario = new Inventario();
		else inventario->limpiar();

		idCuartoActual = 5;
		camX = 0;
		camY = 0;
		ticksJuego = 0;
		ticksOleada = 320;
		ticksCooldownDisparo = 0;
		ticksCooldownDanio = 0;
		cargasHechizoAire = 0;
		tieneLibretaInicial = false;
		proyectiles.clear();
		solicitaEscenaCuarto = false;
		cuartoEscenaPendiente = 0;
		enIntro = true;
		correrActivo = false;
		completado = false;
		solicitaFinNivel = false;

		for (int i = 0; i < 10; i++) cuartosVisitados[i] = false;
		cuartosVisitados[5] = true;
		cuartosExplorados = 1;
		puntaje = 100;

		inicializarLibretas();
		inicializarAldeanos();
		generarArbolesAleatorios();
		actualizarCamara();

		mensajeTemporal = "Nivel 2: Toma la Libreta de Aire y explora los 9 cuartos de Kaln.";
		ticksMensaje = 120;
	}

	void generarOleadaCercana() {
		if (protagonista == nullptr) return;
		const int MAX_ACTIVOS = 45;
		int activos = (int)enemigos.size();
		if (activos >= MAX_ACTIVOS) return;

		System::Random^ rng = gcnew System::Random((int)((System::DateTime::Now.Ticks + ticksJuego * 131) & 0x7FFFFFFF));
		int cantidadLote = rng->Next(25, 31);
		int porGenerar = Math::Min(cantidadLote, MAX_ACTIVOS - activos);

		int px = protagonista->getX();
		int py = protagonista->getY();
		int generados = 0;
		int intentos = 0;

		while (generados < porGenerar && intentos < 500) {
			intentos++;
			int offX = rng->Next(-440, 441);
			int offY = rng->Next(-265, 266);
			if (offX * offX + offY * offY < 160 * 160) continue;

			int ex = px + offX;
			int ey = py + offY;
			if (ex < camX + 16 || ex > camX + 944 || ey < camY + 16 || ey > camY + 584) continue;
			if (hayColision(ex, ey, 16, 16)) continue;

			Enemigo* nuevo = new Enemigo(ex, ey, "Sanguijuela", 2, 1, 4, idCuartoActual);
			int tirada = rng->Next(0, 100);
			int vel = (tirada < 45) ? 2 : ((tirada < 80) ? rng->Next(3, 5) : rng->Next(5, 7));
			nuevo->setVelocidad(vel);
			enemigos.push_back(nuevo);
			generados++;
		}

		if (generados > 0) {
			mensajeTemporal = "[OLEADA] ¡" + std::to_string(generados) + " sanguijuelas emergen cerca!";
			ticksMensaje = 85;
		}
	}

	void actualizarAldeanos() {
		for (size_t i = 0; i < aldeanos.size(); i++) {
			AldeanoN2& al = aldeanos[i];
			al.ticksAnim++;

			int mejorDistSq = 220 * 220;
			int ameaX = 0;
			int ameaY = 0;
			bool peligro = false;

			for (size_t j = 0; j < enemigos.size(); j++) {
				Enemigo* en = enemigos[j];
				if (en == nullptr || !en->getActivo()) continue;
				int dx = al.x - en->getX();
				int dy = al.y - en->getY();
				int dSq = dx * dx + dy * dy;
				if (dSq < mejorDistSq) {
					mejorDistSq = dSq;
					ameaX = en->getX();
					ameaY = en->getY();
					peligro = true;
				}
			}

			al.huyendo = peligro;
			if (!peligro) continue;

			int vx = (al.x >= ameaX) ? 1 : -1;
			int vy = (al.y >= ameaY) ? 1 : -1;
			if (abs(al.x - ameaX) < 10) vx = 0;
			if (abs(al.y - ameaY) < 10) vy = 0;

			int vel = 2;
			bool movio = false;
			if (vx != 0 && !hayColision(al.x + vx * vel, al.y, 18, 20)) {
				al.x += vx * vel;
				al.dir = (vx > 0) ? 3 : 1;
				movio = true;
			}
			if (vy != 0 && !hayColision(al.x, al.y + vy * vel, 18, 20)) {
				al.y += vy * vel;
				if (!movio) al.dir = (vy > 0) ? 0 : 2;
			}
		}
	}

public:
	Nivel2() {
		protagonista = nullptr;
		inventario = nullptr;
		guiaActiva = true;
		bmpTileset = CargarBitmapSeguro("sprites/Tilled_Dirt_Wide.png");
		bmpProtagonista = CargarBitmapSeguro("sprites/Tartah_Spritesheet.png");
		bmpEnemigo = CargarBitmapSeguro("sprites/sanguijuela.png");
		bmpHechizo = CargarBitmapSeguro("sprites/Disparo_Cueva_Spritesheet.png");
		bmpLibreta = CargarBitmapSeguro("sprites/libreta.png");
		array<Bitmap^>^ arrAld = gcnew array<Bitmap^>(3);
		for (int i = 0; i < 3; i++) {
			arrAld[i] = CargarBitmapSeguro("sprites/aldeano_" + (i + 1).ToString() + ".png");
		}
		bmpsAldeanos = arrAld;
		inicializarNivel();
	}

	~Nivel2() {
		limpiarEntidades();
		if (protagonista != nullptr) delete protagonista;
		if (inventario != nullptr) delete inventario;
	}

	void reiniciarNivel() {
		inicializarNivel();
	}

	int getCantidadInventario() { return inventario->getCantidad(); }
	std::string getInventarioNombre(int i) { return inventario->getItem(i).nombre; }
	std::string getInventarioDescripcion(int i) { return inventario->getItem(i).descripcion; }
	int getVidas() { return protagonista != nullptr ? protagonista->getVidas() : 3; }
	int getTiempoSegundos() { return (ticksJuego * 25) / 1000; }
	bool getEnIntro() { return enIntro; }
	bool getGuiaActiva() { return guiaActiva; }
	void alternarGuia() { guiaActiva = !guiaActiva; }
	int getCuartosExplorados() { return cuartosExplorados; }
	int getTotalCuartos() { return 9; }
	int getPuntaje() { return puntaje; }

	bool consumirSolicitudFinNivel() {
		bool val = solicitaFinNivel;
		solicitaFinNivel = false;
		return val;
	}

	bool consumirSolicitudEscenaCuarto(int& numCuarto) {
		if (!solicitaEscenaCuarto) return false;
		solicitaEscenaCuarto = false;
		numCuarto = cuartoEscenaPendiente;
		return true;
	}

	String^ getResumenPartida() {
		String^ estado = completado ? "NIVEL COMPLETADO" : "EN CURSO";
		String^ resumen = "Nivel 2 · El Laberinto del Pueblo de Kaln\n";
		resumen += "Estado: " + estado + "\n";
		resumen += "Vidas: " + getVidas().ToString() + "\n";
		resumen += "Cuartos explorados: " + cuartosExplorados.ToString() + "/9\n";
		resumen += "Puntaje: " + puntaje.ToString() + " pts\n";
		resumen += "Cargas de Aire: " + cargasHechizoAire.ToString() + "\n";
		resumen += "Tiempo: " + getTiempoSegundos().ToString() + " s";
		return resumen;
	}

	void obtenerGuiaPasoActual(std::string& titulo, std::string& detalle, int& pasoNum, int& totalPasos, int& targetX, int& targetY) {
		totalPasos = 3;
		if (!tieneLibretaInicial && !libretas.empty()) {
			pasoNum = 1;
			titulo = "Recoge la Libreta de Aire";
			detalle = "Toma la libreta dentro de la casa inicial";
			targetX = libretas[0].x + 16;
			targetY = libretas[0].y + 16;
			return;
		}
		if (cuartosExplorados < 9 && protagonista != nullptr) {
			static const int centros[10][2] = {
				{ 0, 0 }, { 18, 9 }, { 50, 15 }, { 81, 15 }, { 107, 22 },
				{ 13, 29 }, { 50, 32 }, { 80, 33 }, { 18, 49 }, { 61, 50 }
			};
			int px = protagonista->getX();
			int py = protagonista->getY();
			int mejorC = 0;
			int mejorDistSq = 0x7FFFFFFF;
			for (int k = 1; k <= 9; k++) {
				if (cuartosVisitados[k]) continue;
				int cx = centros[k][0] * 32;
				int cy = centros[k][1] * 32;
				int dSq = (cx - px) * (cx - px) + (cy - py) * (cy - py);
				if (dSq < mejorDistSq) {
					mejorDistSq = dSq;
					mejorC = k;
				}
			}
			if (mejorC > 0) {
				pasoNum = 2;
				titulo = std::string("Explora: ") + obtenerNombreCuarto(mejorC);
				detalle = "Cuartos " + std::to_string(cuartosExplorados) + "/9 (portal da menos puntaje)";
				targetX = centros[mejorC][0] * 32;
				targetY = centros[mejorC][1] * 32;
				return;
			}
		}
		pasoNum = 3;
		titulo = "Cruza el portal noreste";
		detalle = "¡9/9 cuartos explorados! Sal por el portal";
		targetX = 107 * 32 + 24;
		targetY = 8 * 32 + 24;
	}

	void obtenerMisiones(std::vector<MisionInfo>& lista, int& indiceActual) {
		lista.clear();
		bool paso1 = tieneLibretaInicial;
		bool paso2 = (cuartosExplorados >= 9);
		bool paso3 = completado;
		lista.push_back(MisionInfo{ "1. Recoger Libreta de Hechizos", "Encuentra tu primera Libreta de Hechizos de Aire.", paso1 });
		lista.push_back(MisionInfo{ "2. Explorar los 9 cuartos de Kaln", "Visita todas las zonas (" + std::to_string(cuartosExplorados) + "/9) para cinemáticas y máximo puntaje.", paso2 });
		lista.push_back(MisionInfo{ "3. Alcanzar el portal noreste", "Esquiva las oleadas de sanguijuelas y cruza la salida.", paso3 });
		indiceActual = paso3 ? 2 : ((paso1 && paso2) ? 2 : (paso1 ? 1 : 0));
	}

	void obtenerPersonajes(std::vector<PersonajeInfo>& lista) {
		lista.clear();
		lista.push_back(PersonajeInfo{ "Tartah", "Habitante de Kaln (Ojos de Plata)", "Joven ingenioso del pueblo de Kaln. Recorre las calles nocturnas protegiendo a los aldeanos.", "Protagonista", "Tartah" });
		lista.push_back(PersonajeInfo{ "Coco", "Aprendiz de Maga", "Amiga de Tartah en su paso por Kaln. Compartió hechizos básicos de viento para defenderse.", "Amigos", "Coco" });
		lista.push_back(PersonajeInfo{ "Aldeano Dorado", "Habitante de Kaln", "Poblador de túnica dorada que huye despavorido cuando las sanguijuelas se acercan.", "Aliado", "aldeano_1" });
		lista.push_back(PersonajeInfo{ "Aldeano Lila", "Habitante de Kaln", "Vecino de túnica lila que busca refugio entre las casas al escuchar las oleadas.", "Aliado", "aldeano_2" });
		lista.push_back(PersonajeInfo{ "Aldeano Verde", "Habitante de Kaln", "Residente de túnica verde que corre a resguardarse durante el ataque nocturno.", "Aliado", "aldeano_3" });
		lista.push_back(PersonajeInfo{ "Sanguijuelas Sombrías", "Criaturas de la Sombra", "Entidades oscuras que atacan en oleadas con velocidades variadas.", "Hostil", "Sanguijuela" });
	}

	void moverJugador(int dx, int dy, bool corriendo = false) {
		if (protagonista == nullptr || enIntro || completado) return;
		correrActivo = corriendo;
		int paso = corriendo ? (Configuracion::VELOCIDAD_JUGADOR + 4) : (Configuracion::VELOCIDAD_JUGADOR + 1);
		int w = 16;
		int h = 12;
		int offX = 4;
		int offY = 20;

		if (dx != 0) {
			int nx = protagonista->getX() + dx * paso;
			if (!hayColision(nx + offX, protagonista->getY() + offY, w, h)) {
				protagonista->setX(nx);
			}
		}
		if (dy != 0) {
			int ny = protagonista->getY() + dy * paso;
			if (!hayColision(protagonista->getX() + offX, ny + offY, w, h)) {
				protagonista->setY(ny);
			}
		}
		protagonista->moverLibre(dx, dy);
		if (dx != 0 || dy != 0) {
			int freqPaso = corriendo ? 6 : 10;
			if (ticksJuego % freqPaso == 0) GestorAudio::ReproducirSfxPaso();
		}
		actualizarCuartoActual();
	}

	void detenerJugador() {
		correrActivo = false;
		if (protagonista != nullptr) protagonista->detener();
	}

	void interactuar() {
		if (enIntro) {
			enIntro = false;
			solicitaEscenaCuarto = true;
			cuartoEscenaPendiente = 5;
			return;
		}
		activarHechizoAire();
	}

	void procesarTick(bool corriendo = false) {
		if (enIntro) return;
		correrActivo = corriendo;
		ticksJuego++;
		if (ticksMensaje > 0) ticksMensaje--;
		if (ticksCooldownDisparo > 0) ticksCooldownDisparo--;

		ticksOleada++;
		if (ticksOleada >= 400) {
			ticksOleada = 0;
			generarOleadaCercana();
		}

		for (size_t k = 0; k < proyectiles.size(); ) {
			ProyectilN2& pr = proyectiles[k];
			if (!pr.explotando) {
				if (pr.dir == 'W') pr.y -= 12;
				else if (pr.dir == 'S') pr.y += 12;
				else if (pr.dir == 'A') pr.x -= 12;
				else if (pr.dir == 'D') pr.x += 12;

				bool impacto = false;
				for (size_t i = 0; i < enemigos.size(); i++) {
					Enemigo* en = enemigos[i];
					if (en != nullptr && en->getActivo()) {
						if (pr.x < en->getX() + en->getAncho() && pr.x + 32 > en->getX() &&
							pr.y < en->getY() + en->getAlto() && pr.y + 32 > en->getY()) {
							en->setActivo(false);
							puntaje += 15;
							impacto = true;
							mensajeTemporal = "[HECHIZO] ¡Sanguijuela repelida! (+15 pts)";
							ticksMensaje = 55;
							break;
						}
					}
				}
				if (!impacto && hayColision(pr.x + 6, pr.y + 6, 20, 20)) {
					impacto = true;
				}
				if (impacto) {
					pr.explotando = true;
					pr.ticks = 0;
				}
				else if (pr.x < camX - 64 || pr.x > camX + 1024 || pr.y < camY - 64 || pr.y > camY + 664) {
					proyectiles.erase(proyectiles.begin() + k);
					continue;
				}
			}
			else {
				pr.ticks++;
				if (pr.ticks > 10) {
					proyectiles.erase(proyectiles.begin() + k);
					continue;
				}
			}
			k++;
		}

		if (protagonista != nullptr) {
			Rectangle rectPlayer(protagonista->getX(), protagonista->getY(), protagonista->getAncho(), protagonista->getAlto());
			for (size_t i = 0; i < libretas.size(); i++) {
				if (libretas[i].recogida) continue;
				Rectangle rectItem(libretas[i].x, libretas[i].y, 32, 32);
				if (rectPlayer.IntersectsWith(rectItem)) {
					libretas[i].recogida = true;
					GestorAudio::ReproducirSfxAccion();
					if (!tieneLibretaInicial) {
						tieneLibretaInicial = true;
						cargasHechizoAire += 15;
						inventario->agregarItem("Libreta de Hechizos de Aire", "Permite lanzar ráfagas de aire con [Espacio] para repeler sanguijuelas.", "Herramienta Mágica");
						mensajeTemporal = "[ITEM] Libreta de Aire obtenida (+15 cargas)";
					}
					else {
						cargasHechizoAire = Math::Min(25, cargasHechizoAire + 8);
						mensajeTemporal = "[RECARGA] +8 cargas de Aire (Total: " + std::to_string(cargasHechizoAire) + ")";
					}
					ticksMensaje = 90;
				}
			}
		}

		persecucionEnemigos();

		actualizarAldeanos();

		for (size_t i = 0; i < enemigos.size(); ) {
			Enemigo* en = enemigos[i];
			bool fuera = (en == nullptr || !en->getActivo() ||
				en->getX() + en->getAncho() < camX - 260 ||
				en->getX() > camX + 960 + 260 ||
				en->getY() + en->getAlto() < camY - 260 ||
				en->getY() > camY + 600 + 260);
			if (fuera) {
				if (en != nullptr) delete en;
				enemigos.erase(enemigos.begin() + i);
			}
			else {
				i++;
			}
		}

		if (enemigos.empty() && ticksJuego >= 60 && ticksOleada < 340) {
			ticksOleada = 340;
		}

		if (ticksCooldownDanio > 0) {
			ticksCooldownDanio--;
		}
		else {
			colisionEnemigos();
		}

		if (!completado && protagonista != nullptr) {
			int cx = (protagonista->getX() + protagonista->getAncho() / 2) / 32;
			int cy = (protagonista->getY() + protagonista->getAlto() / 2) / 32;
			if (cx >= 105 && cx <= 109 && cy >= 6 && cy <= 10) {
				puntaje += (cuartosExplorados >= 9 ? 400 : 50);
				completado = true;
				solicitaFinNivel = true;
			}
		}

		actualizarCamara();
		protagonista->animar(corriendo);
		for (size_t i = 0; i < enemigos.size(); i++) {
			enemigos[i]->animar();
		}
	}

	void dibujar(Graphics^ g) {
		if (enIntro) {
			dibujarIntro(g);
			return;
		}
		dibujarMapa(g);
		dibujarPortalSalida(g);
		dibujarLibretas(g);
		dibujarAldeanos(g);
		dibujarEnemigos(g);
		dibujarProtagonista(g);
		dibujarArboles(g);
		dibujarHechizo(g);
		dibujarHUD(g);
		dibujarMensajes(g);
	}

	void activarHechizoAire() {
		if (protagonista == nullptr || inventario == nullptr || enIntro || completado) return;
		if (!tieneLibretaInicial) {
			mensajeTemporal = "[AVISO] Aún no tienes la Libreta de Hechizos de Aire.";
			ticksMensaje = 60;
			return;
		}
		if (cargasHechizoAire <= 0) {
			mensajeTemporal = "[AVISO] Sin cargas. Busca otra libreta en los cuartos.";
			ticksMensaje = 70;
			return;
		}
		if (ticksCooldownDisparo > 0 || (int)proyectiles.size() >= 12) return;

		char dir = 'D';
		if (protagonista->getDireccion() == 0) dir = 'S';
		else if (protagonista->getDireccion() == 1) dir = 'A';
		else if (protagonista->getDireccion() == 2) dir = 'W';
		else if (protagonista->getDireccion() == 3) dir = 'D';

		cargasHechizoAire--;
		ticksCooldownDisparo = 6;
		GestorAudio::ReproducirSfxAccion();

		ProyectilN2 nuevo;
		nuevo.x = protagonista->getX() + protagonista->getAncho() / 2 - 16;
		nuevo.y = protagonista->getY() + protagonista->getAlto() / 2 - 16;
		nuevo.dir = dir;
		nuevo.explotando = false;
		nuevo.ticks = 0;
		proyectiles.push_back(nuevo);

		mensajeTemporal = "[HECHIZO] Ráfaga de aire. Cargas restantes: " + std::to_string(cargasHechizoAire);
		ticksMensaje = 60;
	}

private:
	void dibujarIntro(Graphics^ g) {
		String^ cuerpo =
			"Tras huir de la plaza de Kaln, Tartah recorre los cuartos\n"
			"y calles del pueblo mientras hordas de sanguijuelas acechan.\n\n"
			"Recoge libretas para recargar hechizos de aire [Espacio], ayuda a los\n"
			"aldeanos a escapar, explora los 9 cuartos por puntaje y sal al noreste.";
		DibujarIntroActoComun(g, 960, 600, "ACTO II", "El Laberinto del Pueblo de Kaln", cuerpo, "Presiona Espacio para continuar");
	}

	void dibujarMapa(Graphics^ g) {
		int tileSize = Configuracion::TILE_SIZE;
		int inicioFila = Math::Max(0, camY / tileSize);
		int finFila = Math::Min((int)mapa.size() - 1, inicioFila + 600 / tileSize + 2);
		int inicioCol = Math::Max(0, camX / tileSize);
		int finCol = Math::Min((int)mapa[0].length() - 1, inicioCol + 960 / tileSize + 2);

		Bitmap^ bmp = bmpTileset;
		if (bmp == nullptr) return;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;

		SolidBrush^ vPoste = gcnew SolidBrush(Color::FromArgb(255, 86, 58, 36));
		SolidBrush^ vTabla = gcnew SolidBrush(Color::FromArgb(255, 132, 94, 58));
		SolidBrush^ vBorde = gcnew SolidBrush(Color::FromArgb(255, 48, 32, 20));

		for (int f = inicioFila; f <= finFila; f++) {
			for (int c = inicioCol; c <= finCol; c++) {
				char ch = mapa[f][c];
				int px = c * tileSize - camX;
				int py = f * tileSize - camY;

				int idX = 1;
				int idY = 1;

				if (ch == ' ' || ch == 'V') {
					int randomPasto = mapaPasto[f][c];
					idX = randomPasto % 4;
					idY = 2 + (randomPasto / 4);
				}
				else if (ch == '.') { idX = 3; idY = 0; }
				else if (ch == 'A') { idX = 0; idY = 0; }
				else if (ch == '-') { idX = 2; idY = 0; }
				else if (ch == 'C') { idX = 1; idY = 0; }
				else if (ch == '|') { idX = 2; idY = 1; }
				else if (ch == 'B') { idX = 0; idY = 1; }
				else if (ch == 'D') { idX = 1; idY = 1; }

				int srcX = idX * 16;
				int srcY = idY * 16;

				g->DrawImage(bmp, Rectangle(px, py, tileSize, tileSize), srcX, srcY, 16, 16, GraphicsUnit::Pixel);

				if (ch == 'V') {
					g->FillRectangle(vBorde, px, py + 10, 32, 6);
					g->FillRectangle(vTabla, px, py + 11, 32, 4);
					g->FillRectangle(vBorde, px, py + 20, 32, 6);
					g->FillRectangle(vTabla, px, py + 21, 32, 4);
					g->FillRectangle(vBorde, px + 4, py + 4, 8, 26);
					g->FillRectangle(vPoste, px + 5, py + 5, 6, 24);
					g->FillRectangle(vBorde, px + 20, py + 4, 8, 26);
					g->FillRectangle(vPoste, px + 21, py + 5, 6, 24);
				}
			}
		}

		delete vPoste;
		delete vTabla;
		delete vBorde;
	}

	void dibujarPortalSalida(Graphics^ g) {
		int sx = 107 * 32 - camX;
		int sy = 8 * 32 - camY;
		if (sx < -80 || sx > 1040 || sy < -80 || sy > 680) return;
		SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(80, 120, 210, 245));
		SolidBrush^ centro = gcnew SolidBrush(Color::FromArgb(200, 220, 248, 255));
		g->FillEllipse(aura, sx - 16, sy - 16, 84, 84);
		g->FillEllipse(centro, sx + 4, sy + 4, 44, 44);
		delete aura;
		delete centro;
	}

	void dibujarLibretas(Graphics^ g) {
		Bitmap^ bmp = bmpLibreta;
		for (size_t i = 0; i < libretas.size(); i++) {
			if (libretas[i].recogida) continue;
			int lx = libretas[i].x - camX;
			int ly = libretas[i].y - camY;
			if (lx < -48 || lx > 992 || ly < -48 || ly > 648) continue;
			int flot = (ticksJuego / 12 + (int)i) % 2;
			if (bmp != nullptr) {
				g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
				g->DrawImage(bmp, lx, ly - flot * 2, 32, 32);
			}
			else {
				SolidBrush^ pincelLibro = gcnew SolidBrush(Color::Gold);
				g->FillRectangle(pincelLibro, lx, ly, 32, 32);
				delete pincelLibro;
			}
		}
	}

	void dibujarAldeanos(Graphics^ g) {
		array<Bitmap^>^ arr = bmpsAldeanos;
		SolidBrush^ borde = gcnew SolidBrush(Color::FromArgb(255, 22, 18, 24));
		SolidBrush^ piel = gcnew SolidBrush(Color::FromArgb(255, 240, 206, 178));
		SolidBrush^ pelo = gcnew SolidBrush(Color::FromArgb(255, 76, 54, 40));
		SolidBrush^ panico = gcnew SolidBrush(Color::FromArgb(255, 242, 88, 88));
		array<Color>^ tunicas = gcnew array<Color>{
			Color::FromArgb(255, 198, 152, 68),
			Color::FromArgb(255, 76, 146, 98),
			Color::FromArgb(255, 84, 142, 196),
			Color::FromArgb(255, 178, 92, 92)
		};

		for (size_t i = 0; i < aldeanos.size(); i++) {
			const AldeanoN2& al = aldeanos[i];
			int ax = al.x - camX;
			int ay = al.y - camY;
			if (ax < -48 || ax > 992 || ay < -48 || ay > 648) continue;

			Bitmap^ sprite = (arr != nullptr && al.colorIdx >= 0 && al.colorIdx < arr->Length) ? arr[al.colorIdx] : nullptr;
			if (sprite != nullptr) {
				int cols = (sprite->Width >= 240) ? 10 : ((sprite->Width >= 128) ? 8 : 4);
				int rows = (sprite->Height >= 240) ? 8 : 4;
				int fw = Math::Max(1, sprite->Width / cols);
				int fh = Math::Max(1, sprite->Height / rows);
				int colF = al.huyendo ? ((al.ticksAnim / 5) % cols) : ((cols >= 4) ? 1 : 0);
				static const int filaPorDir[4] = { 0, 2, 3, 1 };
				int rowF = filaPorDir[al.dir & 3] % rows;
				g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
				g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
				g->DrawImage(sprite, Rectangle(ax - 6, ay - 16, fw * 2, fh * 2), Rectangle(colF * fw, rowF * fh, fw, fh), GraphicsUnit::Pixel);
			}
			else {
				SolidBrush^ tunica = gcnew SolidBrush(tunicas[al.colorIdx % 4]);
				int pasoY = al.huyendo ? ((al.ticksAnim / 5) % 2) * 2 : 0;
				g->FillRectangle(borde, ax + 2, ay - 6 - pasoY, 16, 28);
				g->FillRectangle(pelo, ax + 4, ay - 4 - pasoY, 12, 5);
				g->FillRectangle(piel, ax + 5, ay + 1 - pasoY, 10, 6);
				g->FillRectangle(tunica, ax + 4, ay + 7 - pasoY, 12, 11);
				g->FillRectangle(borde, ax + 5, ay + 18 - pasoY, 4, 4);
				g->FillRectangle(borde, ax + 11, ay + 18 - pasoY, 4, 4);
				delete tunica;
			}

			if (al.huyendo) {
				g->FillRectangle(panico, ax + 8, ay - 25, 4, 6);
				g->FillRectangle(panico, ax + 8, ay - 18, 4, 2);
			}
		}

		delete borde;
		delete piel;
		delete pelo;
		delete panico;
	}

	void dibujarEnemigos(Graphics^ g) {
		Bitmap^ bmp = bmpEnemigo;
		if (bmp == nullptr) {
			SolidBrush^ pincelEne = gcnew SolidBrush(Color::Red);
			for (size_t i = 0; i < enemigos.size(); i++) {
				if (enemigos[i] != nullptr && enemigos[i]->getActivo()) {
					g->FillRectangle(pincelEne, enemigos[i]->getX() - camX, enemigos[i]->getY() - camY, enemigos[i]->getAncho(), enemigos[i]->getAlto());
				}
			}
			delete pincelEne;
			return;
		}

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;

		int frameW = bmp->Width / 8;
		int frameH = bmp->Height / 4;
		int drawW = frameW * 2;
		int drawH = frameH * 2;

		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en != nullptr && en->getActivo()) {
				int px = en->getX() - camX;
				int py = en->getY() - camY;
				int offsetX = (drawW - en->getAncho()) / 2;
				int offsetY = drawH - en->getAlto();
				Rectangle srcRect(en->getIdX() * frameW, en->getIdY() * frameH, frameW, frameH);
				Rectangle destRect(px - offsetX, py - offsetY, drawW, drawH);
				g->DrawImage(bmp, destRect, srcRect, GraphicsUnit::Pixel);
			}
		}
	}

	void dibujarProtagonista(Graphics^ g) {
		if (protagonista == nullptr) return;
		Bitmap^ bmp = bmpProtagonista;
		if (bmp == nullptr) return;
		if (ticksCooldownDanio > 0 && ticksCooldownDanio % 3 == 0) return;
		if (correrActivo && protagonista->getEnMovimiento()) {
			DibujarPolvoCorrerComun(g, protagonista->getX(), protagonista->getY(), protagonista->getAncho(), protagonista->getAlto(), protagonista->getDireccion(), ticksJuego, camX, camY, 1);
		}
		protagonista->dibujar(g, bmp, camX, camY, 1, ticksJuego);
	}

	void dibujarArboles(Graphics^ g) {
		for (size_t i = 0; i < arboles.size(); i++) {
			if (arboles[i] != nullptr) {
				arboles[i]->dibujar(g, nullptr, camX, camY, 1, ticksJuego);
			}
		}
	}

	void dibujarHechizo(Graphics^ g) {
		if (proyectiles.empty()) return;
		Bitmap^ bmp = bmpHechizo;
		if (bmp == nullptr) return;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;

		int frameW = bmp->Width / 4;
		int frameH = bmp->Height / 4;

		for (size_t k = 0; k < proyectiles.size(); k++) {
			const ProyectilN2& pr = proyectiles[k];
			int idY = 3;
			if (pr.dir == 'W') idY = 0;
			else if (pr.dir == 'S') idY = 1;
			else if (pr.dir == 'A') idY = 2;

			int idX = 0;
			if (pr.explotando) {
				idX = 1 + (pr.ticks / 4);
				if (idX > 3) idX = 3;
			}

			int px = pr.x - camX;
			int py = pr.y - camY;
			Rectangle srcRect(idX * frameW, idY * frameH, frameW, frameH);
			Rectangle destRect(px, py, frameW * 2, frameH * 2);
			g->DrawImage(bmp, destRect, srcRect, GraphicsUnit::Pixel);
		}
	}

	void dibujarHUD(Graphics^ g) {
		std::string tit;
		std::string det;
		int paso = 1;
		int tot = 2;
		int targetX = 107 * 32 + 24;
		int targetY = 8 * 32 + 24;
		obtenerGuiaPasoActual(tit, det, paso, tot, targetX, targetY);

		if (guiaActiva && !completado) {
			int jSX = protagonista->getX() + protagonista->getAncho() / 2 - camX;
			int jSY = protagonista->getY() + protagonista->getAlto() / 2 - camY;
			DibujarFlechaGuiaComun(g, jSX, jSY, targetX - camX, targetY - camY, 960, 600, ticksJuego);
		}

		SolidBrush^ panel = gcnew SolidBrush(Color::FromArgb(228, 10, 16, 24));
		g->FillRectangle(panel, 16, 16, 444, 42);

		for (int i = 0; i < 3; i++) {
			SolidBrush^ cor = gcnew SolidBrush((i < getVidas()) ? Color::FromArgb(220, 210, 58, 76) : Color::FromArgb(120, 70, 58, 62));
			int cx = 28 + i * 20;
			int cy = 29;
			g->FillEllipse(cor, cx, cy, 8, 8);
			g->FillEllipse(cor, cx + 6, cy, 8, 8);
			g->FillPolygon(cor, gcnew array<Point>{ Point(cx, cy + 6), Point(cx + 14, cy + 6), Point(cx + 7, cy + 15) });
			delete cor;
		}

		System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Bold);
		System::Drawing::Font^ fObj = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
		System::Drawing::Font^ fSub = gcnew System::Drawing::Font("Segoe UI", 8.5f, FontStyle::Regular);
		String^ estadoLibre = tieneLibretaInicial ? ("AIRE: " + cargasHechizoAire.ToString()) : "SIN LIBRETA";
		g->DrawString("TARTAH  |  " + estadoLibre + "  |  PTS: " + puntaje.ToString(), fTexto, Brushes::Gainsboro, PointF(100.0f, 29.0f));

		if (guiaActiva) {
			g->FillRectangle(panel, 16, 66, 300, 102);
			SolidBrush^ barraLateral = gcnew SolidBrush(Color::FromArgb(220, 214, 198, 166));
			g->FillRectangle(barraLateral, 16, 66, 3, 102);
			delete barraLateral;

			int distM = (int)(Math::Sqrt((double)((targetX - protagonista->getX()) * (targetX - protagonista->getX()) + (targetY - protagonista->getY()) * (targetY - protagonista->getY()))) / 32.0);
			g->DrawString("MISIÓN (" + paso.ToString() + "/" + tot.ToString() + ")", fSub, Brushes::Silver, PointF(28.0f, 72.0f));
			g->DrawString(ToManaged(tit), fObj, Brushes::Khaki, PointF(28.0f, 88.0f));
			g->DrawString(ToManaged(det) + " (" + distM.ToString() + " m)", fSub, Brushes::LightGray, RectangleF(28.0f, 108.0f, 276.0f, 18.0f));

			g->DrawString("CUARTOS EXPLORADOS: " + cuartosExplorados.ToString() + "/9", fSub, Brushes::LightGreen, PointF(28.0f, 130.0f));
			SolidBrush^ bFondo = gcnew SolidBrush(Color::FromArgb(255, 24, 34, 48));
			SolidBrush^ bFill = gcnew SolidBrush(Color::FromArgb(255, 76, 178, 114));
			Pen^ pBorde = gcnew Pen(Color::FromArgb(140, 214, 198, 166), 1.0f);
			g->FillRectangle(bFondo, 28, 148, 268, 10);
			int anchoFill = (cuartosExplorados * 268) / 9;
			g->FillRectangle(bFill, 28, 148, anchoFill, 10);
			g->DrawRectangle(pBorde, 28, 148, 268, 10);
			delete bFondo;
			delete bFill;
			delete pBorde;
		}
		delete panel;
		delete fTexto;
		delete fObj;
		delete fSub;
	}

	void dibujarMensajes(Graphics^ g) {
		if (ticksMensaje > 0) {
			System::Drawing::Font^ fuente = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Bold);
			SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(190, 8, 12, 18));
			SizeF tam = g->MeasureString(ToManaged(mensajeTemporal), fuente);
			int ancho = (int)tam.Width + 32;
			int x = (960 - ancho) / 2;
			g->FillRectangle(fondo, x, 542, ancho, 34);
			g->DrawString(ToManaged(mensajeTemporal), fuente, Brushes::WhiteSmoke, PointF((float)(x + 16), 549.0f));
			delete fuente;
			delete fondo;
		}
	}

	void actualizarCamara() {
		camX = protagonista->getX() + protagonista->getAncho() / 2 - 960 / 2;
		camY = protagonista->getY() + protagonista->getAlto() / 2 - 600 / 2;
		if (camX < 0) camX = 0;
		if (camY < 0) camY = 0;
		int maxX = (int)mapa[0].length() * 32 - 960;
		int maxY = (int)mapa.size() * 32 - 600;
		if (camX > maxX) camX = maxX;
		if (camY > maxY) camY = maxY;
	}

	void persecucionEnemigos() {
		if (protagonista == nullptr) return;
		int px = protagonista->getX();
		int py = protagonista->getY();

		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en == nullptr || !en->getActivo()) continue;

			int ex = en->getX();
			int ey = en->getY();
			int dx = 0, dy = 0;
			if (px > ex) dx = 1; else if (px < ex) dx = -1;
			if (py > ey) dy = 1; else if (py < ey) dy = -1;

			int vel = en->getVelocidad();
			if (vel < 1) vel = 1;
			bool fueraVista = (ex + en->getAncho() < camX || ex > camX + 960 || ey + en->getAlto() < camY || ey > camY + 600);
			if (fueraVista) vel += 2;
			int paso = (dx != 0 && dy != 0 && vel >= 4) ? (vel * 3) / 4 : vel;

			if (dx > 0) en->setIdY(2);
			else if (dx < 0) en->setIdY(3);
			else if (dy > 0) en->setIdY(1);
			else if (dy < 0) en->setIdY(0);

			if (dx != 0) {
				for (int s = paso; s >= 1; s--) {
					if (!hayColision(ex + dx * s, ey, en->getAncho(), en->getAlto())) {
						en->setX(ex + dx * s);
						break;
					}
				}
			}
			if (dy != 0) {
				for (int s = paso; s >= 1; s--) {
					if (!hayColision(en->getX(), ey + dy * s, en->getAncho(), en->getAlto())) {
						en->setY(ey + dy * s);
						break;
					}
				}
			}
		}
	}

	void colisionEnemigos() {
		if (protagonista == nullptr) return;
		int px = protagonista->getX();
		int py = protagonista->getY();
		int pw = protagonista->getAncho();
		int ph = protagonista->getAlto();

		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en == nullptr || !en->getActivo()) continue;
			int ex = en->getX();
			int ey = en->getY();
			int ew = en->getAncho();
			int eh = en->getAlto();

			if (px < ex + ew && px + pw > ex && py < ey + eh && py + ph > ey) {
				protagonista->restarVida();
				GestorAudio::ReproducirSfxAccion();
				ticksCooldownDanio = 42;
				mensajeTemporal = "[DAÑO] ¡Una sanguijuela sombría te ha golpeado!";
				ticksMensaje = 70;
				if (protagonista->getVidas() <= 0) {
					reiniciarNivel();
					mensajeTemporal = "Has perdido todas tus vidas. Nivel 2 reiniciado.";
					ticksMensaje = 110;
				}
				break;
			}
		}
	}

	void actualizarCuartoActual() {
		int px = protagonista->getX() + protagonista->getAncho() / 2;
		int py = protagonista->getY() + protagonista->getAlto() / 2;
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto > 0 && nuevoCuarto < 10 && !cuartosVisitados[nuevoCuarto]) {
				cuartosVisitados[nuevoCuarto] = true;
				int cont = 0;
				for (int k = 1; k <= 9; k++) {
					if (cuartosVisitados[k]) cont++;
				}
				cuartosExplorados = cont;
				puntaje += 100;
				if (nuevoCuarto != 1) {
					solicitaEscenaCuarto = true;
					cuartoEscenaPendiente = nuevoCuarto;
				}
				mensajeTemporal = std::string("[") + obtenerNombreCuarto(nuevoCuarto) + "] +100 pts (" + std::to_string(cuartosExplorados) + "/9)";
				ticksMensaje = 95;
			}
		}
	}

	int determinarCuarto(int px, int py) {
		int cx = px / 32;
		int cy = py / 32;
		if (cx >= 7 && cx <= 29 && cy >= 5 && cy <= 13) return 1;
		if (cx >= 39 && cx <= 61 && cy >= 11 && cy <= 19) return 2;
		if (cx >= 71 && cx <= 91 && cy >= 11 && cy <= 19) return 3;
		if (cx >= 99 && cx <= 115 && cy >= 17 && cy <= 27) return 4;
		if (cx >= 7 && cx <= 19 && cy >= 25 && cy <= 33) return 5;
		if (cx >= 39 && cx <= 61 && cy >= 28 && cy <= 36) return 6;
		if (cx >= 71 && cx <= 89 && cy >= 29 && cy <= 37) return 7;
		if (cx >= 7 && cx <= 29 && cy >= 45 && cy <= 53) return 8;
		if (cx >= 39 && cx <= 83 && cy >= 46 && cy <= 54) return 9;
		return 0;
	}
};

#endif
