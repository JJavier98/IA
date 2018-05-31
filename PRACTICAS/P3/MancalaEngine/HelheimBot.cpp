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
	
}

string HelheimBot::getName() {
	return "HelheimBot"; // Sustituir por el nombre del bot
}

bool HelheimBot::ganoPartida(const GameState &state, bool soyYo)
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

		Player 	yo, tu;

		if(soyYo)
		{
			yo = state.getCurrentPlayer();
			tu = (Player) ( ( ( (int) yo ) + 1 ) % 2);
		}
		else
		{
			tu = state.getCurrentPlayer();
			yo = (Player) ( ( ( (int) yo ) + 1 ) % 2);
		}

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

			if( ( (soy0 and puntos_0 > puntos_1) or (!soy0 and puntos_0 < puntos_1) ) and (semillas[0] == 0 or semillas[1] == 0) )
			{
				objetivo = true;
			}
		}
		return objetivo;
	}
	else
		return false;
}

bool HelheimBot::pierdoPartida(const GameState &state, bool soyYo)
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

		Player 	yo, tu;

		if(soyYo)
		{
			yo = state.getCurrentPlayer();
			tu = (Player) ( ( ( (int) yo ) + 1 ) % 2);
		}
		else
		{
			tu = state.getCurrentPlayer();
			yo = (Player) ( ( ( (int) yo ) + 1 ) % 2);
		}

		if((int) yo == 0)
			soy0 = true;

		if(state.isFinalState() and state.getScore(yo) < state.getScore(tu))
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

			if(  ( (soy0 and puntos_0 < puntos_1) or (!soy0 and puntos_0 > puntos_1) ) and (semillas[0] == 0 or semillas[1] == 0) )
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

int HelheimBot::calcularUtilidad(const GameState &anterior, const GameState &actual, bool soyYo)
{
	/*
	Pierdo la partida -> utilidad = 0
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/

/*
	Player p_anterior = anterior.getCurrentPlayer();
	Player p_actual = actual.getCurrentPlayer();
	Player contrario = (Player) ( ( ( (int) p_actual ) + 1 ) % 2);

	bool soy_anterior = false;
	if( (soyYo and p_actual == p_anterior) or (!soyYo and contrario == p_anterior))
		soy_anterior = true;


	int semillas_antes = anterior.getSeedsAt(p_anterior, GRANERO),
		semillas_despues = actual.getSeedsAt(p_anterior, GRANERO),
		diferencia_semillas = semillas_despues - semillas_antes;

	int utilidad;

	if(diferencia_semillas == 0)
	{
		if(soy_anterior)
			utilidad = 1;
		else
			utilidad = 12;
	}
	else
		if(diferencia_semillas == 1)
		{
			if(p_anterior == p_actual)
			{
				if(soy_anterior)
					utilidad = 11;
				else
					utilidad = 2;
			}
			else
			{
				int num_casillas_enemigas_agrandadas = 0;

				for (int i = 1; i <= 6; ++i)
				{
					int semill_antes = anterior.getSeedsAt(contrario, (Position) i),
						semill_despues = actual.getSeedsAt(contrario, (Position) i);

					if(semill_despues > semill_antes)
						num_casillas_enemigas_agrandadas++;
				}
				if(soy_anterior)
					utilidad = 8 - num_casillas_enemigas_agrandadas;
				else
					utilidad = num_casillas_enemigas_agrandadas + 5;
			}
		}
		else
			if(diferencia_semillas > 1)
			{
				if(soy_anterior)
					utilidad = 12;
				else
					utilidad = 1;
			}

	int casilla_movida = -1;
	int semill_antes, semill_despues;
	Player contrario_a_anterior = (Player) ( ( ( (int) p_anterior ) + 1 ) % 2);

	for (int i = 1; i <= 6 and casilla_movida == -1; ++i)
	{
		semill_antes = anterior.getSeedsAt(p_anterior, (Position) i),
		semill_despues = actual.getSeedsAt(p_anterior, (Position) i);

		if(semill_despues == 0 and semill_antes > 0)
			casilla_movida = i;
	}

	if(semill_antes >= 4 and anterior.getSeedsAt(contrario_a_anterior, (Position) casilla_movida) == 0)
	{
		if(soy_anterior)
			utilidad +=3;
		else
			utilidad -= 3;
	}

	return utilidad;
	*/
	cerr << endl << endl << turno << endl << endl;
	return actual.getScore(turno) - actual.getScore(oponente);
}

