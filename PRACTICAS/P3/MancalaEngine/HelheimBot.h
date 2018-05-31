/*
 * HelheimBot.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class HelheimBot:Bot {
public:
	HelheimBot();
	~HelheimBot();


	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);

private:
	const int profundidad_max = 20;
	Player turno;
	Player oponente;

	int valorMIN(const GameState &state, const GameState &simulacion, int & profundidad, int & alfa, int & beta);
	int valorMAX(const GameState &state, const GameState &simulacion, int & profundidad, int & alfa, int & beta);
	Move MINI_MAX(const GameState &state);
	bool ganoPartida(const GameState &state, bool soyYo);
	bool pierdoPartida(const GameState &state, bool soyYo);
	int calcularUtilidad(const GameState &state, const GameState &simulacion, bool soyYo);
};

#endif /* MANUPCBOT_H_ */
