#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

list<Action> AvanzaASaltosDeCaballo(){
	list<Action> secuencia;
	secuencia.push_back(actFORWARD);
	secuencia.push_back(actFORWARD);
	secuencia.push_back(actTURN_R);
	secuencia.push_back(actFORWARD);
	return secuencia;
}

/** Devuelve la ubicación siguiente según el avance del agente */
ubicacion NextCasilla(const ubicacion &pos) {
	ubicacion next = pos;
	switch (pos.brujula) {
		case norte:
			next.f = pos.f - 1;
			break;
		case noreste:
			next.f = pos.f - 1;
			next.c = pos.c +  1;
			break;
		case este:
			next.c = pos.c + 1;
			break;
		case sureste:
			next.f = pos.f + 1;
			next.c = pos.c + 1;
			break;
		case sur:
			next.f = pos.f + 1;
			break;
		case suroeste:
			next.f = pos.f + 1;
			next.c = pos.c - 1;
			break;
		case oeste:
			next.c = pos.c -1;
			break;
		case noroeste:
			next.f = pos.f - 1;
			next.c = pos.c - 1;
			break;
	}

	return next;
}

/** Devuelve si una ubicación en el mapa es transitable para el agente */
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa) {
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
}

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, se devuelve como saluda el mismo estado de entrada.
*/
stateNO apply(const Action &a, const stateNO &st, const vector<vector<unsigned char>> &mapa) {
	stateNO st_result = st;
	ubicacion sig_ubicacion;
	switch(a)    {
		case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonánbulo
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion == st.sonanmbulo)) {
				st_result.jugador = sig_ubicacion;
			}
			break;
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
			break;
		case actTURN_R:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
			break;
	}

	return st_result;
}

/** Encuentra si el elemento item está en lista */
bool Find(const stateNO &item, const list<nodeNO> &lista) {
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}

// Soluciones del tutorial para el nivel 0, van desde la iniciales (comentadas) hasta la final (V3)
/** Primera aproximación a la implementación de la búsqueda en anchura */
/*bool AnchuraSoloJugador(const stateNO &inicio, const ubicacion &final,
						const vector<vector<unsigned char>> &mapa) {
	stateNO current_state = inicio;
	list<stateNO> frontier;
	list<stateNO> explored;
	frontier.push_back(current_state);
	bool SolutionFound = (current_state.jugador.f == final.f and
						  current_state.jugador.c == final.c);

	while (!frontier.empty() and !SolutionFound) {
		frontier.pop_front();
		explored.push_back(current_state);

		// Generar hijo actFORWARD
		stateNO child_forward = apply(actFORWARD, current_state, mapa);
		if (child_forward.jugador.f == final.f and child_forward.jugador.c == final.c) {
			current_state = child_forward;
			SolutionFound = true;
		}
		else if (!Find(child_forward, frontier) and !Find(child_forward, explored)) {
			frontier.push_back(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			stateNO child_turnl = apply(actTURN_L, current_state, mapa);
			if (!Find(child_turnl, frontier) and !Find(child_turnl, explored)) {
				frontier.push_back(child_turnl);
			}

			stateNO child_turnr = apply(actTURN_R, current_state, mapa);
			if (!Find(child_turnr, frontier) and !Find(child_turnr, explored)) {
				frontier.push_back(child_turnr);
			}
		}

	if (!SolutionFound and !frontier.empty())
		current_state = frontier.front();
	}

	return SolutionFound;
}*/

/** Versión segunda de la búsqueda en anchura: ahora sí devuelve un plan */
/*list<Action> AnchuraSoloJugador_V2(const stateNO &inicio, const ubicacion &final,
								   const vector<vector<unsigned char>> &mapa) {
	nodeNO current_node;
	list<nodeNO> frontier;
	list<nodeNO> explored;
	list<Action> plan;
	current_node.st = inicio;

	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound) {
		frontier.pop_front();
		explored.push_back(current_node);

		// Generar hijo actFORWARD
		nodeNO child_forward = current_node;
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		if (child_forward.st.jugador.f == final.f and child_forward.st.jugador.c == final.c) {
			child_forward.secuencia.push_back(actFORWARD);
			current_node = child_forward;
			SolutionFound = true;
		}
		else if (!Find(child_forward.st, frontier) and !Find(child_forward.st, explored)) {
			child_forward.secuencia.push_back(actFORWARD);
			frontier.push_back(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeNO child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (!Find(child_turnl.st, frontier) and !Find(child_turnl.st, explored)) {
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeNO child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (!Find(child_turnr.st, frontier) and !Find(child_turnr.st, explored)) {
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
		}

		if (!SolutionFound and !frontier.empty()) {
			current_node = frontier.front();
		}
	}

	if (SolutionFound) {
		plan = current_node.secuencia;
	}

	return plan;
}*/

