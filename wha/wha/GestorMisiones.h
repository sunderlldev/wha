#ifndef GestorMisiones_h
#define GestorMisiones_h
#include <string>
#include <vector>
#include "NPC.h"
#include "Protagonista.h"
#include "Inventario.h"

class GestorMisiones {
private:
	int puntosMisiones;
	bool enModalMisiones;
	bool enDetalleMision;
	int seleccionMision;
	std::string objetivoActual;

public:
	GestorMisiones()
		: puntosMisiones(0), enModalMisiones(false), enDetalleMision(false),
		  seleccionMision(0), objetivoActual("Hablar con Maestro Qifrey") {}
	~GestorMisiones() {}

	int getPuntosMisiones() const { return puntosMisiones; }
	void setPuntosMisiones(int p) { puntosMisiones = p; }
	void sumarPuntosMision(int p) { puntosMisiones += p; }

	bool getEnModalMisiones() const { return enModalMisiones; }
	void setEnModalMisiones(bool emm) { enModalMisiones = emm; }

	bool getEnDetalleMision() const { return enDetalleMision; }
	void setEnDetalleMision(bool edm) { enDetalleMision = edm; }

	int getSeleccionMision() const { return seleccionMision; }
	void setSeleccionMision(int sm) { seleccionMision = sm; }

	std::string getObjetivoActual() const { return objetivoActual; }
	void setObjetivoActual(const std::string& obj) { objetivoActual = obj; }

	int getBonoTiempo(int seg) const {
		if (seg <= 60) return 50;
		if (seg <= 120) return 30;
		if (seg <= 180) return 20;
		if (seg <= 240) return 10;
		return 5;
	}

	int getPuntajeTotalNivel(int seg) const {
		return puntosMisiones + getBonoTiempo(seg);
	}

	void obtenerDatosMisiones(Protagonista* protagonista, NPC* qifrey, int estadoDialogo,
	                          std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas,
	                          bool misionMyrphonActiva = false,
	                          bool myrphonRescatado = false,
	                          bool dioVaraRicheh = false) {
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

		bool tieneTintaColeccion = (inv != nullptr && (inv->tieneItem("Frasco de Tinta") || inv->tieneItem("Tinta de Viento")));
		bool confianzaAmigos = (qifrey != nullptr && qifrey->getConfianza() >= 2);

		titulos.push_back("Coleccion de Tinta");
		descripciones.push_back("Explora los rincones secretos del taller para hallar frascos de tinta arcaica perdidos y entregaselos a Qifrey.");
		desbloqueadas.push_back(true);
		if (confianzaAmigos) {
			estados.push_back("COMPLETADA");
		} else if (tieneTintaColeccion) {
			estados.push_back("EN PROGRESO");
		} else {
			estados.push_back("DISPONIBLE");
		}

		titulos.push_back("Rescate de Myrphon");
		descripciones.push_back("Encuentra y rescata a Myrphon, la mascota de Richeh, atrapada en el laberinto subterraneo Serpentback.");
		if (misionMyrphonActiva || myrphonRescatado || dioVaraRicheh) {
			desbloqueadas.push_back(true);
			if (dioVaraRicheh) {
				estados.push_back("COMPLETADA");
			} else if (myrphonRescatado) {
				estados.push_back("ENTREGAR");
			} else {
				estados.push_back("EN PROGRESO");
			}
		} else {
			desbloqueadas.push_back(false);
			estados.push_back("BLOQUEADA");
		}
	}
};

#endif
