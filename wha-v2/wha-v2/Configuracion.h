#pragma once

namespace Configuracion {
	enum EstadoApp {
		MENU_PRINCIPAL = 0,
		EN_JUEGO = 1
	};

	static const int TILE_SIZE = 32;
	static const int ZOOM = 1;
	static const float ESCALA_PERSONAJE = 2.0f;
	static const int VELOCIDAD_JUGADOR = 3;
	static const int ANCHO_JUEGO = 960;
	static const int ALTO_JUEGO = 600;
	static const int FILAS_MAPA_NIVEL1 = 80;
	static const int COLUMNAS_MAPA_NIVEL1 = 150;
	static const int FILAS_MAPA_NIVEL3 = 44;
	static const int COLUMNAS_MAPA_NIVEL3 = 104;
	static const int cantZeldaWidth = 10;
	static const int cantZeldaHeight = 8;
	static const int cantZeldaWalking = 10;
	static const int cantZeldaQuiet = 3;
	static const int FPSZeldaSprtChange = 2;
}
