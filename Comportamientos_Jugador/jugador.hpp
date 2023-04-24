#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct stateNO {
	ubicacion jugador;
	ubicacion sonanmbulo;

	bool operator== (const stateNO &x) const {
		if (jugador == x.jugador and sonanmbulo.f == x.sonanmbulo.f 
			and sonanmbulo.c == x.sonanmbulo.c) {
				return true;
		}
		else {
			return false;
		}
	}  
};

struct nodeNO {
	stateNO st;
	list <Action> secuencia;

	bool operator==(const nodeNO &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeNO &n) const {
		if (st.jugador.f < n.st.jugador.f) {
			return true;
		}
		else if (st.jugador.f == n.st.jugador.f and st.jugador.c < n.st.jugador.c) {
			return true;
		}
		else if (st.jugador.f == n.st.jugador.f and st.jugador.c == n.st.jugador.c and st.jugador.brujula < n.st.jugador.brujula) {
			return true;
		} 
		else {
			return false;
		}
	}
};


class ComportamientoJugador : public Comportamiento {
	public:
		ComportamientoJugador(unsigned int size) : Comportamiento(size) {
		// Inicializar Variables de Estado
		}
		ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
		// Inicializar Variables de Estado
		hayPlan = false;
		}
		ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
		~ComportamientoJugador(){}

		Action think(Sensores sensores);
		int interact(Action accion, int valor);
		void VisualizaPlan(const stateNO &st, const list <Action> &plan);


	private:
		// Declarar Variables de Estado
		list<Action> plan;  //Almacena el plan de la ejecución.
		bool hayPlan;       //si verdad indica que se está siguiendo un plan.
		stateNO c_state;
		ubicacion goal;
};

#endif