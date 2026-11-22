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
#include "BloqueMapa.h"
#include "Arbol.h"
#include "CajaMovible.h"
#include "MapaTileMatrix.h"
#include "Inventario.h"
#include "GestorAudio.h"

using namespace System;
using namespace System::Drawing;

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

struct PersonajeInfo {
	std::string nombre;
	std::string rol;
	std::string descripcion;
	std::string confianza;
};

class ControladorJuego {
private:
	Protagonista* jugador;
	MapaTileMatrix* mapa;
	MapaTileMatrix* mapaPozo;
	MapaTileMatrix* mapaActual;
	std::vector<NPC*> listaNPCs;
	NPC* richehPozo;
	std::vector<BloqueMapa*> listaObjetos;
	std::vector<BloqueMapa*> objetosPozo;
	std::vector<Arbol*> listaArboles;
	std::vector<CajaMovible*> listaCajas;
	BloqueMapa* myrphonObjeto;
	BloqueMapa* pozoObjeto;
	BloqueMapa* grimorioObjeto;
	int cajasMovidasContador;
	Inventario* inventario;
	GestorAudio audio;
	bool audioHabilitado;

	gcroot<Bitmap^> bmpCoco;
	gcroot<Bitmap^> bmpQifrey;
	gcroot<Bitmap^> bmpTartah;
	gcroot<Bitmap^> bmpCoustas;
	gcroot<String^> ultimaUbicacion;
	gcroot<String^> dialogoTextoManaged;

	int camX;
	int camY;
	int nivelActual;
	int ticksJuego;
	int ticksTituloUbicacion;
	int ticksTituloNivel;
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
	bool libroEncontrado;
	bool pozoDescubierto;
	bool paredRocaRota;
	bool entroTorre;
	bool gatoAcariciado;
	bool trollDescubierto;
	bool misionCompletada;
	bool finMostrado;

	bool solicitaMinijuego;
	bool solicitaTrollface;
	bool solicitaFinNivel;

	bool transicionPozoActiva;
	bool transicionBajandoPozo;
	int ticksTransicionPozo;

	bool myrphonCorriendo;
	int ticksMyrphonCorriendo;
	bool myrphonConRicheh;

	int ticksMensajeTemporal;
	std::string mensajeTemporal;

	int tipoObjetivo;
	int indiceObjetivo;
	std::string promptInteraccion;
	std::string objetivoActual;

	static Bitmap^ CargarBitmapSeguro(String^ nombreArchivo) {
		if (System::IO::File::Exists(nombreArchivo)) return gcnew Bitmap(nombreArchivo);
		String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
		String^ ruta1 = System::IO::Path::Combine(baseDir, nombreArchivo);
		if (System::IO::File::Exists(ruta1)) return gcnew Bitmap(ruta1);
		String^ ruta2 = System::IO::Path::Combine(baseDir, "..\\..\\wha-v2\\" + nombreArchivo);
		if (System::IO::File::Exists(ruta2)) return gcnew Bitmap(ruta2);
		String^ ruta3 = System::IO::Path::Combine(baseDir, "..\\..\\..\\wha-v2\\wha-v2\\" + nombreArchivo);
		if (System::IO::File::Exists(ruta3)) return gcnew Bitmap(ruta3);
		return gcnew Bitmap(32, 32);
	}

	std::string textoConfianza(int valor) {
		if (valor <= 0) return "Sin confianza";
		if (valor == 1) return "Neutral";
		return "Amigos";
	}

