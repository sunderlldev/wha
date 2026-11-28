#pragma once

#include <gcroot.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>
#include <utility>
#include "Configuracion.h"
#include "Utilidades.h"
#include "Protagonista.h"
#include "NPC.h"
#include "Enemigo.h"
#include "BloqueMapa.h"
#include "Arbol.h"
#include "CajaMovible.h"
#include "MapaNivel1.h"
#include "Inventario.h"
#include "GestorAudio.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

enum TipoInteraccion {
	INTERACCION_NINGUNA = 0,
	INTERACCION_QIFREY = 1,
	INTERACCION_AGOTT = 2,
	INTERACCION_RICHEH = 3,
	INTERACCION_OBJETO = 4,
	INTERACCION_OBJETO_POZO = 5,
	INTERACCION_SALIDA_POZO = 6,
	INTERACCION_LETRERO_CURIOSIDADES = 7,
	INTERACCION_LETRERO_DIARIO = 8,
	INTERACCION_PARED_ROCA = 9
};

struct GotaLluvia {
	float x;
	float y;
	float velX;
	float velY;
	int largo;
};

class Nivel1 {
private:
	Protagonista* jugador;
	MapaNivel1* mapa;
	MapaNivel1* mapaPozo;
	MapaNivel1* mapaActual;
	std::vector<NPC*> listaNPCs;
	NPC* richehPozo;
	std::vector<Enemigo*> listaEnemigos;
	std::vector<ProyectilMago> proyectilesEnemigos;
	std::vector<BloqueMapa*> listaObjetos;
	std::vector<BloqueMapa*> objetosPozo;
	std::vector<Arbol*> listaArboles;
	std::vector<CajaMovible*> listaCajas;
	std::vector<GotaLluvia> gotasLluvia;
	BloqueMapa* myrphonObjeto;
	BloqueMapa* pozoObjeto;
	BloqueMapa* grimorioObjeto;
	int cajasMovidasContador;
	Inventario* inventario;
	GestorAudio audio;
	bool audioHabilitado;

	gcroot<Bitmap^> bmpCoco;
	gcroot<Bitmap^> bmpQifrey;
	gcroot<Bitmap^> bmpAgott;
	gcroot<Bitmap^> bmpRicheh;
	gcroot<Bitmap^> bmpEnemigo;
	gcroot<Bitmap^> bmpDisparo;
	gcroot<Bitmap^> bmpMyrphon;
	gcroot<String^> ultimaUbicacion;
	gcroot<String^> dialogoTextoManaged;

	int camX;
	int camY;
	int nivelActual;
	int ticksJuego;
	int ticksTituloUbicacion;
	int ticksTituloNivel;
	int invulnerableTicks;
	bool enIntro;
	bool enPozo;
	int posAtelierX;
	int posAtelierY;

	bool dialogoActivo;
	std::string dialogoNpc;
	int estadoDialogo;
	std::string dialogoHablante;
	std::string dialogoRol;
	int dialogoConfianza;
	std::vector<std::string> dialogoLineas;
	std::vector<std::string> dialogoOpciones;
	int dialogoOpcionSeleccionada;
	int caracteresVisibles;
	int ticksTypewriter;
	std::string dialogoTextoPlano;

	bool habloQifrey;
	bool habloAgott;
	bool habloRicheh;
	bool pidioMantoQifrey;
	int confianzaQifrey;
	int confianzaAgott;
	int confianzaRicheh;
	int respuestaInicialAgott;
	bool richehEnojada;
	bool misionMyrphonActiva;
	bool myrphonRescatado;
	bool dioVaraRicheh;
	bool agottSabeMyrphon;
	bool capaCrafteada;
	bool tintaMagicaEntregada;
	bool tintaSecundariaEntregada;
	bool grimorioDescubierto;
	bool libroEncontrado;
	bool pozoDescubierto;
	bool paredRocaRota;
	bool entroTorre;
	bool monologoCruceActivado;
	bool gatoAcariciado;
	bool trollDescubierto;
	bool misionCompletada;
	bool finMostrado;

	bool solicitaMinijuego;
	bool solicitaTrollface;
	bool solicitaFinNivel;
	bool solicitaDerrota;

	bool transicionPozoActiva;
	bool transicionBajandoPozo;
	int ticksTransicionPozo;

	bool myrphonHuyendo;
	bool myrphonEscapado;
	int ticksMyrphonHuyendo;

	bool myrphonCorriendo;
	int ticksMyrphonCorriendo;
	bool myrphonConRicheh;
	bool vioMyrphon;
	bool vioEnemigos;

	int ticksMensajeTemporal;
	std::string mensajeTemporal;

