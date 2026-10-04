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
	bool agottSabeMyrphon;

public:
	GestorDialogos()
		: enDialogo(false), estadoDialogo(0), npcDialogoActual(""),
		  respuestaInicialAgott(0), richehEnojada(false),
		  misionMyrphonActiva(false), myrphonRescatado(false),
		  dioVaraRicheh(false), agottSabeMyrphon(false) {}
	~GestorDialogos() {}

	bool getEnDialogo() const { return enDialogo; }
	int getEstadoDialogo() const { return estadoDialogo; }
	std::string getNpcDialogoActual() const { return npcDialogoActual; }
	bool getRichehEnojada() const { return richehEnojada; }
	bool getMisionMyrphonActiva() const { return misionMyrphonActiva; }
	bool getMyrphonRescatado() const { return myrphonRescatado; }
	bool getDioVaraRicheh() const { return dioVaraRicheh; }

	void setRichehEnojada(bool re) { richehEnojada = re; }
	void setMisionMyrphonActiva(bool ma) { misionMyrphonActiva = ma; }
	void setMyrphonRescatado(bool mr) { myrphonRescatado = mr; }
	void setDioVaraRicheh(bool dv) { dioVaraRicheh = dv; }

	void iniciarDialogo(const std::string& npc, int estadoInicial) {
		enDialogo = true;
		npcDialogoActual = npc;
		estadoDialogo = estadoInicial;
	}

	void iniciarDialogoAgott() {
		if (agottSabeMyrphon) {
			iniciarDialogo("Agott", 230);
		} else {
			int estadoIni = (respuestaInicialAgott == 2) ? 220 : 210;
			iniciarDialogo("Agott", estadoIni);
		}
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
			lineas.push_back("Hola, Coco. ¿Qué necesitas en el taller hoy?");
			opciones.push_back("[1] Maestro Qifrey, ¿podrías ayudarme a confeccionar la Capa Mágica?");
			opciones.push_back("[2] Solo venía a explorar el taller y ver tus libros.");
			opciones.push_back("[3] Maestro, necesito materiales especiales para mis prácticas.");
			if (inv != nullptr && (inv->tieneItem("Frasco de Tinta") || inv->tieneItem("Tinta de Viento"))) {
				opciones.push_back("[4] ¡Maestro, encontré una tinta especial explorando el taller!");
			}
			break;

		case 10:
			if (inv != nullptr) {
				bool tieneTela = inv->tieneItem("Tela");
				bool tieneTinta = inv->tieneItem("Tinta magica");
				bool tieneLibro = inv->tieneItem("Libro de hechizos");
				int total = (tieneTela ? 1 : 0) + (tieneTinta ? 1 : 0) + (tieneLibro ? 1 : 0);

				if (total == 0) {
					lineas.push_back("Para la Capa Mágica necesito 3 objetos: Tela, Tinta mágica y Libro.");
					lineas.push_back("Aún no tienes ninguno.\n¡Busca en el taller y en el almacén abandonado!");
					opciones.push_back("[1] Está bien, iré a buscarlos por el atelier.");
				} else {
					lineas.push_back("Te faltan materiales para confeccionar la Capa Mágica.");
					std::string faltantes = "Aún necesitas encontrar: ";
					if (!tieneTela) faltantes += "[Tela] ";
					if (!tieneTinta) faltantes += "[Tinta mágica] ";
					if (!tieneLibro) faltantes += "[Libro de hechizos] ";
					lineas.push_back(faltantes);
					lineas.push_back("¡Vuelve cuando tengas los 3 ingredientes completos!");
					opciones.push_back("[1] Entendido, buscaré lo que falta.");
				}
			}
			break;

		case 11:
			lineas.push_back("¡Está bien, te haré la capa!");
			lineas.push_back("");
			lineas.push_back("Confeccionando capa mágica...");
			lineas.push_back("");
			lineas.push_back("¡Aquí está! Te daré esta capa, pero úsala responsablemente.");
			opciones.push_back("[1] ¡Entendido!");
			break;

		case 12:
			lineas.push_back("[CONFECCIÓN EXITOSA: Capa Mágica obtenida]");
			lineas.push_back("Se consumieron: Tela, Tinta mágica y Libro de hechizos.");
			lineas.push_back("La Capa Mágica ha sido equipada y agregada a tu inventario.");
			opciones.push_back("[1] ¡Muchas gracias, Maestro Qifrey!");
			break;

		case 15:
			lineas.push_back("Te queda excelente la Capa Mágica, Coco.");
			lineas.push_back("Recuerda usar tus alas de aprendiz\ncon verdadera sabiduría y responsabilidad.");
			opciones.push_back("[1] ¡Gracias, Maestro Qifrey!");
			break;

		case 20:
			lineas.push_back("Eres bienvenida en el taller siempre, Coco.");
			lineas.push_back("Cuídate de las corrientes del gran río.\nCruza siempre por los puentes arcanos.");
			opciones.push_back("[1] Gracias por el consejo, Maestro.");
			break;

		case 30:
			lineas.push_back("Las prácticas de hechicería requieren precisión y paciencia.");
			lineas.push_back("¿Qué tipo de material mágico estás buscando exactamente?");
			opciones.push_back("[1] Busco una tinta que reaccione al flujo mágico del pergamino.");
			opciones.push_back("[2] Cualquier material básico me servirá para practicar.");
			break;

		case 31:
			lineas.push_back("La tinta mágica de plata es muy delicada y poderosa.");
			lineas.push_back("¿Sabrás usarla con cuidado y verdadero respeto al atelier?");
			opciones.push_back("[1] Prometo seguir las reglas del atelier y ser responsable.");
			opciones.push_back("[2] Intentaré tener cuidado, aunque a veces me cuesta.");
			break;

		case 32:
			lineas.push_back("Bien dicho, Coco. Veo determinación y honestidad en tus ojos.");
			lineas.push_back("Te doy este objeto: Tinta mágica.");
			opciones.push_back("[1] ¡Muchas gracias Qifrey, me servirá de mucho!");
			break;

		case 33:
			lineas.push_back("[HAS OBTENIDO: Tinta mágica]");
			lineas.push_back("Se ha agregado a tu inventario.");
			lineas.push_back("¡Tu vínculo y confianza con Maestro Qifrey han aumentado!");
			opciones.push_back("[1] Continuar explorando");
			break;

		case 34:
			lineas.push_back("Ya te he entregado la Tinta mágica, Coco.");
			lineas.push_back("Revisa tu mochila y dale buen uso en tus pergaminos.");
			opciones.push_back("[1] Entendido, Maestro.");
			break;

		case 35:
			lineas.push_back("En la hechicería no existe tal cosa como un material 'básico'.");
			lineas.push_back("Los sellos no despiertan con tinta ordinaria; solo la tinta de plata,");
			lineas.push_back("nacida de minerales arcanos, canaliza el flujo mágico al pergamino.");
			lineas.push_back("Es la única que poseo en el taller... ¿Aún deseas aceptarla, Coco?");
			opciones.push_back("[1] ¡Sí, por favor! Prometo cuidarla y esforzarme al máximo.");
			break;

		case 40:
			lineas.push_back("¡Increíble hallazgo, Coco! Esta tinta arcaica es justo lo");
			lineas.push_back("que necesitábamos en el taller para restaurar pergaminos.");
			lineas.push_back("Demuestras una gran curiosidad y respeto por este atelier.");
			lineas.push_back("¡Has demostrado ser una verdadera amiga y gran aprendiz!");
			opciones.push_back("[1] ¡Me alegra mucho ser de ayuda, Maestro!");
			break;

		case 41:
			lineas.push_back("[MISIÓN SECUNDARIA COMPLETADA]");
			lineas.push_back("Entregaste la tinta arcaica al Maestro Qifrey.");
			lineas.push_back("Tu nivel de confianza con Qifrey ahora es: Amigos (+50 pts)");
			opciones.push_back("[1] Continuar explorando");
			break;

		case 99:
			lineas.push_back("[INVENTARIO LLENO: Capacidad máxima 6 objetos alcanzada]");
			lineas.push_back("No puedes recibir más objetos en este momento.");
			opciones.push_back("[1] Volver");
			break;

		case 200:
			lineas.push_back("Vaya, vaya... Miren a quién tenemos aquí.");
			opciones.push_back("[1] Siguiente");
			break;

		case 201:
			lineas.push_back("A la joven y pequeña Coco, ¿por qué entraste a mi torre?");
			opciones.push_back("[1] Necesito encontrar un libro.");
			opciones.push_back("[2] ¿A ti qué te importa, Agott?");
			break;

		case 202:
			lineas.push_back("Puedes encontrarlo en este montón de cajas si quieres...\nAl final, solo son basura.");
			opciones.push_back("[1] Entendido");
			break;

		case 203:
			lineas.push_back("¡Lárgate de aquí!");
			opciones.push_back("[1] Ya me voy...");
			break;

		case 210:
			lineas.push_back("¿Qué pasó ahora, niña?");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Solo pasaba por aquí, ya encontré el libro, Agott.");
			} else {
				opciones.push_back("[1] Sigo buscando el libro, necesito ayuda...");
			}
			if (myrphonRescatado) {
				opciones.push_back("[2] Recuperé a su mascota, está a salvo con Richeh.");
			}
			break;

		case 211:
			lineas.push_back("No esperaba que lo encontraras en esta basura, jaja.");
			opciones.push_back("[1] Continuar");
			break;

		case 212:
			lineas.push_back("Sabía que no eras útil para eso, jajaja.\nPrueba empujando las cajas de la torre.");
			opciones.push_back("[1] Gracias por nada...");
			break;

		case 220:
			lineas.push_back("¿Por qué sigues aquí, Coco? No eres bienvenida.");
			if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				opciones.push_back("[1] Nada, solo quería burlarme de tu cara.");
			} else {
				opciones.push_back("[1] ¡Sigo buscando algo, deja de molestar!");
			}
			if (myrphonRescatado) {
				opciones.push_back("[2] Recuperé a su mascota, está a salvo con Richeh.");
			}
			break;

		case 221:
			lineas.push_back("¡Estúpida niña!");
			opciones.push_back("[1] Salir");
			break;

		case 222:
			lineas.push_back("¿Qué demonios estás buscando?");
			opciones.push_back("[1] ¡¡QUÉ TE IMPORTA!!");
			break;

		case 223:
			lineas.push_back("...");
			opciones.push_back("[1] Salir");
			break;

		case 230:
			lineas.push_back("El taller ya volvió a la normalidad. Deja de perder el tiempo y concéntrate en tu viaje.");
			opciones.push_back("[1] Salir");
			break;

		case 231:
			lineas.push_back("...");
			opciones.push_back("[1] Continuar...");
			break;

		case 232:
			lineas.push_back("(Sus ojos se abren de golpe, perdiendo toda su compostura)");
			lineas.push_back("¿Hablas en serio? ¿E-Esa criatura tan adorable está bien?...");
			opciones.push_back("[1] Continuar...");
			break;

		case 233:
			lineas.push_back("(Se aclara la garganta rápidamente y recupera su postura seria)");
			lineas.push_back("Quiero decir... Qué buena noticia para Richeh.");
			lineas.push_back("Ella... ha estado muy distraída sin él.");
			opciones.push_back("[1] Continuar...");
			break;

		case 234:
			lineas.push_back("Supongo que tengo que reconocer tu esfuerzo.");
			lineas.push_back("No cualquiera sobrevive al laberinto Serpentback...");
			opciones.push_back("[1] Continuar...");
			break;

		case 235:
			lineas.push_back("...Gracias por traerlo de vuelta.");
			lineas.push_back("Has hecho un trabajo aceptable, Coco.");
			opciones.push_back("[1] ¡De nada, Agott!");
			break;

		case 300:
			lineas.push_back("*llorando*");
			opciones.push_back("[1] Hola, ¿por qué estás llorando?");
			opciones.push_back("[2] Disculpa... caí a este pozo buscando un objeto mágico...");
			break;

		case 301:
			lineas.push_back("...No es de tu incumbencia. Vete...");
			lineas.push_back("...Es mi culpa por no haberlo sujetado más fuerte.");
			opciones.push_back("[1] ¿Qué? Pero ¿cómo te puedo ayudar?");
			opciones.push_back("[2] ¿Por eso estás llorando?");
			break;

		case 302:
			lineas.push_back("¿Ayudarme?... No creo que puedas.");
			lineas.push_back("...Mi Myrphon todavía no regresa. Se quedó atrapado");
			lineas.push_back("en esa horrible cueva Serpentback cuando nos atacaron los magos oscuros...");
			lineas.push_back("...Si de verdad quieres ayudar, tráelo de vuelta a este cuarto.");
			lineas.push_back("A cambio te daré una Varita Mágica que ya no uso.");
			opciones.push_back("[1] ¡Acepto, traeré a Myrphon de vuelta!");
			break;

		case 303:
			lineas.push_back("¡No tienes idea de nada! ¡Lárgate!");
			lineas.push_back("...¡No quiero volver a ver tu cara por aquí!");
			opciones.push_back("[1] Salir...");
			break;

		case 310:
			lineas.push_back("¿Un objeto mágico? Qué persistentes son los viajeros...");
			lineas.push_back("...Tengo una Varita Mágica que ya no quiero usar.");
			lineas.push_back("No me interesa la magia tradicional de la Alianza...");
			lineas.push_back("...Pero no te la daré gratis. Mi mente no está para negociar");
			lineas.push_back("mientras mi pobre Myrphon siga perdido en la oscuridad.");
			opciones.push_back("[1] Lamento escuchar eso... ¿Cómo te puedo ayudar a recuperarlo?");
			opciones.push_back("[2] Vaya, qué mal, y... ¿por un simple animal te pones a llorar?");
			break;

		case 311:
			lineas.push_back("Mi Myrphon se asustó por el ataque de unos magos oscuros");
			lineas.push_back("en el laberinto subterráneo Serpentback...");
			lineas.push_back("...Si entras allí y lo traes a salvo a este cuarto del pozo,");
			lineas.push_back("la Varita Mágica será tuya. ¿Trato?");
			opciones.push_back("[1] Sí.");
			break;

		case 312:
			lineas.push_back("¡No es un simple animal! ¡Es mi amigo!");
			lineas.push_back("...¡Vete de mi cuarto ahora mismo!");
			opciones.push_back("[1] Salir...");
			break;

		case 320:
			lineas.push_back("¿Qué quieres ahora? Te dije que te largaras.");
			opciones.push_back("[1] Está bien, ya me iba.");
			opciones.push_back("[2] Espera, hablo en serio... Quiero ayudarte a buscar a Myrphon.");
			break;

		case 321:
			lineas.push_back("(Te mira de reojo de forma desconfiada) ...¿De verdad?...");
			lineas.push_back("...Está bien. Se perdió en el laberinto Serpentback por culpa");
			lineas.push_back("de unos magos oscuros. Tráelo de vuelta a este cuarto.");
			lineas.push_back("Si lo logras, te daré la Varita Mágica que buscas. No me falles.");
			opciones.push_back("[1] ¡Haré lo mejor que pueda y te lo traeré!");
			break;

		case 330:
			lineas.push_back("¿Pudiste encontrarlo? ¿Dónde está mi Myrphon?");
			opciones.push_back("[1] Aún no...");
			break;

		case 331:
			lineas.push_back("Por favor, date prisa... El laberinto Serpentback es muy oscuro");
			lineas.push_back("y debe tener mucho miedo. Estaré esperando aquí.");
			opciones.push_back("[1] Entendido...");
			break;

		case 350:
			lineas.push_back("(Sus ojos se abren de par en par al ver al animal) \"¡¡Myrphon!!\"");
			opciones.push_back("[1] Aquí está, sano y salvo.");
			break;

		case 351:
			lineas.push_back("(Abraza fuertemente a su mascota mientras llora de alegría)");
			lineas.push_back("¡Muchas gracias! Pensé que no volvería a verlo...");
			lineas.push_back("...Lo prometido es deuda. Toma esto, es la Varita Mágica");
			lineas.push_back("de la Alianza. A mí no me sirve para mi tipo de magia,");
			lineas.push_back("pero a ti te será muy útil... Gracias de nuevo, aventurero.");
			lineas.push_back("Ahora, si me disculpas, pasaré tiempo con mi amigo.");
			opciones.push_back("[1] ¡Muchas gracias, Richeh!");
			break;

		case 360:
			lineas.push_back("( •u• ): ¡Hola! Myrphon y yo estamos muy felices");
			lineas.push_back("gracias a ti. ¡¡Ten cuidado en tus viajes!!");
			opciones.push_back("[1] ¡Nos vemos, Richeh!");
			break;

		case 400:
			lineas.push_back("[DATO CURIOSO DE RICHEH #1]");
			lineas.push_back("Richeh es hermana mayor de Eini, uno de los mejores estudiantes");
			lineas.push_back("de la estricta academia de magos (la Alianza)...");
			opciones.push_back("[1] Continuar...");
			break;

		case 401:
			lineas.push_back("...Mientras él es perfeccionista, sigue todas las reglas");
			lineas.push_back("y se preocupa por el estatus, Richeh lo ignora activamente");
			lineas.push_back("y considera que la academia es una cárcel que destruye la creatividad.");
			opciones.push_back("[1] Cerrar");
			break;

		case 402:
			lineas.push_back("[DATO CURIOSO DE RICHEH #2]");
			lineas.push_back("Cuando Richeh se enfada o no quiere hacer algo, se pone");
			lineas.push_back("rígida como un mueble y sus compañeras tienen que");
			lineas.push_back("cargarla en peso para moverla.");
			opciones.push_back("[1] Cerrar");
			break;

		case 404:
			lineas.push_back("[DATO CURIOSO DE RICHEH #3]");
			lineas.push_back("La autora, Kamome Shirahama, diseñó las túnicas y el cabello");
			lineas.push_back("de Richeh con líneas muy rectas, pesadas y rígidas...");
			opciones.push_back("[1] Continuar...");
			break;

		case 405:
			lineas.push_back("...para reflejar visualmente lo 'cuadrada' y terca");
			lineas.push_back("que es su personalidad.");
			opciones.push_back("[1] Cerrar");
			break;

		case 410:
			lineas.push_back("[DIARIO DEL ATELIER: MYRPHON PERDIDO]");
			lineas.push_back("Richeh, compañera de cuarto de Agott, no le gustan las visitas");
			lineas.push_back("desde que se perdió Myrphon, un pequeño pingüino con rasgos de grifo.");
			lineas.push_back("Myrphon se perdió tras el ataque inesperado de unos magos oscuros");
			lineas.push_back("en el laberinto subterráneo Serpentback durante el segundo examen...");
			opciones.push_back("[1] Continuar...");
			break;

		case 411:
			lineas.push_back("...El examen evaluaba si un aprendiz usa la magia en secreto.");
			lineas.push_back("Se canceló por el caos, el pobre Myrphon quedó atrapado.");
			lineas.push_back("Richeh y Agott tienen la esperanza de que algún aventurero");
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
					inv->agregarItem(new ItemMagico(0, 0, "Capa Mágica", "Capa mágica que otorga la habilidad de planear por los cielos.", "Equipamiento Mágico", true));
				}
				if (qifrey != nullptr) {
					qifrey->setCrafteoCapa(true);
					qifrey->setConfianza(2);
				}
				puntosMisiones += 100;
				objetivoActual = "¡Capa Mágica crafteada! ¡Misión cumplida!";
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
			} else if (opcion == 2) {
				estadoDialogo = 35;
			} else {
				enDialogo = false;
				estadoDialogo = 0;
			}
		} else if (estadoDialogo == 35) {
			if (opcion == 1) {
				estadoDialogo = 32;
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
					} else if (inv->agregarItem(new ItemMagico(0, 0, "Tinta magica", "Tinta de plata otorgada por Qifrey para trazar sellos.", "Consumible Mágico", true))) {
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
			if (opcion == 2 && myrphonRescatado) {
				estadoDialogo = 231;
			} else if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
				estadoDialogo = 211;
			} else {
				estadoDialogo = 212;
			}
		} else if (estadoDialogo == 211 || estadoDialogo == 212) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo == 220) {
			if (opcion == 2 && myrphonRescatado) {
				estadoDialogo = 231;
			} else if (libroEncontrado || (inv != nullptr && inv->tieneItem("Libro de hechizos"))) {
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
		} else if (estadoDialogo == 223 || estadoDialogo == 230) {
			enDialogo = false;
			estadoDialogo = 0;
		} else if (estadoDialogo >= 231 && estadoDialogo <= 234) {
			estadoDialogo++;
		} else if (estadoDialogo == 235) {
			agottSabeMyrphon = true;
			if (agott != nullptr) {
				agott->setConfianza(agott->getConfianza() + 1);
			}
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
				inv->agregarItem(new ItemMagico(0, 0, "Vara magica", "Varita Mágica de la Alianza entregada por Richeh tras rescatar a Myrphon. Permite lanzar fuego para derribar muros.", "Herramienta Mágica", true));
				puntosMisiones += 50;
				promptFlotante = "[Recibiste: Vara mágica]";
			}
			if (richeh != nullptr) {
				richeh->setYaHablo(true);
				richeh->setConfianza(2);
				richeh->setExpresion(2);
			}
			objetivoActual = "Usar la Vara mágica para derribar el muro del almacén";
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
