/*
 * HelheimBot.cpp
 *
 *  Created on: 31 may. 2018
 *      Author: Jota
 */

#include "HelheimBot.h"

#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

HelheimBot::HelheimBot() {
	// Inicializar las variables necesarias para ejecutar la partida
}

HelheimBot::~HelheimBot() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void HelheimBot::initialize() {
	
}

string HelheimBot::getName() {
	return "HelheimBot"; // Sustituir por el nombre del bot
}

bool HelheimBot::puedoRobar(const GameState &state, Player p)
{
	bool quizas = false;
	bool seguro = false;
	Player p2 = p == turno? oponente : turno;
	vector<int> a_robar;

	for (int i = 1; i < 7; ++i)
	{
		if (state.getSeedsAt(p, (Position) i) == 0 and state.getSeedsAt(p2, (Position) (7-i) ) > 0 )
		{
			quizas = true;
			a_robar.push_back(i);
		}
	}

	for (int i = 0; i < a_robar.size() and !seguro and quizas; ++i)
	{
		for (int j = 1; j < 7 and !seguro; ++j)
		{
			if( ( j < a_robar[i] and state.getSeedsAt(p, (Position) j) == (j + 6 + 7 - a_robar[i]) ) or ( j > a_robar[i] and state.getSeedsAt(p, (Position) j) == (j - a_robar[i]) ) )
			{
				seguro = true;
			}
		}
	}

	return seguro;
}

int HelheimBot::calcularUtilidad(const GameState &anterior, const GameState &actual)
{
	int diferencia_puntuacion = actual.getScore(turno) - actual.getScore(oponente);
	diferencia_puntuacion *= 3;

	bool es_mi_turno = actual.getCurrentPlayer() == turno;

	if(puedoRobar(actual, turno))
		diferencia_puntuacion += 1;
	else if(puedoRobar(actual, oponente))
		diferencia_puntuacion -= 1;

	if(anterior.getCurrentPlayer() == actual.getCurrentPlayer())
	{
		if(es_mi_turno)
			diferencia_puntuacion+=1;
		else
			diferencia_puntuacion-=1;
	}

	return diferencia_puntuacion;
}

int HelheimBot::valorMIN_MAX(const GameState &anterior, const GameState &actual, int profundidad, int alfa, int beta)
{
	int valor;
	bool es_mi_turno = turno == actual.getCurrentPlayer();

	if(es_mi_turno)
	{
		valor = -144;
	}
	else
	{
		valor = 144;
	}

	GameState simulacion;

	if(profundidad == profundidad_max or actual.isFinalState())
	{
		return calcularUtilidad(anterior, actual);
	}
	else
	{
		bool salir = false;

		for (int i = 1; i < 7 and !salir; ++i)
		{
			int utilidad_calculada = 0;

			if(actual.getSeedsAt(actual.getCurrentPlayer(), (Position) i) > 0 )
			{

				simulacion = actual.simulateMove( (Move) i );
				
				if(simulacion.isValidState())
				{

					utilidad_calculada = valorMIN_MAX(actual, simulacion, profundidad+1, alfa, beta);

					if(es_mi_turno)
					{
						if(utilidad_calculada > valor)
						{

							valor = utilidad_calculada;
						}

						if(valor >= beta)
						{
							salir = true;
						} 
						else if(valor > alfa)
						{
							alfa = valor;
						}
					}
					else
					{
						if(utilidad_calculada < valor)
						{

							valor = utilidad_calculada;
						}

						if(valor <= alfa)
						{
							salir = true;
						} 
						else if(valor < beta)
						{
							beta = valor;
						}
					}
				}
			}
		}
	}
	return valor;
}

Move HelheimBot::MINI_MAX(const GameState &state)
{
	Move mejor_movimiento = M_NONE;	// Mejor acción que podemos tomar (se va a calcular).
	int mejor_utilidad = -144;			// Dato referente al mejor movimiento posible. Se irá actualizando para encontrar el mejor.
	GameState simulacion;			// Estado del tablero simulado en el que cargaremos las distintas posibilidades de elección.
	int alfa = -144;
	int beta = 144;

	for (int i = 1; i < 7; ++i)
	{
		int utilidad_calculada = -144;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

		if(state.getSeedsAt(state.getCurrentPlayer(), (Position) i) > 0)
		{
			simulacion = state.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'

			if(simulacion.isValidState())
			{	// Si el nuevo estado del tablero es válido y el movimiento es permitido seguimos con el proceso.
				// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.

				utilidad_calculada = valorMIN_MAX(state, simulacion, 1, alfa, beta);	// Calculamos el beneficio que nos aporta ese movimiento.

				if(utilidad_calculada > mejor_utilidad)	// Si el beneficio calculado es mejor que cualquier otro calculado hasta ahora
				{										// actuaizamos el nuevo movimiento

					mejor_utilidad = utilidad_calculada;
					mejor_movimiento = (Move) i;
				}
			}
		}
	}
	return mejor_movimiento;
}

Move HelheimBot::nextMove(const vector<Move> &adversary, const GameState &state) {

	turno= this->getPlayer();
	oponente = turno == J1? J2 : J1;

	Move movimiento= MINI_MAX(state);

	return movimiento;
}