int HelheimBot::valorMIN(const GameState &anterior, const GameState &actual, int & profundidad, int & alfa, int & beta)
{
	/*
	Pierdo la partida -> utilidad = 0
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/
	cerr << "valorMIN" << endl;

	for (int p= 0; p<2; p++)
	{
		for (int i= 1; i<=6; i++)
		{
			cerr << " ::::" << (int) actual.getSeedsAt((Player)p, (Position) i);
		}
		cerr << endl;
	}

	++profundidad;

	int menor_valor = 48;
	GameState simulacion;
	//bool gano = ganoPartida(actual, false);
	//bool pierdo = pierdoPartida(actual, false);

	if(profundidad == profundidad_max or actual.isFinalState())
	{
		int calc = calcularUtilidad(anterior, actual, false);
		cerr << "TOPE en min: " << calc << endl;
		menor_valor = calc;
		if(menor_valor < beta)
		{
			beta = menor_valor;
		}
	}
	/*else if(gano)
	{
		cerr << "gano en min" << endl;
		menor_valor = 16;
		if(menor_valor < beta)
		{
			beta = menor_valor;
		}
	}
	else if (pierdo)
	{
		cerr << "pierdo en min" << endl;
		menor_valor = 0;
		if(menor_valor < beta)
		{
			beta = menor_valor;
		}
	}*/
	else
	{
		bool salir = false;

		for (int i = 1; i < 7 and !salir; ++i)
		{
			cerr << "MOVIMIENTO EN EXPANSION: " << i << " PROFUNDIDAD: " << profundidad << endl;
			int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

			if(actual.getSeedsAt(actual.getCurrentPlayer(), (Position) i) > 0 )
			{
				cerr << "Movimiento " << i << " valido en min" << endl;

				simulacion = actual.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'
				
				if(simulacion.isValidState())	// Si el nuevo estado del tablero es válido seguimos con el proceso.
				{								// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.

					if(actual.getCurrentPlayer() == simulacion.getCurrentPlayer())
					{
						utilidad_calculada = valorMIN(actual, simulacion, profundidad, alfa, beta);
						cerr << "regresion de min - min" << endl;
					}
					else
					{
						utilidad_calculada = valorMAX(actual, simulacion, profundidad, alfa, beta);	// Calculamos el beneficio que nos aporta ese movimiento.
						cerr << "regresion de max - min" << endl;
					}

					if(utilidad_calculada < menor_valor)	// Si el beneficio calculado es menor que cualquier otro calculado hasta ahora
					{										// actuaizamos el nuevo movimiento

						menor_valor = utilidad_calculada;
					}

					if(menor_valor <= alfa)
					{
						salir = true;
					}

					if(menor_valor < beta)
					{
						beta = menor_valor;
					}
				}
			}
		}
	}
	--profundidad;
	return menor_valor;
}

