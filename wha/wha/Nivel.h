#ifndef Nivel_h
#define Nivel_h
#include <iostream>
#include "Mapa.h"

class Nivel {
private:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	Mapa* mapa;

	void generarArbolesAleatorios(int cantidad, int minX, int maxX, int minY, int maxY) {
		// Tipos de árboles disponibles en tu enum
		TipoObjeto tiposDisponibles[] = { arbolPino, arbolFrondoso, arbolGigante };

		for (int i = 0; i < cantidad; i++) {
			// Generar coordenadas X e Y aleatorias dentro del rango del mapa
			int posX = minX + rand() % (maxX - minX + 1);
			int posY = minY + rand() % (maxY - minY + 1);

			// Seleccionar un tipo de árbol al azar (0, 1 o 2)
			TipoObjeto tipoRandom = tiposDisponibles[rand() % 3];

			// Instanciar y agregar al mapa
			mapa->agregarObjeto(new Arbol(posX, posY, tipoRandom));
		}
	}
public:
	Nivel() : numeroNivel(1), nombreNivel("Inicio"), completado(false), mapa(nullptr) {}
	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false) {
		//instancia para el mapeta 1
		this->mapa = new Mapa(filasMapa, columnasMapa);
	}
	~Nivel() {
		if (this->mapa != nullptr) {
			delete this->mapa;
			this->mapa = nullptr;
		}
	};
	//get (obtener)
	int getNumeroNivel() { return this->numeroNivel; }
	std::string getNombreNivel() { return this->nombreNivel; }
	bool getCompletado() { return this->completado; }
	Mapa* getMapa() { return this->mapa; }  //retorna el puntero del mapa
	//set (asignar)
	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) { this->completado = estado; }
	void setMapa(Mapa* nuevoMapa) {
		if (this->mapa != nullptr) delete this->mapa;
		this->mapa = nuevoMapa;
	}
	//acciones
	void inciarNivel() {
		if (this->mapa == nullptr) return;

		// Limpiamos objetos antiguos si estamos reiniciando o recargando el mapa
		// (Asegúrate de que mapa->limpiarObjetos() esté implementado si fuera necesario)

		// Cargar contenido específico dependiendo de qué nivel es
		switch (this->numeroNivel) {
		case 1: // NIVEL 1: Coco
			mapa->agregarObjeto(new Habitacion(36, 11, habitacionInicial, "Atelier_Habitacion de Coco"));
			mapa->agregarObjeto(new Habitacion(39, 152, tallerQifrey, "Atelier_Habitacion de Qifrey"));
			mapa->agregarObjeto(new Habitacion(461, 63, habitacionRichie, "Salon de Estudio"));
			mapa->agregarObjeto(new Habitacion(459, 144, habitacionGusanoPincel, "Atelier_Habitacion de Gusano Pincel"));
			mapa->agregarObjeto(new Habitacion(459, 12, almacenAbandonado, "Almacen Abandonado"));
			mapa->agregarObjeto(new Gato(465, 190, "Junior"));
			//primera parte del rio
			mapa->agregarObjeto(new Rio(236, 1, 40, 47));
			mapa->agregarObjeto(new Rio(245, 43, 44, 17));
			mapa->agregarObjeto(new Rio(257, 57, 40, 32));
			mapa->agregarObjeto(new Rio(264, 87, 41, 8));
			//segunda parte del rio
			mapa->agregarObjeto(new Rio(272, 116, 46, 24));
			mapa->agregarObjeto(new Rio(258, 129, 38, 26));
			mapa->agregarObjeto(new Rio(243, 142, 45, 44));
			mapa->agregarObjeto(new Rio(260, 179, 44, 15));
			mapa->agregarObjeto(new Rio(277, 187, 56, 14));
			//camino
			mapa->agregarObjeto(new Camino(77, 47, 34, 105));
			mapa->agregarObjeto(new Camino(110, 96, 262, 21));
			mapa->agregarObjeto(new Camino(371, 27, 38, 147));
			mapa->agregarObjeto(new Camino(398, 27, 62, 11));
			mapa->agregarObjeto(new Camino(398, 99, 64, 11));
			mapa->agregarObjeto(new Camino(398, 165, 62, 11));
			generarArbolesAleatorios(50, 10, 580, 5, 55);
			break;

		case 2: 
			// NIVEL 2
			break;

		case 3: 
			// NIVEL 3
			break;

		default:
			break;
		}
	};
	bool verificarObjetivo() { return false; };
	void mostrarPrologo() {};
	void mostrarMuerteArbolPlata() {};
	void actualizar() {};
};

#endif // !Nivel_h

