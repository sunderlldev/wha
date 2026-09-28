#ifndef Nivel_h
#define Nivel_h
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Mapa.h"
#include "MapaNivel1.h"
#include "Protagonista.h"
#include "NPC.h"
#include "ItemMagico.h"
#include "Inventario.h"
#include "Caja.h"
#include "CuartoRicheh.h"
#ifdef _WIN32
#include <conio.h>
#endif

class Nivel {
private:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	Mapa* mapa;
	Protagonista* protagonista;
	NPC* qifrey;
	NPC* agott;
	NPC* richeh;
	Mapa* mapaRicheh;
	bool enCuartoRicheh;
	int cocoPrevX;
	int cocoPrevY;
	bool dioVaraRicheh;
	bool transicionBajando;
	bool transicionSubiendo;
	std::vector<Caja*> cajas;
	int indiceCajaLibro;
	int indiceCajaPozo;
	bool libroEncontrado;
	bool pozoEncontrado;
	int cajasMovidasContador;
	int pozoX;
	int pozoY;
	bool primeraVezTorreAgott;
	int respuestaInicialAgott;
	std::string npcDialogoActual;
	std::string mensajeTemporal;
	int ticksMensajeTemporal;
	std::vector<ItemMagico*> itemsSuelo;
	clock_t tiempoInicio;
	int puntosMisiones;
	std::string promptFlotante;
	bool enDialogo;
	int estadoDialogo;
	bool enModalPersonajes;
	int seleccionModal;
	bool enModalMisiones;
	bool enDetalleMision;
	int seleccionMision;
	bool enModalInventario;
	int seleccionInventario;
	bool paredPiedraDestruida;
	int idCuartoActual;
	std::string cartelCuarto;
	int ticksCartelCuarto;
	std::string objetivoActual;
	bool mostrarEstadisticasFin;
	bool avanzaSiguienteNivel;

	void limpiarItemsSuelo() {
		for (size_t i = 0; i < itemsSuelo.size(); i++) {
			if (itemsSuelo[i] != nullptr) {
				delete itemsSuelo[i];
			}
		}
		itemsSuelo.clear();
	}

