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
#include "GestorDialogos.h"
#include "GestorMisiones.h"
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
	std::string mensajeTemporal;
	int ticksMensajeTemporal;
	std::vector<ItemMagico*> itemsSuelo;
	clock_t tiempoInicio;
	std::string promptFlotante;
	bool enModalPersonajes;
	int seleccionModal;
	bool enModalInventario;
	int seleccionInventario;
	bool paredPiedraDestruida;
	int idCuartoActual;
	std::string cartelCuarto;
	int ticksCartelCuarto;
	bool mostrarEstadisticasFin;
	bool avanzaSiguienteNivel;
	GestorDialogos* gestorDialogos;
	GestorMisiones* gestorMisiones;

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
		  primeraVezTorreAgott(true), respuestaInicialAgott(0),
		  mensajeTemporal(""), ticksMensajeTemporal(0),
		  tiempoInicio(0), promptFlotante(""),
		  enModalPersonajes(false), seleccionModal(0),
		  enModalInventario(false), seleccionInventario(0),
		  paredPiedraDestruida(false), idCuartoActual(0), cartelCuarto(""), ticksCartelCuarto(0),
		  mostrarEstadisticasFin(false), avanzaSiguienteNivel(false) {
		gestorDialogos = new GestorDialogos();
		gestorMisiones = new GestorMisiones();
	}

	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false),
		  mapa(nullptr), protagonista(nullptr), qifrey(nullptr), agott(nullptr), richeh(nullptr),
		  mapaRicheh(nullptr), enCuartoRicheh(false), cocoPrevX(0), cocoPrevY(0),
		  dioVaraRicheh(false), transicionBajando(false), transicionSubiendo(false),
		  indiceCajaLibro(-1), indiceCajaPozo(-1), libroEncontrado(false),
		  pozoEncontrado(false), cajasMovidasContador(0), pozoX(-1), pozoY(-1),
		  primeraVezTorreAgott(true), respuestaInicialAgott(0),
		  mensajeTemporal(""), ticksMensajeTemporal(0),
		  tiempoInicio(0), promptFlotante(""),
		  enModalPersonajes(false), seleccionModal(0),
		  enModalInventario(false), seleccionInventario(0),
		  paredPiedraDestruida(false), idCuartoActual(0), cartelCuarto(""), ticksCartelCuarto(0),
		  mostrarEstadisticasFin(false), avanzaSiguienteNivel(false) {
		this->mapa = new Mapa(filasMapa, columnasMapa);
		this->gestorDialogos = new GestorDialogos();
		this->gestorMisiones = new GestorMisiones();
	}

	virtual ~Nivel() {
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
		if (this->gestorDialogos != nullptr) {
			delete this->gestorDialogos;
			this->gestorDialogos = nullptr;
		}
		if (this->gestorMisiones != nullptr) {
			delete this->gestorMisiones;
			this->gestorMisiones = nullptr;
		}
		limpiarItemsSuelo();
		limpiarCajas();
	}

	void entrarCuartoRicheh() {
		if (protagonista != nullptr) {
			cocoPrevX = protagonista->getX();
			cocoPrevY = protagonista->getY();
			protagonista->setX(8);
			protagonista->setY(6);
		}
		enCuartoRicheh = true;
		promptFlotante = "";
		cartelCuarto = "[SOTANO SECRETO DE RICHEH]";
		ticksCartelCuarto = 60;
	}

	void salirCuartoRicheh() {
		if (protagonista != nullptr) {
			protagonista->setX(cocoPrevX);
			protagonista->setY(cocoPrevY);
		}
		enCuartoRicheh = false;
		promptFlotante = "";
		cartelCuarto = "[TORRE DE AGOTT]";
		ticksCartelCuarto = 60;
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

	bool getLibroEncontrado() const { return this->libroEncontrado; }
	bool getPozoEncontrado() const { return this->pozoEncontrado; }
	int getPozoX() const { return this->pozoX; }
	int getPozoY() const { return this->pozoY; }
	int getCajasMovidasContador() const { return this->cajasMovidasContador; }
	const std::vector<Caja*>& getCajas() const { return this->cajas; }

	int getNumeroNivel() const { return this->numeroNivel; }
	std::string getNombreNivel() const { return this->nombreNivel; }
	bool getCompletado() const { return this->completado; }
	Mapa* getMapa() {
		if (enCuartoRicheh && mapaRicheh != nullptr) {
			return this->mapaRicheh;
		}
		return this->mapa;
	}
	const std::vector<ItemMagico*>& getItemsSuelo() const { return this->itemsSuelo; }
	const std::string& getPromptFlotante() const { return this->promptFlotante; }

	bool getEnDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEnDialogo() : false; }
	int getEstadoDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEstadoDialogo() : 0; }
	std::string getNpcDialogoActual() const { return (gestorDialogos != nullptr) ? gestorDialogos->getNpcDialogoActual() : ""; }
	bool getEnModalPersonajes() const { return this->enModalPersonajes; }
	int getSeleccionModal() const { return this->seleccionModal; }
	bool getEnModalMisiones() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnModalMisiones() : false; }
	bool getEnDetalleMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnDetalleMision() : false; }
	int getSeleccionMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getSeleccionMision() : 0; }
	bool getEnModalInventario() const { return this->enModalInventario; }
	int getSeleccionInventario() const { return this->seleccionInventario; }
	bool getParedPiedraDestruida() const { return this->paredPiedraDestruida; }
	int getIdCuartoActual() const { return this->idCuartoActual; }
	const std::string& getCartelCuarto() const { return this->cartelCuarto; }
	std::string getObjetivoActual() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getObjetivoActual() : "";
	}
	bool getMostrarEstadisticasFin() const { return this->mostrarEstadisticasFin; }
	bool getAvanzaSiguienteNivel() const { return this->avanzaSiguienteNivel; }

	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) { this->completado = estado; }
	void setEnDialogo(bool ed) { if (gestorDialogos != nullptr) gestorDialogos->setEnDialogo(ed); }
	void setEstadoDialogo(int ed) { if (gestorDialogos != nullptr) gestorDialogos->setEstadoDialogo(ed); }
	void setNpcDialogoActual(const std::string& n) { if (gestorDialogos != nullptr) gestorDialogos->setNpcDialogoActual(n); }
	void setEnModalPersonajes(bool emp) { this->enModalPersonajes = emp; }
	void setEnModalMisiones(bool emm) { if (gestorMisiones != nullptr) gestorMisiones->setEnModalMisiones(emm); }
	void setEnDetalleMision(bool edm) { if (gestorMisiones != nullptr) gestorMisiones->setEnDetalleMision(edm); }
	void setSeleccionMision(int sm) { if (gestorMisiones != nullptr) gestorMisiones->setSeleccionMision(sm); }
	void setEnModalInventario(bool emi) { this->enModalInventario = emi; }
	void setSeleccionInventario(int si) { this->seleccionInventario = si; }
	void setParedPiedraDestruida(bool val) { this->paredPiedraDestruida = val; }
	void setMostrarEstadisticasFin(bool val) { this->mostrarEstadisticasFin = val; }
	void setAvanzaSiguienteNivel(bool val) { this->avanzaSiguienteNivel = val; }
	void setObjetivoActual(const std::string& obj) { if (gestorMisiones != nullptr) gestorMisiones->setObjetivoActual(obj); }

	int determinarCuarto(int px, int py) const {
		if (px >= 22 && px <= 55 && py >= 12 && py <= 28) {
			return 1;
		}
		if (px >= 460 && px <= 590 && py >= 25 && py <= 50) {
			return 2;
		}
		if (px >= 460 && px <= 590 && py >= 60 && py <= 140) {
			return 3;
		}
		if (px >= 460 && px <= 590 && py >= 150 && py <= 190) {
			return 4;
		}
		return 0;
	}

	void actualizarCuartoActual(int px, int py) {
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto == 1) {
				cartelCuarto = "[LA CHOZA DE HECHIZOS]";
				ticksCartelCuarto = 60;
			} else if (nuevoCuarto == 2) {
				cartelCuarto = "[EL ALMACEN ABANDONADO]";
				ticksCartelCuarto = 60;
			} else if (nuevoCuarto == 3) {
				cartelCuarto = "[TORRE DE AGOTT]";
				ticksCartelCuarto = 60;
				if (primeraVezTorreAgott) {
					primeraVezTorreAgott = false;
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogo("Agott", 200);
					}
				}
			} else if (nuevoCuarto == 4) {
				cartelCuarto = "[BOSQUE DE PLATA]";
				ticksCartelCuarto = 60;
			} else {
				cartelCuarto = "";
				ticksCartelCuarto = 0;
			}
		}
	}

	void inciarNivel() {
		this->tiempoInicio = clock();
		this->completado = false;
		if (gestorDialogos != nullptr) gestorDialogos->terminarDialogo();
		if (gestorMisiones != nullptr) {
			gestorMisiones->setPuntosMisiones(0);
			gestorMisiones->setEnModalMisiones(false);
			gestorMisiones->setEnDetalleMision(false);
			gestorMisiones->setSeleccionMision(0);
		}
		this->enModalPersonajes = false;
		this->seleccionModal = 0;
		this->enModalInventario = false;
		this->seleccionInventario = 0;
		this->mostrarEstadisticasFin = false;
		this->avanzaSiguienteNivel = false;
		this->idCuartoActual = 0;
		this->cartelCuarto = "";
		this->ticksCartelCuarto = 0;
		this->paredPiedraDestruida = false;
		this->enCuartoRicheh = false;
		this->dioVaraRicheh = false;
		this->transicionBajando = false;
		this->transicionSubiendo = false;
		this->cajasMovidasContador = 0;
		this->libroEncontrado = false;
		this->pozoEncontrado = false;
		this->pozoX = -1;
		this->pozoY = -1;
		this->primeraVezTorreAgott = true;
		this->respuestaInicialAgott = 0;
		this->mensajeTemporal = "";
		this->ticksMensajeTemporal = 0;

		if (mapaRicheh == nullptr) {
			mapaRicheh = new Mapa(35, 120);
			std::vector<std::string> mRicheh;
			CuartoRicheh::cargarMatriz(mRicheh);
			mapaRicheh->cargarMatriz(mRicheh);
		}

		if (numeroNivel == 1) {
			if (mapa == nullptr) {
				mapa = new Mapa(200, 600);
			}
			std::vector<std::string> matrizCargada;
			MapaNivel1::cargarMatriz(matrizCargada);
			mapa->cargarMatriz(matrizCargada);

			if (protagonista == nullptr) {
				protagonista = new Protagonista(38, 20, "Coco", 3, 1);
			} else {
				protagonista->setX(38);
				protagonista->setY(20);
				protagonista->setVida(3);
				protagonista->setVidaMaxima(3);
			}

			if (qifrey == nullptr) {
				qifrey = new NPC(38, 17, "Qifrey", "Maestro Hechicero", false);
			} else {
				qifrey->setX(38);
				qifrey->setY(17);
				qifrey->setConfianza(0);
				qifrey->setYaHablo(false);
				qifrey->setDioTinta(false);
				qifrey->setCrafteoCapa(false);
			}

			if (agott == nullptr) {
				agott = new NPC(524, 66, "Agott", "Aprendiz de Maga", false);
			} else {
				agott->setX(524);
				agott->setY(66);
				agott->setConfianza(1);
				agott->setYaHablo(false);
			}

			if (richeh == nullptr) {
				richeh = new NPC(22, 10, "Richeh", "Aprendiz de Maga", false);
			} else {
				richeh->setX(22);
				richeh->setY(10);
				richeh->setConfianza(1);
				richeh->setYaHablo(false);
			}

			limpiarItemsSuelo();
			itemsSuelo.push_back(new ItemMagico(505, 34, "Tela", "Trozo de tela arcana resistente y ligera para confeccionar vestiduras.", "Material Magico", false));

			limpiarCajas();
			srand(12345);
			int numCajas = 18;
			std::vector<std::pair<int, int>> posicionesUsadas;

			for (int i = 0; i < numCajas; i++) {
				int bx = 0;
				int by = 0;
				bool posValida = false;
				int intentos = 0;

				while (!posValida && intentos < 100) {
					intentos++;
					bx = 472 + (rand() % (560 - 472 + 1));
					by = 76 + (rand() % (122 - 76 + 1));

					posValida = true;
					for (size_t k = 0; k < posicionesUsadas.size(); k++) {
						if (abs(bx - posicionesUsadas[k].first) < 4 && abs(by - posicionesUsadas[k].second) < 4) {
							posValida = false;
							break;
						}
					}
				}

				if (posValida) {
					posicionesUsadas.push_back(std::make_pair(bx, by));
					Caja* c = new Caja(bx, by);
					cajas.push_back(c);
				}
			}

			if (cajas.size() >= 2) {
				indiceCajaLibro = 5 % cajas.size();
				indiceCajaPozo = 12 % cajas.size();
				if (indiceCajaPozo == indiceCajaLibro) {
					indiceCajaPozo = (indiceCajaPozo + 1) % cajas.size();
				}
			}

			if (gestorMisiones != nullptr) {
				gestorMisiones->setObjetivoActual("Hablar con Maestro Qifrey");
			}
			actualizarCuartoActual(protagonista->getX(), protagonista->getY());
		} else if (numeroNivel == 2) {
			if (gestorMisiones != nullptr) {
				gestorMisiones->setObjetivoActual("Tierras Prohibidas - Fase 2");
			}
		} else if (numeroNivel == 3) {
			if (gestorMisiones != nullptr) {
				gestorMisiones->setObjetivoActual("Gran Arbol de Plata - Fase 3");
			}
		}
	}

	int getSegundosTranscurridos() const {
		if (tiempoInicio == 0) return 0;
		return (int)((clock() - tiempoInicio) / CLOCKS_PER_SEC);
	}

	int getBonoTiempo() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getBonoTiempo(getSegundosTranscurridos()) : 0;
	}

	int getPuntajeTotalNivel() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getPuntajeTotalNivel(getSegundosTranscurridos()) : 0;
	}

	void sumarPuntosMision(int p) {
		if (gestorMisiones != nullptr) gestorMisiones->sumarPuntosMision(p);
	}

	int getPuntosMisiones() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getPuntosMisiones() : 0;
	}

	bool verificarObjetivo() {
		return this->avanzaSiguienteNivel;
	}

	void obtenerDatosMisiones(std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas) {
		if (gestorMisiones != nullptr) {
			int ed = (gestorDialogos != nullptr) ? gestorDialogos->getEstadoDialogo() : 0;
			gestorMisiones->obtenerDatosMisiones(protagonista, qifrey, ed, titulos, descripciones, estados, desbloqueadas);
		}
	}

	void obtenerDatosDialogo(std::string& hablante, std::string& rol, int& confianza,
	                         std::vector<std::string>& lineas,
	                         std::vector<std::string>& opciones) {
		if (gestorDialogos != nullptr) {
			gestorDialogos->obtenerDatosDialogo(protagonista, qifrey, agott, richeh, dioVaraRicheh, libroEncontrado,
			                                    hablante, rol, confianza, lineas, opciones);
		}
	}

	void procesarOpcionDialogo(int opcion) {
		if (gestorDialogos != nullptr && gestorMisiones != nullptr) {
			std::string obj = gestorMisiones->getObjetivoActual();
			int pts = gestorMisiones->getPuntosMisiones();
			gestorDialogos->procesarOpcionDialogo(opcion, protagonista, qifrey, agott, richeh,
			                                      dioVaraRicheh, libroEncontrado, respuestaInicialAgott,
			                                      promptFlotante, obj, pts, completado, mostrarEstadisticasFin);
			gestorMisiones->setObjetivoActual(obj);
			gestorMisiones->setPuntosMisiones(pts);
		}
	}

	void actualizarProximidad() {
		if (protagonista == nullptr) return;
		bool ed = (gestorDialogos != nullptr && gestorDialogos->getEnDialogo());
		bool emm = (gestorMisiones != nullptr && gestorMisiones->getEnModalMisiones());
		if (enModalPersonajes || emm || enModalInventario || ed || mostrarEstadisticasFin) {
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
		bool huboCambio = false;

		if (this->protagonista != nullptr) {
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
		}

		if (ticksCartelCuarto > 0) {
			ticksCartelCuarto--;
			if (ticksCartelCuarto == 0) {
				cartelCuarto = "";
				actualizarProximidad();
				huboCambio = true;
			}
		}

		if (ticksMensajeTemporal > 0) {
			ticksMensajeTemporal--;
			if (ticksMensajeTemporal == 0) {
				mensajeTemporal = "";
				actualizarProximidad();
				huboCambio = true;
			}
		}

#ifdef _WIN32
		if (_kbhit()) {
			int tecla = _getch();
			if (tecla == 0 || tecla == 224) {
				tecla = _getch();
			}

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

			if (gestorMisiones != nullptr && gestorMisiones->getEnModalMisiones()) {
				if (!gestorMisiones->getEnDetalleMision()) {
					int sm = gestorMisiones->getSeleccionMision();
					if (tecla == 'w' || tecla == 'W') {
						if (sm > 0) {
							gestorMisiones->setSeleccionMision(sm - 1);
							huboCambio = true;
						}
					} else if (tecla == 's' || tecla == 'S') {
						if (sm < 2) {
							gestorMisiones->setSeleccionMision(sm + 1);
							huboCambio = true;
						}
					} else if (tecla == '1') {
						gestorMisiones->setSeleccionMision(0);
						gestorMisiones->setEnDetalleMision(true);
						huboCambio = true;
					} else if (tecla == '2') {
						gestorMisiones->setSeleccionMision(1);
						gestorMisiones->setEnDetalleMision(true);
						huboCambio = true;
					} else if (tecla == '3') {
						gestorMisiones->setSeleccionMision(2);
						gestorMisiones->setEnDetalleMision(true);
						huboCambio = true;
					} else if (tecla == 13) {
						gestorMisiones->setEnDetalleMision(true);
						huboCambio = true;
					} else if (tecla == 'm' || tecla == 'M' || tecla == 27) {
						gestorMisiones->setEnModalMisiones(false);
						gestorMisiones->setEnDetalleMision(false);
						huboCambio = true;
					}
				} else {
					if (tecla == 13 || tecla == 27 || tecla == 'm' || tecla == 'M') {
						gestorMisiones->setEnDetalleMision(false);
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

			if (gestorDialogos != nullptr && gestorDialogos->getEnDialogo()) {
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
					gestorDialogos->terminarDialogo();
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
				if (gestorMisiones != nullptr) {
					gestorMisiones->setEnModalMisiones(true);
					gestorMisiones->setEnDetalleMision(false);
					gestorMisiones->setSeleccionMision(0);
				}
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
							if (gestorDialogos != nullptr) {
								gestorDialogos->iniciarDialogo("Richeh", 300);
							}
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
					if (mapaRicheh != nullptr) {
						bool colision = false;
						for (int r = 0; r < 2; r++) {
							for (int c = 0; c < 2; c++) {
								if (!mapaRicheh->esPosicionValida(nx + c, ny + r)) {
									colision = true;
									break;
								}
							}
							if (colision) break;
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
						if (gestorDialogos != nullptr) {
							gestorDialogos->iniciarDialogo("Qifrey", 1);
						}
						huboCambio = true;
						return huboCambio;
					}
				}

				if (agott != nullptr) {
					int ax = agott->getX();
					int ay = agott->getY();
					if (px + 1 >= ax - 2 && px <= ax + 3 && py + 1 >= ay - 2 && py <= ay + 3) {
						if (gestorDialogos != nullptr) {
							int estadoIni = (respuestaInicialAgott == 2) ? 220 : 210;
							gestorDialogos->iniciarDialogo("Agott", estadoIni);
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
