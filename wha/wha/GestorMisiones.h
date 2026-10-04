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

	void cargarMisionesNivel1(Protagonista* protagonista, NPC* qifrey, int estadoDialogo,
	                          std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas,
	                          bool misionMyrphonActiva,
	                          bool myrphonRescatado,
	                          bool dioVaraRicheh) {
		Inventario* inv = (protagonista != nullptr) ? protagonista->getInventario() : nullptr;
		bool habloConQifrey = (qifrey != nullptr && (qifrey->getDioTinta() || qifrey->getCrafteoCapa() || estadoDialogo > 1));
		bool tieneFibra = (inv != nullptr && inv->tieneItem("Fibra de Arbol"));
		bool tieneTinta = (inv != nullptr && inv->tieneItem("Tinta magica"));
		bool tieneLibro = (inv != nullptr && inv->tieneItem("Grimorio de Trazos"));
		bool crafteoCapa = (qifrey != nullptr && qifrey->getCrafteoCapa());
		bool tieneMateriales = (tieneFibra && tieneTinta && tieneLibro) || crafteoCapa;

		titulos.push_back("Hablar con Qifrey");
		descripciones.push_back("Encuentra al Maestro Qifrey en el atelier y dialoga\ncon él sobre la confección de la Capa Mágica.");
		desbloqueadas.push_back(true);
		if (habloConQifrey) {
			estados.push_back("COMPLETADA");
		} else {
			estados.push_back("EN PROGRESO");
		}

		titulos.push_back("Conseguir Materiales");
		descripciones.push_back("Recolecta en el atelier los 3 materiales indispensables:\nFibra de Arbol, Grimorio de Trazos y Tinta magica.");
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

		titulos.push_back("Confeccionar Capa Mágica");
		descripciones.push_back("Regresa con el Maestro Qifrey y entrega los materiales\npara confeccionar la legendaria Capa Mágica.");
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

		titulos.push_back("Colección de Tinta");
		descripciones.push_back("Explora los rincones secretos del taller para hallar frascos\nde tinta arcaica perdidos y entregárselos a Qifrey.");
		desbloqueadas.push_back(true);
		if (confianzaAmigos) {
			estados.push_back("COMPLETADA");
		} else if (tieneTintaColeccion) {
			estados.push_back("EN PROGRESO");
		} else {
			estados.push_back("DISPONIBLE");
		}

		titulos.push_back("Rescate de Myrphon");
		descripciones.push_back("Encuentra y rescata a Myrphon, la mascota de Richeh,\nperdida tras el ataque de Sombreros de Ala Ancha en Serpentback.");
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

	void cargarMisionesNivel2(Protagonista* protagonista,
	                          std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas) {
		(void)protagonista;
		titulos.push_back("Explorar el Pueblo");
		descripciones.push_back("Recorre el laberinto de calles, casas y callejones\nde Kaln buscando pistas sobre Coustas.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");

		titulos.push_back("Testimonios de Aldeanos");
		descripciones.push_back("Lee los letreros y testimonios dispersos en el pueblo\npara comprender los recientes disturbios.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");

		titulos.push_back("Rastro de Ininia");
		descripciones.push_back("Sigue las huellas y susurros de la hechicera Ininia\npara descubrir su intervencion en Kaln.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");

		titulos.push_back("Camino al Arbol de Plata");
		descripciones.push_back("Localiza el pasaje que conecta el pueblo con el sendero\nhacia el Gran Arbol de Plata.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");
	}

	void cargarMisionesNivel3(std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas) {
		titulos.push_back("El Gran Arbol");
		descripciones.push_back("Asciende por las ramas ancestrales del Arbol de Plata\nenfrentando la hechiceria prohibida.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");

		titulos.push_back("El Contrahechizo");
		descripciones.push_back("Halla el grabado arcano primordial para revertir\nla petrificacion y sellar el pacto.");
		desbloqueadas.push_back(true);
		estados.push_back("EN PROGRESO");
	}

	void obtenerDatosMisiones(int numeroNivel, Protagonista* protagonista, NPC* qifrey, int estadoDialogo,
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

		switch (numeroNivel) {
		case 1:
			cargarMisionesNivel1(protagonista, qifrey, estadoDialogo, titulos, descripciones, estados, desbloqueadas, misionMyrphonActiva, myrphonRescatado, dioVaraRicheh);
			break;
		case 2:
			cargarMisionesNivel2(protagonista, titulos, descripciones, estados, desbloqueadas);
			break;
		case 3:
			cargarMisionesNivel3(titulos, descripciones, estados, desbloqueadas);
			break;
		default:
			cargarMisionesNivel1(protagonista, qifrey, estadoDialogo, titulos, descripciones, estados, desbloqueadas, misionMyrphonActiva, myrphonRescatado, dioVaraRicheh);
			break;
		}
	}
};

#endif
