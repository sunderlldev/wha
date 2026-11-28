#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <gcroot.h>
#include "Configuracion.h"
#include "Utilidades.h"
#include "Protagonista.h"
#include "NPC.h"
#include "Enemigo.h"
#include "BloqueMapa.h"
#include "Inventario.h"
#include "MapaNivel3.h"
#include "GestorAudio.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

struct ParticulaPlata {
	float x;
	float y;
	float velY;
	int tam;
};

class Nivel3 {
private:
	std::vector<std::string> mapa;
	Protagonista* protagonista;
	Inventario* inventario;
	NPC* dagda;
	NPC* ininia;
	std::vector<Enemigo*> enemigos;
	std::vector<ProyectilMago> proyectilesEnemigos;
	std::vector<BloqueMapa*> objetos;
	std::vector<ParticulaPlata> particulas;

	gcroot<Bitmap^> bmpCoustas;
	gcroot<Bitmap^> bmpSillaCoustas;
	gcroot<Bitmap^> bmpDagda;
	gcroot<Bitmap^> bmpIninia;
	gcroot<Bitmap^> bmpEnemigo;
	gcroot<Bitmap^> bmpDisparo;
	gcroot<Bitmap^> bmpRoca;
	gcroot<Bitmap^> bmpMuro;
	gcroot<Bitmap^> bmpCamino;
	gcroot<Bitmap^> bmpPasto;
	gcroot<Bitmap^> bmpMadera;
	gcroot<Bitmap^> bmpAgua;
	gcroot<Bitmap^> bmpPuerta;
	gcroot<Bitmap^> bmpArbolPlata;
	gcroot<String^> dialogoManaged;

	int camX;
	int camY;
	int ticksJuego;
	int ticksTituloNivel;
	int invulnerableTicks;
	int cooldownVuelo;
	int ticksImpulsoVuelo;
	int impulsoDx;
	int impulsoDy;
	int runasReunidas;
	bool portonAbierto;
	bool habloDagda;
	bool decisionTomada;
	bool aceptoSemilla;
	bool resistenciaActiva;
	int ticksResistencia;
	bool enIntro;
	bool guiaActiva;
	bool correrActivo;
	bool completado;
	bool solicitaFinNivel;

