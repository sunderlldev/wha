#pragma once

#include <string>
#include <vector>

struct PaginaDialogo {
	std::string texto;
	std::vector<std::string> opciones;
};

class GestorDialogos {
private:
	bool activo;
	std::string hablante;
	std::string rol;
	std::vector<PaginaDialogo> paginas;
	int paginaActual;
	int opcionSeleccionada;

public:
	GestorDialogos() {
		activo = false;
		hablante = "";
		rol = "";
		paginaActual = 0;
		opcionSeleccionada = 0;
	}

	bool estaActivo() { return activo; }
	std::string getHablante() { return hablante; }
	std::string getRol() { return rol; }
	int getPaginaActual() { return paginaActual; }
	int getTotalPaginas() { return (int)paginas.size(); }
	int getOpcionSeleccionada() { return opcionSeleccionada; }

	std::string getTextoActual() {
		if (!activo || paginaActual < 0 || paginaActual >= (int)paginas.size()) return "";
		return paginas[paginaActual].texto;
	}

	std::vector<std::string> getOpcionesActuales() {
		if (!activo || paginaActual < 0 || paginaActual >= (int)paginas.size()) return std::vector<std::string>();
		return paginas[paginaActual].opciones;
	}

	bool paginaTieneOpciones() {
		return getOpcionesActuales().size() > 0;
	}

	bool esUltimaPagina() {
		return paginaActual >= (int)paginas.size() - 1;
	}

	void iniciar(std::string nombreHablante, std::string rolHablante, std::vector<PaginaDialogo> nuevasPaginas) {
		hablante = nombreHablante;
		rol = rolHablante;
		paginas = nuevasPaginas;
		paginaActual = 0;
		opcionSeleccionada = 0;
		activo = !paginas.empty();
	}

	void iniciarSimple(std::string nombreHablante, std::string rolHablante, std::string texto) {
		std::vector<PaginaDialogo> nuevas;
		PaginaDialogo pagina;
		pagina.texto = texto;
		nuevas.push_back(pagina);
		iniciar(nombreHablante, rolHablante, nuevas);
	}

	void cerrar() {
		activo = false;
		paginas.clear();
		paginaActual = 0;
		opcionSeleccionada = 0;
	}

	void avanzar() {
		if (!activo) return;
		if (paginaTieneOpciones()) return;
		if (paginaActual + 1 < (int)paginas.size()) {
			paginaActual++;
			opcionSeleccionada = 0;
		}
		else {
			cerrar();
		}
	}

	void moverSeleccion(int delta) {
		if (!activo) return;
		int total = (int)getOpcionesActuales().size();
		if (total <= 0) return;
		opcionSeleccionada = (opcionSeleccionada + delta + total) % total;
	}

	void seleccionarOpcion(int indice) {
		int total = (int)getOpcionesActuales().size();
		if (indice >= 0 && indice < total) {
			opcionSeleccionada = indice;
		}
	}

	void mostrarRespuesta(std::string texto) {
		std::vector<PaginaDialogo> nuevas;
		PaginaDialogo pagina;
		pagina.texto = texto;
		nuevas.push_back(pagina);
		paginas = nuevas;
		paginaActual = 0;
		opcionSeleccionada = 0;
		activo = true;
	}
};
