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
#include "TorreAgott.h"
#ifdef _WIN32
#include <conio.h>
#endif

class Nivel {
private:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	bool mostrarEstadisticasFin;
	Mapa* mapa;
	Protagonista* protagonista;
	NPC* qifrey;
	NPC* agott;
	std::vector<ItemMagico*> itemsSuelo;
	bool paredPiedraDestruida;
	int idCuartoActual;
	std::string promptFlotante;
	std::string mensajeTemporal;
	int ticksMensajeTemporal;
	clock_t tiempoInicio;
	clock_t tiempoFin;
	int modalActivo;
	int seleccionModal;
	CuartoRicheh* cuartoRicheh;
	TorreAgott* torreAgott;
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

public:
	Nivel()
		: numeroNivel(1), nombreNivel("Atelier de Qifrey"), completado(false),
		  mostrarEstadisticasFin(false), mapa(nullptr), protagonista(nullptr),
		  qifrey(nullptr), agott(nullptr), paredPiedraDestruida(false),
		  idCuartoActual(0), promptFlotante(""), mensajeTemporal(""),
		  ticksMensajeTemporal(0), tiempoInicio(0), tiempoFin(0),
		  modalActivo(0), seleccionModal(0) {
		cuartoRicheh = new CuartoRicheh();
		torreAgott = new TorreAgott();
		gestorDialogos = new GestorDialogos();
		gestorMisiones = new GestorMisiones();
	}

	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false),
		  mostrarEstadisticasFin(false), mapa(nullptr), protagonista(nullptr),
		  qifrey(nullptr), agott(nullptr), paredPiedraDestruida(false),
		  idCuartoActual(0), promptFlotante(""), mensajeTemporal(""),
		  ticksMensajeTemporal(0), tiempoInicio(0), tiempoFin(0),
		  modalActivo(0), seleccionModal(0) {
		this->mapa = new Mapa(filasMapa, columnasMapa);
		this->cuartoRicheh = new CuartoRicheh();
		this->torreAgott = new TorreAgott();
		this->gestorDialogos = new GestorDialogos();
		this->gestorMisiones = new GestorMisiones();
	}

	virtual ~Nivel() {
		if (this->mapa != nullptr) {
			delete this->mapa;
			this->mapa = nullptr;
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
		if (this->cuartoRicheh != nullptr) {
			delete this->cuartoRicheh;
			this->cuartoRicheh = nullptr;
		}
		if (this->torreAgott != nullptr) {
			delete this->torreAgott;
			this->torreAgott = nullptr;
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
	}

	void mostrarMensajeTemporal(const std::string& msg, int ticks) {
		this->mensajeTemporal = msg;
		this->ticksMensajeTemporal = ticks;
	}

	void entrarCuartoRicheh() {
		if (cuartoRicheh != nullptr) {
			cuartoRicheh->entrar(protagonista);
		}
		promptFlotante = "";
		mostrarMensajeTemporal("[SOTANO SECRETO DE RICHEH]", 60);
	}

	void salirCuartoRicheh() {
		if (cuartoRicheh != nullptr) {
			cuartoRicheh->salir(protagonista);
		}
		promptFlotante = "";
		mostrarMensajeTemporal("[TORRE DE AGOTT]", 60);
	}

	bool getEnCuartoRicheh() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getActivo() : false;
	}
	NPC* getRicheh() {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getRicheh() : nullptr;
	}
	bool getTransicionBajando() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getTransicionBajando() : false;
	}
	bool getTransicionSubiendo() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getTransicionSubiendo() : false;
	}
	void setTransicionBajando(bool tb) {
		if (cuartoRicheh != nullptr) cuartoRicheh->setTransicionBajando(tb);
	}
	void setTransicionSubiendo(bool ts) {
		if (cuartoRicheh != nullptr) cuartoRicheh->setTransicionSubiendo(ts);
	}

	Protagonista* getProtagonista() { return this->protagonista; }
	NPC* getQifrey() { return this->qifrey; }
	NPC* getAgott() { return this->agott; }

	bool getLibroEncontrado() const { return (torreAgott != nullptr) ? torreAgott->getLibroEncontrado() : false; }
	bool getPozoEncontrado() const { return (torreAgott != nullptr) ? torreAgott->getPozoEncontrado() : false; }
	int getPozoX() const { return (torreAgott != nullptr) ? torreAgott->getPozoX() : -1; }
	int getPozoY() const { return (torreAgott != nullptr) ? torreAgott->getPozoY() : -1; }
	int getCajasMovidasContador() const { return (torreAgott != nullptr) ? torreAgott->getCajasMovidasContador() : 0; }
	const std::vector<Caja*>& getCajas() const {
		static const std::vector<Caja*> vacio;
		return (torreAgott != nullptr) ? torreAgott->getCajas() : vacio;
	}

	int getNumeroNivel() const { return this->numeroNivel; }
	std::string getNombreNivel() const { return this->nombreNivel; }
	bool getCompletado() const { return this->completado; }
	Mapa* getMapa() {
		if (getEnCuartoRicheh() && cuartoRicheh != nullptr) {
			return cuartoRicheh->getMapa();
		}
		return this->mapa;
	}
	const std::vector<ItemMagico*>& getItemsSuelo() const { return this->itemsSuelo; }
	const std::string& getPromptFlotante() const { return this->promptFlotante; }

	bool getEnDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEnDialogo() : false; }
	int getEstadoDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEstadoDialogo() : 0; }
	std::string getNpcDialogoActual() const { return (gestorDialogos != nullptr) ? gestorDialogos->getNpcDialogoActual() : ""; }
	bool getEnModalPersonajes() const { return this->modalActivo == 1; }
	int getSeleccionModal() const { return this->seleccionModal; }
	bool getEnModalMisiones() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnModalMisiones() : false; }
	bool getEnDetalleMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnDetalleMision() : false; }
	int getSeleccionMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getSeleccionMision() : 0; }
	bool getEnModalInventario() const { return this->modalActivo == 2; }
	int getSeleccionInventario() const { return this->seleccionModal; }
	bool getParedPiedraDestruida() const { return this->paredPiedraDestruida; }
	int getIdCuartoActual() const { return this->idCuartoActual; }
	const std::string& getCartelCuarto() const { return this->mensajeTemporal; }
	std::string getObjetivoActual() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getObjetivoActual() : "";
	}
	bool getMostrarEstadisticasFin() const { return this->mostrarEstadisticasFin; }
	bool getAvanzaSiguienteNivel() const { return this->completado && !this->mostrarEstadisticasFin; }

	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) {
		this->completado = estado;
		if (estado && tiempoFin == 0) {
			tiempoFin = clock();
		}
	}
	void setEnDialogo(bool ed) { if (gestorDialogos != nullptr) gestorDialogos->setEnDialogo(ed); }
	void setEstadoDialogo(int ed) { if (gestorDialogos != nullptr) gestorDialogos->setEstadoDialogo(ed); }
	void setNpcDialogoActual(const std::string& n) { if (gestorDialogos != nullptr) gestorDialogos->setNpcDialogoActual(n); }
	void setEnModalPersonajes(bool emp) { this->modalActivo = emp ? 1 : 0; }
	void setEnModalMisiones(bool emm) { if (gestorMisiones != nullptr) gestorMisiones->setEnModalMisiones(emm); }
	void setEnDetalleMision(bool edm) { if (gestorMisiones != nullptr) gestorMisiones->setEnDetalleMision(edm); }
	void setSeleccionMision(int sm) { if (gestorMisiones != nullptr) gestorMisiones->setSeleccionMision(sm); }
	void setEnModalInventario(bool emi) { this->modalActivo = emi ? 2 : 0; }
	void setSeleccionInventario(int si) { this->seleccionModal = si; }
	void setParedPiedraDestruida(bool val) { this->paredPiedraDestruida = val; }
	void setMostrarEstadisticasFin(bool val) {
		this->mostrarEstadisticasFin = val;
		if (val && tiempoFin == 0) {
			tiempoFin = clock();
		}
	}
	void setAvanzaSiguienteNivel(bool val) {
		this->completado = val;
	}
	void setObjetivoActual(const std::string& obj) { if (gestorMisiones != nullptr) gestorMisiones->setObjetivoActual(obj); }

	int determinarCuarto(int px, int py) const {
		if (px >= 22 && px <= 55 && py >= 12 && py <= 28) return 1;
		if (px >= 460 && px <= 590 && py >= 25 && py <= 50) return 2;
		if (px >= 460 && px <= 590 && py >= 60 && py <= 140) return 3;
		if (px >= 460 && px <= 590 && py >= 150 && py <= 190) return 4;
		return 0;
	}

	void actualizarCuartoActual(int px, int py) {
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto == 1) {
				mostrarMensajeTemporal("[LA CHOZA DE HECHIZOS]", 60);
			} else if (nuevoCuarto == 2) {
				mostrarMensajeTemporal("[EL ALMACEN ABANDONADO]", 60);
			} else if (nuevoCuarto == 3) {
				mostrarMensajeTemporal("[TORRE DE AGOTT]", 60);
				if (agott != nullptr && !agott->getYaHablo()) {
					agott->setYaHablo(true);
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogo("Agott", 200);
					}
				}
			} else if (nuevoCuarto == 4) {
				mostrarMensajeTemporal("[BOSQUE DE PLATA]", 60);
			} else {
				mensajeTemporal = "";
				ticksMensajeTemporal = 0;
			}
		}
	}

	void inciarNivel() {
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		if (gestorDialogos != nullptr) gestorDialogos->terminarDialogo();
		if (gestorMisiones != nullptr) {
			gestorMisiones->setPuntosMisiones(0);
			gestorMisiones->setEnModalMisiones(false);
			gestorMisiones->setEnDetalleMision(false);
			gestorMisiones->setSeleccionMision(0);
		}
		this->modalActivo = 0;
		this->seleccionModal = 0;
		this->mostrarEstadisticasFin = false;
		this->idCuartoActual = 0;
		this->mensajeTemporal = "";
		this->ticksMensajeTemporal = 0;
		this->paredPiedraDestruida = false;

		if (cuartoRicheh != nullptr) {
			cuartoRicheh->reiniciar();
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

			limpiarItemsSuelo();
			itemsSuelo.push_back(new ItemMagico(505, 34, "Tela", "Trozo de tela arcana resistente y ligera para confeccionar vestiduras.", "Material Magico", false));

			if (torreAgott != nullptr) {
				torreAgott->reiniciar();
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
		clock_t tActual = (tiempoFin > 0) ? tiempoFin : clock();
		return (int)((tActual - tiempoInicio) / CLOCKS_PER_SEC);
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
		return this->completado && !this->mostrarEstadisticasFin;
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
			NPC* richeh = getRicheh();
			bool dioVara = (richeh != nullptr && richeh->getYaHablo());
			bool libroEnc = (torreAgott != nullptr && torreAgott->getLibroEncontrado());
			gestorDialogos->obtenerDatosDialogo(protagonista, qifrey, agott, richeh, dioVara, libroEnc,
			                                    hablante, rol, confianza, lineas, opciones);
		}
	}

	void procesarOpcionDialogo(int opcion) {
		if (gestorDialogos != nullptr && gestorMisiones != nullptr) {
			std::string obj = gestorMisiones->getObjetivoActual();
			int pts = gestorMisiones->getPuntosMisiones();
			NPC* richeh = getRicheh();
			bool dioVara = (richeh != nullptr && richeh->getYaHablo());
			bool libroEnc = (torreAgott != nullptr && torreAgott->getLibroEncontrado());
			gestorDialogos->procesarOpcionDialogo(opcion, protagonista, qifrey, agott, richeh,
			                                      dioVara, libroEnc, promptFlotante, obj, pts,
			                                      completado, mostrarEstadisticasFin);
			if (mostrarEstadisticasFin && tiempoFin == 0) {
				tiempoFin = clock();
			}
			gestorMisiones->setObjetivoActual(obj);
			gestorMisiones->setPuntosMisiones(pts);
		}
	}

	void actualizarProximidad() {
		if (protagonista == nullptr) return;
		bool ed = (gestorDialogos != nullptr && gestorDialogos->getEnDialogo());
		bool emm = (gestorMisiones != nullptr && gestorMisiones->getEnModalMisiones());
		if (modalActivo != 0 || emm || ed || mostrarEstadisticasFin) {
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

		if (getEnCuartoRicheh()) {
			if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDelPozo(px, py)) {
				promptFlotante = "[E] Subir a la torre";
				return;
			}
			NPC* richeh = getRicheh();
			if (richeh != nullptr) {
				int rx = richeh->getX();
				int ry = richeh->getY();
				if (px + 1 >= rx - 2 && px <= rx + 3 && py + 1 >= ry - 2 && py <= ry + 3) {
					promptFlotante = "[ENTER] Hablar con Richeh";
					return;
				}
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

		if (torreAgott != nullptr && torreAgott->estaCercaDelPozo(px, py)) {
			promptFlotante = "[E] Bajar al pozo";
			return;
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
			NPC* richeh = getRicheh();
			if (richeh != nullptr) {
				richeh->actualizarAnimacion(30);
			}
			if (this->protagonista->getFrameActual() != framePrevio) {
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
					huboCambio = true;
				} else if (tecla == 'c' || tecla == 'C' || tecla == 27) {
					mostrarEstadisticasFin = false;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (modalActivo == 1) {
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
					modalActivo = 0;
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

			if (modalActivo == 2) {
				if (tecla == 'w' || tecla == 'W') {
					if (seleccionModal > 0) {
						seleccionModal--;
						huboCambio = true;
					}
				} else if (tecla == 's' || tecla == 'S') {
					if (seleccionModal < 3) {
						seleccionModal++;
						huboCambio = true;
					}
				} else if (tecla == '1') {
					seleccionModal = 0;
					huboCambio = true;
				} else if (tecla == '2') {
					seleccionModal = 1;
					huboCambio = true;
				} else if (tecla == '3') {
					seleccionModal = 2;
					huboCambio = true;
				} else if (tecla == '4') {
					seleccionModal = 3;
					huboCambio = true;
				} else if (tecla == 'i' || tecla == 'I' || tecla == 27 || tecla == 13) {
					modalActivo = 0;
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
				modalActivo = 1;
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
				modalActivo = 2;
				seleccionModal = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (getEnCuartoRicheh()) {
				if (tecla == 'e' || tecla == 'E') {
					int px = protagonista->getX();
					int py = protagonista->getY();
					if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDelPozo(px, py)) {
						setTransicionSubiendo(true);
						huboCambio = true;
						return huboCambio;
					}
				}

				if (tecla == 13) {
					int px = protagonista->getX();
					int py = protagonista->getY();
					NPC* richeh = getRicheh();
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
					Mapa* mapaR = (cuartoRicheh != nullptr) ? cuartoRicheh->getMapa() : nullptr;
					if (mapaR != nullptr) {
						bool colision = false;
						for (int r = 0; r < 2; r++) {
							for (int c = 0; c < 2; c++) {
								if (!mapaR->esPosicionValida(nx + c, ny + r)) {
									colision = true;
									break;
								}
							}
							if (colision) break;
						}
						NPC* richeh = getRicheh();
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
				int px = protagonista->getX();
				int py = protagonista->getY();
				if (torreAgott != nullptr && torreAgott->estaCercaDelPozo(px, py)) {
					setTransicionBajando(true);
					huboCambio = true;
					return huboCambio;
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
							gestorDialogos->iniciarDialogoAgott();
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

				int cajaEmpujada = (torreAgott != nullptr) ? torreAgott->detectarColisionCaja(nx, ny) : -1;

				if (cajaEmpujada != -1 && torreAgott != nullptr) {
					if (torreAgott->intentarEmpujarCaja(cajaEmpujada, dx, dy, nx, ny, mapa, agott, itemsSuelo, mensajeTemporal, ticksMensajeTemporal)) {
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
