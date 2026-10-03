#ifndef GestorDialogos_h
#define GestorDialogos_h
#include <string>
#include <vector>
#include "NPC.h"
#include "Protagonista.h"
#include "Inventario.h"
#include "ItemMagico.h"

class GestorDialogos {
private:
	bool enDialogo;
	int estadoDialogo;
	std::string npcDialogoActual;
	int respuestaInicialAgott;

public:
	GestorDialogos() : enDialogo(false), estadoDialogo(0), npcDialogoActual(""), respuestaInicialAgott(0) {}
	~GestorDialogos() {}

	bool getEnDialogo() const { return enDialogo; }
	int getEstadoDialogo() const { return estadoDialogo; }
	std::string getNpcDialogoActual() const { return npcDialogoActual; }
	int getRespuestaInicialAgott() const { return respuestaInicialAgott; }

	void setEnDialogo(bool ed) { enDialogo = ed; }
	void setEstadoDialogo(int ed) { estadoDialogo = ed; }
	void setNpcDialogoActual(const std::string& n) { npcDialogoActual = n; }
	void setRespuestaInicialAgott(int r) { respuestaInicialAgott = r; }

	void iniciarDialogo(const std::string& npc, int estadoInicial) {
		enDialogo = true;
		npcDialogoActual = npc;
		estadoDialogo = estadoInicial;
	}

	void iniciarDialogoAgott() {
		int estadoIni = (respuestaInicialAgott == 2) ? 220 : 210;
		iniciarDialogo("Agott", estadoIni);
	}

	void terminarDialogo() {
		enDialogo = false;
		estadoDialogo = 0;
	}

