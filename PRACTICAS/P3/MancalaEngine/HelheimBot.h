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
	const int profundidad_max = 5;

	int valorMIN(const GameState &state, const GameState &simulacion, int & profundidad);
	int valorMAX(const GameState &state, const GameState &simulacion, int & profundidad);
	Move MINI_MAX(const GameState &state);
	bool esObjetivo(const GameState &state);
	int calcularUtilidad(const GameState &state, const GameState &simulacion);
};

#endif /* MANUPCBOT_H_ */
