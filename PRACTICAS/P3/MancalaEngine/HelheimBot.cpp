/*
 * HelheimBot.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "HelheimBot.h"

#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <limits>
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

bool HelheimBot::esObjetivo(const GameState &state)
{
	// J1 = 0
	// J2 = 1

	if(state.isValidState())
	{
		bool objetivo = false,
			 soy0 = false;

		int puntos_0 = state.getScore((Player) 0),
			puntos_1 = state.getScore((Player) 1),
			semillas[2] = {0, 0};

		Player 	yo = state.getCurrentPlayer(),
				tu = (Player) ( ( ( (int) yo ) + 1 ) % 2);

		if((int) yo == 0)
			soy0 = true;

		if(state.isFinalState() and state.getScore(yo) > state.getScore(tu))
			objetivo = true;
		else
		{

			for (int i = 0; i <= 1; ++i)
			{
				for (int j = 1; j <= 6; ++j)
				{
					int aux = state.getSeedsAt( (Player) i, (Position) j );

					if(aux > 0 )
						semillas[i] += aux;
				}
			}

			if(semillas[0] > 0 and semillas[1] == 0)
			{
				puntos_0 += semillas[0];
			}
			else if(semillas[0] == 0 and semillas[1] > 0)
			{
				puntos_1 += semillas[1];
			}

			if( (soy0 and puntos_0 > puntos_1) or (!soy0 and puntos_0 < puntos_1) )
			{
				objetivo = true;
			}
		}
		/*if(objetivo)
			for (int p= 0; p<2; p++)
			{
				for (int i= 1; i<=6; i++)
				{
					cerr << (int) state.getSeedsAt((Player)p, (Position) i) << " ::::";
				}
				cerr << endl;
			}*/
		return objetivo;
	}
	else
		return false;
}

int HelheimBot::calcularUtilidad(const GameState &anterior, const GameState &actual)
{
	/*
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/

	Player yo = anterior.getCurrentPlayer();
	Player next = actual.getCurrentPlayer();

	int semillas_antes = anterior.getSeedsAt(yo, GRANERO),
		semillas_despues = actual.getSeedsAt(yo, GRANERO),
		diferencia_semillas = semillas_despues - semillas_antes;

	int utilidad;

	if(diferencia_semillas == 0)
		utilidad = 1;
	else
		if(diferencia_semillas == 1)
		{
			if(yo == next)
				utilidad = 3;
			else
				utilidad = 2;
		}
		else
			if(diferencia_semillas > 1)
				utilidad = 4;

	return utilidad;
}

int HelheimBot::valorMIN(const GameState &anterior, const GameState &actual, int & profundidad)
{
	/*
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/
	for (int p= 0; p<2; p++)
	{
		for (int i= 1; i<=6; i++)
		{
			cerr << (int) actual.getSeedsAt((Player)p, (Position) i) << " ::::";
		}
		cerr << endl;
	}

	cerr << "1" << endl;

	++profundidad;

	int menor_valor = 5;
	GameState simulacion;
	bool es_objetivo = esObjetivo(actual);

	if(es_objetivo or profundidad == profundidad_max)
	{
		cerr << "2" << endl;
		if(es_objetivo)
			return 5;
		else
		{
			cerr << "TOPE en min" << endl;
			return calcularUtilidad(anterior, actual);
		}
	}
	else
	{
		cerr << "3" << endl;
		for (int i = 1; i < 7; ++i)
		{
			cerr << "4" << endl;
			int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

			simulacion = actual.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'
			cerr << "5" << endl;
			if(simulacion.isValidState())	// Si el nuevo estado del tablero es válido seguimos con el proceso.
			{								// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.
				cerr << "6" << endl;
				for (int p= 0; p<2; p++)
				{
					for (int i= 1; i<=6; i++)
					{
						cerr << (int) simulacion.getSeedsAt((Player)p, (Position) i) << " ::::";
					}
					cerr << endl;
				}
				utilidad_calculada = valorMAX(actual, simulacion, profundidad);	// Calculamos el beneficio que nos aporta ese movimiento.
				cerr << "7" << endl;
				if(utilidad_calculada < menor_valor)	// Si el beneficio calculado es menor que cualquier otro calculado hasta ahora
				{										// actuaizamos el nuevo movimiento

					menor_valor = utilidad_calculada;
				}
			}
		}
	}
	return menor_valor;
}

int HelheimBot::valorMAX(const GameState &anterior, const GameState &actual, int & profundidad)
{
	/*
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/
	cerr << "8" << endl;

	++profundidad;
	
	int mayor_valor = 1;
	GameState simulacion;
	bool es_objetivo = esObjetivo(actual);

	if(es_objetivo or profundidad == profundidad_max)
	{
		cerr << "9" << endl;
		if(es_objetivo)
			return 5;
		else
		{
			cerr << "TOPE en max" << endl;
			return calcularUtilidad(anterior, actual);
		}
	}
	else
	{
		cerr << "10" << endl;
		for (int i = 1; i < 7; ++i)
		{
			cerr << "11" << endl;
			int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

			simulacion = actual.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'
			cerr << "12" << endl;
			if(simulacion.isValidState())	// Si el nuevo estado del tablero es válido seguimos con el proceso.
			{								// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.
				cerr << "13" << endl;
				utilidad_calculada = valorMIN(actual, simulacion, profundidad);	// Calculamos el beneficio que nos aporta ese movimiento.
				cerr << "14" << endl;
				if(utilidad_calculada > mayor_valor)	// Si el beneficio calculado es menor que cualquier otro calculado hasta ahora
				{										// actuaizamos el nuevo movimiento

					mayor_valor = utilidad_calculada;
				}
			}
		}
	}
	return mayor_valor;
}

Move HelheimBot::MINI_MAX(const GameState &state)
{
	Move mejor_movimiento = M_NONE;	// Mejor acción que podemos tomar (se va a calcular).
	int mejor_utilidad = 0;			// Dato referente al mejor movimiento posible. Se irá actualizando para encontrar el mejor.
	GameState simulacion;			// Estado del tablero simulado en el que cargaremos las distintas posibilidades de elección.
	int prof = 0;

	for (int i = 1; i < 7; ++i)
	{
		int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

		simulacion = state.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'

		if(simulacion.isValidState())	// Si el nuevo estado del tablero es válido seguimos con el proceso.
		{								// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.

			utilidad_calculada = valorMIN(state, simulacion, prof);	// Calculamos el beneficio que nos aporta ese movimiento.

			if(utilidad_calculada > mejor_utilidad)	// Si el beneficio calculado es mejor que cualquier otro calculado hasta ahora
			{										// actuaizamos el nuevo movimiento

				mejor_utilidad = utilidad_calculada;
				mejor_movimiento = (Move) i;
			}
		}
	}
	return mejor_movimiento;
}

Move HelheimBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	Player turno= this->getPlayer();
	long timeout= this->getTimeOut();

	Move movimiento= MINI_MAX(state);

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