	bool dialogoActivo;
	std::string dialogoNpc;
	int estadoDialogo;
	std::string dialogoHablante;
	std::string dialogoRol;
	std::vector<std::string> dialogoLineas;
	std::vector<std::string> dialogoOpciones;
	int dialogoSeleccion;
	int charsVisibles;
	int ticksTypewriter;
	std::string mensajeTemporal;
	int ticksMensaje;
	std::string promptInteraccion;
	std::string objetivoActual;

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		return CargarBitmapSeguroComun(nombreArchivo, false);
	}

	void limpiarEntidades() {
		if (dagda != nullptr) {
			delete dagda;
			dagda = nullptr;
		}
		if (ininia != nullptr) {
			delete ininia;
			ininia = nullptr;
		}
		for (size_t i = 0; i < enemigos.size(); i++) delete enemigos[i];
		enemigos.clear();
		proyectilesEnemigos.clear();
		for (size_t i = 0; i < objetos.size(); i++) delete objetos[i];
		objetos.clear();
	}

	static bool esSolido(char ch) {
		return ch == '#' || ch == 'O' || ch == 'P' || ch == 'T' || ch == 'X' || ch == '~' || ch == 'C';
	}

	bool posicionLibre(int px, int py) {
		int ts = Configuracion::TILE_SIZE;
		int w = protagonista->getAncho();
		int h = protagonista->getAlto();
		int xs[4] = { px + 4, px + w - 4, px + 4, px + w - 4 };
		int ys[4] = { py + 12, py + 12, py + h - 2, py + h - 2 };
		for (int i = 0; i < 4; i++) {
			int col = xs[i] / ts;
			int fila = ys[i] / ts;
			if (fila < 0 || fila >= (int)mapa.size() || col < 0 || col >= (int)mapa[fila].size()) return false;
			if (esSolido(mapa[fila][col])) return false;
		}
		Rectangle r(px + 3, py + 12, w - 6, h - 14);
		if (dagda != nullptr && dagda->getVisible() && r.IntersectsWith(dagda->getCollisionRect())) return false;
		if (ininia != nullptr && ininia->getVisible() && r.IntersectsWith(ininia->getCollisionRect())) return false;
		return true;
	}

	void actualizarObjetivo() {
		if (decisionTomada) {
			objetivoActual = "Destino sellado junto al Gran Árbol de Plata";
		}
		else if (resistenciaActiva) {
			objetivoActual = "Resiste la embestida de Ininia con tu Capa de Vuelo [F]";
		}
		else if (habloDagda) {
			objetivoActual = "Acércate a Ininia frente al Gran Árbol de Plata";
		}
		else if (portonAbierto) {
			objetivoActual = "Auxilia a Dagda en el Santuario del Árbol de Plata";
		}
		else if (runasReunidas >= 2) {
			objetivoActual = "Abre el Portón Rúnico del Santuario (Este)";
		}
		else {
			objetivoActual = "Reúne las 2 Runas del Desfiladero (" + std::to_string(runasReunidas) + "/2)";
		}
	}

	void inicializarParticulas() {
		particulas.clear();
		unsigned int sem = 918273u;
		for (int i = 0; i < 64; i++) {
			sem = sem * 1664525u + 1013904223u;
			ParticulaPlata p;
			p.x = (float)(sem % 960);
			sem = sem * 1664525u + 1013904223u;
			p.y = (float)(sem % 600);
			p.velY = 0.35f + (float)((sem % 5) * 0.14f);
			p.tam = 2 + (int)(sem % 2);
			particulas.push_back(p);
		}
	}

	void actualizarParticulas() {
		for (size_t i = 0; i < particulas.size(); i++) {
			particulas[i].y += particulas[i].velY;
			particulas[i].x += (float)Math::Sin((ticksJuego + (int)i * 7) * 0.05) * 0.25f;
			if (particulas[i].y > 604) {
				particulas[i].y = -4;
			}
		}
	}

	void prepararTextoDialogo() {
		std::string full = "";
		for (size_t i = 0; i < dialogoLineas.size(); i++) {
			if (i > 0) full += "\n";
			full += dialogoLineas[i];
		}
		dialogoManaged = ToManaged(full);
		charsVisibles = 0;
		ticksTypewriter = 0;
		dialogoSeleccion = 0;
	}

	void cerrarDialogo() {
		dialogoActivo = false;
		dialogoLineas.clear();
		dialogoOpciones.clear();
		dialogoManaged = "";
	}

	void iniciarDialogo(const std::string& npc, int estadoInicial) {
		dialogoActivo = true;
		dialogoNpc = npc;
		estadoDialogo = estadoInicial;
		if (npc == "Dagda") {
			dialogoHablante = "Dagda";
			dialogoRol = "Protector de Coustas";
		}
		else if (npc == "Ininia") {
			dialogoHablante = "Ininia";
			dialogoRol = "Sombrero de Ala Ancha";
		}
		else if (npc == "Arbol") {
			dialogoHablante = "Gran Árbol de Plata";
			dialogoRol = "Santuario Ancestral";
		}
		else if (npc == "CoustasInicio" || npc == "Pergamino1" || npc == "Pergamino2" || npc == "ConsejoCapa") {
			if (npc == "CoustasInicio" || npc == "ConsejoCapa") dialogoHablante = "Coustas";
			else dialogoHablante = "Pergamino Antiguo";
			dialogoRol = "Historia";
		}
		else {
			dialogoHablante = "Letrero";
			dialogoRol = "Crónica";
		}
		cargarLineasDialogo();
	}

	void iniciarDialogoSimple(const std::string& hablante, const std::string& rol, const std::string& texto) {
		dialogoActivo = true;
		dialogoNpc = "Simple";
		estadoDialogo = -1;
		dialogoHablante = hablante;
		dialogoRol = rol;
		dialogoLineas.clear();
		dialogoLineas.push_back(texto);
		dialogoOpciones.clear();
		dialogoOpciones.push_back("[1] Cerrar");
		prepararTextoDialogo();
	}

	void cargarLineasDialogo() {
		dialogoLineas.clear();
		dialogoOpciones.clear();

		if (dialogoNpc == "Dagda") {
			if (estadoDialogo == 500) {
				dialogoLineas.push_back("¡Coustas...! Lograste cruzar el desfiladero sano y salvo...");
				dialogoLineas.push_back("Los Sombreros de Ala Ancha me hirieron al cubrir tu caravana.");
				dialogoLineas.push_back("Esa Capa de Vuelo que tejieron Coco y Tartah te trajo hasta aquí.");
				dialogoOpciones.push_back("[1] ¡Dagda, resiste! Buscaremos ayuda juntos...");
				dialogoOpciones.push_back("[2] No voy a dejarte atrás en este santuario.");
			}
			else if (estadoDialogo == 501) {
				dialogoLineas.push_back("Escúchame, muchacho... Una de ellos aguarda bajo el Árbol de Plata.");
				dialogoLineas.push_back("Te prometerá sanar tus piernas con trazos prohibidos... No cedas.");
				dialogoLineas.push_back("Tu música vale más que cualquier milagro oscuro.");
				dialogoOpciones.push_back("[1] Avanzar hacia Ininia bajo el Árbol de Plata");
			}
			else {
				dialogoLineas.push_back("Coustas... pase lo que pase, protege tu luz.");
				dialogoOpciones.push_back("[1] Cerrar");
			}
		}
		else if (dialogoNpc == "Ininia") {
			if (estadoDialogo == 600) {
				dialogoLineas.push_back("Qué conmovedor vínculo entre un bardo y su protector...");
				dialogoLineas.push_back("Pero Dagda se desangra frente a tus ojos, pequeño Coustas.");
				dialogoLineas.push_back("La Asamblea jamás te devolverá las piernas ni salvará su vida.");
				dialogoOpciones.push_back("[1] ¿Qué buscas exactamente aquí?");
				dialogoOpciones.push_back("[2] La magia prohibida solo trae tragedia.");
			}
			else if (estadoDialogo == 601) {
				dialogoLineas.push_back("Soy Ininia. Toma esta Semilla Prohibida del Gran Árbol de Plata.");
				dialogoLineas.push_back("Traza el sello proscrito sobre la carne: cierra sus heridas");
				dialogoLineas.push_back("y vuelve a correr libre por los valles tocando tu laúd.");
				dialogoOpciones.push_back("[1] [Aceptar la Semilla] Salvaré a Dagda, cueste lo que cueste");
				dialogoOpciones.push_back("[2] [Rechazar] ¡No traicionaré la fe de Dagda, Coco y Tartah!");
			}
			else if (estadoDialogo == 602) {
				dialogoLineas.push_back("La semilla ya echa raíces oscuras entre tus manos, bardo.");
				dialogoLineas.push_back("El pacto de la Noche de Plata está sellado; desde hoy");
				dialogoLineas.push_back("cargas con la sombra de los Sombreros de Ala Ancha.");
				dialogoOpciones.push_back("[1] Sellar el Pacto Prohibido");
			}
			else if (estadoDialogo == 603) {
				dialogoLineas.push_back("¡Necio! Desprecias un milagro frente a la mismísima muerte...");
				dialogoLineas.push_back("Si rehúsas el pacto, ¡no saldrás de este santuario con vida!");
				dialogoOpciones.push_back("[1] ¡Esquivar su embestida con la Capa de Vuelo!");
			}
		}
		else if (dialogoNpc == "Arbol") {
			if (estadoDialogo == 604) {
				dialogoLineas.push_back("El resplandor puro del Gran Árbol de Plata responde a tu voluntad");
				dialogoLineas.push_back("y obliga a Ininia a retroceder hacia la penumbra.");
				dialogoLineas.push_back("La savia serena envuelve a Dagda hasta la llegada de Coco y Qifrey.");
				dialogoOpciones.push_back("[1] Concluir la travesía bajo la luz de plata");
			}
		}
		else if (dialogoNpc == "CoustasInicio") {
			dialogoLineas.push_back("¡La caravana fue emboscada por los Sombreros de Ala Ancha!");
			dialogoLineas.push_back("Dagda corrió herido hacia el Santuario del Árbol de Plata para salvarme.");
			dialogoLineas.push_back("La niebla oscura reduce mi visión e invierte el mando [W/A/S/D].");
			dialogoLineas.push_back("Debo hallar los 2 Pergaminos del Sello para abrir el Portón del Este.");
			dialogoOpciones.push_back("[1] Avanzar por el desfiladero");
		}
		else if (dialogoNpc == "Pergamino1") {
			dialogoLineas.push_back("Crónica de Dagda: Tras perder la movilidad de mis piernas en un");
			dialogoLineas.push_back("derrumbe, Dagda cuidó de mí por los caminos. Coco y Tartah me");
			dialogoLineas.push_back("tejieron esta Capa de Vuelo antes de quedar atrapados en el desfiladero.");
			dialogoOpciones.push_back("[1] Guardar el pergamino (" + std::to_string(runasReunidas) + "/2)");
		}
		else if (dialogoNpc == "ConsejoCapa") {
			dialogoLineas.push_back("¡Es verdad! Con la Capa de Vuelo de Coco y Tartah puedo esquivar.");
			dialogoLineas.push_back("Presiona [F] para impulsar la silla y atravesar las ráfagas oscuras.");
			dialogoOpciones.push_back("[1] Entendido");
		}
		else if (dialogoNpc == "Pergamino2") {
			dialogoLineas.push_back("Aviso del Consejo: Los 2 Sellos resguardan el Árbol de Plata de");
			dialogoLineas.push_back("Ininia y los Sombreros de Ala Ancha, quienes buscan obligarme a usar");
			dialogoLineas.push_back("magia prohibida para sanar a Dagda y recuperar mis piernas.");
			dialogoOpciones.push_back("[1] Guardar el pergamino (" + std::to_string(runasReunidas) + "/2)");
		}
		prepararTextoDialogo();
	}

	void dispararRafagaIninia() {
		if (ininia == nullptr || !ininia->getVisible()) return;
		int ox = ininia->getX();
		int oy = ininia->getY();
		int cx = protagonista->getX() - ox;
		int cy = protagonista->getY() - oy;
		int dirBase = (abs(cx) >= abs(cy)) ? ((cx < 0) ? 1 : 3) : ((cy < 0) ? 2 : 0);
		proyectilesEnemigos.push_back(ProyectilMago::CrearPorDireccion(ox, oy, dirBase, 5));
		int d2 = (dirBase == 1 || dirBase == 3) ? 0 : 1;
		int d3 = (dirBase == 1 || dirBase == 3) ? 2 : 3;
		if (proyectilesEnemigos.size() < 20) {
			proyectilesEnemigos.push_back(ProyectilMago::CrearPorDireccion(ox, oy, d2, 4));
			proyectilesEnemigos.push_back(ProyectilMago::CrearPorDireccion(ox, oy, d3, 4));
		}
	}

	void procesarOpcion(int op) {
		if (dialogoNpc == "Simple") {
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Dagda") {
			if (estadoDialogo == 500) {
				estadoDialogo = 501;
				cargarLineasDialogo();
				return;
			}
			habloDagda = true;
			actualizarObjetivo();
			cerrarDialogo();
			mensajeTemporal = "Acércate a Ininia bajo el Gran Árbol de Plata.";
			ticksMensaje = 95;
			return;
		}
		if (dialogoNpc == "Ininia") {
			if (estadoDialogo == 600) {
				estadoDialogo = 601;
				cargarLineasDialogo();
				return;
			}
			if (estadoDialogo == 601) {
				if (op == 1) {
					aceptoSemilla = true;
					inventario->agregarItem("Semilla Prohibida", "Semilla oscura del Gran Árbol de Plata entregada por Ininia.", "Reliquia Prohibida");
					estadoDialogo = 602;
				}
				else {
					aceptoSemilla = false;
					inventario->agregarItem("Rama de Plata Pura", "Rama luminosa que simboliza la fe de Dagda, Coco y Tartah.", "Reliquia del Santuario");
					estadoDialogo = 603;
				}
				cargarLineasDialogo();
				return;
			}
			if (estadoDialogo == 602) {
				decisionTomada = true;
				completado = true;
				solicitaFinNivel = true;
				actualizarObjetivo();
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 603) {
				resistenciaActiva = true;
				ticksResistencia = 110;
				proyectilesEnemigos.clear();
				dispararRafagaIninia();
				actualizarObjetivo();
				cerrarDialogo();
				mensajeTemporal = "¡Esquiva la embestida de Ininia con [F] mientras carga la luz!";
				ticksMensaje = 105;
				return;
			}
		}
		if (dialogoNpc == "Arbol" && estadoDialogo == 604) {
			decisionTomada = true;
			completado = true;
			solicitaFinNivel = true;
			actualizarObjetivo();
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Pergamino1") {
			iniciarDialogo("ConsejoCapa", 711);
			return;
		}
		if (dialogoNpc == "Pergamino2" && runasReunidas >= 2) {
			cerrarDialogo();
			mensajeTemporal = "¡Ya tienes los 2 Pergaminos! Ve al Portón del Este.";
			ticksMensaje = 110;
			return;
		}
		cerrarDialogo();
	}

	void iniciar() {
		MapaNivel3::cargarMatriz(mapa);
		limpiarEntidades();

		if (protagonista == nullptr) protagonista = new Protagonista(7 * 32, 22 * 32, 3);
		else {
			protagonista->setX(7 * 32);
			protagonista->setY(22 * 32);
			protagonista->setVidas(3);
			protagonista->setDireccion(3);
			protagonista->detener();
		}
		if (inventario == nullptr) inventario = new Inventario();
		else inventario->limpiar();

		inventario->agregarItem("Capa de Vuelo", "Tejida por Coco y Tartah. Pulsa [F] para impulsarte sobre las sombras.", "Equipamiento Mágico");
		inventario->agregarItem("Laúd de Coustas", "Instrumento inseparable que acompaña sus viajes junto a Dagda.", "Objeto Personal");

		dagda = new NPC(77 * 32, 22 * 32, 4, "Dagda", "Protector de Coustas");
		dagda->setMirada(1);
		ininia = new NPC(84 * 32, 23 * 32, 5, "Ininia", "Sombrero de Ala Ancha");
		ininia->setMirada(1);

		enemigos.push_back(new Enemigo(18 * 32, 22 * 32, 1, 15 * 32, 27 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(26 * 32, 22 * 32, 1, 20 * 32, 24 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(32 * 32, 10 * 32, 1, 7 * 32, 17 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(42 * 32, 10 * 32, 0, 39 * 32, 52 * 32, "Sombrero de Ala Ancha", 3));
		enemigos.push_back(new Enemigo(60 * 32, 14 * 32, 1, 8 * 32, 19 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(47 * 32, 22 * 32, 0, 35 * 32, 57 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(32 * 32, 34 * 32, 1, 27 * 32, 37 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(42 * 32, 34 * 32, 0, 40 * 32, 52 * 32, "Sombrero de Ala Ancha", 3));
		enemigos.push_back(new Enemigo(60 * 32, 30 * 32, 1, 26 * 32, 37 * 32, "Sombrero de Ala Ancha", 2));
		enemigos.push_back(new Enemigo(65 * 32, 22 * 32, 1, 20 * 32, 24 * 32, "Sombrero de Ala Ancha", 2));

		objetos.push_back(new BloqueMapa(12 * 32, 19 * 32, ObjetoLetrero, "Caravana Emboscada", "Crónica: Nuestra caravana fue asaltada aquí. Usa [F] para activar la Capa de Vuelo de Coco y Tartah."));
		objetos.push_back(new BloqueMapa(27 * 32, 19 * 32, ObjetoLetrero, "Desfiladero de las Ruinas", "Letrero: El Portón del Santuario exige 2 Pergaminos del Sello (Ruinas del Norte y Barranco del Sur)."));
		objetos.push_back(new BloqueMapa(55 * 32, 9 * 32, ObjetoPergamino, "Pergamino I: Crónica de Dagda", "Pergamino del norte con la historia del derrumbe de Coustas y Dagda."));
		objetos.push_back(new BloqueMapa(55 * 32, 35 * 32, ObjetoPergamino, "Pergamino II: Sello del Santuario", "Pergamino del sur con la advertencia sobre Ininia y el Árbol de Plata."));
		objetos.push_back(new BloqueMapa(36 * 32, 9 * 32, ObjetoHoja, "Hierbas de Plata", "Remedio de las ruinas que restaura +1 corazón."));
		objetos.push_back(new BloqueMapa(36 * 32, 35 * 32, ObjetoVial, "Tónico de Plata", "Elixir del bosque que recupera +1 corazón."));
		objetos.push_back(new BloqueMapa(71 * 32, 19 * 32, ObjetoLetrero, "Santuario Ancestral", "Letrero: Santuario del Gran Árbol de Plata. Dagda aguarda herido junto a las losas antiguas."));

		camX = 0;
		camY = 0;
		ticksJuego = 0;
		ticksTituloNivel = 160;
		invulnerableTicks = 0;
		cooldownVuelo = 0;
		ticksImpulsoVuelo = 0;
		impulsoDx = 0;
		impulsoDy = 0;
		runasReunidas = 0;
		portonAbierto = false;
		habloDagda = false;
		decisionTomada = false;
		aceptoSemilla = false;
		resistenciaActiva = false;
		ticksResistencia = 0;
		enIntro = true;
		completado = false;
		solicitaFinNivel = false;
		cerrarDialogo();
		mensajeTemporal = "";
		ticksMensaje = 0;
		promptInteraccion = "";
		actualizarObjetivo();
	}

	void actualizarProximidad() {
		promptInteraccion = "";
		if (enIntro || dialogoActivo || resistenciaActiva) return;
		int cx = protagonista->getX() + protagonista->getAncho() / 2;
		int cy = protagonista->getY() + protagonista->getAlto() / 2;

		if (!portonAbierto && cx >= 64 * 32 && cx <= 69 * 32 && cy >= 18 * 32 && cy <= 26 * 32) {
			if (runasReunidas >= 2) {
				promptInteraccion = "Presiona [Espacio] para abrir el Portón Rúnico del Santuario";
			}
			else {
				promptInteraccion = "Portón Sellado: Necesitas las 2 Runas del Desfiladero (" + std::to_string(runasReunidas) + "/2)";
			}
			return;
		}
		if (dagda != nullptr && dagda->estaCerca(cx, cy)) {
			promptInteraccion = "Presiona [Espacio] para hablar con Dagda";
			return;
		}
		if (ininia != nullptr && ininia->getVisible() && ininia->estaCerca(cx, cy)) {
			if (habloDagda) promptInteraccion = "Presiona [Espacio] para confrontar a Ininia";
			else promptInteraccion = "Auxilia primero a Dagda antes de acercarte a Ininia";
			return;
		}
		Rectangle r = protagonista->getRectangle();
		for (size_t i = 0; i < objetos.size(); i++) {
			if (!objetos[i]->getVisible() || objetos[i]->getRecolectado()) continue;
			if (objetos[i]->getInteractionRect().IntersectsWith(r)) {
				if (objetos[i]->getTipo() == ObjetoLetrero) {
					promptInteraccion = "Presiona [Espacio] para leer: " + objetos[i]->getNombre();
				}
				else {
					promptInteraccion = "Presiona [Espacio] para recoger: " + objetos[i]->getNombre();
				}
				return;
			}
		}
	}

public:
	Nivel3() {
		protagonista = nullptr;
		inventario = nullptr;
		dagda = nullptr;
		ininia = nullptr;
		guiaActiva = true;
		correrActivo = false;

		bmpCoustas = CargarBitmapSeguro("sprites/Coustas_Spritesheet.png");
		bmpSillaCoustas = CargarBitmapSeguro("sprites/Coustas_Silla_Spritesheet.png");
		Bitmap^ qifreyBase = CargarBitmapSeguro("sprites/Qifrey_Spritesheet.png");
		bmpDagda = CrearVarianteDagdaComun(qifreyBase);
		if (qifreyBase != nullptr) delete qifreyBase;

		bmpEnemigo = CargarBitmapSeguro("sprites/Mago_Cueva_Spritesheet.png");
		bmpIninia = CrearVarianteIniniaComun(bmpEnemigo);
		bmpDisparo = CargarBitmapSeguro("sprites/Disparo_Cueva_Spritesheet.png");
		bmpRoca = CargarBitmapSeguro("sprites/tile_roca.png");
		bmpMuro = CargarBitmapSeguro("sprites/tile_muro.png");
		bmpCamino = CargarBitmapSeguro("sprites/tile_camino.png");
		bmpPasto = CargarBitmapSeguro("sprites/tile_pasto.png");
		bmpMadera = CargarBitmapSeguro("sprites/tile_madera.png");
		bmpAgua = CargarBitmapSeguro("sprites/tile_agua.png");
		bmpPuerta = CargarBitmapSeguro("sprites/tile_puerta.png");
		bmpArbolPlata = CargarBitmapSeguro("sprites/tile_arbol_plata.png");

		inicializarParticulas();
		iniciar();
	}

	~Nivel3() {
		limpiarEntidades();
		if (protagonista != nullptr) delete protagonista;
		if (inventario != nullptr) delete inventario;
	}

	void reiniciarNivel() {
		iniciar();
	}

	bool getDialogoActivo() { return dialogoActivo; }
	bool getEnIntro() { return enIntro; }
	bool getGuiaActiva() { return guiaActiva; }
	void alternarGuia() { guiaActiva = !guiaActiva; }

	void obtenerMisiones(std::vector<MisionInfo>& lista, int& indiceActual) {
		lista.clear();
		bool p1 = (runasReunidas >= 2);
		bool p2 = portonAbierto;
		bool p3 = habloDagda;
		bool p4 = decisionTomada;
		lista.push_back(MisionInfo{ "1. Reunir las 2 Runas del Sello", "Recorre las Ruinas del Norte y el Barranco Sur esquivando patrullas.", p1 });
		lista.push_back(MisionInfo{ "2. Abrir el Portón del Santuario", "Acércate al portón rúnico del este con ambas runas.", p2 });
		lista.push_back(MisionInfo{ "3. Auxiliar a Dagda", "Cruza al santuario del Árbol de Plata y escucha a Dagda.", p3 });
		lista.push_back(MisionInfo{ "4. Confrontar a Ininia", "Decide tu camino frente al Gran Árbol de Plata.", p4 });
		indiceActual = 3;
		for (int i = 0; i < (int)lista.size(); i++) {
			if (!lista[i].completada) {
				indiceActual = i;
				break;
			}
		}
	}

	void obtenerPersonajes(std::vector<PersonajeInfo>& lista) {
		lista.clear();
		lista.push_back(PersonajeInfo{ "Coustas", "Bardo Errante", "Músico que perdió la movilidad de sus piernas tras un derrumbe y viaja con la Capa de Vuelo de Coco y Tartah.", "Protagonista", "Coustas" });
		lista.push_back(PersonajeInfo{ "Dagda", "Protector de Coustas", "Trovador y guardián leal de Coustas. Resultó herido al defender la caravana ante el santuario.", habloDagda ? "Amigos" : "Aliado", "Dagda" });
		lista.push_back(PersonajeInfo{ "Ininia", "Sombrero de Ala Ancha", "Aprendiz prohibida que ofrece una semilla del Árbol de Plata para forzar un pacto sombrío.", decisionTomada ? (aceptoSemilla ? "Pacto" : "Rechazada") : "Enigmática", "Ininia" });
		lista.push_back(PersonajeInfo{ "Sombrero de Ala Ancha", "Patrulla del Desfiladero", "Hechicero renegado que custodia las ruinas y dispara ráfagas oscuras.", "Hostil", "Sombrero" });
	}

	void obtenerGuiaPasoActual(std::string& titulo, std::string& detalle, int& pasoNum, int& totalPasos, int& targetX, int& targetY) {
		totalPasos = 4;
		if (runasReunidas < 2) {
			pasoNum = 1;
			titulo = "Reúne las Runas del Sello";
			detalle = "Busca las dos runas en las ruinas norte y sur";
			targetX = 55 * 32 + 16;
			targetY = 9 * 32 + 16;
			for (size_t i = 0; i < objetos.size(); i++) {
				if (!objetos[i]->getVisible() || objetos[i]->getRecolectado()) continue;
				if (objetos[i]->getTipo() == ObjetoPergamino) {
					targetX = objetos[i]->getX() + 16;
					targetY = objetos[i]->getY() + 16;
					break;
				}
			}
			return;
		}
		if (!portonAbierto) {
			pasoNum = 2;
			titulo = "Abre el Portón Rúnico";
			detalle = "Ve al portón del este en el desfiladero";
			targetX = 68 * 32;
			targetY = 22 * 32 + 16;
			return;
		}
		if (!habloDagda) {
			pasoNum = 3;
			titulo = "Auxilia a Dagda";
			detalle = "Acércate a Dagda en el santuario";
			targetX = (dagda != nullptr) ? (dagda->getX() + 12) : (77 * 32);
			targetY = (dagda != nullptr) ? (dagda->getY() + 16) : (22 * 32);
			return;
		}
		pasoNum = 4;
		titulo = resistenciaActiva ? "Resiste a Ininia [F]" : "Confronta a Ininia";
		detalle = resistenciaActiva ? "Esquiva los disparos hasta que cargue la luz" : "Acércate bajo el Gran Árbol de Plata";
		targetX = (ininia != nullptr) ? (ininia->getX() + 12) : (84 * 32);
		targetY = (ininia != nullptr) ? (ininia->getY() + 16) : (23 * 32);
	}

	int getVidas() { return protagonista != nullptr ? protagonista->getVidas() : 3; }
	int getTiempoSegundos() { return (ticksJuego * 25) / 1000; }
	int getCantidadInventario() { return inventario->getCantidad(); }
	std::string getInventarioNombre(int i) { return inventario->getItem(i).nombre; }
	std::string getInventarioDescripcion(int i) { return inventario->getItem(i).descripcion; }
	bool getAceptoSemilla() { return aceptoSemilla; }

	bool consumirSolicitudFinNivel() {
		bool val = solicitaFinNivel;
		solicitaFinNivel = false;
		return val;
	}

	String^ getResumenPartida() {
		String^ estado = completado ? "NIVEL COMPLETADO" : "EN CURSO";
		String^ resumen = "Nivel 3 · La Emboscada y el Gran Árbol de Plata\n";
		resumen += "Estado: " + estado + "\n";
		resumen += "Vidas: " + getVidas().ToString() + "\n";
		resumen += "Runas del Sello: " + runasReunidas.ToString() + "/2\n";
		resumen += "Objetivo: " + ToManaged(objetivoActual) + "\n";
		resumen += "Tiempo: " + getTiempoSegundos().ToString() + " s";
		return resumen;
	}

	void activarCapaVuelo(int dx, int dy) {
		if (enIntro || dialogoActivo || completado) return;
		if (cooldownVuelo > 0) {
			mensajeTemporal = "Capa de Vuelo recargando...";
			ticksMensaje = 35;
			return;
		}
		if (dx != 0 || dy != 0) {
			dx = -dx;
			dy = -dy;
		}
		else {
			int dir = protagonista->getDireccion();
			if (dir == 1) dx = -1;
			else if (dir == 3) dx = 1;
			else if (dir == 2) dy = -1;
			else dy = 1;
		}
		impulsoDx = dx;
		impulsoDy = dy;
		ticksImpulsoVuelo = 10;
		cooldownVuelo = 64;
		invulnerableTicks = 16;
		GestorAudio::ReproducirSfxAccion();
		mensajeTemporal = "¡Impulso con la Capa de Vuelo!";
		ticksMensaje = 36;
	}

	void moverJugador(int dx, int dy, bool corriendo = false) {
		if (enIntro || dialogoActivo || completado) return;
		correrActivo = corriendo;
		if (dx == 0 && dy == 0) {
			protagonista->setEnMovimiento(false);
			return;
		}
		dx = -dx;
		dy = -dy;
		int base = corriendo ? 7 : 4;
		int paso = (dx != 0 && dy != 0) ? (corriendo ? 5 : 3) : base;
		if (dx != 0) {
			int nx = protagonista->getX() + dx * paso;
			if (posicionLibre(nx, protagonista->getY())) protagonista->setX(nx);
		}
		if (dy != 0) {
			int ny = protagonista->getY() + dy * paso;
			if (posicionLibre(protagonista->getX(), ny)) protagonista->setY(ny);
		}
		protagonista->moverLibre(dx, dy);
		int freqPaso = corriendo ? 6 : 10;
		if (ticksJuego % freqPaso == 0) GestorAudio::ReproducirSfxPaso();
	}

	void detenerJugador() {
		correrActivo = false;
		if (protagonista != nullptr) protagonista->detener();
	}

	void moverSeleccion(int delta) {
		if (!dialogoActivo) return;
		int total = (int)dialogoOpciones.size();
		if (total <= 0) return;
		dialogoSeleccion = (dialogoSeleccion + delta + total) % total;
	}

	void elegirOpcionDialogo(int indice) {
		if (!dialogoActivo) return;
		String^ txt = dialogoManaged;
		if (txt != nullptr && charsVisibles < txt->Length) return;
		if (indice < 0 || indice >= (int)dialogoOpciones.size()) return;
		dialogoSeleccion = indice;
		procesarOpcion(indice + 1);
	}

	void interactuar() {
		if (enIntro) {
			enIntro = false;
			ticksTituloNivel = 140;
			iniciarDialogo("CoustasInicio", 700);
			return;
		}
		if (dialogoActivo) {
			String^ txt = dialogoManaged;
			if (txt != nullptr && charsVisibles < txt->Length) {
				charsVisibles = txt->Length;
				return;
			}
			if (!dialogoOpciones.empty()) {
				int idx = dialogoSeleccion;
				if (idx < 0 || idx >= (int)dialogoOpciones.size()) idx = 0;
				procesarOpcion(idx + 1);
			}
			else cerrarDialogo();
			return;
		}
		if (resistenciaActiva) return;

		int cx = protagonista->getX() + protagonista->getAncho() / 2;
		int cy = protagonista->getY() + protagonista->getAlto() / 2;

		if (!portonAbierto && cx >= 64 * 32 && cx <= 69 * 32 && cy >= 18 * 32 && cy <= 26 * 32) {
			if (runasReunidas >= 2) {
				portonAbierto = true;
				GestorAudio::ReproducirSfxAccion();
				for (int r = 20; r <= 24; r++) mapa[r][68] = 'D';
				mensajeTemporal = "¡El Portón Rúnico del Santuario se ha abierto!";
				ticksMensaje = 110;
				actualizarObjetivo();
			}
			else {
				mensajeTemporal = "Necesitas las 2 Runas del Sello del Desfiladero para abrir este portón.";
				ticksMensaje = 95;
			}
			return;
		}

		if (dagda != nullptr && dagda->estaCerca(cx, cy)) {
			iniciarDialogo("Dagda", 500);
			return;
		}

		if (ininia != nullptr && ininia->getVisible() && ininia->estaCerca(cx, cy)) {
			if (!habloDagda) {
				mensajeTemporal = "Auxilia primero a Dagda antes de enfrentar a Ininia.";
				ticksMensaje = 90;
			}
			else if (!decisionTomada) {
				iniciarDialogo("Ininia", 600);
			}
			return;
		}

		Rectangle r = protagonista->getRectangle();
		for (size_t i = 0; i < objetos.size(); i++) {
			if (!objetos[i]->getVisible() || objetos[i]->getRecolectado()) continue;
			if (!objetos[i]->getInteractionRect().IntersectsWith(r)) continue;
			if (objetos[i]->getTipo() == ObjetoLetrero) {
				iniciarDialogoSimple("Crónica del Camino", "Registro", objetos[i]->getDescripcion());
				return;
			}
			std::string nom = objetos[i]->getNombre();
			objetos[i]->recolectar();
			GestorAudio::ReproducirSfxAccion();
			if (nom == "Hierbas de Plata" || nom == "Tónico de Plata") {
				protagonista->sumarVida();
				inventario->agregarItem(nom, objetos[i]->getDescripcion(), "Curación");
				mensajeTemporal = "[" + nom + "] +1 Corazón restaurado";
			}
			else {
				runasReunidas++;
				inventario->agregarItem(nom, objetos[i]->getDescripcion(), "Pergamino del Sello");
				actualizarObjetivo();
				if (nom.find("Pergamino I:") != std::string::npos) {
					iniciarDialogo("Pergamino1", 710);
				}
				else {
					iniciarDialogo("Pergamino2", 720);
				}
			}
			ticksMensaje = 95;
			return;
		}
	}

	void procesarTick(Graphics^ g, int viewW, int viewH, bool corriendo = false) {
		if (g == nullptr) return;
		if (enIntro) {
			dibujarIntro(g, viewW, viewH);
			return;
		}
		correrActivo = corriendo;

		ticksJuego++;
		if (ticksTituloNivel > 0) ticksTituloNivel--;
		if (ticksMensaje > 0) ticksMensaje--;
		if (invulnerableTicks > 0) invulnerableTicks--;
		if (cooldownVuelo > 0) cooldownVuelo--;

		if (ticksImpulsoVuelo > 0) {
			ticksImpulsoVuelo--;
			int nx = protagonista->getX() + impulsoDx * 11;
			if (posicionLibre(nx, protagonista->getY())) protagonista->setX(nx);
			int ny = protagonista->getY() + impulsoDy * 11;
			if (posicionLibre(protagonista->getX(), ny)) protagonista->setY(ny);
		}

		protagonista->animar(corriendo);
		actualizarParticulas();

		if (dialogoActivo) {
			String^ txt = dialogoManaged;
			if (txt != nullptr && charsVisibles < txt->Length) {
				ticksTypewriter++;
				if (ticksTypewriter % 2 == 0) {
					charsVisibles += 2;
					if (charsVisibles > txt->Length) charsVisibles = txt->Length;
					if (ticksTypewriter % 4 == 0) GestorAudio::ReproducirSfxDialogo();
				}
			}
		}
		else {
			if (resistenciaActiva) {
				if (ticksResistencia > 0) ticksResistencia--;
				if (ticksResistencia > 15 && ticksResistencia % 28 == 0) {
					dispararRafagaIninia();
				}
				if (ticksResistencia <= 0) {
					resistenciaActiva = false;
					proyectilesEnemigos.clear();
					if (ininia != nullptr) ininia->setVisible(false);
					GestorAudio::ReproducirSfxAccion();
					iniciarDialogo("Arbol", 604);
					actualizarObjetivo();
				}
			}

			Rectangle rJug(protagonista->getX() + 4, protagonista->getY() + 10, protagonista->getAncho() - 8, protagonista->getAlto() - 12);
			for (size_t i = 0; i < enemigos.size(); i++) {
				Enemigo* e = enemigos[i];
				if (e == nullptr || !e->getActivo()) continue;
				e->moverPatrulla();
				int dxE = protagonista->getX() - e->getX();
				int dyE = protagonista->getY() - e->getY();
				if ((ticksJuego + (int)i * 23) % 72 == 0 && (dxE * dxE + dyE * dyE) <= 440 * 440 && proyectilesEnemigos.size() < 16) {
					proyectilesEnemigos.push_back(ProyectilMago::CrearPorDireccion(e->getX() + 4, e->getY() + 4, e->getDireccion(), 5));
				}
				if (invulnerableTicks == 0 && rJug.IntersectsWith(e->getCollisionRect())) {
					GestorAudio::ReproducirSfxAccion();
					protagonista->restarVida();
					invulnerableTicks = 55;
					int backX = protagonista->getX() - 30;
					if (posicionLibre(backX, protagonista->getY())) protagonista->setX(backX);
					mensajeTemporal = "¡Un Sombrero de Ala Ancha te emboscó! (-1 corazón)";
					ticksMensaje = 90;
					if (protagonista->getVidas() <= 0) {
						iniciar();
						enIntro = false;
						mensajeTemporal = "Has caído en el desfiladero. Nivel 3 reiniciado.";
						ticksMensaje = 120;
						return;
					}
					break;
				}
			}

			int tsP = Configuracion::TILE_SIZE;
			for (size_t i = 0; i < proyectilesEnemigos.size(); ) {
				ProyectilMago& p = proyectilesEnemigos[i];
				if (p.impactando) {
					p.avanzarImpacto();
					if (!p.activo) {
						proyectilesEnemigos.erase(proyectilesEnemigos.begin() + i);
						continue;
					}
					i++;
					continue;
				}
				p.x += p.dx;
				p.y += p.dy;
				p.distanciaRecorrida += (p.dx != 0 ? (p.dx > 0 ? p.dx : -p.dx) : (p.dy > 0 ? p.dy : -p.dy));
				int filP = (p.y + 12) / tsP;
				int colP = (p.x + 12) / tsP;
				bool chocaPared = (filP < 0 || filP >= (int)mapa.size() || colP < 0 || colP >= (int)mapa[filP].size() || esSolido(mapa[filP][colP]));
				if (p.distanciaRecorrida > 420 || chocaPared) {
					p.iniciarImpacto();
					i++;
					continue;
				}
				if (rJug.IntersectsWith(p.getCollisionRect())) {
					p.iniciarImpacto();
					if (invulnerableTicks == 0) {
						GestorAudio::ReproducirSfxAccion();
						protagonista->restarVida();
						invulnerableTicks = 55;
						int backX = protagonista->getX() - 24;
						if (posicionLibre(backX, protagonista->getY())) protagonista->setX(backX);
						mensajeTemporal = "¡Un disparo oscuro te alcanzó! (-1 corazón)";
						ticksMensaje = 90;
						if (protagonista->getVidas() <= 0) {
							iniciar();
							enIntro = false;
							mensajeTemporal = "Has caído en el desfiladero. Nivel 3 reiniciado.";
							ticksMensaje = 120;
							return;
						}
					}
				}
				i++;
			}
		}

		int ts = Configuracion::TILE_SIZE;
		camX = protagonista->getX() + protagonista->getAncho() / 2 - viewW / 2;
		camY = protagonista->getY() + protagonista->getAlto() / 2 - viewH / 2;
		int maxCamX = Configuracion::COLUMNAS_MAPA_NIVEL3 * ts - viewW;
		int maxCamY = Configuracion::FILAS_MAPA_NIVEL3 * ts - viewH;
		if (maxCamX < 0) camX = maxCamX / 2;
		else {
			if (camX < 0) camX = 0;
			if (camX > maxCamX) camX = maxCamX;
		}
		if (maxCamY < 0) camY = maxCamY / 2;
		else {
			if (camY < 0) camY = 0;
			if (camY > maxCamY) camY = maxCamY;
		}

		actualizarProximidad();
		dibujarMapa(g, viewW, viewH);
		dibujarObjetos(g);
		dibujarNPCs(g);
		dibujarArbolPlata(g);
		dibujarEnemigos(g);
		dibujarCoustasConSilla(g);
		dibujarParticulas(g);
		dibujarOscuridadCoustas(g, viewW, viewH);
		dibujarHUD(g, viewW, viewH);
		if (dialogoActivo) dibujarCuadroDialogo(g, viewW, viewH);
	}

private:
	void dibujarMapa(Graphics^ g, int viewW, int viewH) {
		int ts = Configuracion::TILE_SIZE;
		int cIni = Math::Max(0, camX / ts);
		int cFin = Math::Min(Configuracion::COLUMNAS_MAPA_NIVEL3 - 1, (camX + viewW) / ts + 1);
		int rIni = Math::Max(0, camY / ts);
		int rFin = Math::Min(Configuracion::FILAS_MAPA_NIVEL3 - 1, (camY + viewH) / ts + 1);

		SolidBrush^ bRoca = gcnew SolidBrush(Color::FromArgb(255, 58, 60, 68));
		SolidBrush^ bSenda = gcnew SolidBrush(Color::FromArgb(255, 52, 48, 56));
		SolidBrush^ bPasto = gcnew SolidBrush(Color::FromArgb(255, 38, 56, 48));
		SolidBrush^ bTinteNoct = gcnew SolidBrush(Color::FromArgb(88, 12, 18, 34));
		SolidBrush^ bLosa = gcnew SolidBrush(Color::FromArgb(255, 52, 62, 82));
		SolidBrush^ bLosaBorde = gcnew SolidBrush(Color::FromArgb(255, 76, 90, 118));
		SolidBrush^ bLosaJunta = gcnew SolidBrush(Color::FromArgb(255, 34, 42, 58));
		SolidBrush^ bPuente = gcnew SolidBrush(Color::FromArgb(255, 96, 68, 44));
		SolidBrush^ bPuenteRiel = gcnew SolidBrush(Color::FromArgb(255, 62, 42, 26));
		SolidBrush^ bPilar = gcnew SolidBrush(Color::FromArgb(255, 84, 96, 118));
		SolidBrush^ bPilarSombra = gcnew SolidBrush(Color::FromArgb(255, 42, 50, 66));
		int pulsoRuna = 150 + (int)(Math::Sin(ticksJuego * 0.14) * 70.0);
		SolidBrush^ bRunaPilar = gcnew SolidBrush(Color::FromArgb(pulsoRuna, 152, 224, 246));
		SolidBrush^ bPortonSello = gcnew SolidBrush(Color::FromArgb(230, 146, 96, 212));
		SolidBrush^ bCarreta = gcnew SolidBrush(Color::FromArgb(255, 68, 46, 28));
		SolidBrush^ bHierro = gcnew SolidBrush(Color::FromArgb(255, 112, 120, 134));

		Bitmap^ texMuro = bmpMuro;
		Bitmap^ texRoca = bmpRoca;
		Bitmap^ texAgua = bmpAgua;
		Bitmap^ texPuerta = bmpPuerta;
		Bitmap^ texCamino = bmpCamino;
		Bitmap^ texPasto = bmpPasto;
		Bitmap^ texMadera = bmpMadera;

		for (int r = rIni; r <= rFin; r++) {
			for (int c = cIni; c <= cFin; c++) {
				char ch = mapa[r][c];
				int x = c * ts - camX;
				int y = r * ts - camY;
				Rectangle dst(x, y, ts, ts);
				if (ch == '#') {
					if (texMuro != nullptr) g->DrawImage(texMuro, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					else g->FillRectangle(bRoca, dst);
				}
				else if (ch == 'O') {
					if (texPasto != nullptr) {
						g->DrawImage(texPasto, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
						g->FillRectangle(bTinteNoct, dst);
					}
					if (texRoca != nullptr) g->DrawImage(texRoca, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					else g->FillRectangle(bRoca, dst);
				}
				else if (ch == 'C') {
					if (texMadera != nullptr) g->DrawImage(texMadera, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					else g->FillRectangle(bCarreta, dst);
					g->FillRectangle(bCarreta, x + 2, y + 4, 28, 4);
					g->FillRectangle(bCarreta, x + 2, y + 24, 28, 4);
					g->FillRectangle(bHierro, x + 4, y + 8, 4, 16);
					g->FillRectangle(bHierro, x + 24, y + 8, 4, 16);
					if (r == 17 && c == 11) {
						g->FillRectangle(bHierro, x + 10, y + 10, 12, 12);
						g->FillRectangle(bCarreta, x + 13, y + 13, 6, 6);
					}
				}
				else if (ch == '~') {
					if (texAgua != nullptr) g->DrawImage(texAgua, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
				}
				else if (ch == '=') {
					if (texMadera != nullptr) g->DrawImage(texMadera, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					else g->FillRectangle(bPuente, dst);
					g->FillRectangle(bPuenteRiel, x, y, ts, 3);
					g->FillRectangle(bPuenteRiel, x, y + ts - 3, ts, 3);
					g->FillRectangle(bPuenteRiel, x + 15, y + 3, 2, ts - 6);
				}
				else if (ch == '_') {
					g->FillRectangle(bLosaJunta, dst);
					g->FillRectangle(bLosa, x + 1, y + 1, ts - 2, ts - 2);
					g->FillRectangle(bLosaBorde, x + 2, y + 2, ts - 6, 2);
					g->FillRectangle(bLosaBorde, x + 2, y + 2, 2, ts - 6);
				}
				else if (ch == 'X') {
					if (texPuerta != nullptr) g->DrawImage(texPuerta, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					g->FillRectangle(bPilarSombra, x + 4, y + 4, 24, 24);
					g->FillRectangle(bPortonSello, x + 8, y + 6, 16, 4);
					g->FillRectangle(bPortonSello, x + 14, y + 10, 4, 12);
					g->FillRectangle(bPortonSello, x + 8, y + 22, 16, 4);
				}
				else if (ch == 'D') {
					if (texPuerta != nullptr) g->DrawImage(texPuerta, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
				}
				else if (ch == 'T') {
					g->FillRectangle(bLosa, dst);
				}
				else if (ch == 'P') {
					g->FillRectangle(bLosa, dst);
					g->FillRectangle(bPilarSombra, x + 6, y + 2, 20, 28);
					g->FillRectangle(bPilar, x + 8, y + 4, 16, 24);
					g->FillRectangle(bLosaBorde, x + 6, y + 2, 20, 4);
					g->FillRectangle(bLosaBorde, x + 6, y + 26, 20, 4);
					g->FillRectangle(bRunaPilar, x + 13, y + 13, 6, 6);
				}
				else if (ch == ';') {
					if (texCamino != nullptr) g->DrawImage(texCamino, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
					else g->FillRectangle(bSenda, dst);
				}
				else {
					if (texPasto != nullptr) {
						g->DrawImage(texPasto, dst, 0, 0, 32, 32, GraphicsUnit::Pixel);
						g->FillRectangle(bTinteNoct, dst);
					}
					else g->FillRectangle(bPasto, dst);
				}
			}
		}

		delete bRoca;
		delete bSenda;
		delete bPasto;
		delete bTinteNoct;
		delete bLosa;
		delete bLosaBorde;
		delete bLosaJunta;
		delete bPuente;
		delete bPuenteRiel;
		delete bPilar;
		delete bPilarSombra;
		delete bRunaPilar;
		delete bPortonSello;
		delete bCarreta;
		delete bHierro;
	}

	void dibujarArbolPlata(Graphics^ g) {
		int ax = 89 * 32 - camX;
		int ay = 19 * 32 - camY;
		if (ax < -260 || ax > 1220 || ay < -260 || ay > 860) return;

		int pulso = 22 + (int)(Math::Sin(ticksJuego * 0.1) * 14.0);
		if (resistenciaActiva) pulso += 36;

		SolidBrush^ aura = gcnew SolidBrush(Color::FromArgb(pulso, 176, 222, 255));
		SolidBrush^ raiz = gcnew SolidBrush(Color::FromArgb(255, 118, 128, 148));
		SolidBrush^ troncoSombra = gcnew SolidBrush(Color::FromArgb(255, 134, 146, 168));
		SolidBrush^ troncoLuz = gcnew SolidBrush(Color::FromArgb(255, 198, 210, 228));
		SolidBrush^ copaSombra = gcnew SolidBrush(Color::FromArgb(255, 142, 174, 208));
		SolidBrush^ copaMedia = gcnew SolidBrush(Color::FromArgb(255, 194, 218, 240));
		SolidBrush^ copaBrillo = gcnew SolidBrush(Color::FromArgb(255, 232, 244, 255));

		g->FillRectangle(aura, ax - 54, ay - 96, 172, 196);
		g->FillRectangle(raiz, ax - 16, ay + 84, 96, 12);
		g->FillRectangle(raiz, ax - 24, ay + 90, 24, 8);
		g->FillRectangle(raiz, ax + 64, ay + 90, 24, 8);

		g->FillRectangle(troncoSombra, ax + 10, ay - 22, 44, 112);
		g->FillRectangle(troncoLuz, ax + 16, ay - 18, 28, 104);
		g->FillRectangle(troncoSombra, ax + 24, ay + 24, 8, 28);

		g->FillRectangle(copaSombra, ax - 42, ay - 88, 148, 72);
		g->FillRectangle(copaSombra, ax - 54, ay - 68, 172, 44);
		g->FillRectangle(copaMedia, ax - 32, ay - 98, 128, 66);
		g->FillRectangle(copaMedia, ax - 44, ay - 78, 152, 42);
		g->FillRectangle(copaBrillo, ax - 18, ay - 108, 100, 40);
		g->FillRectangle(copaBrillo, ax - 8, ay - 118, 80, 20);

		Bitmap^ bArbol = bmpArbolPlata;
		if (bArbol != nullptr) {
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->DrawImage(bArbol, Rectangle(ax + 16, ay - 76, 32, 32), 0, 0, 32, 32, GraphicsUnit::Pixel);
		}

		delete aura;
		delete raiz;
		delete troncoSombra;
		delete troncoLuz;
		delete copaSombra;
		delete copaMedia;
		delete copaBrillo;
	}

	void dibujarObjetos(Graphics^ g) {
		for (size_t i = 0; i < objetos.size(); i++) {
			objetos[i]->dibujar(g, nullptr, camX, camY, 1, ticksJuego);
		}
	}

	void dibujarNPCs(Graphics^ g) {
		if (dagda != nullptr) dagda->dibujarConGrid(g, bmpDagda, camX, camY, 1, ticksJuego, 4, 4, false);
		if (ininia != nullptr) ininia->dibujarConGrid(g, bmpIninia, camX, camY, 1, ticksJuego, 4, 4, true);
	}

	void dibujarEnemigos(Graphics^ g) {
		for (size_t i = 0; i < enemigos.size(); i++) {
			enemigos[i]->dibujarEnemigo(g, bmpEnemigo, camX, camY, 1);
		}
		for (size_t i = 0; i < proyectilesEnemigos.size(); i++) {
			proyectilesEnemigos[i].dibujar(g, bmpDisparo, camX, camY, 1, 24);
		}
	}

	void dibujarCoustasConSilla(Graphics^ g) {
		if (protagonista == nullptr) return;
		if (invulnerableTicks > 0 && invulnerableTicks % 3 == 0) return;

		int px = protagonista->getX() - camX;
		int py = protagonista->getY() - camY;
		int dir = protagonista->getDireccion();

		if (correrActivo && protagonista->getEnMovimiento()) {
			DibujarPolvoCorrerComun(g, protagonista->getX(), protagonista->getY(), protagonista->getAncho(), protagonista->getAlto(), dir, ticksJuego, camX, camY, 1);
		}

		if (ticksImpulsoVuelo > 0) {
			SolidBrush^ estela = gcnew SolidBrush(Color::FromArgb(145, 134, 224, 212));
			for (int k = 1; k <= 3; k++) {
				int ex = px - impulsoDx * k * 10;
				int ey = py - impulsoDy * k * 8;
				g->FillRectangle(estela, ex + 4, ey + 6, 16, 12);
			}
			delete estela;
		}

		protagonista->dibujar(g, bmpCoustas, camX, camY, 1, ticksJuego);

		Bitmap^ bSilla = bmpSillaCoustas;
		if (bSilla != nullptr && bSilla->Width >= 128 && bSilla->Height >= 128) {
			int fila = (dir >= 0 && dir < 4) ? dir : 0;
			int col = (protagonista->getEnMovimiento() || ticksImpulsoVuelo > 0) ? ((ticksJuego / 4) % 4) : 0;
			int cw = bSilla->Width / 4;
			int ch = bSilla->Height / 4;
			g->InterpolationMode = InterpolationMode::NearestNeighbor;
			g->PixelOffsetMode = PixelOffsetMode::Half;
			g->DrawImage(bSilla, Rectangle(px - 20, py - 32, 64, 64), Rectangle(col * cw, fila * ch, cw, ch), GraphicsUnit::Pixel);
		}
	}

	void dibujarOscuridadCoustas(Graphics^ g, int viewW, int viewH) {
		if (protagonista == nullptr) return;
		int cx = protagonista->getX() + protagonista->getAncho() / 2 - camX;
		int cy = protagonista->getY() + protagonista->getAlto() / 2 - camY;
		int r = 148;

		GraphicsPath^ pathOscuro = gcnew GraphicsPath(FillMode::Alternate);
		pathOscuro->AddRectangle(Rectangle(0, 0, viewW, viewH));
		pathOscuro->AddEllipse(cx - r, cy - r, r * 2, r * 2);
		SolidBrush^ bOscuro = gcnew SolidBrush(Color::FromArgb(242, 4, 6, 12));
		g->FillPath(bOscuro, pathOscuro);
		delete bOscuro;
		delete pathOscuro;

		GraphicsPath^ pathAura = gcnew GraphicsPath();
		pathAura->AddEllipse(cx - r, cy - r, r * 2, r * 2);
		PathGradientBrush^ pgb = gcnew PathGradientBrush(pathAura);
		pgb->CenterPoint = PointF((float)cx, (float)cy);
		pgb->CenterColor = Color::FromArgb(0, 4, 6, 12);
		pgb->SurroundColors = gcnew array<Color>{ Color::FromArgb(242, 4, 6, 12) };
		pgb->FocusScales = PointF(0.36f, 0.36f);
		g->FillPath(pgb, pathAura);
		delete pgb;
		delete pathAura;
	}

	void dibujarParticulas(Graphics^ g) {
		SolidBrush^ luz = gcnew SolidBrush(Color::FromArgb(120, 220, 238, 255));
		for (size_t i = 0; i < particulas.size(); i++) {
			g->FillEllipse(luz, particulas[i].x, particulas[i].y, (float)particulas[i].tam, (float)particulas[i].tam);
		}
		delete luz;
	}

	void dibujarIntro(Graphics^ g, int viewW, int viewH) {
		String^ cuerpo =
			"Tras la emboscada a la caravana, Coustas avanza junto a Dagda\n"
			"hacia el Santuario del Gran Árbol de Plata con la Capa de Vuelo.\n\n"
			"¡Cuidado! La bruma sombría acorta tu visión e invierte los controles\n"
			"[W/A/S/D]. Reúne los 2 Pergaminos del Sello y usa [F] para impulsarte.";
		DibujarIntroActoComun(g, viewW, viewH, "ACTO III", "El Árbol de Plata", cuerpo, "Presiona Espacio para continuar");
	}

	void dibujarHUD(Graphics^ g, int viewW, int viewH) {
		std::string guiaTit;
		std::string guiaDet;
		int pasoNum = 1;
		int totalPasos = 4;
		int targetX = 0;
		int targetY = 0;
		obtenerGuiaPasoActual(guiaTit, guiaDet, pasoNum, totalPasos, targetX, targetY);

		if (guiaActiva && !dialogoActivo && !decisionTomada) {
			int jSX = protagonista->getX() + protagonista->getAncho() / 2 - camX;
			int jSY = protagonista->getY() + protagonista->getAlto() / 2 - camY;
			DibujarFlechaGuiaComun(g, jSX, jSY, targetX - camX, targetY - camY, viewW, viewH, ticksJuego);
		}

		SolidBrush^ panel = gcnew SolidBrush(Color::FromArgb(228, 10, 16, 24));
		g->FillRectangle(panel, 16, 16, 392, 42);

		for (int i = 0; i < 3; i++) {
			SolidBrush^ cor = gcnew SolidBrush((i < getVidas()) ? Color::FromArgb(220, 210, 58, 76) : Color::FromArgb(120, 70, 58, 62));
			int cx = 28 + i * 20;
			int cy = 29;
			g->FillEllipse(cor, cx, cy, 8, 8);
			g->FillEllipse(cor, cx + 6, cy, 8, 8);
			g->FillPolygon(cor, gcnew array<Point>{ Point(cx, cy + 6), Point(cx + 14, cy + 6), Point(cx + 7, cy + 15) });
			delete cor;
		}

		System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Regular);
		System::Drawing::Font^ fObj = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
		System::Drawing::Font^ fSub = gcnew System::Drawing::Font("Segoe UI", 8.5f, FontStyle::Regular);
		System::Drawing::Font^ fPrompt = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);
		String^ estadoVuelo = (cooldownVuelo <= 0) ? "CAPA [F]" : "RECARGA...";
		g->DrawString("SELLOS " + runasReunidas.ToString() + "/2 | INVERTIDO | " + estadoVuelo, fTexto, Brushes::Gainsboro, PointF(100.0f, 29.0f));

		if (guiaActiva) {
			g->FillRectangle(panel, 16, 66, 292, 82);
			SolidBrush^ barra = gcnew SolidBrush(Color::FromArgb(220, 214, 198, 166));
			g->FillRectangle(barra, 16, 66, 3, 82);
			delete barra;
			int distM = (int)(Math::Sqrt((double)((targetX - protagonista->getX()) * (targetX - protagonista->getX()) + (targetY - protagonista->getY()) * (targetY - protagonista->getY()))) / 32.0);
			g->DrawString("MISIÓN ACTUAL (" + pasoNum.ToString() + "/" + totalPasos.ToString() + ")", fSub, Brushes::Silver, PointF(28.0f, 72.0f));
			g->DrawString(ToManaged(guiaTit), fObj, Brushes::Khaki, PointF(28.0f, 89.0f));
			g->DrawString(ToManaged(guiaDet) + " (" + distM.ToString() + " m)", fSub, Brushes::LightGray, RectangleF(28.0f, 110.0f, 270.0f, 34.0f));
		}
		delete panel;
		delete fSub;

		std::string txt = "";
		if (ticksMensaje > 0 && !mensajeTemporal.empty()) txt = mensajeTemporal;
		else if (!promptInteraccion.empty()) txt = promptInteraccion;
		if (!txt.empty() && !dialogoActivo) {
			String^ tp = ToManaged(txt);
			SizeF tam = g->MeasureString(tp, fPrompt);
			int ancho = (int)tam.Width + 36;
			int x = (viewW - ancho) / 2;
			int y = viewH - 54;
			SolidBrush^ bg = gcnew SolidBrush(Color::FromArgb(190, 8, 12, 18));
			g->FillRectangle(bg, x, y, ancho, 32);
			delete bg;
			g->DrawString(tp, fPrompt, Brushes::WhiteSmoke, PointF((float)(x + 18), (float)(y + 6)));
		}

		if (ticksTituloNivel > 0) {
			System::Drawing::Font^ fTit = gcnew System::Drawing::Font("Segoe UI", 16.0f, FontStyle::Bold);
			System::Drawing::Font^ fSubT = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Italic);
			int alpha = Math::Min(230, ticksTituloNivel * 4);
			SolidBrush^ titCol = gcnew SolidBrush(Color::FromArgb(alpha, 210, 226, 248));
			String^ t1 = "Nivel 3";
			String^ t2 = "La Emboscada y el Gran Árbol de Plata";
			SizeF s1 = g->MeasureString(t1, fTit);
			SizeF s2 = g->MeasureString(t2, fSubT);
			g->DrawString(t1, fTit, titCol, PointF(viewW / 2.0f - s1.Width / 2.0f, 86.0f));
			g->DrawString(t2, fSubT, Brushes::Gainsboro, PointF(viewW / 2.0f - s2.Width / 2.0f, 122.0f));
			delete titCol;
			delete fTit;
			delete fSubT;
		}

		delete fTexto;
		delete fObj;
		delete fPrompt;
	}

	void dibujarCuadroDialogo(Graphics^ g, int viewW, int viewH) {
		if (!dialogoActivo) return;
		String^ completo = dialogoManaged;
		if (completo == nullptr) completo = "";
		bool conOpciones = (charsVisibles >= completo->Length) && !dialogoOpciones.empty();
		int x = 44;
		int ancho = viewW - 88;
		int anchoTexto = ancho - 44;

		System::Drawing::Font^ fNom = gcnew System::Drawing::Font("Segoe UI", 12.0f, FontStyle::Bold);
		System::Drawing::Font^ fTxt = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Regular);
		System::Drawing::Font^ fOpc = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
		System::Drawing::Font^ fAyu = gcnew System::Drawing::Font("Segoe UI", 8.5f, FontStyle::Regular);

		int vis = Math::Min(charsVisibles, completo->Length);
		String^ sub = completo->Substring(0, vis);
		array<Char>^ sep = gcnew array<Char>{ '\n' };
		array<String^>^ lineasAll = completo->Split(sep);
		array<String^>^ lineasVis = sub->Split(sep);

		int altoTexto = 0;
		for each (String^ ln in lineasAll) {
			int h = (int)Math::Ceiling(g->MeasureString(ln, fTxt, anchoTexto).Height);
			altoTexto += Math::Max(h, (int)Math::Ceiling(fTxt->GetHeight(g))) + 5;
		}
		int alto = Math::Min(viewH - 28, Math::Max(170, 30 + altoTexto + 10 + (int)dialogoOpciones.size() * 30 + 34));
		int y = viewH - alto - 14;

		SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(242, 8, 12, 18));
		g->FillRectangle(fondo, x, y, ancho, alto);
		delete fondo;
		SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(140, 174, 200, 232));
		g->FillRectangle(linea, x, y, ancho, 1);
		g->FillRectangle(linea, x, y + 28 + altoTexto + 4, ancho, 1);
		delete linea;

		g->DrawString(ToManaged(dialogoHablante), fNom, Brushes::LightSkyBlue, PointF((float)(x + 22), (float)(y + 10)));
		float yCur = (float)(y + 34);
		for each (String^ ln in lineasVis) {
			int h = Math::Max((int)Math::Ceiling(g->MeasureString(ln, fTxt, anchoTexto).Height), (int)Math::Ceiling(fTxt->GetHeight(g)));
			g->DrawString(ln, fTxt, Brushes::WhiteSmoke, RectangleF((float)(x + 22), yCur, (float)anchoTexto, (float)h + 2.0f));
			yCur += h + 5;
		}

		if (conOpciones) {
			int yOpc = y + 38 + altoTexto;
			for (size_t i = 0; i < dialogoOpciones.size() && i < 4; i++) {
				bool sel = (i == (size_t)dialogoSeleccion);
				if (sel) g->DrawString("›", fOpc, Brushes::Gold, PointF((float)(x + 18), (float)yOpc));
				g->DrawString(ToManaged(dialogoOpciones[i]), fOpc, sel ? Brushes::Gold : Brushes::Gainsboro, PointF((float)(x + 34), (float)yOpc));
				yOpc += 30;
			}
			g->DrawString("[1-2] Elegir   ·   [Espacio] Confirmar", fAyu, Brushes::Gray, PointF((float)(x + 22), (float)(y + alto - 22)));
		}

		delete fNom;
		delete fTxt;
		delete fOpc;
		delete fAyu;
	}
};