int HelheimBot::valorMAX(const GameState &anterior, const GameState &actual, int & profundidad, int & alfa, int & beta)
{
	/*
	Pierdo la partida -> utilidad = 0
	No consigo beneficio -> utilidad = 1
	Añado una semilla al granero -> utilidad = 2
	Añado la ultima semilla al granero (vuelvo a tirar) -> utilidad = 3
	Se produce un robo a mi favor -> utilidad = 4
	Gano la partida -> utilidad = 5
	*/
	cerr << "valorMAX" << endl;

	for (int p= 0; p<2; p++)
	{
		for (int i= 1; i<=6; i++)
		{
			cerr << " ::::" << (int) actual.getSeedsAt((Player)p, (Position) i);
		}
		cerr << endl;
	}

	++profundidad;
	
	int mayor_valor = -48;
	GameState simulacion;
	//bool gano = ganoPartida(actual, true);
	//bool pierdo = pierdoPartida(actual, true);

	if (profundidad == profundidad_max or actual.isFinalState())
	{
		int calc = calcularUtilidad(anterior, actual, true);
		cerr << "TOPE en max: " << calc << endl;
		mayor_valor = calc;
		if(mayor_valor > alfa)
			alfa = mayor_valor;
	}
	/*else if(gano)
	{
		cerr << "gano en max" << endl;
		mayor_valor = 16;
		if(mayor_valor > alfa)
			alfa = mayor_valor;
	}
	else if(pierdo)
	{
		cerr << "pierdo en max" << endl;
		mayor_valor = 0;
		if(mayor_valor > alfa)
			alfa = mayor_valor;
	}*/
	else
	{
		bool salir = false;

		for (int i = 1; i < 7 and !salir; ++i)
		{
			cerr << "MOVIMIENTO EN EXPANSION: " << i << " PROFUNDIDAD: " << profundidad << endl;
			int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

			if(actual.getSeedsAt(actual.getCurrentPlayer(), (Position) i) > 0 )
			{
				cerr << "Movimiento " << i << " valido en max" << endl;

				simulacion = actual.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'
				
				if(simulacion.isValidState())	// Si el nuevo estado del tablero es válido seguimos con el proceso.
				{								// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.

					if(actual.getCurrentPlayer() == simulacion.getCurrentPlayer())
					{
						utilidad_calculada = valorMAX(actual, simulacion, profundidad, alfa, beta);	// Calculamos el beneficio que nos aporta ese movimiento.
						cerr << "regresion de max - max" << endl;
					}
					else
					{
						utilidad_calculada = valorMIN(actual, simulacion, profundidad, alfa, beta);	// Calculamos el beneficio que nos aporta ese movimiento.
						cerr << "regresion de min - max" << endl;
					}

					if(utilidad_calculada > mayor_valor)	// Si el beneficio calculado es menor que cualquier otro calculado hasta ahora
					{										// actuaizamos el nuevo movimiento

						mayor_valor = utilidad_calculada;
					}

					if(mayor_valor >= beta)
					{
						salir = true;
					}

					if(mayor_valor > alfa)
						alfa = mayor_valor;
				}
			}
		}
	}
	--profundidad;
	return mayor_valor;
}

Move HelheimBot::MINI_MAX(const GameState &state)
{
	Move mejor_movimiento = M_NONE;	// Mejor acción que podemos tomar (se va a calcular).
	int mejor_utilidad = -48;			// Dato referente al mejor movimiento posible. Se irá actualizando para encontrar el mejor.
	GameState simulacion;			// Estado del tablero simulado en el que cargaremos las distintas posibilidades de elección.
	int profundidad = 0;
	int alfa = -48;
	int beta = 48;

	for (int i = 1; i < 7; ++i)
	{
		cerr << "MOVIMIENTO EN EXPANSION MINIMAX: " << i << endl;
		int utilidad_calculada = 0;	// Valor local de la utilidad referente al movimiento que se estudia en el momento.

		simulacion = state.simulateMove( (Move) i );  // Cargamos en 'simulacion' el tablero resultante de aplicar la accion 'i'

		if(simulacion.isValidState() and state.getSeedsAt(state.getCurrentPlayer(), (Position) i) > 0)
		{	// Si el nuevo estado del tablero es válido y el movimiento es permitido seguimos con el proceso.
			// Si ningún movimiento es factible devolvemos 'M_NONE' y seremos descalificados.

			utilidad_calculada = valorMIN(state, simulacion, profundidad, alfa, beta);	// Calculamos el beneficio que nos aporta ese movimiento.

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

	turno= this->getPlayer();
	oponente = (Player) ( ( ( (int) turno ) + 1 ) % 2);

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
