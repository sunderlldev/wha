#ifndef Nivel_h
#define Nivel_h
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Mapa.h"
#include "Protagonista.h"
#include "NPC.h"
#include "ItemMagico.h"
#include "Inventario.h"
#include "Caja.h"
#include "CuartoRicheh.h"
#include "GestorDialogos.h"
#include "GestorMisiones.h"
#include "TorreAgott.h"
#include "Letrero.h"
#ifdef _WIN32
#include <conio.h>
#endif

class Nivel {
protected:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	bool mostrarEstadisticasFin;
	Mapa* mapa;
	Protagonista* protagonista;
	NPC* qifrey;
	NPC* agott;
	std::vector<ItemMagico*> itemsSuelo;
	std::vector<Letrero*> letreros;
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

	void limpiarLetreros() {
		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr) {
				delete letreros[i];
			}
		}
		letreros.clear();
	}

public:
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
		limpiarLetreros();
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
	int getModalActivo() const { return this->modalActivo; }
	bool getEnModalPersonajes() const { return this->modalActivo == 1; }
	bool getEnModalInventario() const { return this->modalActivo == 2; }
	bool getEnModalMisiones() const { return this->modalActivo == 3; }
	int getSeleccionModal() const { return this->seleccionModal; }
	bool getEnDetalleMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnDetalleMision() : false; }
	int getSeleccionMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getSeleccionMision() : 0; }
	int getSeleccionInventario() const { return this->seleccionModal; }
	std::string getObjetivoActual() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getObjetivoActual() : "";
	}
	bool getMostrarEstadisticasFin() const { return this->mostrarEstadisticasFin; }

	void setModalActivo(int m) { this->modalActivo = m; }
	void setEnDialogo(bool ed) { if (gestorDialogos != nullptr) gestorDialogos->setEnDialogo(ed); }
	void setEstadoDialogo(int ed) { if (gestorDialogos != nullptr) gestorDialogos->setEstadoDialogo(ed); }
	void setNpcDialogoActual(const std::string& n) { if (gestorDialogos != nullptr) gestorDialogos->setNpcDialogoActual(n); }
	void setEnModalPersonajes(bool emp) { this->modalActivo = emp ? 1 : 0; }
	void setEnModalMisiones(bool emm) { this->modalActivo = emm ? 3 : 0; if (gestorMisiones != nullptr) gestorMisiones->setEnModalMisiones(emm); }
	void setEnDetalleMision(bool edm) { if (gestorMisiones != nullptr) gestorMisiones->setEnDetalleMision(edm); }
	void setSeleccionMision(int sm) { if (gestorMisiones != nullptr) gestorMisiones->setSeleccionMision(sm); }
	void setEnModalInventario(bool emi) { this->modalActivo = emi ? 2 : 0; }
	void setSeleccionInventario(int si) { this->seleccionModal = si; }
	void setObjetivoActual(const std::string& obj) { if (gestorMisiones != nullptr) gestorMisiones->setObjetivoActual(obj); }

	virtual int determinarCuarto(int px, int py) const {
		(void)px;
		(void)py;
		return 0;
	}

	virtual void actualizarCuartoActual(int px, int py) {
		(void)px;
		(void)py;
	}

	virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) {
		(void)px;
		(void)py;
		(void)pw;
		(void)ph;
		return false;
	}

	virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) {
		(void)px;
		(void)py;
		(void)pw;
		(void)ph;
		return false;
	}

	virtual void inciarNivel() = 0;

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
		if ((mostrarEstadisticasFin || completado) && tiempoFin == 0) {
			tiempoFin = clock();
		}
		bool ed = (gestorDialogos != nullptr && gestorDialogos->getEnDialogo());
		if (modalActivo != 0 || ed || mostrarEstadisticasFin) {
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
				int pw = protagonista->getAncho();
				int ph = protagonista->getAlto();
				int rx = richeh->getX();
				int ry = richeh->getY();
				int rw = richeh->getAncho();
				int rh = richeh->getAlto();
				int distX = (px + pw <= rx) ? (rx - (px + pw)) : ((rx + rw <= px) ? (px - (rx + rw)) : 0);
				int distY = (py + ph <= ry) ? (ry - (py + ph)) : ((ry + rh <= py) ? (py - (ry + rh)) : 0);
				if (distX <= 2 && distY <= 2) {
					promptFlotante = "[ENTER] Hablar con Richeh";
					return;
				}
			}
			return;
		}

		int pw = protagonista->getAncho();
		int ph = protagonista->getAlto();

		if (verificarProximidadEspecial(px, py, pw, ph)) {
			return;
		}

		if (qifrey != nullptr) {
			int qx = qifrey->getX();
			int qy = qifrey->getY();
			int qw = qifrey->getAncho();
			int qh = qifrey->getAlto();
			int distX = (px + pw <= qx) ? (qx - (px + pw)) : ((qx + qw <= px) ? (px - (qx + qw)) : 0);
			int distY = (py + ph <= qy) ? (qy - (py + ph)) : ((qy + qh <= py) ? (py - (qy + qh)) : 0);
			if (distX <= 2 && distY <= 2) {
				promptFlotante = "[ENTER] Interactuar con Qifrey";
				return;
			}
		}

		if (agott != nullptr) {
			int ax = agott->getX();
			int ay = agott->getY();
			int aw = agott->getAncho();
			int ah = agott->getAlto();
			int distX = (px + pw <= ax) ? (ax - (px + pw)) : ((ax + aw <= px) ? (px - (ax + aw)) : 0);
			int distY = (py + ph <= ay) ? (ay - (py + ph)) : ((ay + ah <= py) ? (py - (ay + ah)) : 0);
			if (distX <= 2 && distY <= 2) {
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
				int iw = itemsSuelo[i]->getAncho();
				int ih = itemsSuelo[i]->getAlto();
				int distX = (px + pw <= ix) ? (ix - (px + pw)) : ((ix + iw <= px) ? (px - (ix + iw)) : 0);
				int distY = (py + ph <= iy) ? (iy - (py + ph)) : ((iy + ih <= py) ? (py - (iy + ih)) : 0);
				if (distX <= 2 && distY <= 2) {
					promptFlotante = "[ENTER] Recoger: " + itemsSuelo[i]->getNombre();
					return;
				}
			}
		}

		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr) {
				int lx = letreros[i]->getX();
				int ly = letreros[i]->getY();
				int lw = letreros[i]->getAncho();
				int lh = letreros[i]->getAlto();
				int distX = (px + pw <= lx) ? (lx - (px + pw)) : ((lx + lw <= px) ? (px - (lx + lw)) : 0);
				int distY = (py + ph <= ly) ? (ly - (py + ph)) : ((ly + lh <= py) ? (py - (ly + lh)) : 0);
				if (distX <= 3 && distY <= 3) {
					promptFlotante = "[E / ENTER] Leer letrero";
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

			if (modalActivo == 3) {
				if (gestorMisiones != nullptr) {
					if (!gestorMisiones->getEnDetalleMision()) {
						int sm = gestorMisiones->getSeleccionMision();
						if (tecla == 'w' || tecla == 'W') {
							if (sm > 0) {
								gestorMisiones->setSeleccionMision(sm - 1);
								huboCambio = true;
							}
						} else if (tecla == 's' || tecla == 'S') {
							if (sm < 3) {
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
						} else if (tecla == '4') {
							gestorMisiones->setSeleccionMision(3);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == 13) {
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == 'm' || tecla == 'M' || tecla == 27) {
							modalActivo = 0;
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
					if (seleccionModal < 5) {
						seleccionModal++;
						huboCambio = true;
					}
				} else if (tecla >= '1' && tecla <= '6') {
					seleccionModal = tecla - '1';
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
				} else if (tecla == '4') {
					procesarOpcionDialogo(4);
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
				modalActivo = 3;
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

				if (tecla == 13 || tecla == 'e' || tecla == 'E') {
					int px = protagonista->getX();
					int py = protagonista->getY();
					int pw = protagonista->getAncho();
					int ph = protagonista->getAlto();
					NPC* richeh = getRicheh();
					if (richeh != nullptr) {
						int rx = richeh->getX();
						int ry = richeh->getY();
						int rw = richeh->getAncho();
						int rh = richeh->getAlto();
						int distX = (px + pw <= rx) ? (rx - (px + pw)) : ((rx + rw <= px) ? (px - (rx + rw)) : 0);
						int distY = (py + ph <= ry) ? (ry - (py + ph)) : ((ry + rh <= py) ? (py - (ry + rh)) : 0);
						if (distX <= 2 && distY <= 2) {
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
					for (int paso = 0; paso < 2; paso++) {
						int nx = protagonista->getX() + dx;
						int ny = protagonista->getY() + dy;
						Mapa* mapaR = (cuartoRicheh != nullptr) ? cuartoRicheh->getMapa() : nullptr;
						if (mapaR != nullptr) {
							bool colision = false;
							for (int r = 0; r < protagonista->getAlto(); r++) {
								for (int c = 0; c < protagonista->getAncho(); c++) {
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
								if (nx < rx + richeh->getAncho() && nx + protagonista->getAncho() > rx &&
								    ny < ry + richeh->getAlto() && ny + protagonista->getAlto() > ry) {
									colision = true;
								}
							}
							if (!colision) {
								protagonista->setX(nx);
								protagonista->setY(ny);
								huboCambio = true;
							} else {
								break;
							}
						}
					}
				}
				actualizarProximidad();
				return huboCambio;
			}

			if (tecla == 'e' || tecla == 'E') {
				int px = protagonista->getX();
				int py = protagonista->getY();
				int pw = protagonista->getAncho();
				int ph = protagonista->getAlto();
				if (torreAgott != nullptr && torreAgott->estaCercaDelPozo(px, py)) {
					setTransicionBajando(true);
					huboCambio = true;
					return huboCambio;
				}
				for (size_t i = 0; i < letreros.size(); i++) {
					if (letreros[i] != nullptr) {
						int lx = letreros[i]->getX();
						int ly = letreros[i]->getY();
						int lw = letreros[i]->getAncho();
						int lh = letreros[i]->getAlto();
						int distX = (px + pw <= lx) ? (lx - (px + pw)) : ((lx + lw <= px) ? (px - (lx + lw)) : 0);
						int distY = (py + ph <= ly) ? (ly - (py + ph)) : ((ly + lh <= py) ? (py - (ly + lh)) : 0);
						if (distX <= 3 && distY <= 3) {
							mensajeTemporal = letreros[i]->getTexto();
							ticksMensajeTemporal = 100;
							promptFlotante = mensajeTemporal;
							actualizarProximidad();
							huboCambio = true;
							return huboCambio;
						}
					}
				}
			}

			if (tecla == 13) {
				int px = protagonista->getX();
				int py = protagonista->getY();
				int pw = protagonista->getAncho();
				int ph = protagonista->getAlto();

				if (procesarInteraccionEspecial(px, py, pw, ph)) {
					huboCambio = true;
					return huboCambio;
				}

				if (qifrey != nullptr) {
					int qx = qifrey->getX();
					int qy = qifrey->getY();
					int qw = qifrey->getAncho();
					int qh = qifrey->getAlto();
					int distX = (px + pw <= qx) ? (qx - (px + pw)) : ((qx + qw <= px) ? (px - (qx + qw)) : 0);
					int distY = (py + ph <= qy) ? (qy - (py + ph)) : ((qy + qh <= py) ? (py - (qy + qh)) : 0);
					if (distX <= 2 && distY <= 2) {
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
					int aw = agott->getAncho();
					int ah = agott->getAlto();
					int distX = (px + pw <= ax) ? (ax - (px + pw)) : ((ax + aw <= px) ? (px - (ax + aw)) : 0);
					int distY = (py + ph <= ay) ? (ay - (py + ph)) : ((ay + ah <= py) ? (py - (ay + ah)) : 0);
					if (distX <= 2 && distY <= 2) {
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
						int iw = itemsSuelo[i]->getAncho();
						int ih = itemsSuelo[i]->getAlto();
						int distX = (px + pw <= ix) ? (ix - (px + pw)) : ((ix + iw <= px) ? (px - (ix + iw)) : 0);
						int distY = (py + ph <= iy) ? (iy - (py + ph)) : ((iy + ih <= py) ? (py - (iy + ih)) : 0);
						if (distX <= 2 && distY <= 2) {
							Inventario* inv = protagonista->getInventario();
							if (inv != nullptr) {
								if (inv->agregarItem(new ItemMagico(0, 0, itemsSuelo[i]->getNombre(), itemsSuelo[i]->getDescripcion(), itemsSuelo[i]->getTipoItem(), true))) {
									itemsSuelo[i]->setRecogido(true);
									sumarPuntosMision(25);
									promptFlotante = "[Recogiste: " + itemsSuelo[i]->getNombre() + "]";
									huboCambio = true;
									return huboCambio;
								} else {
									promptFlotante = "[Mochila llena: Max 6 items!]";
									huboCambio = true;
									return huboCambio;
								}
							}
						}
					}
				}

				for (size_t i = 0; i < letreros.size(); i++) {
					if (letreros[i] != nullptr) {
						int lx = letreros[i]->getX();
						int ly = letreros[i]->getY();
						int lw = letreros[i]->getAncho();
						int lh = letreros[i]->getAlto();
						int distX = (px + pw <= lx) ? (lx - (px + pw)) : ((lx + lw <= px) ? (px - (lx + lw)) : 0);
						int distY = (py + ph <= ly) ? (ly - (py + ph)) : ((ly + lh <= py) ? (py - (ly + lh)) : 0);
						if (distX <= 3 && distY <= 3) {
							mensajeTemporal = letreros[i]->getTexto();
							ticksMensajeTemporal = 100;
							promptFlotante = mensajeTemporal;
							actualizarProximidad();
							huboCambio = true;
							return huboCambio;
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
				for (int paso = 0; paso < 2; paso++) {
					int nx = protagonista->getX() + dx;
					int ny = protagonista->getY() + dy;

					int cajaEmpujada = (torreAgott != nullptr) ? torreAgott->detectarColisionCaja(nx, ny) : -1;

					if (cajaEmpujada != -1 && torreAgott != nullptr) {
						if (torreAgott->intentarEmpujarCaja(cajaEmpujada, dx, dy, nx, ny, mapa, agott, itemsSuelo, mensajeTemporal, ticksMensajeTemporal)) {
							protagonista->setX(nx);
							protagonista->setY(ny);
							actualizarCuartoActual(nx, ny);
							huboCambio = true;
						} else {
							break;
						}
					} else if (mapa != nullptr) {
						bool colision = false;
						for (int r = 0; r < protagonista->getAlto(); r++) {
							for (int c = 0; c < protagonista->getAncho(); c++) {
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
							if (nx < qx + qifrey->getAncho() && nx + protagonista->getAncho() > qx &&
							    ny < qy + qifrey->getAlto() && ny + protagonista->getAlto() > qy) {
								colision = true;
							}
						}

						if (!colision && agott != nullptr) {
							int ax = agott->getX();
							int ay = agott->getY();
							if (nx < ax + agott->getAncho() && nx + protagonista->getAncho() > ax &&
							    ny < ay + agott->getAlto() && ny + protagonista->getAlto() > ay) {
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
						} else {
							break;
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