	void obtenerDatosDialogo(Protagonista* protagonista, NPC* qifrey, NPC* agott, NPC* richeh,
	                         bool dioVaraRicheh, bool libroEncontrado,
	                         std::string& hablante, std::string& rol, int& confianza,
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
			lineas.push_back("Hola, Coco. Que necesitas en el taller hoy?");
			opciones.push_back("[1] Maestro Qifrey, podrias ayudarme a craftear la Capa Magica?");
			opciones.push_back("[2] Solo venia a explorar el taller y ver tus libros.");
			opciones.push_back("[3] Maestro, necesito materiales especiales para mis practicas.");
			if (inv != nullptr && (inv->tieneItem("Frasco de Tinta") || inv->tieneItem("Tinta de Viento"))) {
				opciones.push_back("[4] Maestro, encontre una tinta especial explorando el taller!");
			}
			break;

		case 10:
			if (inv != nullptr) {
				bool tieneTela = inv->tieneItem("Tela");
				bool tieneTinta = inv->tieneItem("Tinta magica");
				bool tieneLibro = inv->tieneItem("Libro de hechizos");
				int total = (tieneTela ? 1 : 0) + (tieneTinta ? 1 : 0) + (tieneLibro ? 1 : 0);

				if (total == 0) {
					lineas.push_back("Para la Capa Magica necesito 3 items: Tela, Tinta magica y Libro.");
					lineas.push_back("Aun no tienes ninguno.\nBusca en el taller y el almacen abandonado!");
					opciones.push_back("[1] Esta bien, ire a buscarlos por el atelier.");
				} else {
					lineas.push_back("Te faltan materiales para craftear la Capa Magica.");
					std::string faltantes = "Aun necesitas encontrar: ";
					if (!tieneTela) faltantes += "[Tela] ";
					if (!tieneTinta) faltantes += "[Tinta magica] ";
					if (!tieneLibro) faltantes += "[Libro de hechizos] ";
					lineas.push_back(faltantes);
					lineas.push_back("Vuelve cuando tengas los 3 ingredientes completos!");
					opciones.push_back("[1] Entendido, buscare lo que falta.");
				}
			}
			break;

		case 11:
			lineas.push_back("Esta bien, te hare la capa!");
			lineas.push_back("");
			lineas.push_back("Crafteando capa magica...");
			lineas.push_back("");
			lineas.push_back("Aqui esta, te dare esta capa pero ojo... usalo responsablemente!");
			opciones.push_back("[1] Entendido!");
			break;

		case 12:
			lineas.push_back("[CRAFTEO EXITOSO: Capa magica obtenida]");
			lineas.push_back("Se consumieron: Tela, Tinta magica y Libro de hechizos.");
			lineas.push_back("La Capa Magica ha sido equipada y agregada a tu inventario.");
			opciones.push_back("[1] Muchas gracias Maestro Qifrey!");
			break;

		case 15:
			lineas.push_back("Te queda excelente la Capa Magica, Coco.");
			lineas.push_back("Recuerda usar tus alas de aprendiz\ncon verdadera sabiduria y responsabilidad.");
			opciones.push_back("[1] Gracias Maestro Qifrey!");
			break;

		case 20:
			lineas.push_back("Eres bienvenida en el taller siempre, Coco.");
			lineas.push_back("Cuidate de las corrientes del gran rio.\nCruza siempre por los puentes arcanos.");
			opciones.push_back("[1] Gracias por el consejo, Maestro.");
			break;

		case 30:
			lineas.push_back("Las practicas de hechiceria requieren precision y paciencia.");
			lineas.push_back("Que tipo de material magico estas buscando exactamente?");
			opciones.push_back("[1] Busco una tinta que reaccione al flujo magico del pergamino.");
			opciones.push_back("[2] Cualquier material basico me servira para practicar.");
			break;

		case 31:
			lineas.push_back("La tinta magica de plata es muy delicada y poderosa.");
			lineas.push_back("Sabras usarla con cuidado y verdadero respeto al atelier?");
			opciones.push_back("[1] Prometo seguir las reglas del atelier y ser responsable.");
			opciones.push_back("[2] Intentare tener cuidado, aunque a veces me cuesta.");
			break;

		case 32:
			lineas.push_back("Bien dicho, Coco. Veo determinacion y honestidad en tus ojos.");
			lineas.push_back("Te doy este item: Tinta magica.");
			opciones.push_back("[1] Muchas gracias Qifrey, me servira de mucho!");
			break;

		case 33:
			lineas.push_back("[HAS OBTENIDO: Tinta magica]");
			lineas.push_back("Se ha agregado a tu inventario.");
			lineas.push_back("Tu vinculo y confianza con Maestro Qifrey han aumentado!");
			opciones.push_back("[1] Continuar explorando");
			break;

		case 34:
			lineas.push_back("Ya te he entregado la Tinta magica, Coco.");
			lineas.push_back("Revisa tu mochila y dale buen uso en tus pergaminos.");
			opciones.push_back("[1] Entendido Maestro.");
			break;

		case 40:
			lineas.push_back("Increible hallazgo, Coco! Esta tinta arcaica es justo lo");
			lineas.push_back("que necesitabamos en el taller para restaurar pergaminos.");
			lineas.push_back("Demuestras una gran curiosidad y respeto por este atelier.");
			lineas.push_back("Has demostrado ser una verdadera amiga y gran aprendiz!");
			opciones.push_back("[1] Me alegra mucho ser de ayuda, Maestro!");
			break;

		case 41:
			lineas.push_back("[MISION SECUNDARIA COMPLETADA]");
			lineas.push_back("Entregaste la tinta arcaica al Maestro Qifrey.");
			lineas.push_back("Tu nivel de confianza con Qifrey ahora es: Amigos (+50 pts)");
			opciones.push_back("[1] Continuar explorando");
			break;

		case 99:
			lineas.push_back("[INVENTARIO LLENO: Capacidad maxima 6 items alcanzada]");
			lineas.push_back("No puedes recibir mas items en este momento.");
			opciones.push_back("[1] Volver");
			break;

		case 200:
			lineas.push_back("Vaya vaya... miren a quien tenemos aqui.");
			opciones.push_back("[1] Siguiente");
			break;

		case 201:
			lineas.push_back("A la joven y pequena Coco, porque entraste a mi torre?");
			opciones.push_back("[1] Necesito encontrar un libro");
			opciones.push_back("[2] A ti que te importa, Agott?");
			break;

		case 202:
			lineas.push_back("Puedes encontrarlo en este resto de cajas si quieres...\nAl final, solo son basura.");
			opciones.push_back("[1] Entendido");
			break;

		case 203:
			lineas.push_back("Largate de aqui!");
			opciones.push_back("[1] Ya me voy...");
			break;

		case 210:
			lineas.push_back("Que paso ahora, nina?");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Solo pasaba por aqui, ya encontre el libro, Agott.");
			} else {
				opciones.push_back("[1] Sigo buscando el libro, necesito ayuda...");
			}
			break;