/** Versión tercera de la búsqueda en anchura: ahora sí devuelve un plan */
list <Action> AnchuraSoloJugador_V3(const stateNO &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa) {
	nodeNO current_node;
	list <nodeNO> frontier;
	set <nodeNO> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound) {
		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo actFORWARD
		nodeNO child_forward = current_node;
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		if (child_forward.st.jugador.f == final.f and child_forward.st.jugador.c == final.c) {
			child_forward.secuencia.push_back(actFORWARD);
			current_node = child_forward;
			SolutionFound = true;
		}
		else if (explored.find(child_forward) == explored.end()) {
			child_forward.secuencia.push_back(actFORWARD);
			frontier.push_back(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTurn_L
			nodeNO child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl) == explored.end()) {
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}

			// Generar hijo actTurn_R
			nodeNO child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr) == explored.end()) {
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
		}

		if (!SolutionFound and !frontier.empty()) {
			current_node = frontier.front();
			while(!frontier.empty() and explored.find(current_node) != explored.end()) {
				frontier.pop_front();
				current_node = frontier.front();
			}
		}
	}

	if (SolutionFound) {
		plan = current_node.secuencia;
	}

	return plan;
}

/** pone a cero todos los elementos de una matriz */
void AnularMatriz(vector<vector<unsigned char>> &matriz) {
	for (int i = 0; i <matriz.size(); i++) {
		for (int j = 0; j < matriz[0].size(); j++) {
			matriz[i][j] = 0;
		}
	}
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actIDLE;

	if (sensores.nivel != 4) {
		if (!hayPlan) {
			cout << "Calculando un nuevo plan" << endl;
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.sonanmbulo.f = sensores.SONposF;
			c_state.sonanmbulo.c = sensores.SONposC;
			c_state.sonanmbulo.brujula = sensores.SONsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			switch (sensores.nivel) {
				case 0:
					plan = AnchuraSoloJugador_V3(c_state, goal, mapaResultado);
					break;
				case 1: // Incluir aquí la llamada al algoritmo de búsqueda para el nivel 1
					cout << "Pendiente de implementar el nivel 1\n";
					break;
				case 2: // Incluir aquí la llamada al algoritmo de búsqueda para el nivel 2
					cout << "Pendiente de implementar el nivel 2\n";
					break;
				case 3: // Incluir aquí la llamada al algoritmo de búsqueda para el nivel 3
					cout << "Pendiente de implementar el nivel 3\n";
					break;
			}

			if (plan.size() > 0) {
				VisualizaPlan(c_state, plan);
				hayPlan = true;
			}
		}

		if (hayPlan and plan.size() > 0) {
			cout << "Ejecutando la siguiente accion del plan" << endl;
			accion = plan.front();
			plan.pop_front();
		}

		if (plan.size() == 0) {
			cout << "Se completó el plan" << endl;
			hayPlan = false;
		}
	}
	else {
		// Incluir aquí la llamada al algoritmo de búsqueda para el nivel 4
	}
	

	return accion;
}

/** Permite pintar sobre el mapa del simuladorel plan partiendo desde ek estado st */
void ComportamientoJugador::VisualizaPlan(const stateNO &st, const list <Action> &plan) {
	AnularMatriz(mapaConPlan);
	stateNO cst = st;

	auto it = plan.begin();
	while(it != plan.end()) {
		switch (*it) {
		case actFORWARD:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_R:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 2) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case actSON_FORWARD:
			cst.sonanmbulo = NextCasilla(cst.sonanmbulo);
			mapaConPlan[cst.sonanmbulo.f][cst.sonanmbulo.c] = 2;
			break;
		case actSON_TURN_SR:
			cst.sonanmbulo.brujula = (Orientacion)((cst.sonanmbulo.brujula + 1) % 8);
			break;
		case actSON_TURN_SL:
			cst.sonanmbulo.brujula = (Orientacion)((cst.sonanmbulo.brujula + 7) % 8);
			break;
		}
		it++;
	}
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}