	void limpiarCajas() {
		for (size_t i = 0; i < cajas.size(); i++) {
			if (cajas[i] != nullptr) {
				delete cajas[i];
			}
		}
		cajas.clear();
	}

public:
	Nivel()
		: numeroNivel(1), nombreNivel("Atelier de Qifrey"), completado(false),
		  mapa(nullptr), protagonista(nullptr), qifrey(nullptr), agott(nullptr), richeh(nullptr),
		  mapaRicheh(nullptr), enCuartoRicheh(false), cocoPrevX(0), cocoPrevY(0),
		  dioVaraRicheh(false), transicionBajando(false), transicionSubiendo(false),
		  indiceCajaLibro(-1), indiceCajaPozo(-1), libroEncontrado(false),
		  pozoEncontrado(false), cajasMovidasContador(0), pozoX(-1), pozoY(-1),
		  primeraVezTorreAgott(true), respuestaInicialAgott(0), npcDialogoActual(""),
		  mensajeTemporal(""), ticksMensajeTemporal(0),
		  tiempoInicio(0), puntosMisiones(0), promptFlotante(""),
		  enDialogo(false), estadoDialogo(0), enModalPersonajes(false),
		  seleccionModal(0), enModalMisiones(false), enDetalleMision(false),
		  seleccionMision(0), enModalInventario(false), seleccionInventario(0),
		  paredPiedraDestruida(false), idCuartoActual(0), cartelCuarto(""), ticksCartelCuarto(0),
		  objetivoActual("Hablar con Maestro Qifrey"),
		  mostrarEstadisticasFin(false), avanzaSiguienteNivel(false) {}

	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false),
		  mapa(nullptr), protagonista(nullptr), qifrey(nullptr), agott(nullptr), richeh(nullptr),
		  mapaRicheh(nullptr), enCuartoRicheh(false), cocoPrevX(0), cocoPrevY(0),
		  dioVaraRicheh(false), transicionBajando(false), transicionSubiendo(false),
		  indiceCajaLibro(-1), indiceCajaPozo(-1), libroEncontrado(false),
		  pozoEncontrado(false), cajasMovidasContador(0), pozoX(-1), pozoY(-1),
		  primeraVezTorreAgott(true), respuestaInicialAgott(0), npcDialogoActual(""),
		  mensajeTemporal(""), ticksMensajeTemporal(0),
		  tiempoInicio(0), puntosMisiones(0), promptFlotante(""),
		  enDialogo(false), estadoDialogo(0), enModalPersonajes(false),
		  seleccionModal(0), enModalMisiones(false), enDetalleMision(false),
		  seleccionMision(0), enModalInventario(false), seleccionInventario(0),
		  paredPiedraDestruida(false), idCuartoActual(0), cartelCuarto(""), ticksCartelCuarto(0),
		  objetivoActual("Hablar con Maestro Qifrey"),
		  mostrarEstadisticasFin(false), avanzaSiguienteNivel(false) {
		this->mapa = new Mapa(filasMapa, columnasMapa);
	}

	~Nivel() {
		if (this->mapa != nullptr) {
			delete this->mapa;
			this->mapa = nullptr;
		}
		if (this->mapaRicheh != nullptr) {
			delete this->mapaRicheh;
			this->mapaRicheh = nullptr;
		}
		if (this->protagonista != nullptr) {
			delete this->protagonista;
			this->protagonista = nullptr;
		}
		if (this->qifrey != nullptr) {
			delete this->qifrey;
			this->qifrey = nullptr;
		}
		if (this->agott != nullptr) {
			delete this->agott;
			this->agott = nullptr;
		}
		if (this->richeh != nullptr) {
			delete this->richeh;
			this->richeh = nullptr;
		}
		limpiarItemsSuelo();
		limpiarCajas();
	}

	void entrarCuartoRicheh() {
		if (protagonista != nullptr) {
			cocoPrevX = protagonista->getX();
			cocoPrevY = protagonista->getY();
			protagonista->setX(10);
			protagonista->setY(6);
		}
		enCuartoRicheh = true;
		idCuartoActual = 6;
		cartelCuarto = "[SOTANO DE RICHEH]";
		ticksCartelCuarto = 75;
	}

	void salirCuartoRicheh() {
		if (protagonista != nullptr) {
			protagonista->setX(cocoPrevX);
			protagonista->setY(cocoPrevY);
		}
		enCuartoRicheh = false;
		actualizarCuartoActual(protagonista->getX(), protagonista->getY());
	}

	bool getEnCuartoRicheh() const { return this->enCuartoRicheh; }
	NPC* getRicheh() { return this->richeh; }
	bool getTransicionBajando() const { return this->transicionBajando; }
	bool getTransicionSubiendo() const { return this->transicionSubiendo; }
	void setTransicionBajando(bool tb) { this->transicionBajando = tb; }
	void setTransicionSubiendo(bool ts) { this->transicionSubiendo = ts; }

	Protagonista* getProtagonista() { return this->protagonista; }
	NPC* getQifrey() { return this->qifrey; }
	NPC* getAgott() { return this->agott; }
	const std::vector<Caja*>& getCajas() const { return this->cajas; }
	bool getLibroEncontrado() const { return this->libroEncontrado; }
	bool getPozoEncontrado() const { return this->pozoEncontrado; }
	int getPozoX() const { return this->pozoX; }
	int getPozoY() const { return this->pozoY; }
	int getCajasMovidasContador() const { return this->cajasMovidasContador; }
	const std::vector<ItemMagico*>& getItemsSuelo() const { return this->itemsSuelo; }
	int getNumeroNivel() const { return this->numeroNivel; }
	std::string getNombreNivel() const { return this->nombreNivel; }
	bool getCompletado() const { return this->completado; }
	Mapa* getMapa() {
		if (enCuartoRicheh && mapaRicheh != nullptr) {
			return this->mapaRicheh;
		}
		return this->mapa;
	}
	const std::string& getPromptFlotante() const { return this->promptFlotante; }
	bool getEnDialogo() const { return this->enDialogo; }
	int getEstadoDialogo() const { return this->estadoDialogo; }
	bool getEnModalPersonajes() const { return this->enModalPersonajes; }
	int getSeleccionModal() const { return this->seleccionModal; }
	bool getEnModalMisiones() const { return this->enModalMisiones; }
	bool getEnDetalleMision() const { return this->enDetalleMision; }
	int getSeleccionMision() const { return this->seleccionMision; }
	bool getEnModalInventario() const { return this->enModalInventario; }
	int getSeleccionInventario() const { return this->seleccionInventario; }
	bool getParedPiedraDestruida() const { return this->paredPiedraDestruida; }
	int getIdCuartoActual() const { return this->idCuartoActual; }
	const std::string& getCartelCuarto() const { return this->cartelCuarto; }
	const std::string& getObjetivoActual() const { return this->objetivoActual; }
	bool getMostrarEstadisticasFin() const { return this->mostrarEstadisticasFin; }
	bool getAvanzaSiguienteNivel() const { return this->avanzaSiguienteNivel; }

	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) { this->completado = estado; }
	void setEnDialogo(bool ed) { this->enDialogo = ed; }
	void setEstadoDialogo(int ed) { this->estadoDialogo = ed; }
	void setEnModalPersonajes(bool emp) { this->enModalPersonajes = emp; }
	void setEnModalMisiones(bool emm) { this->enModalMisiones = emm; }
	void setEnDetalleMision(bool edm) { this->enDetalleMision = edm; }
	void setSeleccionMision(int sm) { this->seleccionMision = sm; }
	void setEnModalInventario(bool emi) { this->enModalInventario = emi; }
	void setSeleccionInventario(int si) { this->seleccionInventario = si; }
	void setParedPiedraDestruida(bool val) { this->paredPiedraDestruida = val; }
	void setMostrarEstadisticasFin(bool val) { this->mostrarEstadisticasFin = val; }
	void setAvanzaSiguienteNivel(bool val) { this->avanzaSiguienteNivel = val; }

	int determinarCuarto(int px, int py) const {
		if (px >= 38 && px <= 125 && py >= 152 && py <= 193) {
			return 1;
		}
		if (px >= 458 && px <= 575 && py >= 11 && py <= 49) {
			return 2;
		}
		if (px >= 465 && px <= 578 && py >= 63 && py <= 134) {
			return 3;
		}
		if (px >= 458 && px <= 580 && py >= 143 && py <= 186) {
			return 4;
		}
		if (px >= 38 && px <= 125 && py >= 10 && py <= 45) {
			return 5;
		}
		return 0;
	}

	void actualizarCuartoActual(int px, int py) {
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (idCuartoActual == 1) {
				cartelCuarto = "[LA CHOZA DE HECHIZOS]";
				ticksCartelCuarto = 75;
			} else if (idCuartoActual == 2) {
				cartelCuarto = "[EL ALMACEN]";
				ticksCartelCuarto = 75;
			} else if (idCuartoActual == 3) {
				cartelCuarto = "[TORRE DE AGOTT]";
				ticksCartelCuarto = 75;
				if (primeraVezTorreAgott) {
					primeraVezTorreAgott = false;
					enDialogo = true;
					estadoDialogo = 200;
					npcDialogoActual = "Agott";
					if (agott != nullptr) {
						agott->setYaHablo(true);
					}
				}
			} else if (idCuartoActual == 4) {
				cartelCuarto = "[BOSQUE DE PLATA]";
				ticksCartelCuarto = 75;
			} else if (idCuartoActual == 5) {
				cartelCuarto = "[HABITACION DE COCO]";
				ticksCartelCuarto = 75;
			} else {
				ticksCartelCuarto = 0;
				cartelCuarto = "";
			}
		}
	}

	void inciarNivel() {
		if (this->mapa == nullptr) return;
		this->tiempoInicio = clock();
		this->puntosMisiones = 0;
		this->enDialogo = false;
		this->estadoDialogo = 0;
		this->enModalPersonajes = false;
		this->seleccionModal = 0;
		this->enModalMisiones = false;
		this->enDetalleMision = false;
		this->seleccionMision = 0;
		this->enModalInventario = false;
		this->seleccionInventario = 0;
		this->paredPiedraDestruida = false;
		this->idCuartoActual = 0;
		this->cartelCuarto = "";
		this->ticksCartelCuarto = 0;
		this->promptFlotante = "";
		this->mostrarEstadisticasFin = false;
		this->avanzaSiguienteNivel = false;
		this->completado = false;
		this->primeraVezTorreAgott = true;
		this->respuestaInicialAgott = 0;
		this->libroEncontrado = false;
		this->pozoEncontrado = false;
		this->cajasMovidasContador = 0;
		this->pozoX = -1;
		this->pozoY = -1;
		this->npcDialogoActual = "";
		this->mensajeTemporal = "";
		this->ticksMensajeTemporal = 0;
		this->enCuartoRicheh = false;
		this->dioVaraRicheh = false;
		this->transicionBajando = false;
		this->transicionSubiendo = false;

		limpiarItemsSuelo();
		limpiarCajas();

		if (this->protagonista != nullptr) {
			delete this->protagonista;
			this->protagonista = nullptr;
		}
		if (this->qifrey != nullptr) {
			delete this->qifrey;
			this->qifrey = nullptr;
		}
		if (this->agott != nullptr) {
			delete this->agott;
			this->agott = nullptr;
		}
		if (this->richeh != nullptr) {
			delete this->richeh;
			this->richeh = nullptr;
		}
		if (this->mapaRicheh != nullptr) {
			delete this->mapaRicheh;
			this->mapaRicheh = nullptr;
		}

		if (this->numeroNivel == 1) {
			std::vector<std::string> matrizNivel1;
			MapaNivel1::cargarMatriz(matrizNivel1);
			this->mapa->cargarMatriz(matrizNivel1);

			this->mapaRicheh = new Mapa(25, 82);
			std::vector<std::string> matrizRicheh;
			CuartoRicheh::cargarMatriz(matrizRicheh);
			this->mapaRicheh->cargarMatriz(matrizRicheh);

			for (int wy = 27; wy <= 35; wy++) {
				this->mapa->setCaracter(459, wy, '%');
				this->mapa->setCaracter(460, wy, '%');
			}

			this->protagonista = new Protagonista(50, 20, "Coco", 3, 1);
			this->qifrey = new NPC(78, 162, "Qifrey", 'Q', "Maestro Hechicero", false);
			this->agott = new NPC(515, 68, "Agott", 'A', "Aprendiz de Maga", false);
			this->agott->setConfianza(2);
			this->richeh = new NPC(55, 18, "Richeh", 'R', "Aprendiz de Maga", false);

			itemsSuelo.push_back(new ItemMagico(485, 20, "Tela", "Pieza de tela especial apta para tejer encantamientos.", "Material de Crafteo", false));

			int intentos = 0;
			while (cajas.size() < 18 && intentos < 2000) {
				intentos++;
				int bx = 472 + (rand() % (560 - 472 + 1));
				int by = 76 + (rand() % (122 - 76 + 1));
				bool solapa = false;
				for (size_t i = 0; i < cajas.size(); i++) {
					if (abs(bx - cajas[i]->getX()) < 4 && abs(by - cajas[i]->getY()) < 4) {
						solapa = true;
						break;
					}
				}
				if (abs(bx - 515) < 5 && abs(by - 68) < 5) {
					solapa = true;
				}
				if (!solapa) {
					cajas.push_back(new Caja(bx, by));
				}
			}
			if (cajas.size() >= 2) {
				indiceCajaLibro = rand() % cajas.size();
				indiceCajaPozo = (indiceCajaLibro + 1 + (rand() % (cajas.size() - 1))) % cajas.size();
			}

			actualizarCuartoActual(50, 20);
			this->objetivoActual = "Hablar con Maestro Qifrey";
		} else if (this->numeroNivel == 2) {
			std::vector<std::string> matrizNivel2;
			MapaNivel1::cargarMatriz(matrizNivel2);
			this->mapa->cargarMatriz(matrizNivel2);

			this->protagonista = new Protagonista(50, 20, "Tartah", 3, 2);
			this->qifrey = new NPC(78, 162, "Qifrey", 'Q', "Maestro Hechicero", false);
			this->objetivoActual = "Tierras Prohibidas - Fase 2";
		} else {
			std::vector<std::string> matrizNivel3;
			MapaNivel1::cargarMatriz(matrizNivel3);
			this->mapa->cargarMatriz(matrizNivel3);

			this->protagonista = new Protagonista(50, 20, "Coustas", 3, 3);
			this->qifrey = new NPC(78, 162, "Qifrey", 'Q', "Maestro Hechicero", false);
			this->objetivoActual = "Gran Arbol de Plata - Fase 3";
		}
	}

	void obtenerDatosMisiones(std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas) {
		titulos.clear();
		descripciones.clear();
		estados.clear();
		desbloqueadas.clear();

		Inventario* inv = (protagonista != nullptr) ? protagonista->getInventario() : nullptr;
		bool habloConQifrey = (qifrey != nullptr && (qifrey->getDioTinta() || qifrey->getCrafteoCapa() || estadoDialogo > 1));
		bool tieneTela = (inv != nullptr && inv->tieneItem("Tela"));
		bool tieneTinta = (inv != nullptr && inv->tieneItem("Tinta magica"));
		bool tieneLibro = (inv != nullptr && inv->tieneItem("Libro de hechizos"));
		bool crafteoCapa = (qifrey != nullptr && qifrey->getCrafteoCapa());
		bool tieneMateriales = (tieneTela && tieneTinta && tieneLibro) || crafteoCapa;

		titulos.push_back("Hablar con Qifrey");
		descripciones.push_back("Encuentra al Maestro Qifrey en el atelier y dialoga con el sobre el crafteo de la Capa Magica.");
		desbloqueadas.push_back(true);
		if (habloConQifrey) {
			estados.push_back("COMPLETADA");
		} else {
			estados.push_back("EN PROGRESO");
		}

		titulos.push_back("Conseguir Materiales");
		descripciones.push_back("Recolecta en el atelier los 3 materiales indispensables: Tela, Libro de hechizos y Tinta magica.");
		if (habloConQifrey) {
			desbloqueadas.push_back(true);
			if (tieneMateriales) {
				estados.push_back("COMPLETADA");
			} else {
				estados.push_back("EN PROGRESO");
			}
		} else {
			desbloqueadas.push_back(false);
			estados.push_back("BLOQUEADA");
		}

		titulos.push_back("Craftear Capa Magica");
		descripciones.push_back("Regresa con Maestro Qifrey y entrega los materiales para confeccionar la legendaria Capa Magica.");
		if (tieneMateriales) {
			desbloqueadas.push_back(true);
			if (crafteoCapa) {
				estados.push_back("COMPLETADA");
			} else {
				estados.push_back("EN PROGRESO");
			}
		} else {
			desbloqueadas.push_back(false);
			estados.push_back("BLOQUEADA");
		}
	}

	int getSegundosTranscurridos() const {
		if (tiempoInicio == 0) return 0;
		return (int)((clock() - tiempoInicio) / CLOCKS_PER_SEC);
	}

	int getBonoTiempo() const {
		int seg = getSegundosTranscurridos();
		if (seg <= 60) return 50;
		if (seg <= 120) return 30;
		if (seg <= 180) return 20;
		if (seg <= 240) return 10;
		return 5;
	}

	int getPuntajeTotalNivel() const {
		return puntosMisiones + getBonoTiempo();
	}

	void sumarPuntosMision(int p) {
		this->puntosMisiones += p;
	}

	int getPuntosMisiones() const {
		return this->puntosMisiones;
	}

	bool verificarObjetivo() {
		return this->avanzaSiguienteNivel;
	}

	void obtenerDatosDialogo(std::string& hablante, std::string& rol, int& confianza,
	                         std::vector<std::string>& lineas,
	                         std::vector<std::string>& opciones) {
		if (estadoDialogo >= 300) {
			hablante = (richeh != nullptr) ? richeh->getNombre() : "Richeh";
			rol = (richeh != nullptr) ? richeh->getRolPerspectiva() : "Aprendiz de Maga";
			confianza = (richeh != nullptr) ? richeh->getConfianza() : 0;
		} else if (estadoDialogo >= 200) {
			hablante = (agott != nullptr) ? agott->getNombre() : "Agott";
			rol = (agott != nullptr) ? agott->getRolPerspectiva() : "Aprendiz de Maga";
			confianza = (agott != nullptr) ? agott->getConfianza() : 0;
		} else {
			hablante = (qifrey != nullptr) ? qifrey->getNombre() : "Qifrey";
			rol = (qifrey != nullptr) ? qifrey->getRolPerspectiva() : "Maestro Hechicero";
			confianza = (qifrey != nullptr) ? qifrey->getConfianza() : 0;
		}
		lineas.clear();
		opciones.clear();

		Inventario* inv = (protagonista != nullptr) ? protagonista->getInventario() : nullptr;

		switch (estadoDialogo) {
		case 1:
			lineas.push_back("\"Hola, Coco. Que necesitas en el taller hoy?\"");
			opciones.push_back("[1] Maestro Qifrey, podrias ayudarme a craftear la Capa Magica?");
			opciones.push_back("[2] Solo venia a explorar el taller y ver tus libros.");
			opciones.push_back("[3] Maestro, necesito materiales especiales para mis practicas.");
			break;

		case 10:
			if (inv != nullptr) {
				bool tieneTela = inv->tieneItem("Tela");
				bool tieneTinta = inv->tieneItem("Tinta magica");
				bool tieneLibro = inv->tieneItem("Libro de hechizos");
				int total = (tieneTela ? 1 : 0) + (tieneTinta ? 1 : 0) + (tieneLibro ? 1 : 0);

				if (total == 0) {
					lineas.push_back("\"Para la Capa Magica necesito 3 items: Tela, Tinta magica y Libro.\"");
					lineas.push_back("\"Aun no tienes ninguno. Busca en el atelier y el almacen abandonado!\"");
					opciones.push_back("[1] Esta bien, ire a buscarlos por el atelier.");
				} else {
					lineas.push_back("\"Te faltan materiales para craftear la Capa Magica.\"");
					std::string faltantes = "Aun necesitas encontrar: ";
					if (!tieneTela) faltantes += "[Tela] ";
					if (!tieneTinta) faltantes += "[Tinta magica] ";
					if (!tieneLibro) faltantes += "[Libro de hechizos] ";
					lineas.push_back(faltantes);
					lineas.push_back("\"Vuelve cuando tengas los 3 ingredientes completos!\"");
					opciones.push_back("[1] Entendido, buscare lo que falta.");
				}
			}
			break;

		case 11:
			lineas.push_back("\"Esta bien, te hare la capa!\"");
			lineas.push_back("");
			lineas.push_back("Crafteando capa magica...");
			lineas.push_back("");
			lineas.push_back("\"Aqui esta, te dare esta capa pero ojo... usalo responsablemente!\"");
			opciones.push_back("[1] Entendido!");
			break;

		case 12:
			lineas.push_back("[CRAFTEO EXITOSO: Capa magica obtenida]");
			lineas.push_back("Se consumieron: Tela, Tinta magica y Libro de hechizos.");
			lineas.push_back("La Capa Magica ha sido equipada y agregada a tu inventario.");
			opciones.push_back("[1] Muchas gracias Maestro Qifrey!");
			break;

		case 15:
			lineas.push_back("\"Te queda excelente la Capa Magica, Coco.\"");
			lineas.push_back("\"Recuerda usar tus alas de aprendiz con sabiduria y responsabilidad.\"");
			opciones.push_back("[1] Gracias Maestro Qifrey!");
			break;

		case 20:
			lineas.push_back("\"Eres bienvenida en el taller siempre, Coco.\"");
			lineas.push_back("\"Cuidate de las corrientes del gran rio y cruza siempre por los puentes.\"");
			opciones.push_back("[1] Gracias por el consejo, Maestro.");
			break;

		case 30:
			lineas.push_back("\"Las practicas de hechiceria requieren precision y paciencia.\"");
			lineas.push_back("\"Que tipo de material magico estas buscando exactamente?\"");
			opciones.push_back("[1] Busco una tinta que reaccione al flujo magico del pergamino.");
			opciones.push_back("[2] Cualquier material basico me servira para practicar.");
			break;

		case 31:
			lineas.push_back("\"La tinta magica de plata es muy delicada y poderosa.\"");
			lineas.push_back("\"Sabras usarla con cuidado y verdadero respeto al atelier?\"");
			opciones.push_back("[1] Prometo seguir todas las reglas del atelier y ser responsable.");
			opciones.push_back("[2] Intentare tener cuidado, aunque a veces me cuesta.");
			break;

		case 32:
			lineas.push_back("\"Bien dicho, Coco. Veo determinacion y honestidad en tus ojos.\"");
			lineas.push_back("\"Te doy este item: Tinta magica.\"");
			opciones.push_back("[1] Muchas gracias Qifrey, me servira de mucho!");
			break;

		case 33:
			lineas.push_back("[HAS OBTENIDO: Tinta magica]");
			lineas.push_back("Se ha agregado a tu inventario.");
			lineas.push_back("Tu vinculo y confianza con Maestro Qifrey han aumentado!");
			opciones.push_back("[1] Continuar explorando");
			break;

		case 34:
			lineas.push_back("\"Ya te he entregado la Tinta magica, Coco.\"");
			lineas.push_back("\"Revisa tu mochila y dale buen uso en tus pergaminos.\"");
			opciones.push_back("[1] Entendido Maestro.");
			break;

		case 99:
			lineas.push_back("[INVENTARIO LLENO: Capacidad maxima 3 items alcanzada]");
			lineas.push_back("No puedes recibir mas items en este momento.");
			opciones.push_back("[1] Volver");
			break;

		case 200:
			lineas.push_back("\"Vaya vaya... miren a quien tenemos aqui.\"");
			opciones.push_back("[1] Siguiente");
			break;

		case 201:
			lineas.push_back("\"A la joven y pequena Coco, porque entraste a mi torre?\"");
			opciones.push_back("[1] Necesito encontrar un libro");
			opciones.push_back("[2] A ti que te importa, Agott?");
			break;

		case 202:
			lineas.push_back("\"Puedes encontrarlo en este resto de cajas si quieres, al final... solo son basura\"");
			opciones.push_back("[1] Entendido");
			break;

		case 203:
			lineas.push_back("\"Largate de aqui!\"");
			opciones.push_back("[1] Ya me voy...");
			break;

		case 210:
			lineas.push_back("\"Que paso ahora, nina?\"");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Solo pasaba por aqui, ya encontre el libro, Agott.");
			} else {
				opciones.push_back("[1] Sigo buscando el libro, necesito ayuda...");
			}
			break;

		case 211:
			lineas.push_back("\"No esperaba que lo encuentres en esta basura jaja.\"");
			opciones.push_back("[1] Continuar");
			break;

		case 212:
			lineas.push_back("\"Sabia que no eras util para eso JAJAJA, prueba empujando las cajas.\"");
			opciones.push_back("[1] Gracias por nada...");
			break;

		case 220:
			lineas.push_back("\"Porque sigues aqui, Coco?? No eres bienvenida.\"");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Nada, solo queria burlarme de tu cara.");
			} else {
				opciones.push_back("[1] Sigo buscando algo, deja de molestar!");
			}
			break;

		case 221:
			lineas.push_back("\"Estupida nina!\"");
			opciones.push_back("[1] Salir");
			break;

		case 222:
			lineas.push_back("\"Que demonios estas buscando??\"");
			opciones.push_back("[1] QUE- TE- IMPORTA!!!!");
			break;

		case 223:
			lineas.push_back("\"...\"");
			opciones.push_back("[1] Salir");
			break;

		case 300:
			if (!dioVaraRicheh) {
				lineas.push_back("\"Hola Coco... Que sorpresa verte por aqui abajo.\"");
				lineas.push_back("\"Toma mi vieja vara magica. Con ella podras\"");
				lineas.push_back("\"derribar muros de piedra lanzando fuego.\"");
				opciones.push_back("[1] Muchas gracias Richeh!");
			} else {
				lineas.push_back("\"Usa la vara magica con sabiduria, Coco.\"");
				lineas.push_back("\"Recuerda que el fuego magico responde a tu voluntad.\"");
				opciones.push_back("[1] Entendido Richeh!");
			}
			break;

		default:
			lineas.push_back("\"Continua tu aprendizaje con dedicacion, Coco.\"");
			opciones.push_back("[1] Salir");
			break;
		}
	}

	void procesarOpcionDialogo(int opcion) {
		Inventario* inv = (protagonista != nullptr) ? protagonista->getInventario() : nullptr;

		if (estadoDialogo == 1) {
			if (opcion == 1) {
				if (qifrey != nullptr && qifrey->getCrafteoCapa()) {
					estadoDialogo = 15;
				} else if (inv != nullptr) {
					bool tieneTela = inv->tieneItem("Tela");
					bool tieneTinta = inv->tieneItem("Tinta magica");
					bool tieneLibro = inv->tieneItem("Libro de hechizos");
					if (tieneTela && tieneTinta && tieneLibro) {
						estadoDialogo = 11;
					} else {
						estadoDialogo = 10;
					}
				}
			} else if (opcion == 2) {
				estadoDialogo = 20;
			} else if (opcion == 3) {
				if (qifrey != nullptr && qifrey->getDioTinta()) {
					estadoDialogo = 34;
				} else {
					estadoDialogo = 30;
				}
			}
		} else if (estadoDialogo == 10) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 11) {
			if (opcion == 1) {
				if (inv != nullptr) {
					inv->removerItem("Tela");
					inv->removerItem("Tinta magica");
					inv->removerItem("Libro de hechizos");
					inv->agregarItem(new ItemMagico(0, 0, "Capa magica", "Capa magica que otorga la habilidad de planear por los cielos.", "Equipamiento Magico", true));
				}
				if (protagonista != nullptr) {
					protagonista->setTieneCapaVuelo(true);
				}
				if (qifrey != nullptr) {
					qifrey->setCrafteoCapa(true);
					qifrey->setConfianza(2);
				}
				sumarPuntosMision(100);
				this->objetivoActual = "Capa Magica crafteada! Mision Cumplida";
				estadoDialogo = 12;
			}
		} else if (estadoDialogo == 12) {
			if (opcion == 1) {
				enDialogo = false;
				estadoDialogo = 0;
				completado = true;
				mostrarEstadisticasFin = true;
			}
		} else if (estadoDialogo == 15 || estadoDialogo == 20) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 30) {
			if (opcion == 1) {
				estadoDialogo = 31;
			} else {
				enDialogo = false;
				estadoDialogo = 0;
			}
		} else if (estadoDialogo == 31) {
			if (opcion == 1) {
				estadoDialogo = 32;
			} else {
				enDialogo = false;
				estadoDialogo = 0;
			}
		} else if (estadoDialogo == 32) {
			if (opcion == 1) {
				if (inv != nullptr) {
					if (inv->tieneItem("Tinta magica")) {
						estadoDialogo = 34;
					} else if (inv->agregarItem(new ItemMagico(0, 0, "Tinta magica", "Tinta de plata otorgada por Qifrey para trazar sellos.", "Consumible Magico", true))) {
						if (qifrey != nullptr) {
							qifrey->setDioTinta(true);
							if (qifrey->getConfianza() < 1) qifrey->setConfianza(1);
						}
						sumarPuntosMision(50);
						this->objetivoActual = "Buscar Tela y Libro de hechizos";
						estadoDialogo = 33;
					} else {
						estadoDialogo = 99;
					}
				}
			}
		} else if (estadoDialogo == 33 || estadoDialogo == 34 || estadoDialogo == 99) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 200) {
			estadoDialogo = 201;
		} else if (estadoDialogo == 201) {
			if (opcion == 1) {
				respuestaInicialAgott = 1;
				estadoDialogo = 202;
			} else if (opcion == 2) {
				respuestaInicialAgott = 2;
				if (agott != nullptr && agott->getConfianza() > 0) {
					agott->setConfianza(agott->getConfianza() - 1);
				}
				estadoDialogo = 203;
			}
		} else if (estadoDialogo == 202 || estadoDialogo == 203) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 210) {
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				estadoDialogo = 211;
			} else {
				estadoDialogo = 212;
			}
		} else if (estadoDialogo == 211 || estadoDialogo == 212) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 220) {
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				if (agott != nullptr && agott->getConfianza() > 0) {
					agott->setConfianza(agott->getConfianza() - 1);
				}
				estadoDialogo = 221;
			} else {
				estadoDialogo = 222;
			}
		} else if (estadoDialogo == 221) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 222) {
			if (agott != nullptr && agott->getConfianza() > 0) {
				agott->setConfianza(agott->getConfianza() - 1);
			}
			estadoDialogo = 223;
		} else if (estadoDialogo == 223) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 300) {
			if (!dioVaraRicheh) {
				dioVaraRicheh = true;
				if (inv != nullptr && !inv->tieneItem("Vara magica")) {
					inv->agregarItem(new ItemMagico(0, 0, "Vara magica", "Vara de la infancia de Richeh con la que practicaba de pequena. Lanza bolas de fuego magico para destruir obstaculos de piedra.", "Herramienta Magica", true));
					sumarPuntosMision(25);
					promptFlotante = "[Recogiste: Vara magica]";
				}
				if (richeh != nullptr) {
					richeh->setYaHablo(true);
					richeh->setConfianza(2);
				}
			}
			enDialogo = false;
			estadoDialogo = 0;
		} else {
			enDialogo = false;
			estadoDialogo = 0;
		}
	}

	void actualizarProximidad() {
		if (protagonista == nullptr) return;
		if (enModalPersonajes || enModalMisiones || enModalInventario || enDialogo || mostrarEstadisticasFin) {
			promptFlotante = "";
			return;
		}

		if (ticksMensajeTemporal > 0 && !mensajeTemporal.empty()) {
			promptFlotante = mensajeTemporal;
			return;
		}

		int px = protagonista->getX();
		int py = protagonista->getY();

		promptFlotante = "";

		if (enCuartoRicheh) {
			if (abs(px - 8) <= 3 && abs(py - 6) <= 3) {
				promptFlotante = "[E] Subir a la torre";
				return;
			}
			if (richeh != nullptr) {
				int rx = richeh->getX();
				int ry = richeh->getY();
				if (px + 1 >= rx - 2 && px <= rx + 3 && py + 1 >= ry - 2 && py <= ry + 3) {
					promptFlotante = "[ENTER] Hablar con Richeh";
					return;
				}
			}
			if (ticksCartelCuarto > 0 && !cartelCuarto.empty()) {
				promptFlotante = cartelCuarto;
			}
			return;
		}

		if (!paredPiedraDestruida && px >= 445 && px <= 463 && py >= 25 && py <= 37) {
			Inventario* inv = protagonista->getInventario();
			bool tieneVara = (inv != nullptr && inv->tieneItem("Vara magica"));
			if (tieneVara) {
				promptFlotante = "[ENTER] Usar Vara magica para derribar pared";
			} else {
				promptFlotante = "Coco: \"Este lugar parece estar bloqueado, puedo derribarlo pero necesito magia...\"";
			}
			return;
		}

		if (qifrey != nullptr) {
			int qx = qifrey->getX();
			int qy = qifrey->getY();
			if (px + 1 >= qx - 2 && px <= qx + 3 && py + 1 >= qy - 2 && py <= qy + 3) {
				promptFlotante = "[ENTER] Interactuar con Qifrey";
				return;
			}
		}

		if (agott != nullptr) {
			int ax = agott->getX();
			int ay = agott->getY();
			if (px + 1 >= ax - 2 && px <= ax + 3 && py + 1 >= ay - 2 && py <= ay + 3) {
				promptFlotante = "[ENTER] Hablar con Agott";
				return;
			}
		}

		if (pozoEncontrado) {
			if (px + 1 >= pozoX - 2 && px <= pozoX + 3 && py + 1 >= pozoY - 2 && py <= pozoY + 3) {
				promptFlotante = "[E] Bajar al pozo";
				return;
			}
		}

		for (size_t i = 0; i < itemsSuelo.size(); i++) {
			if (itemsSuelo[i] != nullptr && !itemsSuelo[i]->getRecogido()) {
				int ix = itemsSuelo[i]->getX();
				int iy = itemsSuelo[i]->getY();
				if (abs(px - ix) <= 2 && abs(py - iy) <= 2) {
					promptFlotante = "[ENTER] Recoger: " + itemsSuelo[i]->getNombre();
					return;
				}
			}
		}

		if (ticksCartelCuarto > 0 && !cartelCuarto.empty()) {
			promptFlotante = cartelCuarto;
		}
	}

	bool actualizar() {
		if (this->protagonista == nullptr) return false;
		bool huboCambio = false;

		int framePrevio = this->protagonista->getFrameActual();
		this->protagonista->actualizarAnimacion(30);
		if (this->qifrey != nullptr) {
			this->qifrey->actualizarAnimacion(30);
		}
		if (this->agott != nullptr) {
			this->agott->actualizarAnimacion(30);
		}
		if (this->richeh != nullptr) {
			this->richeh->actualizarAnimacion(30);
		}
		if (this->protagonista->getFrameActual() != framePrevio) {
			huboCambio = true;
		}

		if (ticksCartelCuarto > 0) {
			ticksCartelCuarto--;
		}
		if (ticksMensajeTemporal > 0) {
			ticksMensajeTemporal--;
		}

#ifdef _WIN32
		if (_kbhit()) {
			int tecla = _getch();

			if (mostrarEstadisticasFin) {
				if (tecla == '1' || tecla == 13) {
					mostrarEstadisticasFin = false;
					avanzaSiguienteNivel = true;
					huboCambio = true;
				} else if (tecla == 'c' || tecla == 'C' || tecla == 27) {
					mostrarEstadisticasFin = false;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (enModalPersonajes) {
				if (tecla == 'w' || tecla == 'W') {
					if (seleccionModal > 0) {
						seleccionModal--;
						huboCambio = true;
					}
				} else if (tecla == 's' || tecla == 'S') {
					if (seleccionModal < 5) {
						seleccionModal++;
						huboCambio = true;
					}
				} else if (tecla == 'p' || tecla == 'P' || tecla == 27 || tecla == 13) {
					enModalPersonajes = false;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (enModalMisiones) {
				if (!enDetalleMision) {
					if (tecla == 'w' || tecla == 'W') {
						if (seleccionMision > 0) {
							seleccionMision--;
							huboCambio = true;
						}
					} else if (tecla == 's' || tecla == 'S') {
						if (seleccionMision < 2) {
							seleccionMision++;
							huboCambio = true;
						}
					} else if (tecla == '1') {
						seleccionMision = 0;
						enDetalleMision = true;
						huboCambio = true;
					} else if (tecla == '2') {
						seleccionMision = 1;
						enDetalleMision = true;
						huboCambio = true;
					} else if (tecla == '3') {
						seleccionMision = 2;
						enDetalleMision = true;
						huboCambio = true;
					} else if (tecla == 13) {
						enDetalleMision = true;
						huboCambio = true;
					} else if (tecla == 'm' || tecla == 'M' || tecla == 27) {
						enModalMisiones = false;
						enDetalleMision = false;
						huboCambio = true;
					}
				} else {
					if (tecla == 13 || tecla == 27 || tecla == 'm' || tecla == 'M') {
						enDetalleMision = false;
						huboCambio = true;
					}
				}
				return huboCambio;
			}

			if (enModalInventario) {
				if (tecla == 'w' || tecla == 'W') {
					if (seleccionInventario > 0) {
						seleccionInventario--;
						huboCambio = true;
					}
				} else if (tecla == 's' || tecla == 'S') {
					if (seleccionInventario < 3) {
						seleccionInventario++;
						huboCambio = true;
					}
				} else if (tecla == '1') {
					seleccionInventario = 0;
					huboCambio = true;
				} else if (tecla == '2') {
					seleccionInventario = 1;
					huboCambio = true;
				} else if (tecla == '3') {
					seleccionInventario = 2;
					huboCambio = true;
				} else if (tecla == '4') {
					seleccionInventario = 3;
					huboCambio = true;
				} else if (tecla == 'i' || tecla == 'I' || tecla == 27 || tecla == 13) {
					enModalInventario = false;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (enDialogo) {
				if (tecla == '1') {
					procesarOpcionDialogo(1);
					huboCambio = true;
				} else if (tecla == '2') {
					procesarOpcionDialogo(2);
					huboCambio = true;
				} else if (tecla == '3') {
					procesarOpcionDialogo(3);
					huboCambio = true;
				} else if (tecla == 13) {
					procesarOpcionDialogo(1);
					huboCambio = true;
				} else if (tecla == 27) {
					enDialogo = false;
					estadoDialogo = 0;
					huboCambio = true;
				}
				actualizarProximidad();
				return huboCambio;
			}

			if (tecla == 'p' || tecla == 'P') {
				enModalPersonajes = true;
				seleccionModal = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (tecla == 'm' || tecla == 'M') {
				enModalMisiones = true;
				enDetalleMision = false;
				seleccionMision = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (tecla == 'i' || tecla == 'I') {
				enModalInventario = true;
				seleccionInventario = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (enCuartoRicheh) {
				if (tecla == 'e' || tecla == 'E') {
					int px = protagonista->getX();
					int py = protagonista->getY();
					if (abs(px - 8) <= 3 && abs(py - 6) <= 3) {
						transicionSubiendo = true;
						huboCambio = true;
						return huboCambio;
					}
				}

				if (tecla == 13) {
					int px = protagonista->getX();
					int py = protagonista->getY();
					if (richeh != nullptr) {
						int rx = richeh->getX();
						int ry = richeh->getY();
						if (px + 1 >= rx - 2 && px <= rx + 3 && py + 1 >= ry - 2 && py <= ry + 3) {
							enDialogo = true;
							npcDialogoActual = "Richeh";
							estadoDialogo = 300;
							huboCambio = true;
							return huboCambio;
						}
					}
				}

				int dx = 0;
				int dy = 0;
				if (tecla == 'w' || tecla == 'W') dy = -1;
				else if (tecla == 's' || tecla == 'S') dy = 1;
				else if (tecla == 'a' || tecla == 'A') dx = -1;
				else if (tecla == 'd' || tecla == 'D') dx = 1;

				if (dx != 0 || dy != 0) {
					int nx = protagonista->getX() + dx;
					int ny = protagonista->getY() + dy;
					bool colision = false;
					if (mapaRicheh != nullptr) {
						for (int r = 0; r < 2; r++) {
							for (int c = 0; c < 2; c++) {
								if (!mapaRicheh->esPosicionValida(nx + c, ny + r)) {
									colision = true;
									break;
								}
							}
							if (colision) break;
						}
					}
					if (!colision && richeh != nullptr) {
						int rx = richeh->getX();
						int ry = richeh->getY();
						if (nx + 1 >= rx && nx <= rx + 1 && ny + 1 >= ry && ny <= ry + 1) {
							colision = true;
						}
					}
					if (!colision) {
						protagonista->setX(nx);
						protagonista->setY(ny);
						huboCambio = true;
					}
				}
				actualizarProximidad();
				return huboCambio;
			}

			if (tecla == 'e' || tecla == 'E') {
				if (pozoEncontrado) {
					int px = protagonista->getX();
					int py = protagonista->getY();
					if (px + 1 >= pozoX - 2 && px <= pozoX + 3 && py + 1 >= pozoY - 2 && py <= pozoY + 3) {
						transicionBajando = true;
						huboCambio = true;
						return huboCambio;
					}
				}
			}

			if (tecla == 13) {
				int px = protagonista->getX();
				int py = protagonista->getY();

				if (!paredPiedraDestruida && px >= 445 && px <= 463 && py >= 25 && py <= 37) {
					Inventario* inv = protagonista->getInventario();
					if (inv != nullptr && inv->tieneItem("Vara magica")) {
						paredPiedraDestruida = true;
						if (mapa != nullptr) {
							for (int wy = 27; wy <= 35; wy++) {
								mapa->setCaracter(459, wy, ' ');
								mapa->setCaracter(460, wy, ' ');
							}
						}
						promptFlotante = "[Lanzaste bola de fuego! Pared de piedra destruida]";
						huboCambio = true;
						return huboCambio;
					}
				}

				if (qifrey != nullptr) {
					int qx = qifrey->getX();
					int qy = qifrey->getY();
					if (px + 1 >= qx - 2 && px <= qx + 3 && py + 1 >= qy - 2 && py <= qy + 3) {
						enDialogo = true;
						npcDialogoActual = "Qifrey";
						estadoDialogo = 1;
						huboCambio = true;
						return huboCambio;
					}
				}

				if (agott != nullptr) {
					int ax = agott->getX();
					int ay = agott->getY();
					if (px + 1 >= ax - 2 && px <= ax + 3 && py + 1 >= ay - 2 && py <= ay + 3) {
						enDialogo = true;
						npcDialogoActual = "Agott";
						if (respuestaInicialAgott == 2) {
							estadoDialogo = 220;
						} else {
							estadoDialogo = 210;
						}
						huboCambio = true;
						return huboCambio;
					}
				}

				for (size_t i = 0; i < itemsSuelo.size(); i++) {
					if (itemsSuelo[i] != nullptr && !itemsSuelo[i]->getRecogido()) {
						int ix = itemsSuelo[i]->getX();
						int iy = itemsSuelo[i]->getY();
						if (abs(px - ix) <= 2 && abs(py - iy) <= 2) {
							Inventario* inv = protagonista->getInventario();
							if (inv != nullptr) {
								if (inv->agregarItem(new ItemMagico(0, 0, itemsSuelo[i]->getNombre(), itemsSuelo[i]->getDescripcion(), itemsSuelo[i]->getTipoItem(), true))) {
									itemsSuelo[i]->setRecogido(true);
									sumarPuntosMision(25);
									promptFlotante = "[Recogiste: " + itemsSuelo[i]->getNombre() + "]";
									huboCambio = true;
									return huboCambio;
								} else {
									promptFlotante = "[Mochila llena: Max 4 items!]";
									huboCambio = true;
									return huboCambio;
								}
							}
						}
					}
				}
			}

			int dx = 0;
			int dy = 0;
			if (tecla == 'w' || tecla == 'W') dy = -1;
			else if (tecla == 's' || tecla == 'S') dy = 1;
			else if (tecla == 'a' || tecla == 'A') dx = -1;
			else if (tecla == 'd' || tecla == 'D') dx = 1;

			if (dx != 0 || dy != 0) {
				int nx = protagonista->getX() + dx;
				int ny = protagonista->getY() + dy;

				int cajaEmpujada = -1;
				for (size_t i = 0; i < cajas.size(); i++) {
					if (cajas[i] != nullptr) {
						int bx = cajas[i]->getX();
						int by = cajas[i]->getY();
						if (nx < bx + 3 && nx + 2 > bx && ny < by + 3 && ny + 2 > by) {
							cajaEmpujada = (int)i;
							break;
						}
					}
				}

				if (cajaEmpujada != -1) {
					int nbx = cajas[cajaEmpujada]->getX() + dx;
					int nby = cajas[cajaEmpujada]->getY() + dy;
					bool puedeMoverCaja = true;

					if (nbx < 465 || nbx + 3 > 578 || nby < 64 || nby + 3 > 134) {
						puedeMoverCaja = false;
					}

					if (puedeMoverCaja && mapa != nullptr) {
						for (int r = 0; r < 3; r++) {
							for (int c = 0; c < 3; c++) {
								if (!mapa->esPosicionValida(nbx + c, nby + r)) {
									puedeMoverCaja = false;
									break;
								}
							}
							if (!puedeMoverCaja) break;
						}
					}

					if (puedeMoverCaja) {
						for (size_t j = 0; j < cajas.size(); j++) {
							if ((int)j != cajaEmpujada && cajas[j] != nullptr) {
								int jbx = cajas[j]->getX();
								int jby = cajas[j]->getY();
								if (nbx < jbx + 3 && nbx + 3 > jbx && nby < jby + 3 && nby + 3 > jby) {
									puedeMoverCaja = false;
									break;
								}
							}
						}
					}

					if (puedeMoverCaja && agott != nullptr) {
						int ax = agott->getX();
						int ay = agott->getY();
						if (nbx < ax + 2 && nbx + 3 > ax && nby < ay + 2 && nby + 3 > ay) {
							puedeMoverCaja = false;
						}
					}

					if (puedeMoverCaja && pozoEncontrado) {
						if (nbx < pozoX + 3 && nbx + 3 > pozoX && nby < pozoY + 3 && nby + 3 > pozoY) {
							puedeMoverCaja = false;
						}
					}

					if (puedeMoverCaja) {
						for (size_t j = 0; j < cajas.size(); j++) {
							if ((int)j != cajaEmpujada && cajas[j] != nullptr) {
								int jbx = cajas[j]->getX();
								int jby = cajas[j]->getY();
								if (nx < jbx + 3 && nx + 2 > jbx && ny < jby + 3 && ny + 2 > jby) {
									puedeMoverCaja = false;
									break;
								}
							}
						}
					}

					if (puedeMoverCaja) {
						if (!cajas[cajaEmpujada]->getHaSidoMovida()) {
							cajas[cajaEmpujada]->setHaSidoMovida(true);
							cajasMovidasContador++;

							if (cajaEmpujada == indiceCajaLibro && !libroEncontrado) {
								libroEncontrado = true;
								int origX = cajas[cajaEmpujada]->getOrigX();
								int origY = cajas[cajaEmpujada]->getOrigY();
								itemsSuelo.push_back(new ItemMagico(origX + 1, origY + 1, "Libro de hechizos", "Tomo antiguo con instrucciones de trazos arcanos.", "Grimorio Magico", false));
								if (cajasMovidasContador == 1) {
									mensajeTemporal = "Coco: \"Pff, a la primera!\"";
								} else if (cajasMovidasContador == 2) {
									mensajeTemporal = "Coco: \"Bueno, no costo tanto encontrarlo!\"";
								} else {
									mensajeTemporal = "Coco: \"Por fin, lo encontre!\"";
								}
								ticksMensajeTemporal = 75;
							} else if (cajaEmpujada == indiceCajaPozo && !pozoEncontrado) {
								pozoEncontrado = true;
								pozoX = cajas[cajaEmpujada]->getOrigX();
								pozoY = cajas[cajaEmpujada]->getOrigY();
								mensajeTemporal = "Coco: \"Un pozo con escaleras?? Quien puede esconderse aqui?\"";
								ticksMensajeTemporal = 75;
							}
						}

						cajas[cajaEmpujada]->mover(dx, dy);
						protagonista->setX(nx);
						protagonista->setY(ny);
						actualizarCuartoActual(nx, ny);
						huboCambio = true;
					}
				} else if (mapa != nullptr) {
					bool colision = false;
					for (int r = 0; r < 2; r++) {
						for (int c = 0; c < 2; c++) {
							if (!mapa->esPosicionValida(nx + c, ny + r)) {
								colision = true;
								break;
							}
						}
						if (colision) break;
					}

					if (!colision && qifrey != nullptr) {
						int qx = qifrey->getX();
						int qy = qifrey->getY();
						if (nx + 1 >= qx && nx <= qx + 1 && ny + 1 >= qy && ny <= qy + 1) {
							colision = true;
						}
					}

					if (!colision && agott != nullptr) {
						int ax = agott->getX();
						int ay = agott->getY();
						if (nx + 1 >= ax && nx <= ax + 1 && ny + 1 >= ay && ny <= ay + 1) {
							colision = true;
						}
					}

					if (!colision) {
						if (nx != protagonista->getX() || ny != protagonista->getY()) {
							protagonista->setX(nx);
							protagonista->setY(ny);
							actualizarCuartoActual(nx, ny);
							huboCambio = true;
						}
					}
				}
			}
			actualizarProximidad();
		}
#endif
		return huboCambio;
	}
};

#endif
