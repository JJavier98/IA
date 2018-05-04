/*
 * HelheimBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "HelheimBot.h"

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

HelheimBot::HelheimBot() {
	// Inicializar las variables necesarias para ejecutar la partida

}

HelheimBot::~HelheimBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void HelheimBot::initialize() {
	// Inicializar el bot antes de jugar una partida
}

string HelheimBot::getName() {
	return "HelheimBot"; // Sustituir por el nombre del bot
}

Move HelheimBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Move movimiento= M_NONE;

	// Implementar aquí la selección de la acción a realizar

	// OJO: Recordatorio. NO USAR cin NI cout.
	// Para salidas por consola (debug) utilizar cerr. Ejemplo:
	// cerr<< "Lo que quiero mostrar"<<endl;


	// OJO: Recordatorio. El nombre del bot y de la clase deben coincidir.
	// En caso contrario, el bot no podrá participar en la competición.
	// Se deberá sustituir el nombre HelheimBot como nombre de la clase por otro
	// seleccionado por el alumno. Se deberá actualizar también el nombre
	// devuelto por el método getName() acordemente.

	return movimiento;
}