	void limpiarEntidades() {
		for (size_t i = 0; i < listaNPCs.size(); i++) delete listaNPCs[i];
		listaNPCs.clear();
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
			objetivoActual = "¡Capa Mágica crafteada! ¡Misión cumplida!";
		}
		else if (dioVaraRicheh && !paredRocaRota) {
			objetivoActual = "Usar la Pluma Termica para quebrar la pared de la cueva";
		}
		else if (myrphonRescatado && !dioVaraRicheh) {
			objetivoActual = "Llevar a Myrphon de regreso con Richeh";
		}
		else if (misionMyrphonActiva) {
			objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
		}
		else if (tieneItem("Fibra de Arbol") && tieneItem("Tinta magica") && tieneItem("Grimorio de Trazos")) {
			objetivoActual = "Regresa con Qifrey para confeccionar tu Capa de Aprendiz";
		}
		else if (habloQifrey) {
			objetivoActual = "Reunir Fibra de Arbol, Tinta magica y Grimorio de Trazos";
		}
		else {
			objetivoActual = "Hablar con Maestro Qifrey";
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

	void crearArboles() {
		crearArbol(ArbolGigante, 16, 60);
		crearArbol(ArbolGigante, 26, 70);
		crearArbol(ArbolGigante, 60, 64);
		crearArbol(ArbolGigante, 88, 68);
		crearArbol(ArbolGigante, 92, 20);
		crearArbol(ArbolGigante, 108, 17);
		crearArbol(ArbolGigante, 104, 18);
		crearArbol(ArbolGigante, 82, 58);
		crearArbol(ArbolGigante, 12, 48);
		crearArbol(ArbolGigante, 44, 60);
		crearArbol(ArbolGigante, 58, 10);
		crearArbol(ArbolGigante, 98, 70);
		crearArbol(ArbolGigante, 96, 56);
		crearArbol(ArbolGigante, 108, 57);
		crearArbol(ArbolGigante, 97, 65);
		crearArbol(ArbolGigante, 109, 65);
		crearArbol(ArbolGigante, 16, 63);
		crearArbol(ArbolGigante, 29, 64);
		crearArbol(ArbolGigante, 24, 71);

		unsigned int semilla = 20261118u;
		int colocados = 0;
		int intentos = 0;
		while (colocados < 240 && intentos < 12000) {
			intentos++;
			semilla = semilla * 1664525u + 1013904223u;
			int fila = 4 + (int)(semilla % (Configuracion::FILAS_MAPA_NIVEL1 - 8));
			semilla = semilla * 1664525u + 1013904223u;
			int columna = 4 + (int)(semilla % (Configuracion::COLUMNAS_MAPA_NIVEL1 - 8));
			if (mapa->getCaracter(columna, fila) != ',') continue;
			crearArbol(ArbolPino, columna, fila);
			colocados++;
		}

		colocados = 0;
		intentos = 0;
		while (colocados < 100 && intentos < 8000) {
			intentos++;
			semilla = semilla * 1664525u + 1013904223u;
			int fila = 5 + (int)(semilla % (Configuracion::FILAS_MAPA_NIVEL1 - 10));
			semilla = semilla * 1664525u + 1013904223u;
			int columna = 5 + (int)(semilla % (Configuracion::COLUMNAS_MAPA_NIVEL1 - 10));
			if (mapa->getCaracter(columna, fila) != ',') continue;
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
			dialogoHablante = "Qifrey";
			dialogoRol = "Maestro Hechicero";
			dialogoConfianza = confianzaQifrey;
		}
		else if (npc == "Agott") {
			dialogoHablante = "Agott";
			dialogoRol = "Aprendiz de Maga";
			dialogoConfianza = confianzaAgott;
		}
		else if (npc == "Richeh") {
			dialogoHablante = "Richeh";
			dialogoRol = "Aprendiz de Maga";
			dialogoConfianza = confianzaRicheh;
		}
		else {
			dialogoHablante = "Letrero";
			dialogoRol = "Informacion";
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
				dialogoOpciones.push_back("[1] Maestro Qifrey, ¿podrias ayudarme a confeccionar mi Manto de Aprendiz?");
				dialogoOpciones.push_back("[2] Solo venía a explorar el taller y ver tus libros.");
				dialogoOpciones.push_back("[3] Maestro, necesito materiales especiales para mis prácticas.");
				if (tieneItem("Frasco de Tinta") || tieneItem("Tinta de Viento")) {
					dialogoOpciones.push_back("[4] ¡Maestro, encontré una tinta especial explorando el taller!");
				}
				break;
			case 10:
				dialogoLineas.push_back("Para confeccionar tu Manto de Aprendiz necesito 3 elementos del atelier:");
				dialogoLineas.push_back("Fibra del Arbol de Plata, Tinta magica de conjuracion y el Grimorio de Trazos.");
				dialogoLineas.push_back("¡Explora el taller, la cueva misteriosa y la torre este para reunirlos!");
				dialogoOpciones.push_back("[1] Está bien, iré a buscarlos por el atelier.");
				break;
			case 11:
				dialogoLineas.push_back("¡Excelente trabajo! Has reunido los componentes indispensables.");
				dialogoLineas.push_back("Tratando fibras del Arbol de Plata y sellos protectores...");
				dialogoLineas.push_back("¡Aqui esta tu Capa de Aprendiz!");
				dialogoLineas.push_back("Recuerda que su forro repele la hechiceria exterior.");
				dialogoOpciones.push_back("[1] ¡Entendido!");
				break;
			case 12:
				dialogoLineas.push_back("[CONFECCION EXITOSA: Capa Magica obtenida]");
				dialogoLineas.push_back("Se consumieron: Fibra de Arbol, Tinta magica y Grimorio de Trazos.");
				dialogoLineas.push_back("La Capa Magica ha sido equipada y agregada a tu inventario.");
				dialogoOpciones.push_back("[1] ¡Muchas gracias, Maestro Qifrey!");
				break;
			case 15:
				dialogoLineas.push_back("Te queda excelente la Capa Mágica, Coco.");
				dialogoLineas.push_back("Recuerda usar tus alas de aprendiz\ncon verdadera sabiduría y responsabilidad.");
				dialogoOpciones.push_back("[1] ¡Gracias, Maestro Qifrey!");
				break;
			case 20:
				dialogoLineas.push_back("Eres bienvenida en el taller siempre, Coco.");
				dialogoLineas.push_back("Cuídate de las corrientes del gran río.\nCruza siempre por los puentes arcanos.");
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
				dialogoLineas.push_back("Los sellos no despiertan con tinta ordinaria; solo la tinta de conjuracion,");
				dialogoLineas.push_back("nacida de la savia del Arbol de Plata, canaliza el flujo magico al pergamino.");
				dialogoLineas.push_back("Es la única que poseo en el taller... ¿Aún deseas aceptarla, Coco?");
				dialogoOpciones.push_back("[1] ¡Sí, por favor! Prometo cuidarla y esforzarme al máximo.");
				break;
			case 40:
				dialogoLineas.push_back("¡Increíble hallazgo, Coco! Esta tinta arcaica es justo lo");
				dialogoLineas.push_back("que necesitábamos en el taller para restaurar pergaminos.");
				dialogoLineas.push_back("Demuestras una gran curiosidad y respeto por este atelier.");
				dialogoLineas.push_back("¡Has demostrado ser una verdadera amiga y gran aprendiz!");
				dialogoOpciones.push_back("[1] ¡Me alegra mucho ser de ayuda, Maestro!");
				break;
			case 41:
				dialogoLineas.push_back("[MISIÓN SECUNDARIA COMPLETADA]");
				dialogoLineas.push_back("Entregaste la tinta arcaica al Maestro Qifrey.");
				dialogoLineas.push_back("Tu nivel de confianza con Qifrey ahora es: Amigos (+50 pts)");
				dialogoOpciones.push_back("[1] Continuar explorando");
				break;
			case 99:
				dialogoLineas.push_back("[INVENTARIO LLENO: Capacidad máxima 8 objetos alcanzada]");
				dialogoLineas.push_back("No puedes recibir más objetos en este momento.");
				dialogoOpciones.push_back("[1] Volver");
				break;
			default:
				dialogoLineas.push_back("Continua tu aprendizaje con dedicacion, Coco.");
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
				dialogoLineas.push_back("A la joven y pequeña Coco, ¿por qué entraste a mi torre?");
				dialogoOpciones.push_back("[1] Necesito encontrar un Grimorio de Trazos que perdi.");
				dialogoOpciones.push_back("[2] ¿A ti qué te importa, Agott?");
				break;
			case 202:
				dialogoLineas.push_back("Puedes encontrarlo en este montón de cajas si quieres...\nAl final, solo son basura.");
				dialogoOpciones.push_back("[1] Entendido");
				break;
			case 203:
				dialogoLineas.push_back("¡Lárgate de aquí!");
				dialogoOpciones.push_back("[1] Ya me voy...");
				break;
			case 210:
				dialogoLineas.push_back("¿Qué pasó ahora, niña?");
				if (libroEncontrado || tieneItem("Grimorio de Trazos")) {
					dialogoOpciones.push_back("[1] Solo pasaba por aqui, ya recupere el Grimorio de Trazos, Agott.");
				}
				else {
					dialogoOpciones.push_back("[1] Sigo buscando el Grimorio de Trazos, necesito ayuda...");
				}
				if (myrphonRescatado) {
					dialogoOpciones.push_back("[2] Recuperé a su mascota, está a salvo con Richeh.");
				}
				break;
			case 211:
				dialogoLineas.push_back("No esperaba que lo encontraras en esta basura, jaja.");
				dialogoOpciones.push_back("[1] Continuar");
				break;
			case 212:
				dialogoLineas.push_back("Sabia que te costaria trabajo... Prueba empujando las cajas de mi torre.");
				dialogoOpciones.push_back("[1] Gracias por nada...");
				break;
			case 220:
				dialogoLineas.push_back("¿Por qué sigues aquí, Coco? No eres bienvenida.");
				if (libroEncontrado || tieneItem("Grimorio de Trazos")) {
					dialogoOpciones.push_back("[1] Nada, solo quería burlarme de tu cara.");
				}
				else {
					dialogoOpciones.push_back("[1] ¡Sigo buscando algo, deja de molestar!");
				}
				if (myrphonRescatado) {
					dialogoOpciones.push_back("[2] Recuperé a su mascota, está a salvo con Richeh.");
				}
				break;
			case 221:
				dialogoLineas.push_back("¡Estúpida niña!");
				dialogoOpciones.push_back("[1] Salir");
				break;
			case 222:
				dialogoLineas.push_back("¿Qué demonios estás buscando?");
				dialogoOpciones.push_back("[1] ¡¡QUÉ TE IMPORTA!!");
				break;
			case 223:
				dialogoLineas.push_back("...");
				dialogoOpciones.push_back("[1] Salir");
				break;
			case 230:
				dialogoLineas.push_back("El taller ya volvió a la normalidad.");
				dialogoLineas.push_back("Deja de perder el tiempo y concéntrate en tu viaje.");
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
				dialogoOpciones.push_back("[2] Disculpa... caí a este pozo buscando un objeto mágico...");
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
				dialogoLineas.push_back("en esa horrible cueva Serpentback tras el ataque...");
				dialogoLineas.push_back("...Si de verdad quieres ayudar, trae a Myrphon de vuelta a este cuarto.");
				dialogoOpciones.push_back("[1] ¡Acepto, traeré a Myrphon de vuelta!");
				break;
			case 303:
				dialogoLineas.push_back("¡No tienes idea de nada! ¡Lárgate!");
				dialogoLineas.push_back("...¡No quiero volver a ver tu cara por aquí!");
				dialogoOpciones.push_back("[1] Salir...");
				break;
			case 304:
				dialogoLineas.push_back("(Se seca las lagrimas desesperada con su tunica)");
				dialogoLineas.push_back("¡¿D-De verdad harias eso por mi?! ¡Muchas gracias, por favor!");
				dialogoLineas.push_back("¡Date prisa! Ese laberinto es muy peligroso y Myrphon no sabe defenderse...");
				dialogoLineas.push_back("¡Traelo a salvo, por lo que mas quieras!");
				dialogoOpciones.push_back("[1] ¡No te preocupes, lo traere de vuelta!");
				break;
			case 310:
				dialogoLineas.push_back("¿Un objeto magico?... Tengo la Pluma Termica (Searneedle) de Olruggio.");
				dialogoLineas.push_back("Su punta se calienta al rojo vivo para quebrar roca, pero yo solo dibujo sellos diminutos.");
				dialogoLineas.push_back("...No te la dare gratis mientras mi pobre Myrphon siga atrapado en la oscuridad.");
				dialogoOpciones.push_back("[1] Lamento escuchar eso... ¿Cómo te puedo ayudar a recuperarlo?");
				dialogoOpciones.push_back("[2] Vaya, qué mal, y... ¿por un simple animal te pones a llorar?");
				break;
			case 311:
				dialogoLineas.push_back("Mi Myrphon se asusto por el ataque de los Sombreros de Ala Ancha en Serpentback...");
				dialogoLineas.push_back("Si entras alli y lo rescatas, la Pluma Termica sera tuya. ¿Aceptas?");
				dialogoOpciones.push_back("[1] Sí.");
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
				dialogoLineas.push_back("...Está bien. Se perdió en el laberinto Serpentback tras el ataque");
				dialogoLineas.push_back("de Sombreros de Ala Ancha. Tráelo de vuelta a este cuarto.");
				dialogoLineas.push_back("Traelo sano y salvo. Si lo logras, te entregare la Pluma Termica. No me falles.");
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
				dialogoLineas.push_back("(Sus ojos se abren de par en par al ver al animal) \"¡¡Myrphon!!\"");
				dialogoOpciones.push_back("[1] Aquí está, sano y salvo.");
				break;
			case 351:
				dialogoLineas.push_back("(Abraza fuertemente a su mascota mientras llora de alegría)");
				dialogoLineas.push_back("¡Muchas gracias! Pensé que no volvería a verlo...");
				dialogoLineas.push_back("Lo prometido es deuda. Toma la Pluma Termica Searneedle de Olruggio.");
				dialogoLineas.push_back("Usa su calor para fracturar y fundir el muro de roca de la cueva misteriosa.");
				dialogoOpciones.push_back("[1] ¡Muchas gracias, Richeh!");
				break;
			case 360:
				dialogoLineas.push_back("( •u• ): ¡Hola! Myrphon y yo estamos muy felices");
				dialogoLineas.push_back("gracias a ti. ¡¡Ten cuidado en tus viajes!!");
				dialogoOpciones.push_back("[1] ¡Nos vemos, Richeh!");
				break;
			default:
				dialogoLineas.push_back("*llorando*");
				dialogoOpciones.push_back("[1] Hola, ¿por qué estás llorando?");
				dialogoOpciones.push_back("[2] Disculpa... caí a este pozo buscando un objeto mágico...");
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
				dialogoLineas.push_back("dibujar unicamente la magia que nace de su propio corazon.");
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
				dialogoLineas.push_back("Richeh, compañera de cuarto de Agott, no le gustan las visitas");
				dialogoLineas.push_back("desde que se perdió Myrphon, un pequeño pingüino con rasgos de grifo.");
				dialogoLineas.push_back("Myrphon se perdió tras el ataque inesperado de unos magos oscuros");
				dialogoLineas.push_back("en el laberinto subterráneo Serpentback durante el segundo examen...");
				dialogoOpciones.push_back("[1] Continuar...");
				break;
			case 411:
				dialogoLineas.push_back("...El examen evaluaba si un aprendiz usa la magia en secreto.");
				dialogoLineas.push_back("Se canceló por el caos, el pobre Myrphon quedó atrapado.");
				dialogoLineas.push_back("Richeh y Agott tienen la esperanza de que algún aventurero");
				dialogoLineas.push_back("valiente pueda encontrarlo sano y salvo.");
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
					if (tieneItem("Frasco de Tinta") || tieneItem("Tinta de Viento")) estadoDialogo = 40;
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
					agregarItem("Capa Magica", "Manto de aprendiz tejido con fibras del Arbol de Plata. Protege y repele la hechiceria.", "Equipamiento Magico");
					capaCrafteada = true;
					confianzaQifrey = 2;
					objetivoActual = "¡Capa Mágica crafteada! ¡Misión cumplida!";
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
						objetivoActual = "Buscar Fibra de Arbol y Grimorio de Trazos";
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
				quitarItem("Frasco de Tinta");
				quitarItem("Tinta de Viento");
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
				if (opcion == 2 && myrphonRescatado) estadoDialogo = 231;
				else if (libroEncontrado || tieneItem("Grimorio de Trazos")) estadoDialogo = 211;
				else estadoDialogo = 212;
				actualizarDialogoV1();
				return;
			}
			if (estadoDialogo == 220) {
				if (opcion == 2 && myrphonRescatado) estadoDialogo = 231;
				else if (libroEncontrado || tieneItem("Grimorio de Trazos")) {
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
				confianzaAgott++;
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
				objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
				if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(true);
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
				objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
				if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(true);
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
				objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
				if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(true);
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
					agregarItem("Pluma Termica", "Pluma Searneedle creada por Olruggio. Calienta su punta al rojo vivo, fundiendo y quebrando roca solida.", "Herramienta Arcana");
					mensajeTemporal = "[Recibiste: Pluma Termica]";
					ticksMensajeTemporal = 90;
				}
				confianzaRicheh = 2;
				objetivoActual = "Usar la Pluma Termica para quebrar la pared de la cueva";
				myrphonConRicheh = true;
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
	}

	bool posicionLibre(int nuevoX, int nuevoY) {
		if (nuevoX < 0 || nuevoY < 0) return false;
		if (nuevoX + jugador->getAncho() > mapaActual->getAnchoTotalPixels()) return false;
		if (nuevoY + jugador->getAlto() > mapaActual->getAltoTotalPixels()) return false;
		if (mapaActual->esColisionSolida(nuevoX + 3, nuevoY + 14, jugador->getAncho() - 6, jugador->getAlto() - 16)) return false;
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
		int col = cx / Configuracion::TILE_SIZE;
		int fila = cy / Configuracion::TILE_SIZE;
		return col >= 35 && col <= 47 && fila >= 13 && fila <= 19;
	}

	void actualizarObjetivoCercano() {
		promptInteraccion = "";
		tipoObjetivo = INTERACCION_NINGUNA;
		indiceObjetivo = -1;
		if (dialogoActivo || transicionPozoActiva || myrphonCorriendo) return;
		if (ticksMensajeTemporal > 0 && !mensajeTemporal.empty()) {
			promptInteraccion = "";
			return;
		}

		int centroX = jugador->getX() + jugador->getAncho() / 2;
		int centroY = jugador->getY() + jugador->getAlto() / 2;

		if (enPozo) {
			if (abs(centroX - mapaPozo->getPuntoSalidaPozoX()) <= 60 && abs(centroY - mapaPozo->getPuntoSalidaPozoY()) <= 60) {
				tipoObjetivo = INTERACCION_SALIDA_POZO;
				promptInteraccion = "Presiona [Espacio] para subir a la torre";
				return;
			}
			if (abs(centroX - 22 * 32) <= 42 && abs(centroY - 11 * 32) <= 42) {
				tipoObjetivo = INTERACCION_LETRERO_CURIOSIDADES;
				promptInteraccion = "Presiona [Espacio] para leer: Curiosidades de Richeh";
				return;
			}
			if (abs(centroX - 38 * 32) <= 42 && abs(centroY - 11 * 32) <= 42) {
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
			if (tieneItem("Pluma Termica")) promptInteraccion = "ENTER / ESPACIO: Usar Pluma Termica para quebrar la pared de roca";
			else promptInteraccion = "Coco: La pared de roca esta agrietada... Necesito una pluma termica para quebrarla.";
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
			if (!listaObjetos[i]->getInteractionRect().IntersectsWith(rectJugador)) continue;
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
				else if (tipo == ObjetoTroll) promptInteraccion = "Presiona [Espacio] para examinar el portal extraño";
				else if (tipo == ObjetoMontonCajas) promptInteraccion = "Presiona [Espacio] para revisar las cajas";
				else if (tipo == ObjetoMyrphon) promptInteraccion = "Presiona [Espacio] para entrar al laberinto Serpentback";
				else if (tipo == ObjetoLetrero) promptInteraccion = "Presiona [Espacio] para leer el letrero";
				else promptInteraccion = "Presiona [Espacio] para recoger: " + listaObjetos[i]->getNombre();
			}
		}
	}

	void bajarPozo() {
		if (transicionPozoActiva) return;
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
		if (nombre == "Grimorio de Trazos") libroEncontrado = true;
		mensajeTemporal = "[Recogiste: " + nombre + "]";
		ticksMensajeTemporal = 80;
		actualizarObjetivo();
	}

	void crearCajasTorre() {
		std::srand((unsigned int)std::time(NULL));
		std::vector<std::pair<int, int> > candidatos;
		for (int y = 14; y <= 18; y++) {
			for (int x = 36; x <= 46; x++) {
				if (x == 41 && y == 16) continue;
				if ((x == 40 || x == 41) && y == 18) continue;
				candidatos.push_back(std::make_pair(x, y));
			}
		}
		for (int i = (int)candidatos.size() - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			std::pair<int, int> temp = candidatos[i];
			candidatos[i] = candidatos[j];
			candidatos[j] = temp;
		}
		int numCajas = 6 + (rand() % 3);
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
				grimorioObjeto->setX(caja->getTileX() * 32 + 4);
				grimorioObjeto->setY(caja->getTileY() * 32 + 4);
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
		if (tx < 35 || tx > 47 || ty < 13 || ty > 19) return false;
		if (mapa->esColisionSolida(tx * 32 + 4, ty * 32 + 4, 24, 24)) return false;
		Rectangle destino(tx * 32, ty * 32, 32, 32);
		for (size_t i = 0; i < listaCajas.size(); i++) {
			if (listaCajas[i] == caja) continue;
			if (listaCajas[i]->getTileX() == tx && listaCajas[i]->getTileY() == ty) return false;
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
			if (caja->getContenido() == CajaGrimorio && !libroEncontrado) {
				libroEncontrado = true;
				if (grimorioObjeto != nullptr) {
					grimorioObjeto->setX(origenX * 32 + 4);
					grimorioObjeto->setY(origenY * 32 + 4);
					grimorioObjeto->setVisible(true);
				}
				if (cajasMovidasContador == 1) mensajeTemporal = "Coco: ¡Pff, a la primera!";
				else if (cajasMovidasContador == 2) mensajeTemporal = "Coco: ¡Bueno, no costó tanto encontrarlo!";
				else mensajeTemporal = "Coco: ¡Por fin, lo encontré!";
				ticksMensajeTemporal = 90;
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

public:
	ControladorJuego() {
		camX = 0;
		camY = 0;
		nivelActual = 1;
		ticksJuego = 0;
		ticksTituloUbicacion = 0;
		ticksTituloNivel = 0;
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
		libroEncontrado = false;
		pozoDescubierto = false;
		paredRocaRota = false;
		entroTorre = false;
		gatoAcariciado = false;
		trollDescubierto = false;
		misionCompletada = false;
		finMostrado = false;
		solicitaMinijuego = false;
		solicitaTrollface = false;
		solicitaFinNivel = false;
		transicionPozoActiva = false;
		transicionBajandoPozo = true;
		ticksTransicionPozo = 0;
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
		audioHabilitado = false;

		bmpCoco = CargarBitmapSeguro("sprites/Coco_Spritesheet.png");
		bmpQifrey = CargarBitmapSeguro("sprites/Qifrey_Spritesheet.png");
		bmpTartah = CargarBitmapSeguro("sprites/Tartah_Spritesheet.png");
		bmpCoustas = CargarBitmapSeguro("sprites/Coustas_Spritesheet.png");

		mapa = new MapaTileMatrix();
		mapaPozo = new MapaTileMatrix();
		mapaPozo->cargarPozo();
		mapaActual = mapa;
		inventario = new Inventario();
		jugador = new Protagonista(mapa->getPuntoInicioX() + 4, mapa->getPuntoInicioY(), 1);
		iniciarNivel(1);
	}

	~ControladorJuego() {
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
	std::string getInventarioNombre(int indice) { return inventario->getItem(indice).nombre; }
	std::string getInventarioDescripcion(int indice) { return inventario->getItem(indice).descripcion; }
	std::string getInventarioTipo(int indice) { return inventario->getItem(indice).tipo; }
	std::string getObjetivo() { return objetivoActual; }
	std::string getResumenConfianza(int indice) { return textoConfianza(indice); }

	void iniciarNivel(int numNivel) {
		nivelActual = 1;
		ticksJuego = 0;
		ticksTituloUbicacion = 0;
		ticksTituloNivel = 160;
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
		libroEncontrado = false;
		pozoDescubierto = false;
		paredRocaRota = false;
		entroTorre = false;
		gatoAcariciado = false;
		trollDescubierto = false;
		misionCompletada = false;
		finMostrado = false;
		ultimaUbicacion = nullptr;
		cerrarDialogo();
		objetivoActual = "Hablar con Maestro Qifrey";

		listaNPCs.push_back(new NPC(19 * 32 + 4, 16 * 32, 1, "Qifrey", "Maestro Hechicero"));
		listaNPCs.push_back(new NPC(41 * 32 + 4, 16 * 32, 2, "Agott", "Aprendiz de Maga"));

		agregarObjeto(new BloqueMapa(68 * 32 + 4, 40 * 32 + 4, ObjetoVial, "Tinta de Viento", "Esencia de tinta de viento encontrada junto a la orilla del gran río."));
		agregarObjeto(new BloqueMapa(59 * 32 + 4, 7 * 32 + 4, ObjetoHoja, "Fibra de Arbol", "Fibras del Arbol de Plata para tejer el manto de aprendiz."));
		agregarObjeto(new BloqueMapa(62 * 32 + 4, 7 * 32 + 4, ObjetoVial, "Frasco de Tinta", "Frasco con tinta arcaica de plata preservada en la cueva misteriosa."));
		grimorioObjeto = new BloqueMapa(0, 0, ObjetoPergamino, "Grimorio de Trazos", "Repleto de circulos, flechas y vectores magicos... No se entiende nada si intentas leerlo como un libro ordinario.");
		grimorioObjeto->setVisible(false);
		agregarObjeto(grimorioObjeto);
		pozoObjeto = new BloqueMapa(0, 0, ObjetoPozo, "Pozo de Agott", "Un pozo oculto entre las cajas de la torre.");
		pozoObjeto->setVisible(false);
		agregarObjeto(pozoObjeto);
		agregarObjeto(new BloqueMapa(26 * 32 + 4, 26 * 32 + 4, ObjetoLetrero, "Letrero del Atelier", "Letrero: Choza de Trazos. Dibuja runas con pasión y cuida tus pergaminos."));
		agregarObjeto(new BloqueMapa(30 * 32 + 4, 27 * 32 + 4, ObjetoLetrero, "Historia del Manga", "Letrero: Historia del Manga. Witch Hat Atelier fue creado por la mangaka Kamome Shirahama e inició su publicación el 22 de julio de 2016. Comenzó a lanzarse de forma mensual en la revista Morning Two de la editorial Kodansha, destacando por su magia basada en el arte del dibujo."));
		agregarObjeto(new BloqueMapa(55 * 32 + 4, 21 * 32 + 4, ObjetoLetrero, "Enciclopedia Mágica", "Letrero: Enciclopedia Mágica. Las tres sendas del Atelier: hacia el norte la cueva misteriosa, al este la Torre de Agott, y al sur el sendero hacia el despacho del maestro Qifrey."));
		agregarObjeto(new BloqueMapa(33 * 32 + 4, 21 * 32 + 4, ObjetoLetrero, "Torre de Agott", "Letrero: Torre de Agott. Prohibido el paso sin autorización de Agott."));
		agregarObjeto(new BloqueMapa(27 * 32 + 4, 22 * 32 + 4, ObjetoLetrero, "Despacho de Qifrey", "Letrero: Despacho de Qifrey. Maestro del atelier y protector del agua."));
		agregarObjeto(new BloqueMapa(63 * 32 + 4, 12 * 32 + 4, ObjetoLetrero, "Cueva Misteriosa", "Letrero: Cueva Misteriosa. Peligro: Derrumbe. Usa magia ígnea."));
		agregarObjeto(new BloqueMapa(23 * 32 + 4, 66 * 32 + 4, ObjetoGato, "Gato del Atelier", "El gato blanco de Richeh."));
		agregarObjeto(new BloqueMapa(110 * 32 + 4, 10 * 32 + 4, ObjetoTroll, "Portal Olvidado", "Un portal extraño que no lleva a ninguna parte..."));
		BloqueMapa* myrphon = new BloqueMapa(102 * 32 + 4, 60 * 32 + 4, ObjetoMyrphon, "Myrphon", "La mascota de Richeh, perdida en el bosque.");
		myrphon->setVisible(false);
		agregarObjeto(myrphon);

		crearArboles();
		crearCajasTorre();

		agregarObjetoPozo(new BloqueMapa(10 * 32 + 4, 6 * 32 + 4, ObjetoPozo, "Salida del pozo", "Subir a la torre de Agott."));
		agregarObjetoPozo(new BloqueMapa(22 * 32 + 4, 11 * 32 + 4, ObjetoLetrero, "Curiosidades de Richeh", "DATO CURIOSO DE RICHEH"));
		agregarObjetoPozo(new BloqueMapa(38 * 32 + 4, 11 * 32 + 4, ObjetoLetrero, "Diario del Atelier", "DIARIO DEL ATELIER"));
		richehPozo = new NPC(14 * 32 + 4, 8 * 32, 3, "Richeh", "Aprendiz de Maga");

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

	void moverJugador(int dx, int dy) {
		if (dialogoActivo || transicionPozoActiva || myrphonCorriendo) return;
		if (dx == 0 && dy == 0) {
			jugador->setEnMovimiento(false);
			return;
		}
		int pasoX = 6;
		int pasoY = 6;
		if (dx != 0 && dy != 0) {
			pasoX = 4;
			pasoY = 4;
		}
		if (dx != 0) {
			int nuevoX = jugador->getX() + dx * pasoX;
			if (posicionLibre(nuevoX, jugador->getY())) {
				Rectangle rectSig(nuevoX + 3, jugador->getY() + 14, jugador->getAncho() - 6, jugador->getAlto() - 16);
				bool puedeMover = true;
				if (hayCajaEn(rectSig)) puedeMover = intentarEmpujarCaja(dx, 0, rectSig);
				if (puedeMover) jugador->setX(nuevoX);
			}
		}
		if (dy != 0) {
			int nuevoY = jugador->getY() + dy * pasoY;
			if (posicionLibre(jugador->getX(), nuevoY)) {
				Rectangle rectSig(jugador->getX() + 3, nuevoY + 14, jugador->getAncho() - 6, jugador->getAlto() - 16);
				bool puedeMover = true;
				if (hayCajaEn(rectSig)) puedeMover = intentarEmpujarCaja(0, dy, rectSig);
				if (puedeMover) jugador->setY(nuevoY);
			}
		}
		if (dx < 0) jugador->setDireccion(1);
		else if (dx > 0) jugador->setDireccion(3);
		else if (dy < 0) jugador->setDireccion(2);
		else if (dy > 0) jugador->setDireccion(0);
		jugador->setEnMovimiento(true);
	}

	void detenerJugador() {
		jugador->detener();
	}

	void interactuar() {
		if (dialogoActivo) {
			if (!textoDialogoCompleto()) {
				String^ texto = dialogoTextoManaged;
				if (texto != nullptr) caracteresVisibles = texto->Length;
				return;
			}
			if (!dialogoOpciones.empty()) {
				int indice = dialogoOpcionSeleccionada;
				if (indice < 0 || indice >= (int)dialogoOpciones.size()) indice = 0;
				procesarOpcionDialogo(indice + 1);
			}
			else cerrarDialogo();
			return;
		}
		if (transicionPozoActiva || myrphonCorriendo) return;
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
				mensajeTemporal = "Punta incandescente aplicada: La pared de roca se quebro.";
				ticksMensajeTemporal = 100;
				actualizarObjetivo();
			}
			else {
				mensajeTemporal = "Coco: La pared de roca esta agrietada... Necesito una pluma termica para quebrarla.";
				ticksMensajeTemporal = 90;
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
				iniciarDialogoSimple("Coco", "Aprendiz de Maga", "Miau... El gato blanco de Richeh te observa y ronronea.");
			}
			else if (objeto->getTipo() == ObjetoTroll) {
				solicitaTrollface = true;
			}
			else if (objeto->getTipo() == ObjetoMyrphon) {
				if (misionMyrphonActiva && !myrphonRescatado) solicitaMinijuego = true;
			}
			else if (objeto->getTipo() == ObjetoLetrero) {
				iniciarDialogoSimple("Letrero", "Informacion", objeto->getDescripcion());
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

	void resolverMinijuego(bool exito) {
		reanudarAudioNivel();
		if (exito) {
			myrphonRescatado = true;
			if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(false);
			objetivoActual = "Llevar a Myrphon de regreso con Richeh";
			mensajeTemporal = "¡Atrapaste a Myrphon! Llévaselo de regreso a Richeh.";
			ticksMensajeTemporal = 120;
		}
		else {
			jugador->restarVida();
			if (jugador->getVidas() <= 0) {
				iniciarNivel(1);
				mensajeTemporal = "Has agotado tus vidas. El nivel ha sido reiniciado.";
				ticksMensajeTemporal = 120;
			}
			else {
				mensajeTemporal = "Myrphon se escapó... Inténtalo de nuevo.";
				ticksMensajeTemporal = 100;
				actualizarObjetivo();
			}
		}
	}

	void marcarTrollDescubierto() {
		trollDescubierto = true;
	}

	void procesarTick(Graphics^ g, int viewW, int viewH, int zoom) {
		if (g == nullptr || viewW <= 0 || viewH <= 0) return;
		ticksJuego++;
		if (ticksTituloUbicacion > 0) ticksTituloUbicacion--;
		if (ticksTituloNivel > 0) ticksTituloNivel--;
		if (ticksMensajeTemporal > 0) ticksMensajeTemporal--;
		audio.actualizar();
		jugador->animar();
		actualizarTextoAnimado();
		actualizarCorridaMyrphon();

		if (transicionPozoActiva) {
			if (ticksTransicionPozo > 0) ticksTransicionPozo--;
			if (ticksTransicionPozo <= 0) finalizarTransicionPozo();
		}

		if (!enPozo && !dialogoActivo && !transicionPozoActiva && !myrphonCorriendo && !entroTorre && estaEnTorre()) {
			entroTorre = true;
			habloAgott = true;
			iniciarDialogo("Agott", 200);
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
			if (richehPozo != nullptr) richehPozo->dibujar(g, bmpCoustas, camX, camY, zoom, ticksJuego);
			Bitmap^ iconoMyrphon = nullptr;
			if (myrphonObjeto != nullptr) iconoMyrphon = myrphonObjeto->getIcono();
			if (iconoMyrphon != nullptr) {
				int rx = (richehPozo != nullptr) ? richehPozo->getX() + 20 : 64 * 32;
				int ry = (richehPozo != nullptr) ? richehPozo->getY() : 12 * 32;
				if (myrphonCorriendo) {
					float t = ticksMyrphonCorriendo / 50.0f;
					int sx = mapaPozo->getPuntoInicioPozoX();
					int sy = mapaPozo->getPuntoInicioPozoY();
					int mx = (int)(sx + (rx - sx) * t);
					int my = (int)(sy + (ry - sy) * t);
					g->DrawImage(iconoMyrphon, Rectangle((mx - camX) * zoom, (my - camY) * zoom, 24 * zoom, 24 * zoom));
				}
				else if (myrphonConRicheh) {
					g->DrawImage(iconoMyrphon, Rectangle((rx - camX) * zoom, (ry - camY) * zoom, 24 * zoom, 24 * zoom));
				}
				else if (myrphonRescatado) {
					int mx = jugador->getX() + 18;
					int my = jugador->getY() + 10;
					g->DrawImage(iconoMyrphon, Rectangle((mx - camX) * zoom, (my - camY) * zoom, 24 * zoom, 24 * zoom));
				}
			}
			jugador->dibujar(g, bmpCoco, camX, camY, zoom, ticksJuego);
		}
		else {
			int baseJugador = jugador->getBaseY();
			for (size_t i = 0; i < listaArboles.size(); i++) {
				if (listaArboles[i]->getBaseY() <= baseJugador) listaArboles[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			}
			for (size_t i = 0; i < listaObjetos.size(); i++) listaObjetos[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			for (size_t i = 0; i < listaCajas.size(); i++) listaCajas[i]->dibujar(g, camX, camY, zoom);
			for (size_t i = 0; i < listaNPCs.size(); i++) {
				Bitmap^ bmpNPC = (listaNPCs[i]->getIdPersonaje() == 1) ? bmpQifrey : bmpTartah;
				listaNPCs[i]->dibujar(g, bmpNPC, camX, camY, zoom, ticksJuego);
			}
			if (myrphonRescatado && !myrphonConRicheh && myrphonObjeto != nullptr) {
				Bitmap^ iconoMyrphon = myrphonObjeto->getIcono();
				if (iconoMyrphon != nullptr) {
					int mx = jugador->getX() + 18;
					int my = jugador->getY() + 10;
					g->DrawImage(iconoMyrphon, Rectangle((mx - camX) * zoom, (my - camY) * zoom, 24 * zoom, 24 * zoom));
				}
			}
			jugador->dibujar(g, bmpCoco, camX, camY, zoom, ticksJuego);
			for (size_t i = 0; i < listaArboles.size(); i++) {
				if (listaArboles[i]->getBaseY() > baseJugador) listaArboles[i]->dibujar(g, nullptr, camX, camY, zoom, ticksJuego);
			}
		}

		if (!enPozo && !transicionPozoActiva) dibujarSombraZonas(g, viewW, viewH, camX, camY, zoom);
		actualizarObjetivoCercano();
		dibujarHUD(g, viewW, viewH, ubicacion);
		if (dialogoActivo) dibujarCuadroDialogo(g, viewW, viewH);
		if (transicionPozoActiva) dibujarTransicionPozo(g, viewW, viewH);
	}

	Rectangle rectMundoAPantalla(Rectangle mundo, int camX, int camY, int zoom) {
		return Rectangle((mundo.X - camX) * zoom, (mundo.Y - camY) * zoom, mundo.Width * zoom, mundo.Height * zoom);
	}

	Rectangle expandirRect(Rectangle r, int margen) {
		return Rectangle(r.X - margen, r.Y - margen, r.Width + margen * 2, r.Height + margen * 2);
	}

	void dibujarSombraZonas(Graphics^ g, int viewW, int viewH, int camX, int camY, int zoom) {
		if (enPozo) return;
		int cx = jugador->getX() + jugador->getAncho() / 2;
		int cy = jugador->getY() + jugador->getAlto() / 2;
		int col = cx / Configuracion::TILE_SIZE;
		int fila = cy / Configuracion::TILE_SIZE;
		Rectangle pantalla(0, 0, viewW, viewH);
		Rectangle atelier(8 * 32, 8 * 32, 45 * 32, 33 * 32);
		Rectangle despacho(12 * 32, 12 * 32, 15 * 32, 9 * 32);
		Rectangle torre(34 * 32, 12 * 32, 15 * 32, 9 * 32);
		Rectangle biblioteca(12 * 32, 28 * 32, 15 * 32, 9 * 32);
		Rectangle almacen(34 * 32, 28 * 32, 15 * 32, 9 * 32);
		Rectangle cueva(55 * 32, 4 * 32, 11 * 32, 7 * 32);

		Rectangle jugadorRect = jugador->getRectangle();
		Rectangle zonaDespacho = expandirRect(despacho, 32);
		Rectangle zonaTorre = expandirRect(torre, 32);
		Rectangle zonaBiblioteca = expandirRect(biblioteca, 32);
		Rectangle zonaAlmacen = expandirRect(almacen, 32);
		Rectangle zonaCueva = expandirRect(cueva, 32);

		bool enDespacho = jugadorRect.IntersectsWith(zonaDespacho);
		bool enTorre = jugadorRect.IntersectsWith(zonaTorre);
		bool enBiblioteca = jugadorRect.IntersectsWith(zonaBiblioteca);
		bool enAlmacen = jugadorRect.IntersectsWith(zonaAlmacen);
		bool enCueva = jugadorRect.IntersectsWith(zonaCueva);
		bool enSalon = !enDespacho && !enTorre && !enBiblioteca && !enAlmacen && !enCueva && col >= 9 && col <= 51 && fila >= 9 && fila <= 39;

		Region^ visible = nullptr;
		if (enDespacho) visible = gcnew Region(rectMundoAPantalla(zonaDespacho, camX, camY, zoom));
		else if (enTorre) visible = gcnew Region(rectMundoAPantalla(zonaTorre, camX, camY, zoom));
		else if (enBiblioteca) visible = gcnew Region(rectMundoAPantalla(zonaBiblioteca, camX, camY, zoom));
		else if (enAlmacen) visible = gcnew Region(rectMundoAPantalla(zonaAlmacen, camX, camY, zoom));
		else if (enCueva) visible = gcnew Region(rectMundoAPantalla(zonaCueva, camX, camY, zoom));
		else if (enSalon) {
			visible = gcnew Region(rectMundoAPantalla(atelier, camX, camY, zoom));
			visible->Exclude(rectMundoAPantalla(despacho, camX, camY, zoom));
			visible->Exclude(rectMundoAPantalla(torre, camX, camY, zoom));
			visible->Exclude(rectMundoAPantalla(biblioteca, camX, camY, zoom));
			visible->Exclude(rectMundoAPantalla(almacen, camX, camY, zoom));
		}
		else {
			visible = gcnew Region(pantalla);
			visible->Exclude(rectMundoAPantalla(atelier, camX, camY, zoom));
			visible->Exclude(rectMundoAPantalla(cueva, camX, camY, zoom));
		}

		Region^ oscuro = gcnew Region(pantalla);
		oscuro->Exclude(visible);
		SolidBrush^ sombra = gcnew SolidBrush(Color::FromArgb(190, 2, 4, 8));
		g->FillRegion(sombra, oscuro);
		delete sombra;
		delete oscuro;
		delete visible;
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
		System::Drawing::Font^ fTitulo = gcnew System::Drawing::Font("Segoe UI", 16.0f, FontStyle::Bold);
		System::Drawing::Font^ fSub = gcnew System::Drawing::Font("Segoe UI", 11.0f, FontStyle::Italic);
		System::Drawing::Font^ fPrompt = gcnew System::Drawing::Font("Segoe UI", 10.0f, FontStyle::Bold);

		SolidBrush^ panel = gcnew SolidBrush(Color::FromArgb(228, 8, 12, 18));
		g->FillRectangle(panel, 16, 16, 420, 78);
		for (int i = 0; i < 3; i++) {
			Color colorCorazon = (i < jugador->getVidas()) ? Color::FromArgb(220, 210, 58, 76) : Color::FromArgb(120, 70, 58, 62);
			dibujarCorazon(g, 28 + i * 20, 24, colorCorazon);
		}
		g->DrawString("INVENTARIO  " + inventario->getCantidad().ToString() + "/" + inventario->getCapacidad().ToString(), fTexto, Brushes::Gainsboro, PointF(102.0f, 26.0f));
		g->DrawString("Objetivo: " + ToManaged(objetivoActual), fObjetivo, Brushes::Khaki, RectangleF(28.0f, 50.0f, 396.0f, 38.0f));
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
			SolidBrush^ fondoPrompt = gcnew SolidBrush(Color::FromArgb(175, 8, 12, 18));
			g->FillRectangle(fondoPrompt, x, y, ancho, 32);
			delete fondoPrompt;
			g->DrawString(tp, fPrompt, Brushes::WhiteSmoke, PointF((float)(x + 18), (float)(y + 6)));
		}

		if (ticksTituloNivel > 0) {
			int alpha = Math::Min(230, ticksTituloNivel * 4);
			SolidBrush^ tituloColor = gcnew SolidBrush(Color::FromArgb(alpha, 235, 200, 158));
			SolidBrush^ linea = gcnew SolidBrush(Color::FromArgb(alpha, 200, 158, 78));
			String^ t1 = "N I V E L   1";
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
		for each (String^ linea in lineasVisibles) {
			SizeF medida = g->MeasureString(linea, fTexto, anchoTexto);
			int h = (int)Math::Ceiling(medida.Height);
			int altoLinea = (int)Math::Ceiling(fTexto->GetHeight(g));
			if (h < altoLinea) h = altoLinea;
			g->DrawString(linea, fTexto, Brushes::WhiteSmoke, RectangleF((float)(x + 22), yActual, (float)anchoTexto, (float)h + 2.0f));
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
		System::Drawing::Drawing2D::LinearGradientBrush^ fondo = gcnew System::Drawing::Drawing2D::LinearGradientBrush(
			Rectangle(0, 0, viewW, viewH),
			Color::FromArgb(255, 22, 19, 16),
			Color::FromArgb(255, 3, 5, 8),
			System::Drawing::Drawing2D::LinearGradientMode::Vertical);
		g->FillRectangle(fondo, 0, 0, viewW, viewH);
		delete fondo;

		int anchoPared = (int)(viewW * 0.20f);
		SolidBrush^ piedra = gcnew SolidBrush(Color::FromArgb(255, 42, 44, 48));
		g->FillRectangle(piedra, 0, 0, anchoPared, viewH);
		g->FillRectangle(piedra, viewW - anchoPared, 0, anchoPared, viewH);
		Pen^ junta = gcnew Pen(Color::FromArgb(255, 24, 26, 30), 2.0f);
		for (int y = 0; y < viewH; y += 42) {
			g->DrawLine(junta, 0, y, anchoPared, y);
			g->DrawLine(junta, viewW - anchoPared, y, viewW, y);
			int desfase = ((y / 42) % 2 == 0) ? 34 : 68;
			g->DrawLine(junta, desfase, y, desfase, y + 42);
			g->DrawLine(junta, viewW - desfase, y, viewW - desfase, y + 42);
		}
		delete junta;
		delete piedra;

		SolidBrush^ brillo = gcnew SolidBrush(Color::FromArgb(32, 244, 176, 86));
		g->FillEllipse(brillo, viewW / 2 - 150, -120, 300, 260);
		delete brillo;
		SolidBrush^ fuego = gcnew SolidBrush(Color::FromArgb(210, 255, 200, 112));
		g->FillEllipse(fuego, viewW / 2 - 9, 8, 18, 24);
		g->FillEllipse(fuego, viewW / 2 - 5, 3, 10, 14);
		delete fuego;

		Pen^ soga = gcnew Pen(Color::FromArgb(210, 188, 158, 100), 3.0f);
		Pen^ travesano = gcnew Pen(Color::FromArgb(180, 140, 108, 62), 2.0f);
		g->DrawLine(soga, viewW / 2 - 58, 0, viewW / 2 - 58, viewH);
		g->DrawLine(soga, viewW / 2 + 58, 0, viewW / 2 + 58, viewH);
		for (int y = 24; y < viewH; y += 28) {
			g->DrawLine(travesano, viewW / 2 - 58, y, viewW / 2 + 58, y);
		}
		delete soga;
		delete travesano;

		float t = 1.0f - (ticksTransicionPozo / 60.0f);
		int spriteY = transicionBajandoPozo ? (int)(80 + t * 360) : (int)(440 - t * 360);
		g->DrawImage(bmpCoco, Rectangle(viewW / 2 - 44, spriteY, 72, 96), 0, 0, 24, 32, GraphicsUnit::Pixel);
		if (myrphonRescatado && !dioVaraRicheh && myrphonObjeto != nullptr) {
			Bitmap^ icono = myrphonObjeto->getIcono();
			if (icono != nullptr) g->DrawImage(icono, Rectangle(viewW / 2 + 26, spriteY + 28, 54, 54));
		}

		System::Drawing::Font^ f = gcnew System::Drawing::Font("Segoe UI", 14.0f, FontStyle::Bold);
		System::Drawing::Font^ fSub = gcnew System::Drawing::Font("Segoe UI", 9.0f, FontStyle::Regular);
		String^ txt = transicionBajandoPozo ? "Descendiendo al pozo..." : "Subiendo a la torre...";
		String^ sub = "Torre de Agott  ·  Sótano de Richeh";
		SizeF tam = g->MeasureString(txt, f);
		SizeF tamSub = g->MeasureString(sub, fSub);
		g->DrawString(txt, f, Brushes::Gainsboro, PointF(viewW / 2.0f - tam.Width / 2.0f, 34.0f));
		g->DrawString(sub, fSub, Brushes::Silver, PointF(viewW / 2.0f - tamSub.Width / 2.0f, 60.0f));
		delete f;
		delete fSub;
	}

	void obtenerPersonajes(std::vector<PersonajeInfo>& personajes) {
		personajes.clear();
		PersonajeInfo coco;
		coco.nombre = "Coco";
		coco.rol = "Aprendiz de Maga";
		coco.descripcion = "Protagonista del Nivel 1. Descubrió la verdad sobre el dibujo mágico.";
		coco.confianza = "Amigos";
		personajes.push_back(coco);
		if (habloQifrey) {
			PersonajeInfo p;
			p.nombre = "Qifrey";
			p.rol = "Maestro Hechicero";
			p.descripcion = "Tutor y protector del atelier. Especialista en trazos y magia de agua.";
			p.confianza = textoConfianza(confianzaQifrey);
			personajes.push_back(p);
		}
		if (habloAgott) {
			PersonajeInfo p;
			p.nombre = "Agott";
			p.rol = "Aprendiz de Maga";
			p.descripcion = "Compañera disciplinada y exigente. Aspira a la perfección del trazo.";
			p.confianza = textoConfianza(confianzaAgott);
			personajes.push_back(p);
		}
		if (habloRicheh) {
			PersonajeInfo p;
			p.nombre = "Richeh";
			p.rol = "Aprendiz de Maga";
			p.descripcion = "Amiga reflexiva de Coco. Gran conocedora de runas antiguas.";
			p.confianza = textoConfianza(confianzaRicheh);
			personajes.push_back(p);
		}
		if (misionMyrphonActiva || myrphonRescatado) {
			PersonajeInfo p;
			p.nombre = "Myrphon";
			p.rol = "Mascota de Richeh";
			p.descripcion = "Pingüino-grifo de cuatro patas propiedad de Richeh. Quedó atrapado en la oscuridad del laberinto Serpentback.";
			p.confianza = myrphonRescatado ? "Amigos" : "Perdido";
			personajes.push_back(p);
		}
	}

	void pausarAudio() { audio.pausar(); }
	void reanudarAudioNivel() {
		if (!audioHabilitado) return;
		audio.reanudar();
		if (!audio.estaCargado()) audio.reproducirNivel(1);
	}
	void reproducirAudioMinijuego() {
		if (audioHabilitado) audio.reproducirMinijuego();
	}
	void detenerAudio() { audio.detener(); }
	bool estaAudioHabilitado() { return audioHabilitado; }
	void alternarAudio() {
		audioHabilitado = !audioHabilitado;
		if (audioHabilitado) {
			audio.reproducirNivel(1);
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

	bool guardarPartida() {
		std::ofstream archivo("partida_guardada.txt", std::ios::out | std::ios::trunc);
		if (!archivo.is_open()) return false;
		archivo << "nivel=1\n";
		archivo << "vidas=" << jugador->getVidas() << "\n";
		archivo << "x=" << jugador->getX() << "\n";
		archivo << "y=" << jugador->getY() << "\n";
		archivo << "enPozo=" << (enPozo ? 1 : 0) << "\n";
		archivo << "habloQifrey=" << (habloQifrey ? 1 : 0) << "\n";
		archivo << "habloAgott=" << (habloAgott ? 1 : 0) << "\n";
		archivo << "habloRicheh=" << (habloRicheh ? 1 : 0) << "\n";
		archivo << "confianzaQifrey=" << confianzaQifrey << "\n";
		archivo << "confianzaAgott=" << confianzaAgott << "\n";
		archivo << "confianzaRicheh=" << confianzaRicheh << "\n";
		archivo << "respuestaInicialAgott=" << respuestaInicialAgott << "\n";
		archivo << "richehEnojada=" << (richehEnojada ? 1 : 0) << "\n";
		archivo << "misionMyrphonActiva=" << (misionMyrphonActiva ? 1 : 0) << "\n";
		archivo << "myrphonRescatado=" << (myrphonRescatado ? 1 : 0) << "\n";
		archivo << "dioVaraRicheh=" << (dioVaraRicheh ? 1 : 0) << "\n";
		archivo << "agottSabeMyrphon=" << (agottSabeMyrphon ? 1 : 0) << "\n";
		archivo << "capaCrafteada=" << (capaCrafteada ? 1 : 0) << "\n";
		archivo << "tintaMagicaEntregada=" << (tintaMagicaEntregada ? 1 : 0) << "\n";
		archivo << "libroEncontrado=" << (libroEncontrado ? 1 : 0) << "\n";
		archivo << "pozoDescubierto=" << (pozoDescubierto ? 1 : 0) << "\n";
		archivo << "paredRocaRota=" << (paredRocaRota ? 1 : 0) << "\n";
		archivo << "entroTorre=" << (entroTorre ? 1 : 0) << "\n";
		archivo << "gatoAcariciado=" << (gatoAcariciado ? 1 : 0) << "\n";
		archivo << "trollDescubierto=" << (trollDescubierto ? 1 : 0) << "\n";
		archivo << "misionCompletada=" << (misionCompletada ? 1 : 0) << "\n";
		archivo << "tiempo=" << ticksJuego << "\n";
		archivo << "itemCount=" << inventario->getCantidad() << "\n";
		for (int i = 0; i < inventario->getCantidad(); i++) {
			ItemInventario it = inventario->getItem(i);
			archivo << "item=" << it.nombre << "|" << it.descripcion << "|" << it.tipo << "\n";
		}
		archivo.close();
		return true;
	}

	bool cargarPartida() {
		std::ifstream archivo("partida_guardada.txt");
		if (!archivo.is_open()) return false;
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
			ticksTituloNivel = 0;
			if (valores.find("vidas") != valores.end()) jugador->setVidas(ClampEntero(std::stoi(valores["vidas"]), 1, 3));
			if (valores.find("x") != valores.end()) jugador->setX(std::stoi(valores["x"]));
			if (valores.find("y") != valores.end()) jugador->setY(std::stoi(valores["y"]));
			if (valores.find("enPozo") != valores.end()) enPozo = std::stoi(valores["enPozo"]) != 0;
			if (valores.find("habloQifrey") != valores.end()) habloQifrey = std::stoi(valores["habloQifrey"]) != 0;
			if (valores.find("habloAgott") != valores.end()) habloAgott = std::stoi(valores["habloAgott"]) != 0;
			if (valores.find("habloRicheh") != valores.end()) habloRicheh = std::stoi(valores["habloRicheh"]) != 0;
			if (valores.find("confianzaQifrey") != valores.end()) confianzaQifrey = std::stoi(valores["confianzaQifrey"]);
			if (valores.find("confianzaAgott") != valores.end()) confianzaAgott = std::stoi(valores["confianzaAgott"]);
			if (valores.find("confianzaRicheh") != valores.end()) confianzaRicheh = std::stoi(valores["confianzaRicheh"]);
			if (valores.find("respuestaInicialAgott") != valores.end()) respuestaInicialAgott = std::stoi(valores["respuestaInicialAgott"]);
			if (valores.find("richehEnojada") != valores.end()) richehEnojada = std::stoi(valores["richehEnojada"]) != 0;
			if (valores.find("misionMyrphonActiva") != valores.end()) misionMyrphonActiva = std::stoi(valores["misionMyrphonActiva"]) != 0;
			if (valores.find("myrphonRescatado") != valores.end()) myrphonRescatado = std::stoi(valores["myrphonRescatado"]) != 0;
			if (valores.find("dioVaraRicheh") != valores.end()) dioVaraRicheh = std::stoi(valores["dioVaraRicheh"]) != 0;
			if (valores.find("agottSabeMyrphon") != valores.end()) agottSabeMyrphon = std::stoi(valores["agottSabeMyrphon"]) != 0;
			if (valores.find("capaCrafteada") != valores.end()) capaCrafteada = std::stoi(valores["capaCrafteada"]) != 0;
			if (valores.find("tintaMagicaEntregada") != valores.end()) tintaMagicaEntregada = std::stoi(valores["tintaMagicaEntregada"]) != 0;
			if (valores.find("libroEncontrado") != valores.end()) libroEncontrado = std::stoi(valores["libroEncontrado"]) != 0;
			if (valores.find("pozoDescubierto") != valores.end()) pozoDescubierto = std::stoi(valores["pozoDescubierto"]) != 0;
			if (valores.find("paredRocaRota") != valores.end()) paredRocaRota = std::stoi(valores["paredRocaRota"]) != 0;
			if (valores.find("entroTorre") != valores.end()) entroTorre = std::stoi(valores["entroTorre"]) != 0;
			if (valores.find("gatoAcariciado") != valores.end()) gatoAcariciado = std::stoi(valores["gatoAcariciado"]) != 0;
			if (valores.find("trollDescubierto") != valores.end()) trollDescubierto = std::stoi(valores["trollDescubierto"]) != 0;
			if (valores.find("misionCompletada") != valores.end()) misionCompletada = std::stoi(valores["misionCompletada"]) != 0;
			if (valores.find("tiempo") != valores.end()) ticksJuego = std::stoi(valores["tiempo"]);
			inventario->limpiar();
			for (size_t i = 0; i < items.size(); i++) {
				inventario->agregarItem(items[i].nombre, items[i].descripcion, items[i].tipo);
			}
			if (paredRocaRota) mapa->romperParedRoca();
			if (pozoObjeto != nullptr) pozoObjeto->setVisible(pozoDescubierto);
			if (grimorioObjeto != nullptr) grimorioObjeto->setVisible(libroEncontrado && !tieneItem("Grimorio de Trazos"));
			if (enPozo) mapaActual = mapaPozo;
			else mapaActual = mapa;
			if (myrphonObjeto != nullptr) myrphonObjeto->setVisible(misionMyrphonActiva && !myrphonRescatado);
			if (dioVaraRicheh) myrphonConRicheh = true;
			finMostrado = false;
			cerrarDialogo();
			ultimaUbicacion = nullptr;
			actualizarObjetivo();
			return true;
		}
		catch (...) {
			return false;
		}
	}
};