	int tipoObjetivo;
	int indiceObjetivo;
	std::string promptInteraccion;
	std::string objetivoActual;
	bool guiaActiva;

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		return CargarBitmapSeguroComun(nombreArchivo, true);
	}

	static Bitmap^ CrearVariantePersonaje(Bitmap^ base, Color ropaOscura, Color ropaClara, Color peloOscuro, Color peloClaro) {
		return CrearVarianteSpriteComun(base, ropaOscura, ropaClara, peloOscuro, peloClaro);
	}

	void inicializarLluvia() {
		gotasLluvia.clear();
		unsigned int sem = 13579u;
		for (int i = 0; i < 110; i++) {
			sem = sem * 1664525u + 1013904223u;
			float gx = (float)(sem % Configuracion::ANCHO_JUEGO);
			sem = sem * 1664525u + 1013904223u;
			float gy = (float)(sem % Configuracion::ALTO_JUEGO);
			sem = sem * 1664525u + 1013904223u;
			GotaLluvia g;
			g.x = gx;
			g.y = gy;
			g.velX = -2.4f - (float)(sem % 3) * 0.4f;
			g.velY = 7.5f + (float)(sem % 4) * 1.1f;
			g.largo = 7 + (int)(sem % 5);
			gotasLluvia.push_back(g);
		}
	}

	void actualizarLluvia() {
		for (size_t i = 0; i < gotasLluvia.size(); i++) {
			gotasLluvia[i].x += gotasLluvia[i].velX;
			gotasLluvia[i].y += gotasLluvia[i].velY;
			if (gotasLluvia[i].y > Configuracion::ALTO_JUEGO) {
				gotasLluvia[i].y -= Configuracion::ALTO_JUEGO;
			}
			if (gotasLluvia[i].x < 0) {
				gotasLluvia[i].x += Configuracion::ANCHO_JUEGO;
			}
		}
	}

	std::string textoConfianza(int valor) {
		if (valor <= 0) return "Sin confianza";
		if (valor == 1) return "Neutral";
		return "Amigos";
	}

	void limpiarEntidades() {
		for (size_t i = 0; i < listaNPCs.size(); i++) delete listaNPCs[i];
		listaNPCs.clear();
		for (size_t i = 0; i < listaEnemigos.size(); i++) delete listaEnemigos[i];
		listaEnemigos.clear();
		proyectilesEnemigos.clear();
		for (size_t i = 0; i < listaObjetos.size(); i++) delete listaObjetos[i];
		listaObjetos.clear();
		for (size_t i = 0; i < objetosPozo.size(); i++) delete objetosPozo[i];
		objetosPozo.clear();
		for (size_t i = 0; i < listaArboles.size(); i++) delete listaArboles[i];
		listaArboles.clear();
		for (size_t i = 0; i < listaCajas.size(); i++) delete listaCajas[i];
		listaCajas.clear();
		if (richehPozo != nullptr) {
			delete richehPozo;
			richehPozo = nullptr;
		}
		myrphonObjeto = nullptr;
		pozoObjeto = nullptr;
		grimorioObjeto = nullptr;
	}

	void agregarObjeto(BloqueMapa* objeto) {
		listaObjetos.push_back(objeto);
		if (objeto->getTipo() == ObjetoMyrphon) myrphonObjeto = objeto;
	}

	void agregarObjetoPozo(BloqueMapa* objeto) {
		objetosPozo.push_back(objeto);
	}

	bool tieneItem(const std::string& nombre) {
		return inventario != nullptr && inventario->tieneItem(nombre);
	}

	bool agregarItem(const std::string& nombre, const std::string& descripcion, const std::string& tipo) {
		return inventario != nullptr && inventario->agregarItem(nombre, descripcion, tipo);
	}

	void quitarItem(const std::string& nombre) {
		if (inventario != nullptr) inventario->removerItem(nombre);
	}

	void actualizarObjetivo() {
		if (misionCompletada) {
			objetivoActual = "¡Capa Mágica crafteada! ¡Nivel 1 completado!";
		}
		else if (dioVaraRicheh && !paredRocaRota) {
			objetivoActual = "Usa la Pluma Térmica en la Cueva Misteriosa (Norte)";
		}
		else if (myrphonRescatado && !dioVaraRicheh) {
			objetivoActual = "Lleva a Myrphon de regreso con Richeh al Sótano";
		}
		else if (misionMyrphonActiva) {
			objetivoActual = "Busca a Myrphon en el Laberinto Serpentback (Sur)";
		}
		else if (tieneItem("Fibra de Arbol") && tieneItem("Tinta magica") && tieneItem("Grimorio de Trazos")) {
			objetivoActual = "Regresa con Qifrey para confeccionar tu Capa de Aprendiz";
		}
		else if (pidioMantoQifrey) {
			objetivoActual = "Reúne Fibra de Árbol, Tinta mágica y Grimorio de Trazos";
		}
		else {
			objetivoActual = "Sal de la Choza y habla con el Maestro Qifrey al sur";
		}
	}

	void crearArbol(TipoArbol tipo, int columna, int fila) {
		if (mapa->getCaracter(columna, fila) != ',') return;
		Arbol* arbol = new Arbol(0, 0, tipo);
		int px = columna * Configuracion::TILE_SIZE - arbol->getAncho() / 2;
		int py = (fila + 1) * Configuracion::TILE_SIZE - arbol->getAlto();
		arbol->setX(px);
		arbol->setY(py);
		listaArboles.push_back(arbol);
	}

	bool zonaDespejadaDeArboles(int col, int fila) {
		if (col <= 4 || col >= Configuracion::COLUMNAS_MAPA_NIVEL1 - 5) return false;
		if (fila <= 4 || fila >= Configuracion::FILAS_MAPA_NIVEL1 - 5) return false;
		if (col >= 7 && col <= 32 && fila >= 4 && fila <= 69) return false;
		if (col <= 16 && fila >= 33 && fila <= 39) return false;
		if (col >= 43 && col <= 61 && fila >= 4 && fila <= 75) return false;
		if (col >= 84 && col <= 137 && fila >= 8 && fila <= 72) return false;
		for (int dr = -2; dr <= 2; dr++) {
			for (int dc = -2; dc <= 2; dc++) {
				if (mapa->getCaracter(col + dc, fila + dr) != ',') return false;
			}
		}
		int cx = col * Configuracion::TILE_SIZE;
		int cy = fila * Configuracion::TILE_SIZE;
		for (size_t i = 0; i < listaArboles.size(); i++) {
			int ax = listaArboles[i]->getX() + listaArboles[i]->getAncho() / 2;
			int ay = listaArboles[i]->getBaseY();
			if (std::abs(ax - cx) < 120 && std::abs(ay - cy) < 104) return false;
		}
		return true;
	}

	void crearArboles() {
		int fijos[][2] = {
			{ 36, 15 }, { 38, 52 }, { 68, 16 }, { 71, 52 },
			{ 75, 26 }, { 77, 62 }, { 140, 18 }, { 141, 52 },
			{ 139, 69 }, { 66, 68 }, { 36, 66 }, { 10, 72 }
		};
		for (int i = 0; i < 12; i++) {
			if (zonaDespejadaDeArboles(fijos[i][0], fijos[i][1])) {
				crearArbol(ArbolGigante, fijos[i][0], fijos[i][1]);
			}
		}

		unsigned int semilla = 20261118u;
		int colocados = 0;
		int intentos = 0;
		while (colocados < 110 && intentos < 12000) {
			intentos++;
			semilla = semilla * 1664525u + 1013904223u;
			int fila = 5 + (int)(semilla % (Configuracion::FILAS_MAPA_NIVEL1 - 10));
			semilla = semilla * 1664525u + 1013904223u;
			int columna = 5 + (int)(semilla % (Configuracion::COLUMNAS_MAPA_NIVEL1 - 10));
			if (!zonaDespejadaDeArboles(columna, fila)) continue;
			crearArbol(ArbolPino, columna, fila);
			colocados++;
		}

		colocados = 0;
		intentos = 0;
		while (colocados < 65 && intentos < 9000) {
			intentos++;
			semilla = semilla * 1664525u + 1013904223u;
			int fila = 5 + (int)(semilla % (Configuracion::FILAS_MAPA_NIVEL1 - 10));
			semilla = semilla * 1664525u + 1013904223u;
			int columna = 5 + (int)(semilla % (Configuracion::COLUMNAS_MAPA_NIVEL1 - 10));
			if (!zonaDespejadaDeArboles(columna, fila)) continue;
			crearArbol(ArbolFrondoso, columna, fila);
			colocados++;
		}
	}

	std::string unirLineas(const std::vector<std::string>& lineas) {
		std::string texto = "";
		for (size_t i = 0; i < lineas.size(); i++) {
			if (i > 0) texto += "\n";
			texto += lineas[i];
		}
		return texto;
	}

	void prepararTextoDialogo() {
		dialogoTextoPlano = unirLineas(dialogoLineas);
		dialogoTextoManaged = ToManaged(dialogoTextoPlano);
		caracteresVisibles = 0;
		ticksTypewriter = 0;
		dialogoOpcionSeleccionada = 0;
	}

	void cerrarDialogo() {
		dialogoActivo = false;
		dialogoNpc = "";
		estadoDialogo = 0;
		dialogoLineas.clear();
		dialogoOpciones.clear();
		dialogoTextoPlano = "";
		dialogoTextoManaged = "";
		caracteresVisibles = 0;
		ticksTypewriter = 0;
	}

	void iniciarDialogo(const std::string& npc, int estadoInicial) {
		dialogoActivo = true;
		dialogoNpc = npc;
		estadoDialogo = estadoInicial;
		if (npc == "Qifrey") {
			habloQifrey = true;
			dialogoHablante = "Qifrey";
			dialogoRol = "Maestro Hechicero";
			dialogoConfianza = confianzaQifrey;
		}
		else if (npc == "Agott") {
			habloAgott = true;
			dialogoHablante = "Agott";
			dialogoRol = "Aprendiz de Maga";
			dialogoConfianza = confianzaAgott;
		}
		else if (npc == "Richeh") {
			habloRicheh = true;
			dialogoHablante = "Richeh";
			dialogoRol = "Aprendiz de Maga";
			dialogoConfianza = confianzaRicheh;
		}
		else {
			dialogoHablante = "Letrero";
			dialogoRol = "Información";
			dialogoConfianza = 0;
		}
		actualizarDialogoV1();
	}

	void iniciarDialogoSimple(const std::string& hablante, const std::string& rol, const std::string& texto) {
		dialogoActivo = true;
		dialogoNpc = "Simple";
		estadoDialogo = -1;
		dialogoHablante = hablante;
		dialogoRol = rol;
		dialogoConfianza = 0;
		dialogoLineas.clear();
		dialogoLineas.push_back(texto);
		dialogoOpciones.clear();
		dialogoOpciones.push_back("[1] Cerrar");
		prepararTextoDialogo();
	}

	void actualizarDialogoV1() {
		dialogoLineas.clear();
		dialogoOpciones.clear();

		if (dialogoNpc == "Qifrey") {
			switch (estadoDialogo) {
			case 1:
				dialogoLineas.push_back("Hola, Coco. ¿Qué necesitas en el taller hoy?");
				dialogoOpciones.push_back("[1] Maestro Qifrey, ¿podrías ayudarme a confeccionar mi Manto de Aprendiz?");
				dialogoOpciones.push_back("[2] Solo venía a explorar el taller y ver tus libros.");
				dialogoOpciones.push_back("[3] Maestro, necesito materiales especiales para mis prácticas.");
				if (!tintaSecundariaEntregada && (tieneItem("Frasco de Tinta") || tieneItem("Tinta de Viento"))) {
					dialogoOpciones.push_back("[4] ¡Maestro, encontré una tinta especial explorando el taller!");
				}
				break;
			case 10:
				dialogoLineas.push_back("Para confeccionar tu Manto de Aprendiz necesito 3 elementos del atelier:");
				dialogoLineas.push_back("Fibra del Árbol de Plata, Tinta mágica de conjuración y el Grimorio de Trazos.");
				dialogoLineas.push_back("¡Cruza el puente al este, explora la Torre de Agott y la Cueva Misteriosa!");
				dialogoOpciones.push_back("[1] Está bien, iré a buscarlos por el atelier.");
				break;
			case 11:
				dialogoLineas.push_back("¡Excelente trabajo! Has reunido los componentes indispensables.");
				dialogoLineas.push_back("Tratando fibras del Árbol de Plata y sellos protectores...");
				dialogoLineas.push_back("¡Aquí está tu Capa de Aprendiz!");
				dialogoLineas.push_back("Recuerda que su forro repele la hechicería exterior.");
				dialogoOpciones.push_back("[1] ¡Entendido!");
				break;
			case 12:
				dialogoLineas.push_back("[CONFECCIÓN EXITOSA: Capa Mágica obtenida]");
				dialogoLineas.push_back("Se consumieron: Fibra de Árbol, Tinta mágica y Grimorio de Trazos.");
				dialogoLineas.push_back("La Capa Mágica ha sido equipada y agregada a tu inventario.");
				dialogoOpciones.push_back("[1] ¡Muchas gracias, Maestro Qifrey!");
				break;
			case 15:
				dialogoLineas.push_back("Te queda excelente la Capa Mágica, Coco.");
				dialogoLineas.push_back("Recuerda usar tus alas de aprendiz con sabiduría y responsabilidad.");
				dialogoOpciones.push_back("[1] ¡Gracias, Maestro Qifrey!");
				break;
			case 20:
				dialogoLineas.push_back("Eres bienvenida en el taller siempre, Coco.");
				dialogoLineas.push_back("Cuídate de las corrientes del gran río. Cruza siempre por el puente.");
				dialogoOpciones.push_back("[1] Gracias por el consejo, Maestro.");
				break;
			case 30:
				dialogoLineas.push_back("Las prácticas de hechicería requieren precisión y paciencia.");
				dialogoLineas.push_back("¿Qué tipo de material mágico estás buscando exactamente?");
				dialogoOpciones.push_back("[1] Busco una tinta que reaccione al flujo mágico del pergamino.");
				dialogoOpciones.push_back("[2] Cualquier material básico me servirá para practicar.");
				break;
			case 31:
				dialogoLineas.push_back("La tinta mágica de plata es muy delicada y poderosa.");
				dialogoLineas.push_back("¿Sabrás usarla con cuidado y verdadero respeto al atelier?");
				dialogoOpciones.push_back("[1] Prometo seguir las reglas del atelier y ser responsable.");
				dialogoOpciones.push_back("[2] Intentaré tener cuidado, aunque a veces me cuesta.");
				break;
			case 32:
				dialogoLineas.push_back("Bien dicho, Coco. Veo determinación y honestidad en tus ojos.");
				dialogoLineas.push_back("Te doy este objeto: Tinta mágica.");
				dialogoOpciones.push_back("[1] ¡Muchas gracias Qifrey, me servirá de mucho!");
				break;
			case 33:
				dialogoLineas.push_back("[HAS OBTENIDO: Tinta mágica]");
				dialogoLineas.push_back("Se ha agregado a tu inventario.");
				dialogoLineas.push_back("¡Tu vínculo y confianza con Maestro Qifrey han aumentado!");
				dialogoOpciones.push_back("[1] Continuar explorando");
				break;
			case 34:
				dialogoLineas.push_back("Ya te he entregado la Tinta mágica, Coco.");
				dialogoLineas.push_back("Revisa tu mochila y dale buen uso en tus pergaminos.");
				dialogoOpciones.push_back("[1] Entendido, Maestro.");
				break;
			case 35:
				dialogoLineas.push_back("En la hechicería no existe tal cosa como un material 'básico'.");
				dialogoLineas.push_back("Solo la tinta de conjuración, nacida de la savia del Árbol de Plata,");
				dialogoLineas.push_back("canaliza el flujo mágico al pergamino. ¿Aún deseas aceptarla, Coco?");
				dialogoOpciones.push_back("[1] ¡Sí, por favor! Prometo cuidarla y esforzarme al máximo.");
				break;
			case 40:
				dialogoLineas.push_back("¡Increíble hallazgo, Coco! Esta tinta arcaica es justo lo");
				dialogoLineas.push_back("que necesitábamos en el taller para restaurar pergaminos.");
				dialogoLineas.push_back("Demuestras una gran curiosidad y respeto por este atelier.");
				dialogoOpciones.push_back("[1] ¡Me alegra mucho ser de ayuda, Maestro!");
				break;
			case 41:
				dialogoLineas.push_back("[MISIÓN SECUNDARIA COMPLETADA]");
				dialogoLineas.push_back("Entregaste la tinta especial al Maestro Qifrey.");
				dialogoLineas.push_back("Tu nivel de confianza con Qifrey ahora es: Amigos (+50 pts)");
				dialogoOpciones.push_back("[1] Continuar explorando");
				break;
			case 99:
				dialogoLineas.push_back("[INVENTARIO LLENO: Capacidad máxima 8 objetos alcanzada]");
				dialogoLineas.push_back("No puedes recibir más objetos en este momento.");
				dialogoOpciones.push_back("[1] Volver");
				break;
			default:
				dialogoLineas.push_back("Continúa tu aprendizaje con dedicación, Coco.");
				dialogoOpciones.push_back("[1] Salir");
				break;
			}
		}
		else if (dialogoNpc == "Agott") {
			switch (estadoDialogo) {
			case 200:
				dialogoLineas.push_back("Vaya, vaya... Miren a quién tenemos aquí.");
				dialogoOpciones.push_back("[1] Siguiente");
				break;
			case 201:
				dialogoLineas.push_back("La joven y pequeña Coco... ¿por qué entraste a mi torre?");
				dialogoOpciones.push_back("[1] Necesito encontrar un Grimorio de Trazos para mi práctica.");
				dialogoOpciones.push_back("[2] ¿A ti qué te importa, Agott?");
				break;
			case 202:
				dialogoLineas.push_back("Puedes buscarlo empujando esas cajas si quieres...\nAl final, solo guardo basura ahí.");
				dialogoOpciones.push_back("[1] Entendido");
				break;
			case 203:
				dialogoLineas.push_back("¡Lárgate de aquí!");
				dialogoOpciones.push_back("[1] Ya me voy...");
				break;
			case 210:
				dialogoLineas.push_back("¿Qué pasó ahora, niña?");
				if (tieneItem("Grimorio de Trazos") || capaCrafteada) {
					dialogoOpciones.push_back("[1] Solo pasaba por aquí, ya recuperé el Grimorio de Trazos, Agott.");
				}
				else {
					dialogoOpciones.push_back("[1] Sigo buscando el Grimorio de Trazos, necesito ayuda...");
				}
				if (myrphonRescatado && !agottSabeMyrphon) {
					dialogoOpciones.push_back("[2] Recuperé a la mascota de Richeh, está a salvo en el sótano.");
				}
				break;
			case 211:
				dialogoLineas.push_back("No esperaba que lo encontraras entre tantas cajas, jaja.");
				dialogoOpciones.push_back("[1] Continuar");
				break;
			case 212:
				dialogoLineas.push_back("Sabía que te costaría trabajo... Sigue empujando esas cajas.");
				dialogoOpciones.push_back("[1] Gracias por nada...");
				break;
			case 220:
				dialogoLineas.push_back("¿Por qué sigues aquí, Coco? No eres bienvenida.");
				if (tieneItem("Grimorio de Trazos") || capaCrafteada) {
					dialogoOpciones.push_back("[1] Nada, solo quería demostrar que lo encontré sola.");
				}
				else {
					dialogoOpciones.push_back("[1] ¡Sigo buscando algo, deja de molestar!");
				}
				if (myrphonRescatado && !agottSabeMyrphon) {
					dialogoOpciones.push_back("[2] Recuperé a la mascota de Richeh, está a salvo en el sótano.");
				}
				break;
			case 221:
				dialogoLineas.push_back("¡Estúpida niña!");
				dialogoOpciones.push_back("[1] Salir");
				break;
			case 222:
				dialogoLineas.push_back("¿Qué demonios estás buscando en mi torre?");
				dialogoOpciones.push_back("[1] ¡Ya lo verás!");
				break;
			case 223:
				dialogoLineas.push_back("...");
				dialogoOpciones.push_back("[1] Salir");
				break;
			case 230:
				dialogoLineas.push_back("El taller ya volvió a la normalidad.");
				dialogoLineas.push_back("Deja de perder el tiempo y concéntrate en tu práctica.");
				dialogoOpciones.push_back("[1] Salir");
				break;
			case 231:
				dialogoLineas.push_back("...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 232:
				dialogoLineas.push_back("(Sus ojos se abren de golpe, perdiendo toda su compostura)");
				dialogoLineas.push_back("¿Hablas en serio? ¿E-Esa criatura tan adorable está bien?...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 233:
				dialogoLineas.push_back("(Se aclara la garganta rápidamente y recupera su postura seria)");
				dialogoLineas.push_back("Quiero decir... Qué buena noticia para Richeh.");
				dialogoLineas.push_back("Ella... ha estado muy distraída sin él.");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 234:
				dialogoLineas.push_back("Supongo que tengo que reconocer tu esfuerzo.");
				dialogoLineas.push_back("No cualquiera sobrevive al laberinto Serpentback...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 235:
				dialogoLineas.push_back("...Gracias por traerlo de vuelta.");
				dialogoLineas.push_back("Has hecho un trabajo aceptable, Coco.");
				dialogoOpciones.push_back("[1] ¡De nada, Agott!");
				break;
			default:
				dialogoLineas.push_back("...");
				dialogoOpciones.push_back("[1] Salir");
				break;
			}
		}
		else if (dialogoNpc == "Richeh") {
			switch (estadoDialogo) {
			case 300:
				dialogoLineas.push_back("*llorando*");
				dialogoOpciones.push_back("[1] Hola, ¿por qué estás llorando?");
				dialogoOpciones.push_back("[2] Disculpa... bajé por este pozo buscando un objeto mágico...");
				break;
			case 301:
				dialogoLineas.push_back("...No es de tu incumbencia. Vete...");
				dialogoLineas.push_back("...Es mi culpa por no haberlo sujetado más fuerte.");
				dialogoOpciones.push_back("[1] ¿Qué? Pero ¿cómo te puedo ayudar?");
				dialogoOpciones.push_back("[2] ¿Por eso estás llorando?");
				break;
			case 302:
				dialogoLineas.push_back("¿Ayudarme?... No creo que puedas.");
				dialogoLineas.push_back("...Mi Myrphon todavía no regresa. Se quedó atrapado");
				dialogoLineas.push_back("en el laberinto Serpentback tras el ataque...");
				dialogoLineas.push_back("...Si de verdad quieres ayudar, trae a Myrphon de vuelta a este cuarto.");
				dialogoOpciones.push_back("[1] ¡Acepto, traeré a Myrphon de vuelta!");
				break;
			case 303:
				dialogoLineas.push_back("¡No tienes idea de nada! ¡Lárgate!");
				dialogoLineas.push_back("...¡No quiero volver a ver tu cara por aquí!");
				dialogoOpciones.push_back("[1] Salir...");
				break;
			case 304:
				dialogoLineas.push_back("(Se seca las lágrimas desesperada con su túnica)");
				dialogoLineas.push_back("¡¿D-De verdad harías eso por mí?! ¡Muchas gracias!");
				dialogoLineas.push_back("¡Date prisa! Serpentback está al sur del cruce y es muy peligroso...");
				dialogoLineas.push_back("¡Tráelo a salvo, por lo que más quieras!");
				dialogoOpciones.push_back("[1] ¡No te preocupes, lo traeré de vuelta!");
				break;
			case 310:
				dialogoLineas.push_back("¿Un objeto mágico?... Tengo la Pluma Térmica (Searneedle) de Olruggio.");
				dialogoLineas.push_back("Su punta se calienta al rojo vivo para quebrar roca.");
				dialogoLineas.push_back("...No te la daré mientras mi pobre Myrphon siga atrapado en la oscuridad.");
				dialogoOpciones.push_back("[1] Lamento escuchar eso... ¿Cómo te puedo ayudar a recuperarlo?");
				dialogoOpciones.push_back("[2] Vaya, qué mal, y... ¿por un simple animal te pones a llorar?");
				break;
			case 311:
				dialogoLineas.push_back("Mi Myrphon se asustó por el ataque de los Sombreros de Ala Ancha en Serpentback...");
				dialogoLineas.push_back("Si vas al sur del cruce y lo rescatas, la Pluma Térmica será tuya. ¿Aceptas?");
				dialogoOpciones.push_back("[1] Sí, lo rescataré.");
				break;
			case 312:
				dialogoLineas.push_back("¡No es un simple animal! ¡Es mi amigo!");
				dialogoLineas.push_back("...¡Vete de mi cuarto ahora mismo!");
				dialogoOpciones.push_back("[1] Salir...");
				break;
			case 320:
				dialogoLineas.push_back("¿Qué quieres ahora? Te dije que te largaras.");
				dialogoOpciones.push_back("[1] Está bien, ya me iba.");
				dialogoOpciones.push_back("[2] Espera, hablo en serio... Quiero ayudarte a buscar a Myrphon.");
				break;
			case 321:
				dialogoLineas.push_back("(Te mira de reojo de forma desconfiada) ...¿De verdad?...");
				dialogoLineas.push_back("...Está bien. Se perdió en el laberinto Serpentback al sur del cruce");
				dialogoLineas.push_back("tras el ataque de los Sombreros de Ala Ancha.");
				dialogoLineas.push_back("Tráelo sano y salvo. Si lo logras, te entregaré la Pluma Térmica.");
				dialogoOpciones.push_back("[1] ¡Haré lo mejor que pueda y te lo traeré!");
				break;
			case 330:
				dialogoLineas.push_back("¿Pudiste encontrarlo? ¿Dónde está mi Myrphon?");
				dialogoOpciones.push_back("[1] Aún no...");
				break;
			case 331:
				dialogoLineas.push_back("Por favor, date prisa... El laberinto Serpentback es muy oscuro");
				dialogoLineas.push_back("y debe tener mucho miedo. Estaré esperando aquí.");
				dialogoOpciones.push_back("[1] Entendido...");
				break;
			case 350:
				dialogoLineas.push_back("(Sus ojos se abren de par en par al ver a su pincelero) \"¡¡Myrphon!!\"");
				dialogoOpciones.push_back("[1] Aquí está, sano y salvo.");
				break;
			case 351:
				dialogoLineas.push_back("(Abraza fuertemente a su mascota mientras llora de alegría)");
				dialogoLineas.push_back("¡Muchas gracias! Pensé que no volvería a verlo...");
				dialogoLineas.push_back("Lo prometido es deuda. Toma la Pluma Térmica Searneedle de Olruggio.");
				dialogoLineas.push_back("Úsala para fundir el muro de roca de la Cueva Misteriosa (al norte del cruce).");
				dialogoOpciones.push_back("[1] ¡Muchas gracias, Richeh!");
				break;
			case 360:
				dialogoLineas.push_back("(•u•): ¡Hola! Myrphon y yo estamos muy felices gracias a ti.");
				dialogoLineas.push_back("¡Ten cuidado en tus viajes, Coco!");
				dialogoOpciones.push_back("[1] ¡Nos vemos, Richeh!");
				break;
			default:
				dialogoLineas.push_back("*llorando*");
				dialogoOpciones.push_back("[1] Hola, ¿por qué estás llorando?");
				dialogoOpciones.push_back("[2] Disculpa... bajé por este pozo buscando un objeto mágico...");
				break;
			}
		}
		else if (dialogoNpc == "Letrero") {
			switch (estadoDialogo) {
			case 400:
				dialogoLineas.push_back("[DATO CURIOSO DE RICHEH #1]");
				dialogoLineas.push_back("Richeh es hermana de Riliphin; ambos estudiaron en la estricta");
				dialogoLineas.push_back("Gran Sala de los Hechiceros...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 401:
				dialogoLineas.push_back("...Richeh rechaza la rigidez de la Gran Sala y prefiere");
				dialogoLineas.push_back("dibujar únicamente la magia que nace de su propio corazón.");
				dialogoOpciones.push_back("[1] Cerrar");
				break;
			case 402:
				dialogoLineas.push_back("[DATO CURIOSO DE RICHEH #2]");
				dialogoLineas.push_back("Cuando Richeh se enfada o no quiere hacer algo, se pone");
				dialogoLineas.push_back("rígida como un mueble y sus compañeras tienen que");
				dialogoLineas.push_back("cargarla en peso para moverla.");
				dialogoOpciones.push_back("[1] Cerrar");
				break;
			case 404:
				dialogoLineas.push_back("[DATO CURIOSO DE RICHEH #3]");
				dialogoLineas.push_back("La autora, Kamome Shirahama, diseñó las túnicas y el cabello");
				dialogoLineas.push_back("de Richeh con líneas muy rectas, pesadas y rígidas...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 405:
				dialogoLineas.push_back("...para reflejar visualmente lo 'cuadrada' y terca");
				dialogoLineas.push_back("que es su personalidad.");
				dialogoOpciones.push_back("[1] Cerrar");
				break;
			case 410:
				dialogoLineas.push_back("[DIARIO DEL ATELIER: MYRPHON PERDIDO]");
				dialogoLineas.push_back("Richeh, compañera de cuarto de Agott, no admite visitas");
				dialogoLineas.push_back("desde que se perdió Myrphon, su pequeño pincelero.");
				dialogoLineas.push_back("Se extravió tras el ataque de unos Sombreros de Ala Ancha");
				dialogoLineas.push_back("en Serpentback durante el segundo examen...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 411:
				dialogoLineas.push_back("...El examen se canceló por el caos y Myrphon quedó atrapado.");
				dialogoLineas.push_back("Richeh y Agott esperan que alguien valiente lo traiga de vuelta.");
				dialogoOpciones.push_back("[1] Cerrar");
				break;
			default:
				dialogoLineas.push_back("...");
				dialogoOpciones.push_back("[1] Cerrar");
				break;
			}
		}
		prepararTextoDialogo();
	}

	void iniciarDialogoAgott() {
		habloAgott = true;
		if (agottSabeMyrphon) iniciarDialogo("Agott", 230);
		else if (respuestaInicialAgott == 2) iniciarDialogo("Agott", 220);
		else iniciarDialogo("Agott", 210);
	}

	void iniciarDialogoRicheh() {
		habloRicheh = true;
		if (dioVaraRicheh) iniciarDialogo("Richeh", 360);
		else if (myrphonRescatado) iniciarDialogo("Richeh", 350);
		else if (misionMyrphonActiva) iniciarDialogo("Richeh", 330);
		else if (richehEnojada) iniciarDialogo("Richeh", 320);
		else iniciarDialogo("Richeh", 300);
	}

	void procesarOpcionDialogo(int opcion) {
		if (dialogoNpc == "Simple") {
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Qifrey") {
			if (estadoDialogo == 1) {
				if (opcion == 1) {
					pidioMantoQifrey = true;
					actualizarObjetivo();
					if (capaCrafteada) estadoDialogo = 15;
					else if (tieneItem("Fibra de Arbol") && tieneItem("Tinta magica") && tieneItem("Grimorio de Trazos")) estadoDialogo = 11;
					else estadoDialogo = 10;
				}
				else if (opcion == 2) estadoDialogo = 20;
				else if (opcion == 3) {
					if (tintaMagicaEntregada) estadoDialogo = 34;
					else estadoDialogo = 30;
				}
				else if (opcion == 4) {
					if (!tintaSecundariaEntregada && (tieneItem("Frasco de Tinta") || tieneItem("Tinta de Viento"))) estadoDialogo = 40;
					else {
						cerrarDialogo();
						return;
					}
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 10) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 11) {
				if (opcion == 1) {
					quitarItem("Fibra de Arbol");
					quitarItem("Tinta magica");
					quitarItem("Grimorio de Trazos");
					agregarItem("Capa Magica", "Manto de aprendiz tejido con fibras del Árbol de Plata. Protege y repele la hechicería.", "Equipamiento Mágico");
					capaCrafteada = true;
					confianzaQifrey = 2;
					actualizarObjetivo();
					estadoDialogo = 12;
					actualizarDialogoV1();
				}
				return;
			}
			if (estadoDialogo == 12) {
				if (opcion == 1) {
					misionCompletada = true;
					solicitaFinNivel = true;
					finMostrado = true;
					cerrarDialogo();
				}
				return;
			}
			if (estadoDialogo == 15 || estadoDialogo == 20) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 30) {
				if (opcion == 1) estadoDialogo = 31;
				else if (opcion == 2) estadoDialogo = 35;
				else {
					cerrarDialogo();
					return;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 35) {
				if (opcion == 1) estadoDialogo = 32;
				else {
					cerrarDialogo();
					return;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 31) {
				if (opcion == 1) estadoDialogo = 32;
				else {
					cerrarDialogo();
					return;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 32) {
				if (opcion == 1) {
					if (tieneItem("Tinta magica")) estadoDialogo = 34;
					else if (agregarItem("Tinta magica", "Tinta de plata otorgada por Qifrey para trazar sellos.", "Consumible Mágico")) {
						tintaMagicaEntregada = true;
						if (confianzaQifrey < 1) confianzaQifrey = 1;
						actualizarObjetivo();
						estadoDialogo = 33;
					}
					else estadoDialogo = 99;
					actualizarDialogoV1();
				}
				return;
			}
			if (estadoDialogo == 33 || estadoDialogo == 34 || estadoDialogo == 99) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 40) {
				if (tieneItem("Frasco de Tinta")) quitarItem("Frasco de Tinta");
				else if (tieneItem("Tinta de Viento")) quitarItem("Tinta de Viento");
				tintaSecundariaEntregada = true;
				confianzaQifrey = 2;
				estadoDialogo = 41;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 41) {
				cerrarDialogo();
				return;
			}
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Agott") {
			if (estadoDialogo == 200) {
				estadoDialogo = 201;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 201) {
				if (opcion == 1) {
					respuestaInicialAgott = 1;
					estadoDialogo = 202;
				}
				else if (opcion == 2) {
					respuestaInicialAgott = 2;
					if (confianzaAgott > 0) confianzaAgott--;
					estadoDialogo = 203;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 210) {
				if (opcion == 2 && myrphonRescatado && !agottSabeMyrphon) estadoDialogo = 231;
				else if (tieneItem("Grimorio de Trazos") || capaCrafteada) estadoDialogo = 211;
				else estadoDialogo = 212;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 220) {
				if (opcion == 2 && myrphonRescatado && !agottSabeMyrphon) estadoDialogo = 231;
				else if (tieneItem("Grimorio de Trazos") || capaCrafteada) {
					if (confianzaAgott > 0) confianzaAgott--;
					estadoDialogo = 221;
				}
				else estadoDialogo = 222;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 222) {
				if (confianzaAgott > 0) confianzaAgott--;
				estadoDialogo = 223;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo >= 231 && estadoDialogo <= 234) {
				estadoDialogo++;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 235) {
				agottSabeMyrphon = true;
				confianzaAgott = 2;
				cerrarDialogo();
				return;
			}
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Richeh") {
			if (estadoDialogo == 300) {
				if (opcion == 1) estadoDialogo = 301;
				else if (opcion == 2) estadoDialogo = 310;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 301) {
				if (opcion == 1) estadoDialogo = 302;
				else if (opcion == 2) {
					if (confianzaRicheh > 0) confianzaRicheh--;
					richehEnojada = true;
					estadoDialogo = 303;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 302) {
				misionMyrphonActiva = true;
				if (confianzaRicheh < 2) confianzaRicheh++;
				if (myrphonObjeto != nullptr) {
					myrphonObjeto->setX(116 * 32);
					myrphonObjeto->setY(60 * 32);
					myrphonObjeto->setVisible(true);
				}
				actualizarObjetivo();
				estadoDialogo = 304;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 303 || estadoDialogo == 304) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 310) {
				if (opcion == 1) estadoDialogo = 311;
				else if (opcion == 2) {
					if (confianzaRicheh > 0) confianzaRicheh--;
					richehEnojada = true;
					estadoDialogo = 312;
				}
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 311) {
				misionMyrphonActiva = true;
				if (myrphonObjeto != nullptr) {
					myrphonObjeto->setX(116 * 32);
					myrphonObjeto->setY(60 * 32);
					myrphonObjeto->setVisible(true);
				}
				actualizarObjetivo();
				estadoDialogo = 304;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 312) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 320) {
				if (opcion == 2) {
					estadoDialogo = 321;
					actualizarDialogoV1();
				}
				else cerrarDialogo();
				return;
			}
			if (estadoDialogo == 321) {
				misionMyrphonActiva = true;
				richehEnojada = false;
				if (myrphonObjeto != nullptr) {
					myrphonObjeto->setX(116 * 32);
					myrphonObjeto->setY(60 * 32);
					myrphonObjeto->setVisible(true);
				}
				actualizarObjetivo();
				estadoDialogo = 304;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 330) {
				estadoDialogo = 331;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 331) {
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 350) {
				estadoDialogo = 351;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 351) {
				dioVaraRicheh = true;
				misionMyrphonActiva = false;
				if (!tieneItem("Pluma Termica")) {
					agregarItem("Pluma Termica", "Pluma Searneedle creada por Olruggio. Calienta su punta al rojo vivo para fundir roca sólida.", "Herramienta Arcana");
					mensajeTemporal = "[Recibiste: Pluma Térmica]";
					ticksMensajeTemporal = 100;
				}
				confianzaRicheh = 2;
				myrphonConRicheh = true;
				actualizarObjetivo();
				cerrarDialogo();
				return;
			}
			if (estadoDialogo == 360) {
				cerrarDialogo();
				return;
			}
			cerrarDialogo();
			return;
		}
		if (dialogoNpc == "Letrero") {
			if (estadoDialogo == 400) {
				estadoDialogo = 401;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 404) {
				estadoDialogo = 405;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 410) {
				estadoDialogo = 411;
				actualizarDialogoV1();
				return;
			}
			cerrarDialogo();
			return;
		}
		cerrarDialogo();
	}

	void moverSeleccionDialogo(int delta) {
		if (!dialogoActivo) return;
		int total = (int)dialogoOpciones.size();
		if (total <= 0) return;
		dialogoOpcionSeleccionada = (dialogoOpcionSeleccionada + delta + total) % total;
	}

	void aplicarOpcionDialogo(int indice) {
		if (!dialogoActivo) return;
		String^ texto = dialogoTextoManaged;
		if (texto != nullptr && caracteresVisibles < texto->Length) return;
		if (indice < 0 || indice >= (int)dialogoOpciones.size()) return;
		dialogoOpcionSeleccionada = indice;
		procesarOpcionDialogo(indice + 1);
	}

	bool textoDialogoCompleto() {
		String^ texto = dialogoTextoManaged;
		if (texto == nullptr) return true;
		return caracteresVisibles >= texto->Length;
	}

	void actualizarTextoAnimado() {
		if (!dialogoActivo) return;
		String^ texto = dialogoTextoManaged;
		if (texto == nullptr) return;
		if (caracteresVisibles >= texto->Length) return;
		ticksTypewriter++;
		if (ticksTypewriter % 2 != 0) return;
		caracteresVisibles += 2;
		if (caracteresVisibles > texto->Length) caracteresVisibles = texto->Length;
		if (ticksTypewriter % 4 == 0) GestorAudio::ReproducirSfxDialogo();
	}

	bool posicionLibre(int nuevoX, int nuevoY) {
		if (nuevoX < 0 || nuevoY < 0) return false;
		if (nuevoX + jugador->getAncho() > mapaActual->getAnchoTotalPixels()) return false;
		if (nuevoY + jugador->getAlto() > mapaActual->getAltoTotalPixels()) return false;
		if (mapaActual->esColisionSolida(nuevoX + 3, nuevoY + 14, jugador->getAncho() - 6, jugador->getAlto() - 16)) return false;

		if (!enPozo && !misionMyrphonActiva) {
			int centroNuevoCol = (nuevoX + jugador->getAncho() / 2) / Configuracion::TILE_SIZE;
			int centroNuevoFila = (nuevoY + jugador->getAlto() / 2) / Configuracion::TILE_SIZE;
			if (centroNuevoCol >= 115 && centroNuevoFila >= 55 && centroNuevoFila <= 66) {
				mensajeTemporal = "Coco: Está muy oscuro por aquí... debería volver (habla con Richeh primero).";
				ticksMensajeTemporal = 85;
				return false;
			}
		}

		Rectangle rect(nuevoX + 3, nuevoY + 14, jugador->getAncho() - 6, jugador->getAlto() - 16);
		for (size_t i = 0; i < listaNPCs.size(); i++) {
			if (rect.IntersectsWith(listaNPCs[i]->getCollisionRect())) return false;
		}
		if (enPozo && richehPozo != nullptr) {
			if (rect.IntersectsWith(richehPozo->getCollisionRect())) return false;
		}
		for (size_t i = 0; i < listaArboles.size(); i++) {
			if (rect.IntersectsWith(listaArboles[i]->getTrunkRect())) return false;
		}
		return true;
	}

	int distanciaAlCentro(Entidad* entidad) {
		int cx = jugador->getX() + jugador->getAncho() / 2;
		int cy = jugador->getY() + jugador->getAlto() / 2;
		int ex = entidad->getX() + entidad->getAncho() / 2;
		int ey = entidad->getY() + entidad->getAlto() / 2;
		int dx = cx - ex;
		int dy = cy - ey;
		return (int)std::sqrt((double)(dx * dx + dy * dy));
	}

	bool estaEnTorre() {
		int cx = jugador->getX() + jugador->getAncho() / 2;
		int cy = jugador->getY() + jugador->getAlto() / 2;
		return mapa->determinarCuarto(cx, cy) == 3;
	}

	void actualizarObjetivoCercano() {
		promptInteraccion = "";
		tipoObjetivo = INTERACCION_NINGUNA;
		indiceObjetivo = -1;
		if (enIntro || dialogoActivo || transicionPozoActiva || myrphonCorriendo || myrphonHuyendo) return;

		int centroX = jugador->getX() + jugador->getAncho() / 2;
		int centroY = jugador->getY() + jugador->getAlto() / 2;

		if (enPozo) {
			if (abs(centroX - (mapaPozo->getPuntoSalidaPozoX() + 32)) <= 64 && abs(centroY - (mapaPozo->getPuntoSalidaPozoY() + 32)) <= 64) {
				tipoObjetivo = INTERACCION_SALIDA_POZO;
				promptInteraccion = "Presiona [Espacio] para subir a la torre";
				return;
			}
			if (abs(centroX - (13 * 32 + 16)) <= 42 && abs(centroY - (5 * 32 + 16)) <= 42) {
				tipoObjetivo = INTERACCION_LETRERO_CURIOSIDADES;
				promptInteraccion = "Presiona [Espacio] para leer: Curiosidades de Richeh";
				return;
			}
			if (abs(centroX - (19 * 32 + 16)) <= 42 && abs(centroY - (5 * 32 + 16)) <= 42) {
				tipoObjetivo = INTERACCION_LETRERO_DIARIO;
				promptInteraccion = "Presiona [Espacio] para leer: Diario del Atelier";
				return;
			}
			if (richehPozo != nullptr && richehPozo->estaCerca(centroX, centroY)) {
				tipoObjetivo = INTERACCION_RICHEH;
				promptInteraccion = "Presiona [Espacio] para hablar con Richeh";
				return;
			}
			return;
		}

		if (!paredRocaRota && mapa->estaCercaParedRoca(jugador->getX(), jugador->getY(), jugador->getAncho(), jugador->getAlto())) {
			tipoObjetivo = INTERACCION_PARED_ROCA;
			if (tieneItem("Pluma Termica")) promptInteraccion = "Presiona [Espacio] para usar Pluma Térmica y romper la roca";
			else promptInteraccion = "Coco: La pared de roca está agrietada... Necesito una pluma térmica.";
			return;
		}

		int mejor = 999999;
		for (size_t i = 0; i < listaNPCs.size(); i++) {
			if (listaNPCs[i]->estaCerca(centroX, centroY)) {
				int d = distanciaAlCentro(listaNPCs[i]);
				if (d < mejor) {
					mejor = d;
					if (listaNPCs[i]->getIdPersonaje() == 1) {
						tipoObjetivo = INTERACCION_QIFREY;
						promptInteraccion = "Presiona [Espacio] para interactuar con Qifrey";
					}
					else {
						tipoObjetivo = INTERACCION_AGOTT;
						promptInteraccion = "Presiona [Espacio] para hablar con Agott";
					}
					indiceObjetivo = (int)i;
				}
			}
		}

		Rectangle rectJugador = jugador->getRectangle();
		for (size_t i = 0; i < listaObjetos.size(); i++) {
			if (!listaObjetos[i]->getVisible()) continue;
			if (listaObjetos[i]->getTipo() == ObjetoTroll) {
				Rectangle rectTroll(listaObjetos[i]->getX() - 6, 33 * 32, 48, 7 * 32);
				if (!rectTroll.IntersectsWith(rectJugador)) continue;
			}
			else if (!listaObjetos[i]->getInteractionRect().IntersectsWith(rectJugador)) continue;
			int d = distanciaAlCentro(listaObjetos[i]);
			if (d >= mejor) continue;
			mejor = d;
			indiceObjetivo = (int)i;
			int tipo = listaObjetos[i]->getTipo();
			if (tipo == ObjetoPozo) {
				tipoObjetivo = INTERACCION_OBJETO_POZO;
				promptInteraccion = "Presiona [Espacio] para bajar al pozo";
			}
			else {
				tipoObjetivo = INTERACCION_OBJETO;
				if (tipo == ObjetoGato) promptInteraccion = "Presiona [Espacio] para acariciar al gato de Richeh";
				else if (tipo == ObjetoTroll) promptInteraccion = "Presiona [Espacio] para investigar el borde oculto";
				else if (tipo == ObjetoMontonCajas) promptInteraccion = "Presiona [Espacio] para revisar las cajas";
				else if (tipo == ObjetoMyrphon) promptInteraccion = "Presiona [Espacio] para entrar al Laberinto Serpentback";
				else if (tipo == ObjetoLetrero) promptInteraccion = "Presiona [Espacio] para leer el letrero";
				else promptInteraccion = "Presiona [Espacio] para recoger: " + listaObjetos[i]->getNombre();
			}
		}

		if (tipoObjetivo == INTERACCION_NINGUNA && estaEnTorre()) {
			promptInteraccion = "Empuja las cajas de madera para descubrir qué esconden";
		}
	}

	void bajarPozo() {
		if (transicionPozoActiva) return;
		GestorAudio::ReproducirSfxAccion();
		posAtelierX = jugador->getX();
		posAtelierY = jugador->getY();
		transicionPozoActiva = true;
		transicionBajandoPozo = true;
		ticksTransicionPozo = 60;
		cerrarDialogo();
		audio.pausar();
	}

	void subirPozo() {
		if (transicionPozoActiva) return;
		GestorAudio::ReproducirSfxAccion();
		transicionPozoActiva = true;
		transicionBajandoPozo = false;
		ticksTransicionPozo = 60;
		cerrarDialogo();
		audio.pausar();
	}

	void finalizarTransicionPozo() {
		if (transicionBajandoPozo) {
			enPozo = true;
			mapaActual = mapaPozo;
			jugador->setX(mapaPozo->getPuntoInicioPozoX());
			jugador->setY(mapaPozo->getPuntoInicioPozoY());
			jugador->detener();
			if (myrphonRescatado && !dioVaraRicheh) {
				myrphonCorriendo = true;
				ticksMyrphonCorriendo = 0;
				myrphonConRicheh = false;
			}
		}
		else {
			enPozo = false;
			mapaActual = mapa;
			if (posAtelierX <= 0 || posAtelierY <= 0) {
				posAtelierX = 120 * 32;
				posAtelierY = 36 * 32;
			}
			jugador->setX(posAtelierX);
			jugador->setY(posAtelierY);
			jugador->detener();
			if (dioVaraRicheh) myrphonConRicheh = true;
		}
		ultimaUbicacion = nullptr;
		ticksTituloUbicacion = 0;
		transicionPozoActiva = false;
		reanudarAudioNivel();
	}

	void actualizarCorridaMyrphon() {
		if (myrphonHuyendo) {
			ticksMyrphonHuyendo++;
			if (myrphonObjeto != nullptr) {
				int targetX = 131 * 32;
				int curX = myrphonObjeto->getX();
				if (curX < targetX) myrphonObjeto->setX(curX + 6);
			}
			if (ticksMyrphonHuyendo >= 65) {
				myrphonHuyendo = false;
				myrphonEscapado = true;
				ticksMyrphonHuyendo = 0;
				if (myrphonObjeto != nullptr) {
					myrphonObjeto->setX(131 * 32);
					myrphonObjeto->setY(60 * 32);
				}
				mensajeTemporal = "Myrphon huyó al Laberinto Serpentback. ¡Pulsa [Espacio] frente a la entrada!";
				ticksMensajeTemporal = 120;
			}
			return;
		}

		if (!myrphonCorriendo) return;
		ticksMyrphonCorriendo++;
		if (ticksMyrphonCorriendo < 50) return;
		myrphonCorriendo = false;
		ticksMyrphonCorriendo = 0;
		myrphonConRicheh = true;
		habloRicheh = true;
		iniciarDialogo("Richeh", 350);
	}

	void recogerItem(BloqueMapa* objeto) {
		const std::string& nombre = objeto->getNombre();
		if (!agregarItem(nombre, objeto->getDescripcion(), "Objeto")) {
			mensajeTemporal = "[Mochila llena: ¡Máx. 8 ítems!]";
			ticksMensajeTemporal = 90;
			return;
		}
		objeto->recolectar();
		GestorAudio::ReproducirSfxAccion();
		if (nombre == "Grimorio de Trazos") libroEncontrado = true;
		mensajeTemporal = "[Recogiste: " + nombre + "]";
		ticksMensajeTemporal = 85;
		actualizarObjetivo();
	}

	void crearCajasTorre() {
		std::srand((unsigned int)std::time(NULL));
		std::vector<std::pair<int, int> > candidatos;
		int cols[] = { 117, 121, 125 };
		int rows[] = { 32, 36, 40 };
		for (int r = 0; r < 3; r++) {
			for (int c = 0; c < 3; c++) {
				candidatos.push_back(std::make_pair(cols[c], rows[r]));
			}
		}
		for (int i = (int)candidatos.size() - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			std::pair<int, int> temp = candidatos[i];
			candidatos[i] = candidatos[j];
			candidatos[j] = temp;
		}
		int numCajas = 6;
		if (numCajas > (int)candidatos.size()) numCajas = (int)candidatos.size();
		int indiceLibro = rand() % numCajas;
		int indicePozo = (indiceLibro + 1 + (rand() % (numCajas - 1))) % numCajas;
		cajasMovidasContador = 0;
		for (int i = 0; i < numCajas; i++) {
			CajaMovible* caja = new CajaMovible(candidatos[i].first, candidatos[i].second);
			if (i == indiceLibro) caja->setContenido(CajaGrimorio);
			else if (i == indicePozo) caja->setContenido(CajaPozo);
			listaCajas.push_back(caja);
		}
		for (size_t i = 0; i < listaCajas.size(); i++) {
			CajaMovible* caja = listaCajas[i];
			if (caja->getContenido() == CajaGrimorio && grimorioObjeto != nullptr) {
				grimorioObjeto->setX(caja->getTileX() * 32 + 16);
				grimorioObjeto->setY(caja->getTileY() * 32 + 16);
				grimorioObjeto->setVisible(false);
			}
			if (caja->getContenido() == CajaPozo && pozoObjeto != nullptr) {
				pozoObjeto->setX(caja->getTileX() * 32);
				pozoObjeto->setY(caja->getTileY() * 32);
				pozoObjeto->setVisible(false);
			}
		}
	}

	bool cajaDestinoLibre(CajaMovible* caja, int tx, int ty) {
		if (tx < 113 || tx + 1 > 133 || ty < 30 || ty + 1 > 44) return false;
		if (mapa->esColisionSolida(tx * 32 + 4, ty * 32 + 4, 56, 56)) return false;
		Rectangle destino(tx * 32, ty * 32, 64, 64);
		for (size_t i = 0; i < listaCajas.size(); i++) {
			if (listaCajas[i] == caja) continue;
			if (destino.IntersectsWith(listaCajas[i]->getRectangle())) return false;
		}
		for (size_t i = 0; i < listaNPCs.size(); i++) {
			if (destino.IntersectsWith(listaNPCs[i]->getCollisionRect())) return false;
		}
		if (pozoObjeto != nullptr && pozoObjeto->getVisible() && destino.IntersectsWith(pozoObjeto->getRectangle())) return false;
		if (grimorioObjeto != nullptr && grimorioObjeto->getVisible() && destino.IntersectsWith(grimorioObjeto->getRectangle())) return false;
		return true;
	}

	bool intentarEmpujarCaja(int dx, int dy, Rectangle rectSig) {
		for (size_t i = 0; i < listaCajas.size(); i++) {
			CajaMovible* caja = listaCajas[i];
			if (!rectSig.IntersectsWith(caja->getRectangle())) continue;
			int tx = caja->getTileX() + dx;
			int ty = caja->getTileY() + dy;
			if (!cajaDestinoLibre(caja, tx, ty)) return false;
			int origenX = caja->getTileX();
			int origenY = caja->getTileY();
			if (!caja->getMovida()) {
				caja->setMovida(true);
				cajasMovidasContador++;
			}
			caja->mover(dx, dy);
			if (caja->getContenido() == CajaGrimorio && !grimorioDescubierto) {
				grimorioDescubierto = true;
				if (grimorioObjeto != nullptr) {
					grimorioObjeto->setX(origenX * 32 + 16);
					grimorioObjeto->setY(origenY * 32 + 16);
					grimorioObjeto->setVisible(true);
				}
				if (cajasMovidasContador == 1) mensajeTemporal = "Coco: ¡Pff, a la primera! Ahí está el Grimorio.";
				else if (cajasMovidasContador == 2) mensajeTemporal = "Coco: ¡Bueno, no costó tanto encontrar el Grimorio!";
				else mensajeTemporal = "Coco: ¡Por fin encontré el Grimorio de Trazos!";
				ticksMensajeTemporal = 95;
				actualizarObjetivo();
			}
			else if (caja->getContenido() == CajaPozo && !pozoDescubierto) {
				pozoDescubierto = true;
				if (pozoObjeto != nullptr) {
					pozoObjeto->setX(origenX * 32);
					pozoObjeto->setY(origenY * 32);
					pozoObjeto->setVisible(true);
				}
				mensajeTemporal = "Coco: ¿Un pozo con escaleras? ¿Quién puede esconderse aquí?";
				ticksMensajeTemporal = 110;
				actualizarObjetivo();
			}
			return true;
		}
		return false;
	}

	bool hayCajaEn(Rectangle rectSig) {
		for (size_t i = 0; i < listaCajas.size(); i++) {
			if (rectSig.IntersectsWith(listaCajas[i]->getRectangle())) return true;
		}
		return false;
	}

	void crearEnemigosNivel1() {
		listaEnemigos.push_back(new Enemigo(88 * 32, 46 * 32, 1, 41 * 32, 57 * 32, "Sombrero de Ala Ancha", 2));
		listaEnemigos.push_back(new Enemigo(94 * 32, 60 * 32, 0, 92 * 32, 112 * 32, "Sombrero de Ala Ancha", 2));
		listaEnemigos.push_back(new Enemigo(104 * 32, 63 * 32, 1, 59 * 32, 68 * 32, "Sombrero de Ala Ancha", 2));
	}

	bool aplicarDanioEnemigoNivel1(const std::string& mensaje) {
		GestorAudio::ReproducirSfxAccion();
		jugador->restarVida();
		invulnerableTicks = 50;
		if (jugador->getVidas() <= 0) {
			solicitaDerrota = true;
			return true;
		}
		int retrocesoX = jugador->getX() - 28;
		if (posicionLibre(retrocesoX, jugador->getY())) jugador->setX(retrocesoX);
		mensajeTemporal = mensaje;
		ticksMensajeTemporal = 90;
		return false;
	}

	bool proyectilGolpeaObstaculo(int px, int py) {
		if (px < 0 || py < 0 || px + 24 > mapaActual->getAnchoTotalPixels() || py + 24 > mapaActual->getAltoTotalPixels()) return true;
		if (mapaActual->esColisionSolida(px + 6, py + 6, 12, 12)) return true;
		Rectangle rectP(px + 6, py + 6, 12, 12);
		for (size_t i = 0; i < listaArboles.size(); i++) {
			if (rectP.IntersectsWith(listaArboles[i]->getTrunkRect())) return true;
		}
		return false;
	}

	void actualizarEnemigosNivel1() {
		if (enPozo || enIntro || dialogoActivo || transicionPozoActiva || myrphonHuyendo) return;
		if (invulnerableTicks > 0) invulnerableTicks--;

		Rectangle rectJugador(jugador->getX() + 4, jugador->getY() + 10, jugador->getAncho() - 8, jugador->getAlto() - 12);
		for (size_t i = 0; i < listaEnemigos.size(); i++) {
			Enemigo* e = listaEnemigos[i];
			if (e == nullptr || !e->getActivo()) continue;
			e->moverPatrulla();
			if (distanciaAlCentro(e) <= 420) vioEnemigos = true;
			if ((ticksJuego + (int)i * 21) % 68 == 0 && distanciaAlCentro(e) <= 460 && proyectilesEnemigos.size() < 16) {
				proyectilesEnemigos.push_back(ProyectilMago::CrearPorDireccion(e->getX() + 4, e->getY() + 4, e->getDireccion(), 5));
			}
			if (invulnerableTicks == 0 && rectJugador.IntersectsWith(e->getCollisionRect())) {
				aplicarDanioEnemigoNivel1("¡Un Sombrero de Ala Ancha te atacó! (-1 corazón)");
				return;
			}
		}

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
			if (p.distanciaRecorrida > 420 || proyectilGolpeaObstaculo(p.x, p.y)) {
				p.iniciarImpacto();
				i++;
				continue;
			}
			if (rectJugador.IntersectsWith(p.getCollisionRect())) {
				p.iniciarImpacto();
				if (invulnerableTicks == 0) {
					if (aplicarDanioEnemigoNivel1("¡Un disparo oscuro te alcanzó! (-1 corazón)")) return;
				}
			}
			i++;
		}
	}

public:
	Nivel1() {
		camX = 0;
		camY = 0;
		nivelActual = 1;
		ticksJuego = 0;
		ticksTituloUbicacion = 0;
		ticksTituloNivel = 0;
		invulnerableTicks = 0;
		enIntro = true;
		enPozo = false;
		posAtelierX = 0;
		posAtelierY = 0;
		dialogoActivo = false;
		dialogoNpc = "";
		estadoDialogo = 0;
		dialogoHablante = "";
		dialogoRol = "";
		dialogoConfianza = 0;
		dialogoOpcionSeleccionada = 0;
		caracteresVisibles = 0;
		ticksTypewriter = 0;
		habloQifrey = false;
		habloAgott = false;
		habloRicheh = false;
		pidioMantoQifrey = false;
		confianzaQifrey = 0;
		confianzaAgott = 1;
		confianzaRicheh = 1;
		respuestaInicialAgott = 0;
		richehEnojada = false;
		misionMyrphonActiva = false;
		myrphonRescatado = false;
		dioVaraRicheh = false;
		agottSabeMyrphon = false;
		capaCrafteada = false;
		tintaMagicaEntregada = false;
		tintaSecundariaEntregada = false;
		grimorioDescubierto = false;
		libroEncontrado = false;
		pozoDescubierto = false;
		paredRocaRota = false;
		entroTorre = false;
		monologoCruceActivado = false;
		gatoAcariciado = false;
		trollDescubierto = false;
		misionCompletada = false;
		finMostrado = false;
		solicitaMinijuego = false;
		solicitaTrollface = false;
		solicitaFinNivel = false;
		solicitaDerrota = false;
		transicionPozoActiva = false;
		transicionBajandoPozo = true;
		ticksTransicionPozo = 0;
		myrphonHuyendo = false;
		myrphonEscapado = false;
		ticksMyrphonHuyendo = 0;
		myrphonCorriendo = false;
		ticksMyrphonCorriendo = 0;
		myrphonConRicheh = false;
		ticksMensajeTemporal = 0;
		tipoObjetivo = INTERACCION_NINGUNA;
		indiceObjetivo = -1;
		myrphonObjeto = nullptr;
		pozoObjeto = nullptr;
		grimorioObjeto = nullptr;
		richehPozo = nullptr;
		cajasMovidasContador = 0;
		audioHabilitado = true;
		guiaActiva = true;
		GestorAudio::SetSfxHabilitado(true);

		bmpCoco = CargarBitmapSeguro("sprites/Coco_Spritesheet.png");
		bmpQifrey = CargarBitmapSeguro("sprites/Qifrey_Spritesheet.png");
		bmpAgott = CargarBitmapSeguro("sprites/Agott_Spritesheet.png");
		bmpRicheh = CargarBitmapSeguro("sprites/Richeh_Spritesheet.png");
		bmpEnemigo = CargarBitmapSeguro("sprites/Mago_Cueva_Spritesheet.png");
		bmpDisparo = CargarBitmapSeguro("sprites/Disparo_Cueva_Spritesheet.png");
		bmpMyrphon = CargarBitmapSeguro("sprites/Myrphon_Spritesheet.png");

		inicializarLluvia();

		mapa = new MapaNivel1();
		mapaPozo = new MapaNivel1();
		mapaPozo->cargarPozo();
		mapaActual = mapa;
		inventario = new Inventario();
		jugador = new Protagonista(mapa->getPuntoInicioX() + 4, mapa->getPuntoInicioY(), 1);
		iniciarNivel(1);
		asegurarPartidaRegenerativa();
	}

	~Nivel1() {
		audio.detener();
		limpiarEntidades();
		if (jugador != nullptr) delete jugador;
		if (inventario != nullptr) delete inventario;
		if (mapaPozo != nullptr) delete mapaPozo;
		if (mapa != nullptr) delete mapa;
	}

	int getNivelActual() { return nivelActual; }
	int getVidas() { return jugador->getVidas(); }
	int getTiempoSegundos() { return (ticksJuego * 25) / 1000; }
	int getCantidadInventario() { return inventario->getCantidad(); }
	int getCapacidadInventario() { return inventario->getCapacidad(); }
	bool getMisionCompletada() { return misionCompletada; }
	bool getMyrphonRescatado() { return myrphonRescatado; }
	bool getEnIntro() { return enIntro; }
	bool getEnTorre() { return !enPozo && estaEnTorre(); }
	bool getGuiaActiva() { return guiaActiva; }
	void alternarGuia() { guiaActiva = !guiaActiva; }

	void obtenerMisiones(std::vector<MisionInfo>& lista, int& indiceActual) {
		lista.clear();
		bool paso1 = pidioMantoQifrey;
		bool paso2 = tieneItem("Grimorio de Trazos") || capaCrafteada;
		bool paso3 = pozoDescubierto || habloRicheh || dioVaraRicheh;
		bool paso4 = misionMyrphonActiva || myrphonRescatado || dioVaraRicheh;
		bool paso5 = dioVaraRicheh;
		bool paso6 = paredRocaRota && (tieneItem("Fibra de Arbol") || capaCrafteada);
		bool paso7 = tieneItem("Tinta magica") || capaCrafteada;
		bool paso8 = misionCompletada;

		lista.push_back(MisionInfo{ "1. Hablar con el Maestro Qifrey", "Sal de la choza y baja por el sendero sur hasta su despacho.", paso1 });
		lista.push_back(MisionInfo{ "2. Conseguir el Grimorio de Trazos", "Cruza el puente al este, entra a la Torre de Agott y empuja las cajas.", paso2 });
		lista.push_back(MisionInfo{ "3. Bajar al sótano oculto", "Descubre el pozo debajo de las cajas en la torre y desciende.", paso3 });
		lista.push_back(MisionInfo{ "4. Aceptar el rescate de Myrphon", "Habla con Richeh en el sótano y ofrece ayudar a buscar a su mascota.", paso4 });
		lista.push_back(MisionInfo{ "5. Rescatar a Myrphon y volver con Richeh", "Atrapa a Myrphon en el laberinto Serpentback y recibe la Pluma Térmica.", paso5 });
		lista.push_back(MisionInfo{ "6. Romper la Cueva Misteriosa", "Ve al noreste, quiebra la roca con la Pluma Térmica y recoge la Fibra.", paso6 });
		lista.push_back(MisionInfo{ "7. Obtener la Tinta mágica", "Pídele materiales especiales para tus prácticas al Maestro Qifrey.", paso7 });
		lista.push_back(MisionInfo{ "8. Confeccionar la Capa de Aprendiz", "Regresa con Qifrey y entrégale los tres componentes reunidos.", paso8 });

		indiceActual = 7;
		for (int i = 0; i < (int)lista.size(); i++) {
			if (!lista[i].completada) {
				indiceActual = i;
				break;
			}
		}
	}

	void obtenerGuiaPasoActual(std::string& titulo, std::string& detalle, int& pasoNum, int& totalPasos, int& targetX, int& targetY, bool& mostrarFlecha) {
		totalPasos = 8;
		mostrarFlecha = true;
		if (misionCompletada) {
			pasoNum = 8;
			titulo = "Nivel completado";
			detalle = "Capa de Aprendiz lista";
			targetX = mapa->getPuntoQifreyX() + 14;
			targetY = mapa->getPuntoQifreyY() + 16;
			return;
		}
		if (!pidioMantoQifrey) {
			pasoNum = 1;
			titulo = "Habla con el Maestro Qifrey";
			detalle = "Sigue el camino hacia el sur hasta su despacho";
			targetX = mapa->getPuntoQifreyX() + 14;
			targetY = mapa->getPuntoQifreyY() + 16;
			return;
		}
		if (!tieneItem("Grimorio de Trazos") && !capaCrafteada) {
			pasoNum = 2;
			if (enPozo) {
				titulo = "Sube de la torre";
				detalle = "Ve hacia las escaleras de salida";
				targetX = mapaPozo->getPuntoSalidaPozoX() + 32;
				targetY = mapaPozo->getPuntoSalidaPozoY() + 32;
			}
			else if (grimorioDescubierto && grimorioObjeto != nullptr && grimorioObjeto->getVisible()) {
				titulo = "Recoge el Grimorio de Trazos";
				detalle = "Acércate al pergamino revelado para recogerlo";
				targetX = grimorioObjeto->getX() + 16;
				targetY = grimorioObjeto->getY() + 16;
			}
			else if (estaEnTorre()) {
				titulo = "Busca el Grimorio en la Torre";
				detalle = "Empuja las cajas de esta habitación";
				targetX = 112 * 32;
				targetY = 36 * 32;
				mostrarFlecha = false;
			}
			else {
				titulo = "Ve a la Torre de Agott";
				detalle = "Cruza el puente al este y busca el grimorio";
				targetX = 112 * 32;
				targetY = 36 * 32;
			}
			return;
		}
		if (!dioVaraRicheh) {
			if (!misionMyrphonActiva) {
				pasoNum = (pozoDescubierto || enPozo) ? 4 : 3;
				if (enPozo && richehPozo != nullptr) {
					titulo = "Habla con Richeh";
					detalle = "Acércate en el sótano y ofrece tu ayuda";
					targetX = richehPozo->getX() + 14;
					targetY = richehPozo->getY() + 16;
				}
				else if (pozoDescubierto && pozoObjeto != nullptr && pozoObjeto->getVisible()) {
					titulo = "Baja por el pozo oculto";
					detalle = "Acércate al pozo descubierto para descender";
					targetX = pozoObjeto->getX() + 32;
					targetY = pozoObjeto->getY() + 32;
				}
				else if (estaEnTorre()) {
					titulo = "Encuentra el pozo oculto";
					detalle = "Empuja las cajas de esta habitación";
					targetX = 112 * 32;
					targetY = 36 * 32;
					mostrarFlecha = false;
				}
				else {
					titulo = "Vuelve a la Torre de Agott";
					detalle = "Busca entre las cajas de la torre";
					targetX = 112 * 32;
					targetY = 36 * 32;
				}
				return;
			}
			if (!myrphonRescatado) {
				pasoNum = 5;
				if (enPozo) {
					titulo = "Sal del sótano";
					detalle = "Sube por las escaleras hacia la torre";
					targetX = mapaPozo->getPuntoSalidaPozoX() + 32;
					targetY = mapaPozo->getPuntoSalidaPozoY() + 32;
				}
				else if (myrphonEscapado) {
					titulo = "Atrapa a Myrphon";
					detalle = "Acércate a la entrada del laberinto Serpentback";
					targetX = 131 * 32 + 16;
					targetY = 60 * 32 + 16;
				}
				else {
					titulo = "Busca a Myrphon al sureste";
					detalle = "Toma el desvío sur desde el cruce";
					targetX = 118 * 32;
					targetY = 60 * 32;
				}
				return;
			}
			pasoNum = 5;
			if (enPozo && richehPozo != nullptr) {
				titulo = "Entrega a Myrphon a Richeh";
				detalle = "Habla con Richeh en el sótano";
				targetX = richehPozo->getX() + 14;
				targetY = richehPozo->getY() + 16;
			}
			else {
				titulo = "Regresa con Richeh al sótano";
				detalle = "Vuelve al pozo de la Torre de Agott";
				targetX = (pozoObjeto != nullptr && pozoObjeto->getVisible()) ? pozoObjeto->getX() + 32 : 122 * 32;
				targetY = (pozoObjeto != nullptr && pozoObjeto->getVisible()) ? pozoObjeto->getY() + 32 : 36 * 32;
			}
			return;
		}
		if (!paredRocaRota) {
			pasoNum = 6;
			if (enPozo) {
				titulo = "Sube del sótano";
				detalle = "Usa las escaleras para volver a la torre";
				targetX = mapaPozo->getPuntoSalidaPozoX() + 32;
				targetY = mapaPozo->getPuntoSalidaPozoY() + 32;
			}
			else {
				titulo = "Rompe la roca de la cueva";
				detalle = "Ve al noreste y usa la Pluma Térmica";
				targetX = 106 * 32 + 16;
				targetY = 15 * 32 + 16;
			}
			return;
		}
		if (!tieneItem("Fibra de Arbol") && !capaCrafteada) {
			pasoNum = 6;
			titulo = "Recoge la Fibra de Árbol";
			detalle = "Entra a la cueva noreste para tomarla";
			targetX = 114 * 32 + 16;
			targetY = 14 * 32 + 16;
			return;
		}
		if (!tieneItem("Tinta magica") && !capaCrafteada) {
			pasoNum = 7;
			titulo = "Consigue la Tinta mágica";
			detalle = "Pide materiales especiales al Maestro Qifrey";
			targetX = mapa->getPuntoQifreyX() + 14;
			targetY = mapa->getPuntoQifreyY() + 16;
			return;
		}
		pasoNum = 8;
		titulo = "Confecciona tu Capa";
		detalle = "Entrega los tres componentes a Qifrey";
		targetX = mapa->getPuntoQifreyX() + 14;
		targetY = mapa->getPuntoQifreyY() + 16;
	}
	std::string getInventarioNombre(int indice) { return inventario->getItem(indice).nombre; }
	std::string getInventarioDescripcion(int indice) { return inventario->getItem(indice).descripcion; }
	std::string getInventarioTipo(int indice) { return inventario->getItem(indice).tipo; }
	std::string getObjetivo() { return objetivoActual; }
	std::string getResumenConfianza(int indice) { return textoConfianza(indice); }

	void reordenarCajasTorre() {
		if (enPozo || !estaEnTorre()) return;
		for (size_t i = 0; i < listaCajas.size(); i++) {
			listaCajas[i]->restaurarOrigen();
		}
		if (jugador->getX() >= 115 * 32) {
			jugador->setX(114 * 32);
			jugador->setY(36 * 32);
		}
		mensajeTemporal = "Cajas de la torre restauradas a su lugar inicial.";
		ticksMensajeTemporal = 85;
	}

	void iniciarNivel(int numNivel) {
		nivelActual = numNivel;
		ticksJuego = 0;
		ticksTituloUbicacion = 0;
		ticksTituloNivel = 160;
		invulnerableTicks = 0;
		enIntro = true;
		enPozo = false;
		mapaActual = mapa;
		limpiarEntidades();
		mapa->cargarNivel(1);
		inventario->limpiar();
		jugador->setNivel(1);
		jugador->setX(mapa->getPuntoInicioX() + 4);
		jugador->setY(mapa->getPuntoInicioY());
		jugador->setVidas(3);
		jugador->setDireccion(0);
		jugador->detener();
		habloQifrey = false;
		habloAgott = false;
		habloRicheh = false;
		pidioMantoQifrey = false;
		confianzaQifrey = 0;
		confianzaAgott = 1;
		confianzaRicheh = 1;
		respuestaInicialAgott = 0;
		richehEnojada = false;
		misionMyrphonActiva = false;
		myrphonRescatado = false;
		dioVaraRicheh = false;
		agottSabeMyrphon = false;
		capaCrafteada = false;
		tintaMagicaEntregada = false;
		tintaSecundariaEntregada = false;
		grimorioDescubierto = false;
		libroEncontrado = false;
		pozoDescubierto = false;
		paredRocaRota = false;
		entroTorre = false;
		monologoCruceActivado = false;
		gatoAcariciado = false;
		trollDescubierto = false;
		misionCompletada = false;
		finMostrado = false;
		myrphonHuyendo = false;
		myrphonEscapado = false;
		ticksMyrphonHuyendo = 0;
		myrphonCorriendo = false;
		ticksMyrphonCorriendo = 0;
		myrphonConRicheh = false;
		vioMyrphon = false;
		vioEnemigos = false;
		ultimaUbicacion = nullptr;
		cerrarDialogo();

		NPC* qifrey = new NPC(mapa->getPuntoQifreyX() + 4, mapa->getPuntoQifreyY(), 1, "Qifrey", "Maestro Hechicero");
		qifrey->setMirada(0);
		NPC* agott = new NPC(mapa->getPuntoAgottX() + 4, mapa->getPuntoAgottY(), 2, "Agott", "Aprendiz de Maga");
		agott->setMirada(0);
		listaNPCs.push_back(qifrey);
		listaNPCs.push_back(agott);

		agregarObjeto(new BloqueMapa(47 * 32 + 4, 21 * 32 + 4, ObjetoVial, "Tinta de Viento", "Esencia de tinta de viento encontrada junto a la orilla del gran río."));
		agregarObjeto(new BloqueMapa(114 * 32 + 4, 14 * 32 + 4, ObjetoHoja, "Fibra de Arbol", "Fibras del Árbol de Plata para tejer el manto de aprendiz."));
		agregarObjeto(new BloqueMapa(120 * 32 + 4, 16 * 32 + 4, ObjetoVial, "Frasco de Tinta", "Frasco con tinta arcaica de plata preservada en la cueva misteriosa."));

		grimorioObjeto = new BloqueMapa(0, 0, ObjetoPergamino, "Grimorio de Trazos", "Repleto de círculos, flechas y vectores mágicos... No se entiende nada como un libro ordinario.");
		grimorioObjeto->setVisible(false);
		agregarObjeto(grimorioObjeto);

		pozoObjeto = new BloqueMapa(0, 0, ObjetoPozo, "Pozo de Agott", "Un pozo oculto entre las cajas de la torre.");
		pozoObjeto->setVisible(false);
		agregarObjeto(pozoObjeto);

		agregarObjeto(new BloqueMapa(17 * 32 + 4, 17 * 32 + 4, ObjetoLetrero, "Letrero: Choza de Trazos", "Letrero: Choza de Trazos. Dibuja runas con pasión y cuida tus pergaminos."));
		agregarObjeto(new BloqueMapa(26 * 32 + 4, 35 * 32 + 4, ObjetoLetrero, "Historia del Manga", "Letrero: Historia del Manga. Witch Hat Atelier fue creado por Kamome Shirahama en julio de 2016 (revista Morning Two, Kodansha), destacando por su magia basada en el arte del dibujo."));
		agregarObjeto(new BloqueMapa(86 * 32 + 4, 36 * 32 + 4, ObjetoLetrero, "Enciclopedia Mágica", "Letrero: Cruce de los Tres Caminos. Norte: Cueva Misteriosa. Este: Torre de Agott. Sur: Laberinto Serpentback."));
		agregarObjeto(new BloqueMapa(109 * 32 + 4, 34 * 32 + 4, ObjetoLetrero, "Torre de Agott", "Letrero: Torre de Agott. Prohibido el paso sin autorización de Agott."));
		agregarObjeto(new BloqueMapa(103 * 32 + 4, 14 * 32 + 4, ObjetoLetrero, "Cueva Misteriosa", "Letrero: Cueva Misteriosa. Peligro: Derrumbe. Usa magia ígnea (Pluma Térmica)."));
		agregarObjeto(new BloqueMapa(16 * 32 + 4, 51 * 32 + 4, ObjetoLetrero, "Despacho de Qifrey", "Letrero: Despacho de Qifrey. Maestro del atelier y protector del agua."));
		agregarObjeto(new BloqueMapa(mapa->getPuntoGatoX() + 4, mapa->getPuntoGatoY() + 4, ObjetoGato, "Gato del Atelier", "El gato gris de Richeh."));
		agregarObjeto(new BloqueMapa(mapa->getPuntoTrollX() + 4, mapa->getPuntoTrollY() + 4, ObjetoTroll, "Grieta en la Pared", "Una grieta extraña oculta junto al muro del sendero..."));

		BloqueMapa* myrphon = new BloqueMapa(116 * 32, 60 * 32, ObjetoMyrphon, "Myrphon", "La mascota de Richeh, perdida en el bosque.");
		myrphon->setVisible(false);
		agregarObjeto(myrphon);

		crearArboles();
		crearCajasTorre();
		crearEnemigosNivel1();

		agregarObjetoPozo(new BloqueMapa(mapaPozo->getPuntoSalidaPozoX(), mapaPozo->getPuntoSalidaPozoY(), ObjetoSalidaEscaleras, "Escaleras de salida", "Subir a la torre de Agott."));
		agregarObjetoPozo(new BloqueMapa(13 * 32 + 4, 5 * 32 + 4, ObjetoLetrero, "Curiosidades de Richeh", "DATO CURIOSO DE RICHEH"));
		agregarObjetoPozo(new BloqueMapa(19 * 32 + 4, 5 * 32 + 4, ObjetoLetrero, "Diario del Atelier", "DIARIO DEL ATELIER"));
		richehPozo = new NPC(12 * 32 + 4, 8 * 32, 3, "Richeh", "Aprendiz de Maga");
		richehPozo->setMirada(0);

		myrphonConRicheh = false;
		myrphonCorriendo = false;
		ticksMyrphonCorriendo = 0;
		mensajeTemporal = "";
		ticksMensajeTemporal = 0;
		actualizarObjetivo();
	}

	void reiniciarNivel() {
		iniciarNivel(1);
	}

	void moverJugador(int dx, int dy, bool corriendo = false) {
		if (enIntro || dialogoActivo || transicionPozoActiva || myrphonCorriendo || myrphonHuyendo) return;
		if (dx == 0 && dy == 0) {
			jugador->setEnMovimiento(false);
			return;
		}
		int repeticiones = corriendo ? 2 : 1;
		int paso = corriendo ? ((dx != 0 && dy != 0) ? 4 : 5) : ((dx != 0 && dy != 0) ? 4 : 6);
		for (int rep = 0; rep < repeticiones; rep++) {
			if (dx != 0) {
				int nuevoX = jugador->getX() + dx * paso;
				if (posicionLibre(nuevoX, jugador->getY())) {
					Rectangle rectSig(nuevoX + 3, jugador->getY() + 14, jugador->getAncho() - 6, jugador->getAlto() - 16);
					bool puedeMover = true;
					if (hayCajaEn(rectSig)) puedeMover = intentarEmpujarCaja(dx, 0, rectSig);
					if (puedeMover) jugador->setX(nuevoX);
				}
			}
			if (dy != 0) {
				int nuevoY = jugador->getY() + dy * paso;
				if (posicionLibre(jugador->getX(), nuevoY)) {
					Rectangle rectSig(jugador->getX() + 3, nuevoY + 14, jugador->getAncho() - 6, jugador->getAlto() - 16);
					bool puedeMover = true;
					if (hayCajaEn(rectSig)) puedeMover = intentarEmpujarCaja(0, dy, rectSig);
					if (puedeMover) jugador->setY(nuevoY);
				}
			}
		}
		if (dx < 0) jugador->setDireccion(1);
		else if (dx > 0) jugador->setDireccion(3);
		else if (dy < 0) jugador->setDireccion(2);
		else if (dy > 0) jugador->setDireccion(0);
		jugador->setEnMovimiento(true);
		int freqPaso = corriendo ? 6 : 10;
		if (ticksJuego % freqPaso == 0) GestorAudio::ReproducirSfxPaso();
	}

	void detenerJugador() {
		jugador->detener();
	}

	void interactuar() {
		if (enIntro) {
			enIntro = false;
			ticksTituloNivel = 140;
			return;
		}
		if (dialogoActivo) {
			if (!textoDialogoCompleto()) {
				String^ texto = dialogoTextoManaged;
				if (texto != nullptr) caracteresVisibles = texto->Length;
				return;
			}
			if (!dialogoOpciones.empty()) {
				int indice = dialogoOpcionSeleccionada;
				if (indice < 0 || indice >= (int)dialogoOpciones.size()) indice = 0;
				GestorAudio::ReproducirSfxDialogo();
				procesarOpcionDialogo(indice + 1);
			}
			else cerrarDialogo();
			return;
		}
		if (transicionPozoActiva || myrphonCorriendo || myrphonHuyendo) return;
		actualizarObjetivoCercano();
		if (tipoObjetivo == INTERACCION_QIFREY) {
			habloQifrey = true;
			iniciarDialogo("Qifrey", 1);
			return;
		}
		if (tipoObjetivo == INTERACCION_AGOTT) {
			iniciarDialogoAgott();
			return;
		}
		if (tipoObjetivo == INTERACCION_RICHEH) {
			iniciarDialogoRicheh();
			return;
		}
		if (tipoObjetivo == INTERACCION_SALIDA_POZO) {
			subirPozo();
			return;
		}
		if (tipoObjetivo == INTERACCION_LETRERO_CURIOSIDADES) {
			int estado = 400;
			int azar = rand() % 3;
			if (azar == 1) estado = 402;
			else if (azar == 2) estado = 404;
			iniciarDialogo("Letrero", estado);
			return;
		}
		if (tipoObjetivo == INTERACCION_LETRERO_DIARIO) {
			iniciarDialogo("Letrero", 410);
			return;
		}
		if (tipoObjetivo == INTERACCION_PARED_ROCA) {
			if (tieneItem("Pluma Termica")) {
				quitarItem("Pluma Termica");
				mapa->romperParedRoca();
				paredRocaRota = true;
				GestorAudio::ReproducirSfxAccion();
				mensajeTemporal = "Punta incandescente aplicada: La pared de roca se quebró.";
				ticksMensajeTemporal = 110;
				actualizarObjetivo();
			}
			else {
				mensajeTemporal = "Coco: La pared de roca está agrietada... Necesito una pluma térmica para quebrarla.";
				ticksMensajeTemporal = 95;
			}
			return;
		}
		if (tipoObjetivo == INTERACCION_OBJETO_POZO) {
			bajarPozo();
			return;
		}
		if (tipoObjetivo == INTERACCION_OBJETO) {
			if (indiceObjetivo < 0 || indiceObjetivo >= (int)listaObjetos.size()) return;
			BloqueMapa* objeto = listaObjetos[indiceObjetivo];
			if (objeto->getTipo() == ObjetoGato) {
				gatoAcariciado = true;
				iniciarDialogoSimple("Coco", "Aprendiz de Maga", "Miau... El gato gris de Richeh te observa y ronronea con calma.");
			}
			else if (objeto->getTipo() == ObjetoTroll) {
				solicitaTrollface = true;
			}
			else if (objeto->getTipo() == ObjetoMyrphon) {
				if (misionMyrphonActiva && !myrphonRescatado) solicitaMinijuego = true;
			}
			else if (objeto->getTipo() == ObjetoLetrero) {
				iniciarDialogoSimple("Letrero", "Información", objeto->getDescripcion());
			}
			else if (objeto->getTipo() == ObjetoMontonCajas) {
				recogerItem(objeto);
			}
			else if (objeto->esRecolectable()) {
				recogerItem(objeto);
			}
			return;
		}
	}

	void elegirOpcionDialogo(int indice) {
		aplicarOpcionDialogo(indice);
	}

	void moverSeleccion(int delta) {
		moverSeleccionDialogo(delta);
	}

	bool getDialogoActivo() { return dialogoActivo; }
	bool getEnTransicionPozo() { return transicionPozoActiva; }

	bool consumirSolicitudMinijuego() {
		bool valor = solicitaMinijuego;
		solicitaMinijuego = false;
		return valor;
	}
	bool consumirSolicitudTrollface() {
		bool valor = solicitaTrollface;
		solicitaTrollface = false;
		return valor;
	}
	bool consumirSolicitudFinNivel() {
		bool valor = solicitaFinNivel;
		solicitaFinNivel = false;
		return valor;
	}

	bool consumirSolicitudDerrota() {
		bool valor = solicitaDerrota;
		solicitaDerrota = false;
		return valor;
	}

	void resolverMinijuego(bool exito) {
		if (audioHabilitado) reproducirAudioNivel(1);
		else audio.detener();
		if (exito) {
			vioMyrphon = true;
			vioEnemigos = true;
			myrphonRescatado = true;
			if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(false);
			jugador->setX(124 * 32);
			jugador->setY(60 * 32);
			actualizarObjetivo();
			mensajeTemporal = "¡Atrapaste a Myrphon! Llévaselo de regreso a Richeh en el sótano.";
			ticksMensajeTemporal = 130;
		}
		else {
			jugador->restarVida();
			if (jugador->getVidas() <= 0) {
				solicitaDerrota = true;
			}
			else {
				vioMyrphon = true;
				vioEnemigos = true;
				jugador->setX(124 * 32);
				jugador->setY(60 * 32);
				mensajeTemporal = "Myrphon se escapó... Inténtalo de nuevo en la entrada.";
				ticksMensajeTemporal = 100;
				actualizarObjetivo();
			}
		}
	}

	void marcarTrollDescubierto() {
		trollDescubierto = true;
	}

	void procesarTick(Graphics^ g, int viewW, int viewH, int zoom, bool corriendo = false) {
		if (g == nullptr || viewW <= 0 || viewH <= 0) return;

		if (enIntro) {
			dibujarPantallaIntro(g, viewW, viewH);
			return;
		}

		ticksJuego++;
		if (ticksTituloUbicacion > 0) ticksTituloUbicacion--;
		if (ticksTituloNivel > 0) ticksTituloNivel--;
		if (ticksMensajeTemporal > 0) ticksMensajeTemporal--;
		audio.actualizar();
		jugador->animar(corriendo);
		actualizarTextoAnimado();
		actualizarCorridaMyrphon();
		actualizarLluvia();
		actualizarEnemigosNivel1();

		if (transicionPozoActiva) {
			if (ticksTransicionPozo > 0) ticksTransicionPozo--;
			if (ticksTransicionPozo <= 0) finalizarTransicionPozo();
		}

		if (!enPozo && !dialogoActivo && !transicionPozoActiva && !myrphonCorriendo) {
			int cx = jugador->getX() + jugador->getAncho() / 2;
			int cy = jugador->getY() + jugador->getAlto() / 2;
			int col = cx / Configuracion::TILE_SIZE;
			int fila = cy / Configuracion::TILE_SIZE;

			if (!entroTorre && estaEnTorre()) {
				entroTorre = true;
				habloAgott = true;
				iniciarDialogo("Agott", 200);
			}
			else if (!monologoCruceActivado && col >= 85 && col <= 92 && fila >= 34 && fila <= 39) {
				monologoCruceActivado = true;
				mensajeTemporal = "Coco: ¡Vaya...! Tres caminos se abren ante mí... ¿Hacia dónde debería ir ahora?";
				ticksMensajeTemporal = 140;
			}
			else if (misionMyrphonActiva && !myrphonRescatado && !myrphonHuyendo && !myrphonEscapado && col >= 115 && fila >= 56 && fila <= 65) {
				vioMyrphon = true;
				myrphonHuyendo = true;
				ticksMyrphonHuyendo = 0;
				mensajeTemporal = "¡Myrphon se asusta y corre hacia las sombras de Serpentback!";
				ticksMensajeTemporal = 90;
			}
			else if (misionMyrphonActiva && !myrphonRescatado && myrphonEscapado && col >= 131 && fila >= 58 && fila <= 62) {
				vioMyrphon = true;
				vioEnemigos = true;
				solicitaMinijuego = true;
			}
		}
		if (myrphonObjeto != nullptr && myrphonObjeto->getVisible() && distanciaAlCentro(myrphonObjeto) <= 420) {
			vioMyrphon = true;
		}

		int mundoAncho = viewW / zoom;
		int mundoAlto = viewH / zoom;
		camX = jugador->getX() + jugador->getAncho() / 2 - mundoAncho / 2;
		camY = jugador->getY() + jugador->getAlto() / 2 - mundoAlto / 2;
		int maxCamX = mapaActual->getAnchoTotalPixels() - mundoAncho;
		int maxCamY = mapaActual->getAltoTotalPixels() - mundoAlto;
		if (maxCamX < 0) camX = maxCamX / 2;
		else if (camX < 0) camX = 0;
		else if (camX > maxCamX) camX = maxCamX;
		if (maxCamY < 0) camY = maxCamY / 2;
		else if (camY < 0) camY = 0;
		else if (camY > maxCamY) camY = maxCamY;

		int centroX = jugador->getX() + jugador->getAncho() / 2;
		int centroY = jugador->getY() + jugador->getAlto() / 2;
		String^ ubicacion = mapaActual->getNombreUbicacion(centroX, centroY);
		String^ anterior = ultimaUbicacion;
		if (anterior == nullptr || !ubicacion->Equals(anterior)) {
			ultimaUbicacion = ubicacion;
			ticksTituloUbicacion = 70;
		}

		mapaActual->dibujar(g, camX, camY, viewW, viewH, zoom, ticksJuego);

		if (enPozo) {
			for (size_t i = 0; i < objetosPozo.size(); i++) objetosPozo[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			if (richehPozo != nullptr) richehPozo->dibujar(g, bmpRicheh, camX, camY, zoom, ticksJuego);
			int rx = (richehPozo != nullptr) ? richehPozo->getX() + 26 : 16 * 32;
			int ry = (richehPozo != nullptr) ? richehPozo->getY() + 2 : 8 * 32;
			if (myrphonCorriendo) {
				float t = ticksMyrphonCorriendo / 50.0f;
				int sx = mapaPozo->getPuntoInicioPozoX();
				int sy = mapaPozo->getPuntoInicioPozoY();
				int mx = (int)(sx + (rx - sx) * t);
				int my = (int)(sy + (ry - sy) * t);
				int dxP = rx - sx;
				int dyP = ry - sy;
				int dirP = (abs(dxP) >= abs(dyP)) ? ((dxP >= 0) ? 3 : 1) : ((dyP >= 0) ? 0 : 2);
				DibujarMyrphonSpritesheetComun(g, bmpMyrphon, mx, my, dirP, true, ticksJuego, camX, camY, zoom, 32, 32);
			}
			else if (myrphonConRicheh) {
				DibujarMyrphonSpritesheetComun(g, bmpMyrphon, rx, ry, 0, false, ticksJuego, camX, camY, zoom, 32, 32);
			}
			else if (myrphonRescatado) {
				int mx = jugador->getX() + 22;
				int my = jugador->getY() + 8;
				DibujarMyrphonSpritesheetComun(g, bmpMyrphon, mx, my, jugador->getDireccion(), jugador->getEnMovimiento(), ticksJuego, camX, camY, zoom, 30, 30);
			}
			if (corriendo && jugador->getEnMovimiento()) {
				DibujarPolvoCorrerComun(g, jugador->getX(), jugador->getY(), jugador->getAncho(), jugador->getAlto(), jugador->getDireccion(), ticksJuego, camX, camY, zoom);
			}
			jugador->dibujar(g, bmpCoco, camX, camY, zoom, ticksJuego);
		}
		else {
			int baseJugador = jugador->getBaseY();
			for (size_t i = 0; i < listaArboles.size(); i++) {
				if (listaArboles[i]->getBaseY() <= baseJugador) listaArboles[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			}
			for (size_t i = 0; i < listaObjetos.size(); i++) {
				if (listaObjetos[i]->getTipo() == ObjetoMyrphon && listaObjetos[i]->getVisible()) {
					DibujarMyrphonSpritesheetComun(g, bmpMyrphon, listaObjetos[i]->getX(), listaObjetos[i]->getY(), 3, myrphonHuyendo, ticksJuego, camX, camY, zoom, 32, 32);
					continue;
				}
				listaObjetos[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			}
			for (size_t i = 0; i < listaCajas.size(); i++) listaCajas[i]->dibujar(g, camX, camY, zoom);
			for (size_t i = 0; i < listaNPCs.size(); i++) {
				if (listaNPCs[i]->getIdPersonaje() == 1) {
					listaNPCs[i]->dibujarConGrid(g, bmpQifrey, camX, camY, zoom, ticksJuego, 4, 4, true);
				}
				else {
					listaNPCs[i]->dibujar(g, bmpAgott, camX, camY, zoom, ticksJuego);
				}
			}
			for (size_t i = 0; i < listaEnemigos.size(); i++) {
				listaEnemigos[i]->dibujarEnemigo(g, bmpEnemigo, camX, camY, zoom);
			}
			for (size_t i = 0; i < proyectilesEnemigos.size(); i++) {
				proyectilesEnemigos[i].dibujar(g, bmpDisparo, camX, camY, zoom, 24);
			}
			if (myrphonRescatado && !myrphonConRicheh) {
				int mx = jugador->getX() + 22;
				int my = jugador->getY() + 8;
				DibujarMyrphonSpritesheetComun(g, bmpMyrphon, mx, my, jugador->getDireccion(), jugador->getEnMovimiento(), ticksJuego, camX, camY, zoom, 30, 30);
			}
			if (corriendo && jugador->getEnMovimiento()) {
				DibujarPolvoCorrerComun(g, jugador->getX(), jugador->getY(), jugador->getAncho(), jugador->getAlto(), jugador->getDireccion(), ticksJuego, camX, camY, zoom);
			}
			if (invulnerableTicks == 0 || (invulnerableTicks % 3 != 0)) {
				jugador->dibujar(g, bmpCoco, camX, camY, zoom, ticksJuego);
			}
			for (size_t i = 0; i < listaArboles.size(); i++) {
				if (listaArboles[i]->getBaseY() > baseJugador) listaArboles[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			}
		}

		if (!enPozo && !transicionPozoActiva) {
			dibujarSombraZonas(g, viewW, viewH, camX, camY, zoom);
			dibujarLluvia(g, viewW, viewH, camX, camY, zoom);
		}
		actualizarObjetivoCercano();
		dibujarHUD(g, viewW, viewH, ubicacion);
		if (dialogoActivo) dibujarCuadroDialogo(g, viewW, viewH);
		if (transicionPozoActiva) dibujarTransicionPozo(g, viewW, viewH);
	}

	Rectangle rectMundoAPantalla(Rectangle mundo, int camX, int camY, int zoom) {
		return Rectangle((mundo.X - camX) * zoom, (mundo.Y - camY) * zoom, mundo.Width * zoom, mundo.Height * zoom);
	}

	void dibujarSombraZonas(Graphics^ g, int viewW, int viewH, int camX, int camY, int zoom) {
		if (enPozo) return;
		int cx = jugador->getX() + jugador->getAncho() / 2;
		int cy = jugador->getY() + jugador->getAlto() / 2;
		int cuarto = mapa->determinarCuarto(cx, cy);

		Rectangle pantalla(0, 0, viewW, viewH);
		Rectangle chozaMuros(10 * 32, 6 * 32, 19 * 32, 13 * 32);
		Rectangle chozaInterior(11 * 32, 7 * 32, 17 * 32, 11 * 32);
		Rectangle despachoMuros(10 * 32, 52 * 32, 19 * 32, 15 * 32);
		Rectangle despachoInterior(11 * 32, 53 * 32, 17 * 32, 13 * 32);
		Rectangle torreMuros(112 * 32, 28 * 32, 23 * 32, 18 * 32);
		Rectangle torreInterior(113 * 32, 29 * 32, 21 * 32, 16 * 32);
		Rectangle cuevaMuros(106 * 32, 10 * 32, 21 * 32, 12 * 32);
		Rectangle cuevaInterior(108 * 32, 11 * 32, 18 * 32, 10 * 32);

		if (cuarto == 0) {
			SolidBrush^ sombraTecho = gcnew SolidBrush(Color::FromArgb(195, 4, 7, 12));
			g->FillRectangle(sombraTecho, rectMundoAPantalla(chozaInterior, camX, camY, zoom));
			g->FillRectangle(sombraTecho, rectMundoAPantalla(despachoInterior, camX, camY, zoom));
			g->FillRectangle(sombraTecho, rectMundoAPantalla(torreInterior, camX, camY, zoom));
			if (!paredRocaRota) {
				g->FillRectangle(sombraTecho, rectMundoAPantalla(cuevaInterior, camX, camY, zoom));
			}
			delete sombraTecho;
			return;
		}

		Rectangle rectVisible;
		if (cuarto == 1) rectVisible = chozaMuros;
		else if (cuarto == 2) rectVisible = despachoMuros;
		else if (cuarto == 3) rectVisible = torreMuros;
		else rectVisible = cuevaMuros;

		Region^ oscuro = gcnew Region(pantalla);
		Region^ zonaClara = gcnew Region(rectMundoAPantalla(rectVisible, camX, camY, zoom));
		oscuro->Exclude(zonaClara);
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(195, 2, 4, 8));
		g->FillRegion(sombra, oscuro);
		delete sombra;
		delete zonaClara;
		delete oscuro;
	}

	void dibujarLluvia(Graphics^ g, int viewW, int viewH, int camX, int camY, int zoom) {
		Pen^ pincelLluvia = gcnew Pen(Color::FromArgb(120, 155, 200, 245), 1.4f);
		Pen^ pincelSalpicadura = gcnew Pen(Color::FromArgb(115, 190, 230, 255), 1.0f);
		int ts = Configuracion::TILE_SIZE;
		for (size_t i = 0; i < gotasLluvia.size(); i++) {
			float sx = gotasLluvia[i].x;
			float sy = gotasLluvia[i].y;
			if (sx < 0 || sx >= viewW || sy < 0 || sy >= viewH) continue;
			int col = ((int)(sx / zoom) + camX) / ts;
			int fila = ((int)(sy / zoom) + camY) / ts;
			if (!mapa->esExteriorParaLluvia(col, fila)) continue;
			g->DrawLine(pincelLluvia, sx, sy, sx - 3.0f, sy + (float)gotasLluvia[i].largo);
			if (((ticksJuego + (int)i) % 19 == 0) && mapa->getCaracter(col, fila) == '~') {
				g->DrawEllipse(pincelSalpicadura, sx - 4.0f, sy + 6.0f, 6.0f, 3.0f);
			}
		}
		delete pincelLluvia;
		delete pincelSalpicadura;
	}

	void dibujarPantallaIntro(Graphics^ g, int viewW, int viewH) {
		String^ cuerpo =
			"Tras el accidente que petrificó a su madre, Coco descubre\n"
			"que la magia nace del trazo con tinta sobre pergamino.\n\n"
			"Recorre el atelier de Qifrey, ayuda a tus compañeros y reúne:\n"
			"Tinta mágica, Grimorio de Trazos y Fibra del Árbol de Plata.";
		DibujarIntroActoComun(g, viewW, viewH, "ACTO I", "El Atelier de Qifrey", cuerpo, "Presiona Espacio para continuar");
	}

	void dibujarCorazon(Graphics^ g, int x, int y, Color color) {
		SolidBrush^ pincel = gcnew SolidBrush(color);
		g->FillEllipse(pincel, x, y, 8, 8);
		g->FillEllipse(pincel, x + 6, y, 8, 8);
		g->FillPolygon(pincel, gcnew array<Point>{ Point(x, y + 6), Point(x + 14, y + 6), Point(x + 7, y + 15) });
		delete pincel;
	}

	void dibujarHUD(Graphics^ g, int viewW, int viewH, String^ ubicacion) {
		System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Regular);
		System::Drawing::Font^ fObjetivo = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Bold);
		System::Drawing::Font^ fGuiaSub = gcnew System::Drawing::Font("Segoe UI", 8.5f, FontStyle::Regular);
		System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 16.0f, FontStyle::Bold);
		System::Drawing::Font^ fSub = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Regular);
		System::Drawing::Font^ fPrompt = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);

		std::string guiaTitulo;
		std::string guiaDetalle;
		int pasoNum = 1;
		int totalPasos = 8;
		int targetX = 0;
		int targetY = 0;
		bool mostrarFlecha = true;
		obtenerGuiaPasoActual(guiaTitulo, guiaDetalle, pasoNum, totalPasos, targetX, targetY, mostrarFlecha);

		if (guiaActiva && mostrarFlecha && !dialogoActivo && !transicionPozoActiva && !misionCompletada) {
			int jSX = (jugador->getX() + jugador->getAncho() / 2 - camX) * Configuracion::ZOOM;
			int jSY = (jugador->getY() + jugador->getAlto() / 2 - camY) * Configuracion::ZOOM;
			int tSX = (targetX - camX) * Configuracion::ZOOM;
			int tSY = (targetY - camY) * Configuracion::ZOOM;
			DibujarFlechaGuiaComun(g, jSX, jSY, tSX, tSY, viewW, viewH, ticksJuego);
		}

		SolidBrush^ panel = gcnew SolidBrush(Color::FromArgb(228, 10, 16, 24));
		g->FillRectangle(panel, 16, 16, 360, 42);
		for (int i = 0; i < 3; i++) {
			Color colorCorazon = (i < jugador->getVidas()) ? Color::FromArgb(220, 210, 58, 76) : Color::FromArgb(120, 70, 58, 62);
			dibujarCorazon(g, 28 + i * 20, 29, colorCorazon);
		}
		g->DrawString("INV " + inventario->getCantidad().ToString() + "/" + inventario->getCapacidad().ToString() + "  |  " + ubicacion, fTexto, Brushes::Gainsboro, PointF(100.0f, 29.0f));

		if (guiaActiva) {
			g->FillRectangle(panel, 16, 66, 292, 82);
			SolidBrush^ barra = gcnew SolidBrush(Color::FromArgb(220, 214, 198, 166));
			g->FillRectangle(barra, 16, 66, 3, 82);
			delete barra;
			g->DrawString("MISIÓN ACTUAL (" + pasoNum.ToString() + "/" + totalPasos.ToString() + ")", fGuiaSub, Brushes::Silver, PointF(28.0f, 72.0f));
			g->DrawString(ToManaged(guiaTitulo), fObjetivo, Brushes::Khaki, RectangleF(28.0f, 89.0f, 270.0f, 20.0f));
			String^ detalleFinal = ToManaged(guiaDetalle);
			if (mostrarFlecha) {
				int distM = (int)(std::sqrt((double)((targetX - jugador->getX()) * (targetX - jugador->getX()) + (targetY - jugador->getY()) * (targetY - jugador->getY()))) / 32.0);
				detalleFinal += " (" + distM.ToString() + " m)";
			}
			g->DrawString(detalleFinal, fGuiaSub, Brushes::LightGray, RectangleF(28.0f, 110.0f, 270.0f, 34.0f));
		}
		delete panel;

		std::string textoPrompt = "";
		if (ticksMensajeTemporal > 0 && !mensajeTemporal.empty()) textoPrompt = mensajeTemporal;
		else if (!promptInteraccion.empty()) textoPrompt = promptInteraccion;
		if (!textoPrompt.empty() && !dialogoActivo && !transicionPozoActiva) {
			String^ tp = ToManaged(textoPrompt);
			SizeF tam = g->MeasureString(tp, fPrompt);
			int ancho = (int)tam.Width + 36;
			int x = (viewW - ancho) / 2;
			int y = viewH - 54;
			SolidBrush^ fondoPrompt = gcnew SolidBrush(Color::FromArgb(185, 8, 12, 18));
			g->FillRectangle(fondoPrompt, x, y, ancho, 32);
			delete fondoPrompt;
			g->DrawString(tp, fPrompt, Brushes::WhiteSmoke, PointF((float)(x + 18), (float)(y + 6)));
		}

		if (ticksTituloNivel > 0) {
			int alpha = Math::Min(230, ticksTituloNivel * 4);
			SolidBrush^ tituloColor = gcnew SolidBrush(Color::FromArgb(alpha, 235, 200, 158));
			SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(alpha, 200, 158, 78));
			String^ t1 = "Nivel 1";
			String^ t2 = "El Atelier de Qifrey";
			SizeF s1 = g->MeasureString(t1, fTitulo);
			SizeF s2 = g->MeasureString(t2, fSub);
			float cx = viewW / 2.0f;
			g->DrawString(t1, fTitulo, tituloColor, PointF(cx - s1.Width / 2, 86.0f));
			g->FillRectangle(linea, (int)(cx - s2.Width / 2), 118, (int)s2.Width, 1);
			g->DrawString(t2, fSub, Brushes::Gainsboro, PointF(cx - s2.Width / 2, 126.0f));
			delete tituloColor;
			delete linea;
		}
		else if (ticksTituloUbicacion > 0) {
			int alpha = Math::Min(210, ticksTituloUbicacion * 4);
			SolidBrush^ textoColor = gcnew SolidBrush(Color::FromArgb(alpha, 230, 236, 244));
			SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(alpha, 200, 158, 78));
			System::Drawing::Font^ fUbi = gcnew System::Drawing::Font("Segoe UI", 13.0f, FontStyle::Bold);
			SizeF tam = g->MeasureString(ubicacion, fUbi);
			float cx = viewW / 2.0f;
			g->DrawString(ubicacion, fUbi, textoColor, PointF(cx - tam.Width / 2, 86.0f));
			g->FillRectangle(linea, (int)(cx - tam.Width / 2), 110, (int)tam.Width, 1);
			delete textoColor;
			delete linea;
			delete fUbi;
		}

		delete fTexto;
		delete fObjetivo;
		delete fGuiaSub;
		delete fTitulo;
		delete fSub;
		delete fPrompt;
	}

	void dibujarCuadroDialogo(Graphics^ g, int viewW, int viewH) {
		if (!dialogoActivo) return;
		bool conOpciones = textoDialogoCompleto() && !dialogoOpciones.empty();
		int x = 44;
		int ancho = viewW - 88;
		int anchoTexto = ancho - 44;

		System::Drawing::Font^ fNombre = gcnew System::Drawing::Font("Segoe UI", 12.0f, FontStyle::Bold);
		System::Drawing::Font^ fConfianza = gcnew System::Drawing::Font("Segoe UI", 9.5f, FontStyle::Regular);
		System::Drawing::Font^ fTexto = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Regular);
		System::Drawing::Font^ fOpcion = gcnew System::Drawing::Font("Segoe UI", 10.5f, FontStyle::Regular);
		System::Drawing::Font^ fAyuda = gcnew System::Drawing::Font("Segoe UI", 8.5f, FontStyle::Regular);

		String^ textoCompleto = dialogoTextoManaged;
		if (textoCompleto == nullptr) textoCompleto = "";
		int visibles = caracteresVisibles;
		if (visibles > textoCompleto->Length) visibles = textoCompleto->Length;
		String^ textoVisible = textoCompleto->Substring(0, visibles);
		array<Char>^ separador = gcnew array<Char>{ '\n' };
		array<String^>^ lineasCompletas = textoCompleto->Split(separador);
		array<String^>^ lineasVisibles = textoVisible->Split(separador);

		int altoTexto = 0;
		for each (String^ linea in lineasCompletas) {
			SizeF medida = g->MeasureString(linea, fTexto, anchoTexto);
			int h = (int)Math::Ceiling(medida.Height);
			int altoLinea = (int)Math::Ceiling(fTexto->GetHeight(g));
			if (h < altoLinea) h = altoLinea;
			altoTexto += h + 5;
		}
		if (altoTexto < 24) altoTexto = 24;
		int altoOpciones = (int)dialogoOpciones.size() * 30;
		int alto = 30 + altoTexto + 10 + altoOpciones + 34;
		if (alto < 170) alto = 170;
		if (alto > viewH - 28) alto = viewH - 28;
		int y = viewH - alto - 14;

		SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(240, 8, 12, 18));
		g->FillRectangle(fondo, x, y, ancho, alto);
		delete fondo;
		SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(120, 200, 158, 78));
		g->FillRectangle(linea, x, y, ancho, 1);
		g->FillRectangle(linea, x, y + 28 + altoTexto + 4, ancho, 1);
		delete linea;

		String^ nombre = ToManaged(dialogoHablante);
		g->DrawString(nombre, fNombre, Brushes::Gold, PointF((float)(x + 22), (float)(y + 10)));
		if (dialogoHablante != "Letrero") {
			String^ conf = ToManaged(textoConfianza(dialogoConfianza));
			SizeF tamConf = g->MeasureString(conf, fConfianza);
			g->DrawString(conf, fConfianza, Brushes::Silver, PointF((float)(x + ancho - tamConf.Width - 22), (float)(y + 13)));
		}

		float yActual = (float)(y + 34);
		for each (String^ lineaTexto in lineasVisibles) {
			SizeF medida = g->MeasureString(lineaTexto, fTexto, anchoTexto);
			int h = (int)Math::Ceiling(medida.Height);
			int altoLinea = (int)Math::Ceiling(fTexto->GetHeight(g));
			if (h < altoLinea) h = altoLinea;
			g->DrawString(lineaTexto, fTexto, Brushes::WhiteSmoke, RectangleF((float)(x + 22), yActual, (float)anchoTexto, (float)h + 2.0f));
			yActual += h + 5;
			if (yActual > y + 28 + altoTexto + 2) break;
		}

		if (conOpciones) {
			int yOpcion = y + 38 + altoTexto;
			for (size_t i = 0; i < dialogoOpciones.size() && i < 4; i++) {
				String^ lineaOpcion = ToManaged(dialogoOpciones[i]);
				bool seleccion = (i == dialogoOpcionSeleccionada);
				Brush^ pincel = seleccion ? Brushes::Gold : Brushes::Gainsboro;
				if (seleccion) g->DrawString("›", fOpcion, Brushes::Gold, PointF((float)(x + 18), (float)yOpcion));
				g->DrawString(lineaOpcion, fOpcion, pincel, PointF((float)(x + 34), (float)yOpcion));
				yOpcion += 30;
			}
			g->DrawString("[1-4] Elegir   ·   [Espacio] Confirmar", fAyuda, Brushes::Gray, PointF((float)(x + 22), (float)(y + alto - 22)));
		}
		else if (textoDialogoCompleto()) {
			String^ pie = "Presiona [Espacio] o [Z] para continuar...";
			SizeF tamPie = g->MeasureString(pie, fAyuda);
			g->DrawString(pie, fAyuda, Brushes::LightGoldenrodYellow, PointF((float)(x + ancho - tamPie.Width - 22), (float)(y + alto - 22)));
		}

		delete fNombre;
		delete fConfianza;
		delete fTexto;
		delete fOpcion;
		delete fAyuda;
	}

	void dibujarTransicionPozo(Graphics^ g, int viewW, int viewH) {
		int pxW = 240;
		int pxH = 150;
		Bitmap^ lienzo = gcnew Bitmap(pxW, pxH, PixelFormat::Format32bppArgb);
		Graphics^ pg = Graphics::FromImage(lienzo);
		pg->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;

		SolidBrush^ oscuroFondo = gcnew SolidBrush(Color::FromArgb(255, 8, 10, 15));
		SolidBrush^ sombraShaft = gcnew SolidBrush(Color::FromArgb(255, 13, 16, 23));
		SolidBrush^ luzPasoA = gcnew SolidBrush(Color::FromArgb(255, 34, 30, 28));
		SolidBrush^ luzPasoB = gcnew SolidBrush(Color::FromArgb(255, 76, 62, 42));
		SolidBrush^ luzPasoC = gcnew SolidBrush(Color::FromArgb(255, 166, 128, 72));
		SolidBrush^ muroBase = gcnew SolidBrush(Color::FromArgb(255, 22, 26, 35));
		SolidBrush^ ladrilloA = gcnew SolidBrush(Color::FromArgb(255, 34, 40, 52));
		SolidBrush^ ladrilloB = gcnew SolidBrush(Color::FromArgb(255, 44, 51, 66));
		SolidBrush^ junta = gcnew SolidBrush(Color::FromArgb(255, 12, 15, 20));
		SolidBrush^ bordeMadera = gcnew SolidBrush(Color::FromArgb(255, 52, 34, 18));
		SolidBrush^ madera = gcnew SolidBrush(Color::FromArgb(255, 112, 74, 40));
		SolidBrush^ maderaLuz = gcnew SolidBrush(Color::FromArgb(255, 168, 118, 66));

		pg->FillRectangle(oscuroFondo, 0, 0, pxW, pxH);
		pg->FillRectangle(sombraShaft, 74, 0, 92, pxH);

		if (transicionBajandoPozo) {
			pg->FillRectangle(luzPasoA, 82, 0, 76, 48);
			pg->FillRectangle(luzPasoB, 92, 0, 56, 28);
			pg->FillRectangle(luzPasoC, 100, 0, 40, 12);
		}
		else {
			pg->FillRectangle(luzPasoA, 78, 0, 84, 72);
			pg->FillRectangle(luzPasoB, 86, 0, 68, 42);
			pg->FillRectangle(luzPasoC, 96, 0, 48, 18);
		}

		pg->FillRectangle(muroBase, 0, 0, 76, pxH);
		pg->FillRectangle(muroBase, 164, 0, 76, pxH);
		for (int fila = 0; fila * 10 < pxH; fila++) {
			int by = fila * 10;
			int offset = (fila % 2 == 0) ? 0 : 10;
			for (int bx = -offset; bx < 74; bx += 20) {
				int drawX = Math::Max(0, bx);
				int drawW = Math::Min(74 - drawX, (bx + 19) - drawX);
				if (drawW > 1) {
					pg->FillRectangle(((fila + bx / 20) % 2 == 0) ? ladrilloA : ladrilloB, drawX, by + 1, drawW, 8);
				}
				pg->FillRectangle(junta, 166 + drawX, by + 1, drawW, 8);
				if (drawW > 1) {
					pg->FillRectangle(((fila + bx / 20) % 2 == 0) ? ladrilloB : ladrilloA, 166 + drawX, by + 1, drawW, 8);
				}
			}
			pg->FillRectangle(junta, 0, by + 9, 76, 1);
			pg->FillRectangle(junta, 164, by + 9, 76, 1);
		}
		pg->FillRectangle(junta, 74, 0, 2, pxH);
		pg->FillRectangle(junta, 164, 0, 2, pxH);

		if (transicionBajandoPozo) {
			pg->FillRectangle(ladrilloB, 70, 0, 100, 8);
			pg->FillRectangle(ladrilloA, 74, 8, 92, 4);
			pg->FillRectangle(junta, 70, 12, 100, 1);
		}
		else {
			pg->FillRectangle(bordeMadera, 72, 0, 96, 6);
			pg->FillRectangle(madera, 76, 1, 88, 4);
			pg->FillRectangle(ladrilloA, 74, 138, 92, 12);
			pg->FillRectangle(ladrilloB, 74, 138, 92, 3);
		}

		pg->FillRectangle(bordeMadera, 104, 0, 4, pxH);
		pg->FillRectangle(bordeMadera, 132, 0, 4, pxH);
		pg->FillRectangle(madera, 105, 0, 2, pxH);
		pg->FillRectangle(madera, 133, 0, 2, pxH);
		for (int ry = 8; ry < pxH; ry += 10) {
			pg->FillRectangle(bordeMadera, 107, ry + 1, 26, 2);
			pg->FillRectangle(maderaLuz, 107, ry, 26, 2);
		}

		float t = 1.0f - (ticksTransicionPozo / 60.0f);
		int frame = (ticksJuego / 3) % Configuracion::cantZeldaWalking;
		int cocoX = pxW / 2 - 12;
		int cocoY = transicionBajandoPozo ? (int)(16 + t * 94) : (int)(108 - t * 94);
		int pasoBob = ((ticksJuego / 3) % 2 == 0) ? 0 : -1;
		pg->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		pg->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		pg->DrawImage(bmpCoco, Rectangle(cocoX, cocoY + pasoBob, 24, 32), frame * 24, 6 * 32, 24, 32, GraphicsUnit::Pixel);

		if (myrphonRescatado && !dioVaraRicheh) {
			Bitmap^ bm = bmpMyrphon;
			if (bm != nullptr && bm->Width >= 6 && bm->Height >= 4) {
				pg->DrawImage(bm, Rectangle(cocoX + 16, cocoY + 8, 16, 16), 0, 0, bm->Width / 6, bm->Height / 4, GraphicsUnit::Pixel);
			}
		}

		delete oscuroFondo;
		delete sombraShaft;
		delete luzPasoA;
		delete luzPasoB;
		delete luzPasoC;
		delete muroBase;
		delete ladrilloA;
		delete ladrilloB;
		delete junta;
		delete bordeMadera;
		delete madera;
		delete maderaLuz;
		delete pg;

		g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
		g->PixelOffsetMode = System::Drawing::Drawing2D::PixelOffsetMode::Half;
		g->DrawImage(lienzo, Rectangle(0, 0, viewW, viewH), 0, 0, pxW, pxH, GraphicsUnit::Pixel);
		delete lienzo;

		System::Drawing::Font^ f = gcnew System::Drawing::Font("Segoe UI", 12.0f, FontStyle::Bold);
		String^ txt = transicionBajandoPozo ? "Bajando al sótano de Richeh..." : "Subiendo hacia la Torre de Agott...";
		SizeF tam = g->MeasureString(txt, f);
		SolidBrush^ cajaTexto = gcnew SolidBrush(Color::FromArgb(200, 10, 14, 20));
		int bx = (int)(viewW / 2.0f - tam.Width / 2.0f) - 16;
		g->FillRectangle(cajaTexto, bx, viewH - 58, (int)tam.Width + 32, 34);
		g->DrawString(txt, f, Brushes::Gainsboro, PointF(viewW / 2.0f - tam.Width / 2.0f, (float)(viewH - 51)));
		delete cajaTexto;
		delete f;
	}

	void obtenerPersonajes(std::vector<PersonajeInfo>& personajes) {
		personajes.clear();
		personajes.push_back(PersonajeInfo{ "Coco", "Aprendiz de Maga", "Protagonista del Nivel 1. Descubrió la verdad sobre el dibujo mágico.", "Protagonista", "Coco" });
		if (habloQifrey) {
			personajes.push_back(PersonajeInfo{ "Qifrey", "Maestro Hechicero", "Tutor y protector del atelier. Especialista en trazos y magia de agua.", textoConfianza(confianzaQifrey), "Qifrey" });
		}
		if (habloAgott) {
			personajes.push_back(PersonajeInfo{ "Agott", "Aprendiz de Maga", "Compañera disciplinada y exigente. Aspira a la perfección del trazo.", textoConfianza(confianzaAgott), "Agott" });
		}
		if (habloRicheh) {
			personajes.push_back(PersonajeInfo{ "Richeh", "Aprendiz de Maga", "Amiga reflexiva de Coco. Gran conocedora de runas antiguas.", textoConfianza(confianzaRicheh), "Richeh" });
		}
		if (vioMyrphon || misionMyrphonActiva || myrphonHuyendo || myrphonEscapado || myrphonRescatado || myrphonConRicheh) {
			personajes.push_back(PersonajeInfo{ "Myrphon", "Mascota de Richeh", "Pincelero fiel de Richeh. Se perdió entre las sombras del bosque de Serpentback.", myrphonRescatado ? "Amigos" : "Perdido", "Myrphon" });
		}
		if (vioEnemigos) {
			personajes.push_back(PersonajeInfo{ "Sombrero de Ala Ancha", "Mago de la Magia Prohibida", "Hechicero misterioso que patrulla el bosque de Serpentback y lanza disparos de energía oscura.", "Hostil", "Sombrero" });
		}
	}

	void pausarAudio() { audio.pausar(); }
	void reproducirAudioNivel(int numNivel = 1) {
		if (audioHabilitado) audio.reproducirNivel(numNivel);
	}
	void reanudarAudioNivel(int numNivel = 1) {
		if (!audioHabilitado) return;
		audio.reanudar();
		if (!audio.estaCargado()) audio.reproducirNivel(numNivel);
	}
	void reproducirAudioMinijuego() {
		if (audioHabilitado) audio.reproducirMinijuego();
	}
	void actualizarAudio() {
		if (audioHabilitado) audio.actualizar();
	}
	void detenerAudio() { audio.detener(); }
	bool estaAudioHabilitado() { return audioHabilitado; }
	void alternarAudio(int numNivel = 1) {
		audioHabilitado = !audioHabilitado;
		GestorAudio::SetSfxHabilitado(audioHabilitado);
		if (audioHabilitado) {
			audio.reproducirNivel(numNivel);
			mensajeTemporal = "Música activada (M para silenciar)";
		}
		else {
			audio.detener();
			mensajeTemporal = "Música desactivada (M para activar)";
		}
		ticksMensajeTemporal = 90;
	}

	String^ getResumenPartida() {
		String^ estado = misionCompletada ? "NIVEL COMPLETADO" : "EN CURSO";
		String^ resumen = "Nivel 1 · El Atelier de Qifrey\n";
		resumen += "Estado: " + estado + "\n";
		resumen += "Vidas: " + jugador->getVidas().ToString() + "\n";
		resumen += "Inventario: " + inventario->getCantidad().ToString() + "/" + inventario->getCapacidad().ToString() + "\n";
		resumen += "Objetivo: " + ToManaged(objetivoActual) + "\n";
		resumen += "Tiempo: " + getTiempoSegundos().ToString() + " s";
		return resumen;
	}

	bool asegurarPartidaRegenerativa() {
		std::ifstream prueba(ObtenerRutaGuardadoStdComun("partida_guardada.txt"));
		bool valido = false;
		if (prueba.is_open()) {
			bool tieneNivel = false;
			bool tieneVidas = false;
			std::string linea;
			while (std::getline(prueba, linea)) {
				if (linea.rfind("nivel=", 0) == 0) tieneNivel = true;
				else if (linea.rfind("vidas=", 0) == 0) tieneVidas = true;
			}
			prueba.close();
			valido = tieneNivel && tieneVidas;
		}
		if (valido) return true;
		return guardarPartida();
	}

	bool guardarPartida() {
		std::ofstream archivo(ObtenerRutaGuardadoStdComun("partida_guardada.txt"), std::ios::out | std::ios::trunc);
		if (!archivo.is_open()) return false;
		archivo << "nivel=1\n";
		archivo << "vidas=" << jugador->getVidas() << "\n";
		archivo << "x=" << jugador->getX() << "\n";
		archivo << "y=" << jugador->getY() << "\n";
		archivo << "posAtelierX=" << posAtelierX << "\n";
		archivo << "posAtelierY=" << posAtelierY << "\n";
		archivo << "enPozo=" << (enPozo ? 1 : 0) << "\n";
		archivo << "habloQifrey=" << (habloQifrey ? 1 : 0) << "\n";
		archivo << "habloAgott=" << (habloAgott ? 1 : 0) << "\n";
		archivo << "habloRicheh=" << (habloRicheh ? 1 : 0) << "\n";
		archivo << "pidioMantoQifrey=" << (pidioMantoQifrey ? 1 : 0) << "\n";
		archivo << "confianzaQifrey=" << confianzaQifrey << "\n";
		archivo << "confianzaAgott=" << confianzaAgott << "\n";
		archivo << "confianzaRicheh=" << confianzaRicheh << "\n";
		archivo << "respuestaInicialAgott=" << respuestaInicialAgott << "\n";
		archivo << "richehEnojada=" << (richehEnojada ? 1 : 0) << "\n";
		archivo << "misionMyrphonActiva=" << (misionMyrphonActiva ? 1 : 0) << "\n";
		archivo << "myrphonRescatado=" << (myrphonRescatado ? 1 : 0) << "\n";
		archivo << "myrphonEscapado=" << (myrphonEscapado ? 1 : 0) << "\n";
		archivo << "vioMyrphon=" << (vioMyrphon ? 1 : 0) << "\n";
		archivo << "vioEnemigos=" << (vioEnemigos ? 1 : 0) << "\n";
		archivo << "dioVaraRicheh=" << (dioVaraRicheh ? 1 : 0) << "\n";
		archivo << "agottSabeMyrphon=" << (agottSabeMyrphon ? 1 : 0) << "\n";
		archivo << "capaCrafteada=" << (capaCrafteada ? 1 : 0) << "\n";
		archivo << "tintaMagicaEntregada=" << (tintaMagicaEntregada ? 1 : 0) << "\n";
		archivo << "tintaSecundariaEntregada=" << (tintaSecundariaEntregada ? 1 : 0) << "\n";
		archivo << "grimorioDescubierto=" << (grimorioDescubierto ? 1 : 0) << "\n";
		archivo << "libroEncontrado=" << (libroEncontrado ? 1 : 0) << "\n";
		archivo << "pozoDescubierto=" << (pozoDescubierto ? 1 : 0) << "\n";
		archivo << "paredRocaRota=" << (paredRocaRota ? 1 : 0) << "\n";
		archivo << "entroTorre=" << (entroTorre ? 1 : 0) << "\n";
		archivo << "monologoCruceActivado=" << (monologoCruceActivado ? 1 : 0) << "\n";
		archivo << "gatoAcariciado=" << (gatoAcariciado ? 1 : 0) << "\n";
		archivo << "trollDescubierto=" << (trollDescubierto ? 1 : 0) << "\n";
		archivo << "misionCompletada=" << (misionCompletada ? 1 : 0) << "\n";
		archivo << "tiempo=" << ticksJuego << "\n";
		archivo << "pozoX=" << (pozoObjeto != nullptr ? pozoObjeto->getX() : 0) << "\n";
		archivo << "pozoY=" << (pozoObjeto != nullptr ? pozoObjeto->getY() : 0) << "\n";
		archivo << "grimorioX=" << (grimorioObjeto != nullptr ? grimorioObjeto->getX() : 0) << "\n";
		archivo << "grimorioY=" << (grimorioObjeto != nullptr ? grimorioObjeto->getY() : 0) << "\n";
		archivo << "itemCount=" << inventario->getCantidad() << "\n";
		for (int i = 0; i < inventario->getCantidad(); i++) {
			ItemInventario it = inventario->getItem(i);
			archivo << "item=" << it.nombre << "|" << it.descripcion << "|" << it.tipo << "\n";
		}
		archivo.close();
		return true;
	}

	bool cargarPartida() {
		std::ifstream archivo(ObtenerRutaGuardadoStdComun("partida_guardada.txt"));
		if (!archivo.is_open()) {
			iniciarNivel(1);
			guardarPartida();
			return true;
		}
		std::map<std::string, std::string> valores;
		std::vector<ItemInventario> items;
		std::string linea;
		while (std::getline(archivo, linea)) {
			if (linea.empty()) continue;
			size_t igual = linea.find('=');
			if (igual == std::string::npos) continue;
			std::string clave = linea.substr(0, igual);
			std::string valor = linea.substr(igual + 1);
			if (clave == "item") {
				size_t p1 = valor.find('|');
				size_t p2 = (p1 == std::string::npos) ? std::string::npos : valor.find('|', p1 + 1);
				ItemInventario it;
				it.nombre = valor.substr(0, p1);
				it.descripcion = (p1 == std::string::npos) ? "" : valor.substr(p1 + 1, (p2 == std::string::npos) ? std::string::npos : p2 - p1 - 1);
				it.tipo = (p2 == std::string::npos) ? "Objeto" : valor.substr(p2 + 1);
				items.push_back(it);
			}
			else {
				valores[clave] = valor;
			}
		}
		archivo.close();
		try {
			iniciarNivel(1);
			enIntro = false;
			ticksTituloNivel = 0;
			if (valores.find("vidas") != valores.end()) jugador->setVidas(ClampEntero(std::stoi(valores["vidas"]), 1, 3));
			if (valores.find("x") != valores.end()) jugador->setX(std::stoi(valores["x"]));
			if (valores.find("y") != valores.end()) jugador->setY(std::stoi(valores["y"]));
			if (valores.find("posAtelierX") != valores.end()) posAtelierX = std::stoi(valores["posAtelierX"]);
			if (valores.find("posAtelierY") != valores.end()) posAtelierY = std::stoi(valores["posAtelierY"]);
			if (valores.find("enPozo") != valores.end()) enPozo = std::stoi(valores["enPozo"]) != 0;
			if (valores.find("habloQifrey") != valores.end()) habloQifrey = std::stoi(valores["habloQifrey"]) != 0;
			if (valores.find("habloAgott") != valores.end()) habloAgott = std::stoi(valores["habloAgott"]) != 0;
			if (valores.find("habloRicheh") != valores.end()) habloRicheh = std::stoi(valores["habloRicheh"]) != 0;
			if (valores.find("pidioMantoQifrey") != valores.end()) pidioMantoQifrey = std::stoi(valores["pidioMantoQifrey"]) != 0;
			if (valores.find("confianzaQifrey") != valores.end()) confianzaQifrey = std::stoi(valores["confianzaQifrey"]);
			if (valores.find("confianzaAgott") != valores.end()) confianzaAgott = std::stoi(valores["confianzaAgott"]);
			if (valores.find("confianzaRicheh") != valores.end()) confianzaRicheh = std::stoi(valores["confianzaRicheh"]);
			if (valores.find("respuestaInicialAgott") != valores.end()) respuestaInicialAgott = std::stoi(valores["respuestaInicialAgott"]);
			if (valores.find("richehEnojada") != valores.end()) richehEnojada = std::stoi(valores["richehEnojada"]) != 0;
			if (valores.find("misionMyrphonActiva") != valores.end()) misionMyrphonActiva = std::stoi(valores["misionMyrphonActiva"]) != 0;
			if (valores.find("myrphonRescatado") != valores.end()) myrphonRescatado = std::stoi(valores["myrphonRescatado"]) != 0;
			if (valores.find("myrphonEscapado") != valores.end()) myrphonEscapado = std::stoi(valores["myrphonEscapado"]) != 0;
			if (valores.find("vioMyrphon") != valores.end()) vioMyrphon = std::stoi(valores["vioMyrphon"]) != 0;
			if (valores.find("vioEnemigos") != valores.end()) vioEnemigos = std::stoi(valores["vioEnemigos"]) != 0;
			if (valores.find("dioVaraRicheh") != valores.end()) dioVaraRicheh = std::stoi(valores["dioVaraRicheh"]) != 0;
			if (valores.find("agottSabeMyrphon") != valores.end()) agottSabeMyrphon = std::stoi(valores["agottSabeMyrphon"]) != 0;
			if (valores.find("capaCrafteada") != valores.end()) capaCrafteada = std::stoi(valores["capaCrafteada"]) != 0;
			if (valores.find("tintaMagicaEntregada") != valores.end()) tintaMagicaEntregada = std::stoi(valores["tintaMagicaEntregada"]) != 0;
			if (valores.find("tintaSecundariaEntregada") != valores.end()) tintaSecundariaEntregada = std::stoi(valores["tintaSecundariaEntregada"]) != 0;
			if (valores.find("grimorioDescubierto") != valores.end()) grimorioDescubierto = std::stoi(valores["grimorioDescubierto"]) != 0;
			if (valores.find("libroEncontrado") != valores.end()) libroEncontrado = std::stoi(valores["libroEncontrado"]) != 0;
			if (valores.find("pozoDescubierto") != valores.end()) pozoDescubierto = std::stoi(valores["pozoDescubierto"]) != 0;
			if (valores.find("paredRocaRota") != valores.end()) paredRocaRota = std::stoi(valores["paredRocaRota"]) != 0;
			if (valores.find("entroTorre") != valores.end()) entroTorre = std::stoi(valores["entroTorre"]) != 0;
			if (valores.find("monologoCruceActivado") != valores.end()) monologoCruceActivado = std::stoi(valores["monologoCruceActivado"]) != 0;
			if (valores.find("gatoAcariciado") != valores.end()) gatoAcariciado = std::stoi(valores["gatoAcariciado"]) != 0;
			if (valores.find("trollDescubierto") != valores.end()) trollDescubierto = std::stoi(valores["trollDescubierto"]) != 0;
			if (valores.find("misionCompletada") != valores.end()) misionCompletada = std::stoi(valores["misionCompletada"]) != 0;
			if (valores.find("tiempo") != valores.end()) ticksJuego = std::stoi(valores["tiempo"]);
			inventario->limpiar();
			for (size_t i = 0; i < items.size(); i++) {
				inventario->agregarItem(items[i].nombre, items[i].descripcion, items[i].tipo);
			}
			if (paredRocaRota) mapa->romperParedRoca();
			if (pozoObjeto != nullptr) {
				if (valores.find("pozoX") != valores.end() && std::stoi(valores["pozoX"]) > 0) pozoObjeto->setX(std::stoi(valores["pozoX"]));
				if (valores.find("pozoY") != valores.end() && std::stoi(valores["pozoY"]) > 0) pozoObjeto->setY(std::stoi(valores["pozoY"]));
				pozoObjeto->setVisible(pozoDescubierto);
			}
			if (grimorioObjeto != nullptr) {
				if (valores.find("grimorioX") != valores.end() && std::stoi(valores["grimorioX"]) > 0) grimorioObjeto->setX(std::stoi(valores["grimorioX"]));
				if (valores.find("grimorioY") != valores.end() && std::stoi(valores["grimorioY"]) > 0) grimorioObjeto->setY(std::stoi(valores["grimorioY"]));
				grimorioObjeto->setVisible(grimorioDescubierto && !tieneItem("Grimorio de Trazos") && !capaCrafteada);
			}
			if (enPozo) mapaActual = mapaPozo;
			else mapaActual = mapa;
			if (myrphonObjeto != nullptr) {
				myrphonObjeto->setVisible(misionMyrphonActiva && !myrphonRescatado);
				if (myrphonEscapado) {
					myrphonObjeto->setX(131 * 32);
					myrphonObjeto->setY(60 * 32);
				}
			}
			if (dioVaraRicheh) myrphonConRicheh = true;
			finMostrado = false;
			cerrarDialogo();
			ultimaUbicacion = nullptr;
			actualizarObjetivo();
			return true;
		}
		catch (...) {
			iniciarNivel(1);
			guardarPartida();
			return true;
		}
	}
};