		case 211:
			lineas.push_back("No esperaba que lo encuentres en esta basura jaja.");
			opciones.push_back("[1] Continuar");
			break;

		case 212:
			lineas.push_back("Sabia que no eras util para eso JAJAJA.\nPrueba empujando las cajas de la torre.");
			opciones.push_back("[1] Gracias por nada...");
			break;

		case 220:
			lineas.push_back("Porque sigues aqui, Coco?? No eres bienvenida.");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Nada, solo queria burlarme de tu cara.");
			} else {
				opciones.push_back("[1] Sigo buscando algo, deja de molestar!");
			}
			break;

		case 221:
			lineas.push_back("Estupida nina!");
			opciones.push_back("[1] Salir");
			break;

		case 222:
			lineas.push_back("Que demonios estas buscando??");
			opciones.push_back("[1] QUE- TE- IMPORTA!!!!");
			break;

		case 223:
			lineas.push_back("...");
			opciones.push_back("[1] Salir");
			break;

		case 300:
			if (!dioVaraRicheh) {
				lineas.push_back("Hola Coco... Que sorpresa verte por aqui abajo.");
				lineas.push_back("Toma mi vieja vara magica. Con ella podras");
				lineas.push_back("derribar muros de piedra lanzando fuego.");
				opciones.push_back("[1] Muchas gracias Richeh!");
			} else {
				lineas.push_back("Usa la vara magica con sabiduria, Coco.");
				lineas.push_back("Recuerda que el fuego magico responde a tu voluntad.");
				opciones.push_back("[1] Entendido Richeh!");
			}
			break;

		default:
			lineas.push_back("Continua tu aprendizaje con dedicacion, Coco.");
			opciones.push_back("[1] Salir");
			break;
		}
	}

	void procesarOpcionDialogo(int opcion, Protagonista* protagonista,
	                           NPC* qifrey, NPC* agott, NPC* richeh,
	                           bool& dioVaraRicheh, bool libroEncontrado,
	                           std::string& promptFlotante, std::string& objetivoActual,
	                           int& puntosMisiones, bool& completado, bool& mostrarEstadisticasFin) {
		procesarOpcionDialogo(opcion, protagonista, qifrey, agott, richeh, dioVaraRicheh, libroEncontrado,
		                      this->respuestaInicialAgott, promptFlotante, objetivoActual, puntosMisiones,
		                      completado, mostrarEstadisticasFin);
	}

	void procesarOpcionDialogo(int opcion, Protagonista* protagonista,
	                           NPC* qifrey, NPC* agott, NPC* richeh,
	                           bool& dioVaraRicheh, bool libroEncontrado,
	                           int& respuestaInicialAgott,
	                           std::string& promptFlotante, std::string& objetivoActual,
	                           int& puntosMisiones, bool& completado, bool& mostrarEstadisticasFin) {
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
			} else if (opcion == 4) {
				if (inv != nullptr && (inv->tieneItem("Frasco de Tinta") || inv->tieneItem("Tinta de Viento"))) {
					estadoDialogo = 40;
				} else {
					enDialogo = false;
					estadoDialogo = 0;
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
				puntosMisiones += 100;
				objetivoActual = "Capa Magica crafteada! Mision Cumplida";
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
						puntosMisiones += 50;
						objetivoActual = "Buscar Tela y Libro de hechizos";
						estadoDialogo = 33;
					} else {
						estadoDialogo = 99;
					}
				}
			}
		} else if (estadoDialogo == 33 || estadoDialogo == 34 || estadoDialogo == 99) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 40) {
			if (inv != nullptr) {
				if (inv->tieneItem("Frasco de Tinta")) {
					inv->removerItem("Frasco de Tinta");
				} else if (inv->tieneItem("Tinta de Viento")) {
					inv->removerItem("Tinta de Viento");
				}
			}
			if (qifrey != nullptr) {
				qifrey->setConfianza(2);
			}
			puntosMisiones += 50;
			estadoDialogo = 41;
		} else if (estadoDialogo == 41) {
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
					puntosMisiones += 25;
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
};

#endif
