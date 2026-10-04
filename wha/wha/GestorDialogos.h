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
	bool richehEnojada;
	bool misionMyrphonActiva;
	bool myrphonRescatado;
	bool dioVaraRicheh;
	int cartelCuriosidad;

public:
	GestorDialogos()
		: enDialogo(false), estadoDialogo(0), npcDialogoActual(""),
		  respuestaInicialAgott(0), richehEnojada(false),
		  misionMyrphonActiva(false), myrphonRescatado(false),
		  dioVaraRicheh(false), cartelCuriosidad(0) {}
	~GestorDialogos() {}

	bool getEnDialogo() const { return enDialogo; }
	int getEstadoDialogo() const { return estadoDialogo; }
	std::string getNpcDialogoActual() const { return npcDialogoActual; }
	int getRespuestaInicialAgott() const { return respuestaInicialAgott; }
	bool getRichehEnojada() const { return richehEnojada; }
	bool getMisionMyrphonActiva() const { return misionMyrphonActiva; }
	bool getMyrphonRescatado() const { return myrphonRescatado; }
	bool getDioVaraRicheh() const { return dioVaraRicheh; }
	int getCartelCuriosidad() const { return cartelCuriosidad; }

	void setEnDialogo(bool ed) { enDialogo = ed; }
	void setEstadoDialogo(int ed) { estadoDialogo = ed; }
	void setNpcDialogoActual(const std::string& n) { npcDialogoActual = n; }
	void setRespuestaInicialAgott(int r) { respuestaInicialAgott = r; }
	void setRichehEnojada(bool re) { richehEnojada = re; }
	void setMisionMyrphonActiva(bool ma) { misionMyrphonActiva = ma; }
	void setMyrphonRescatado(bool mr) { myrphonRescatado = mr; }
	void setDioVaraRicheh(bool dv) { dioVaraRicheh = dv; }
	void setCartelCuriosidad(int cc) { cartelCuriosidad = cc; }

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
		(void)dioVaraRicheh;
		if (estadoDialogo >= 400) {
			hablante = "Letrero";
			rol = "Informacion";
			confianza = 0;
		} else if (estadoDialogo >= 300) {
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
			lineas.push_back("*llorando*");
			opciones.push_back("[1] Hola, porque estas llorando?");
			opciones.push_back("[2] Disculpa... cai a este pozo buscando un objeto magico...");
			break;

		case 301:
			lineas.push_back("...No es de tu incumbencia. Vete...");
			lineas.push_back("...Es mi culpa por no haberlo sujetado mas fuerte.");
			opciones.push_back("[1] Que? Pero como te puedo ayudar?");
			opciones.push_back("[2] Por eso estas llorando?");
			break;

		case 302:
			lineas.push_back("¿Ayudarme?... No creo que puedas.");
			lineas.push_back("...Mi Myrphon todavia no regresa. Se quedo atrapado");
			lineas.push_back("en esa horrible cueva Serpentback cuando nos atacaron los magos oscuros...");
			lineas.push_back("...Si de verdad quieres ayudar, traelo de vuelta a este cuarto.");
			lineas.push_back("A cambio te dare una Varita Magica que ya no uso.");
			opciones.push_back("[1] Acepto, traere a Myrphon de vuelta!");
			break;

		case 303:
			lineas.push_back("¡No tienes idea de nada! ¡Largate!");
			lineas.push_back("...¡No quiero volver a ver tu cara por aqui!");
			opciones.push_back("[1] Salir...");
			break;

		case 310:
			lineas.push_back("¿Un item magico? Que persistentes son los viajeros...");
			lineas.push_back("...Tengo una Varita Magica que ya no quiero usar.");
			lineas.push_back("No me interesa la magia tradicional de la Alianza...");
			lineas.push_back("...Pero no te la dare gratis. Mi mente no esta para negociar");
			lineas.push_back("mientras mi pobre Myrphon siga perdido en la oscuridad.");
			opciones.push_back("[1] Lamento escuchar eso... Como te puedo ayudar a recuperarlo?");
			opciones.push_back("[2] Vaya, que mal, y... por un simple animal te pones a llorar?");
			break;

		case 311:
			lineas.push_back("Mi Myrphon se asusto por el ataque de unos magos oscuros");
			lineas.push_back("en el laberinto subterraneo Serpentback...");
			lineas.push_back("...Si entras alli y lo traes a salvo a este cuarto del pozo,");
			lineas.push_back("la Varita Magica sera tuya. ¿Trato?");
			opciones.push_back("[1] Si.");
			break;

		case 312:
			lineas.push_back("¡No es un simple animal! ¡Es mi amigo!");
			lineas.push_back("...¡Vete de mi cuarto ahora mismo!");
			opciones.push_back("[1] Salir...");
			break;

		case 320:
			lineas.push_back("¿Que quieres ahora? Te dije que te largaras.");
			opciones.push_back("[1] Esta bien, ya me iba");
			opciones.push_back("[2] Espera, hablo en serio... Quiero ayudarte a buscar a Myrphon");
			break;

		case 321:
			lineas.push_back("(Te mira de reojo de forma desconfiada) ...¿De verdad?...");
			lineas.push_back("...Esta bien. Se perdio en el laberinto Serpentback por culpa");
			lineas.push_back("de unos magos oscuros. Traelo de vuelta a este cuarto.");
			lineas.push_back("Si lo logras, te dare la Varita Magica que buscas. No me falles.");
			opciones.push_back("[1] Hare lo mejor que pueda y te lo traere!");
			break;

		case 330:
			lineas.push_back("¿Pudiste encontrarlo? ¿Donde esta mi Myrphon?");
			opciones.push_back("[1] Aun no...");
			break;

		case 331:
			lineas.push_back("Por favor, date prisa... El laberinto Serpentback es muy oscuro");
			lineas.push_back("y debe tener mucho miedo. Estare esperando aqui.");
			opciones.push_back("[1] Entendido...");
			break;

		case 350:
			lineas.push_back("(Sus ojos se abren de par en par al ver al animal) \"¡¡Myrphon!!\"");
			opciones.push_back("[1] Aqui esta, sano y salvo.");
			break;

		case 351:
			lineas.push_back("(Abraza fuertemente a su mascota mientras llora de alegria)");
			lineas.push_back("¡Muchas gracias! Pense que no volveria a verlo...");
			lineas.push_back("...Lo prometido es deuda. Toma esto, es la Varita Magica");
			lineas.push_back("de la Alianza. A mi no me sirve para mi tipo de magia,");
			lineas.push_back("pero a ti te sera muy util... Gracias de nuevo, aventurero.");
			lineas.push_back("Ahora, si me disculpas, pasare tiempo con mi amigo.");
			opciones.push_back("[1] Muchas gracias Richeh!");
			break;

		case 360:
			lineas.push_back("( •u• ): Hola!! Myrphon y yo estamos muy felices");
			lineas.push_back("gracias a ti. Ten cuidado en tus viajes!!");
			opciones.push_back("[1] Nos vemos Richeh!");
			break;

		case 400:
			lineas.push_back("[DATO CURIOSO DE RICHEH #1]");
			lineas.push_back("Richeh es hermana mayor de Eini, uno de los mejores estudiantes");
			lineas.push_back("de la estricta academia de magos (la Alianza)...");
			opciones.push_back("[1] Continuar...");
			break;

		case 401:
			lineas.push_back("...Mientras el es perfeccionista, sigue todas las reglas");
			lineas.push_back("y se preocupa por el estatus, Richeh lo ignora activamente");
			lineas.push_back("y considera que la academia es una carcel que destruye la creatividad.");
			opciones.push_back("[1] Cerrar");
			break;

		case 402:
			lineas.push_back("[DATO CURIOSO DE RICHEH #2]");
			lineas.push_back("Cuando Richeh se enfada o no quiere hacer algo, se pone");
			lineas.push_back("rigida como un mueble y sus companeras tienen que");
			lineas.push_back("cargarla en peso para moverla.");
			opciones.push_back("[1] Cerrar");
			break;

		case 404:
			lineas.push_back("[DATO CURIOSO DE RICHEH #3]");
			lineas.push_back("La autora, Kamome Shirahama, diseno las tunicas y el cabello");
			lineas.push_back("de Richeh con lineas muy rectas, pesadas y rigidas...");
			opciones.push_back("[1] Continuar...");
			break;

		case 405:
			lineas.push_back("...para reflejar visualmente lo 'cuadrada' y terca");
			lineas.push_back("que es su personalidad.");
			opciones.push_back("[1] Cerrar");
			break;

		case 410:
			lineas.push_back("[DIARIO DEL ATELIER: MYRPHON PERDIDO]");
			lineas.push_back("Richeh, companera de cuarto de Agott, no le gusta las visitas");
			lineas.push_back("desde que se perdio Myrphon, un pequeno pinguino con rasgos de grifo.");
			lineas.push_back("Myrphon se perdio tras el ataque inesperado de unos magos oscuros");
			lineas.push_back("en el laberinto subterraneo Serpentback durante el segundo examen...");
			opciones.push_back("[1] Continuar...");
			break;

		case 411:
			lineas.push_back("...El examen evaluaba si un aprendiz usa la magia en secreto.");
			lineas.push_back("Se cancelo por el caos, el pobre Myrphon quedo atrapado.");
			lineas.push_back("Richeh y Agott tienen la esperanza de que algun aventurero");
			lineas.push_back("valiente pueda encontrarlo sano y salvo.");
			opciones.push_back("[1] Cerrar");
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
			if (opcion == 1) {
				estadoDialogo = 301;
			} else if (opcion == 2) {
				estadoDialogo = 310;
			}
		} else if (estadoDialogo == 301) {
			if (opcion == 1) {
				estadoDialogo = 302;
			} else if (opcion == 2) {
				if (richeh != nullptr && richeh->getConfianza() > 0) {
					richeh->setConfianza(richeh->getConfianza() - 1);
				}
				richehEnojada = true;
				estadoDialogo = 303;
			}
		} else if (estadoDialogo == 302) {
			misionMyrphonActiva = true;
			if (richeh != nullptr && richeh->getConfianza() < 2) {
				richeh->setConfianza(richeh->getConfianza() + 1);
			}
			puntosMisiones += 15;
			objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 303) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 310) {
			if (opcion == 1) {
				estadoDialogo = 311;
			} else if (opcion == 2) {
				if (richeh != nullptr && richeh->getConfianza() > 0) {
					richeh->setConfianza(richeh->getConfianza() - 1);
				}
				richehEnojada = true;
				estadoDialogo = 312;
			}
		} else if (estadoDialogo == 311) {
			misionMyrphonActiva = true;
			puntosMisiones += 15;
			objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 312) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 320) {
			if (opcion == 1) {
				enDialogo = false;
				estadoDialogo = 0;
			} else if (opcion == 2) {
				estadoDialogo = 321;
			}
		} else if (estadoDialogo == 321) {
			misionMyrphonActiva = true;
			richehEnojada = false;
			puntosMisiones += 15;
			objetivoActual = "Buscar a Myrphon en el laberinto Serpentback";
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 330) {
			estadoDialogo = 331;
		} else if (estadoDialogo == 331) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 350) {
			estadoDialogo = 351;
		} else if (estadoDialogo == 351) {
			dioVaraRicheh = true;
			this->dioVaraRicheh = true;
			misionMyrphonActiva = false;
			if (inv != nullptr && !inv->tieneItem("Vara magica")) {
				inv->agregarItem(new ItemMagico(0, 0, "Vara magica", "Varita Magica de la Alianza entregada por Richeh tras rescatar a Myrphon. Permite lanzar fuego para derribar muros.", "Herramienta Magica", true));
				puntosMisiones += 50;
				promptFlotante = "[Recibiste: Vara magica]";
			}
			if (richeh != nullptr) {
				richeh->setYaHablo(true);
				richeh->setConfianza(2);
				richeh->setExpresion(2);
			}
			objetivoActual = "Usar la Vara magica para derribar el muro del almacen";
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 360) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 400) {
			estadoDialogo = 401;
		} else if (estadoDialogo == 401 || estadoDialogo == 402) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 404) {
			estadoDialogo = 405;
		} else if (estadoDialogo == 405) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 410) {
			estadoDialogo = 411;
		} else if (estadoDialogo == 411) {
			enDialogo = false;
			estadoDialogo = 0;
		} else {
			enDialogo = false;
			estadoDialogo = 0;
		}
	}
};

#endif
