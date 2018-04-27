#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <utility>
#include <stack>

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	plan.clear();
	bool encontrado = false;
	int dx[4] = {-1, 0, 1, 0};
	int dy[4] = {0, 1, 0, -1};


	vector <vector<bool> > visitado;		// casillas visitadas
	vector<vector<estado> > previo;			// casilla anterior por la que se ha pasado
	visitado.resize(100);
	previo.resize(100);


	estado a;
	a.fila = -3;
	a.columna = -3;
	a.orientacion = -3;
	for(int i = 0 ; i < 100 ; ++i)
	{
		visitado[i].resize(100);
		previo[i].resize(100);
		for (int j = 0; j < 100; ++j)
		{
			visitado[i][j] = false;
			previo[i][j] = a;

		}
	}
	
	queue<estado> Q;						// Todos los posibles estados por los que paso hasta destino
	estado inicial = origen;				// casilla de origen y orientación

	Q.push(inicial);
	visitado[inicial.fila][inicial.columna];

	while( !Q.empty())								//Mientras cola no este vacia
	{
		estado actual = Q.front();         			//Obtengo de la cola el estado actual, en un comienzo será el inicial
		Q.pop();                           			//Saco el elemento de la cola


		for( int i = 0; i < 4 && !encontrado; i++)					//Recorremos hasta 4 porque tenemos 4 posibles adyacentes
		{
			int nx = dx[i] + actual.fila;      		//nx y ny tendran la coordenada adyacente
			int ny = dy[i] + actual.columna;

			if(mapaResultado[nx][ny] != 'B' && mapaResultado[nx][ny] != 'A' &&
			 mapaResultado[nx][ny] != 'P' && mapaResultado[nx][ny] != 'M' && mapaResultado[nx][ny] != 'D' && !visitado[nx][ny])
			{
				int ori;

				if (nx < actual.fila)
					ori = 0;
				else if(nx > actual.fila)
					ori = 2;
				else if(ny < actual.columna)
					ori = 3;
				else if(ny > actual.columna)
					ori = 1;

				estado adyacente;
				adyacente.fila = nx;
				adyacente.columna = ny;
				adyacente.orientacion = ori;  			//Creamos estado adyacente aumento en 1 la distancia recorrida
				Q.push(adyacente);                     //Agregamos adyacente a la cola
				visitado[nx][ny] = true;

				if( (adyacente.fila == destino.fila) && (adyacente.columna == destino.columna))			//Si se llego al destino (punto final)
				{
					encontrado = true;
				}
				previo[nx][ny] = actual;
			}
		}
	}

	if(encontrado)
	{
		int i = destino.fila;
		int j = destino.columna;
		stack<estado> aux;
		estado nuevo, dest;

		dest.fila = destino.fila;
		dest.columna = destino.columna;
		if(previo[i][j].fila < destino.fila)
			dest.orientacion = 2;
		else if(previo[i][j].fila > destino.fila)
			dest.orientacion = 0;
		else if(previo[i][j].columna > destino.columna)
			dest.orientacion = 3;
		else if(previo[i][j].columna < destino.columna)
			dest.orientacion = 1;


		aux.push(dest);

		while(i!=origen.fila or j!=origen.columna)
		{
			estado pre = previo[i][j];
			aux.push(pre);
			i=pre.fila;
			j=pre.columna;
		}
/*
		while(!aux.empty())
		{
			cout << aux.top().fila << "," << aux.top().columna << "," << aux.top().orientacion << endl;
			aux.pop();
		}
*/
		aux.pop();
		//cout << nuevo.fila << "," << nuevo.columna << endl;
		//cout << antiguo.fila << "," << antiguo.columna << endl << endl;
		while(!aux.empty())
		{
			nuevo = aux.top();
			
			switch(nuevo.orientacion)
			{
				case 0:
					if(inicial.orientacion == 1)
						plan.push_back(actTURN_L);
					else if(inicial.orientacion == 2)
					{
						plan.push_back(actTURN_L);
						plan.push_back(actTURN_L);
					}
					else if(inicial.orientacion == 3)
						plan.push_back(actTURN_R);
				break;

				case 1:
					if(inicial.orientacion == 0)
						plan.push_back(actTURN_R);
					else if(inicial.orientacion == 2)
					{
						plan.push_back(actTURN_L);
					}
					else if(inicial.orientacion == 3)
					{
						plan.push_back(actTURN_R);
						plan.push_back(actTURN_R);
					}
				break;

				case 2:
					if(inicial.orientacion == 0)
					{
						plan.push_back(actTURN_R);
						plan.push_back(actTURN_R);
					}
					else if(inicial.orientacion == 1)
					{
						plan.push_back(actTURN_R);
					}
					else if(inicial.orientacion == 3)
					{
						plan.push_back(actTURN_L);
					}
				break;

				case 3:
					if(inicial.orientacion == 0)
					{
						plan.push_back(actTURN_L);
					}
					else if(inicial.orientacion == 1)
					{
						plan.push_back(actTURN_R);
						plan.push_back(actTURN_R);
					}
					else if(inicial.orientacion == 2)
					{
						plan.push_back(actTURN_R);
					}
				break;
			}
			plan.push_back(actFORWARD);
			inicial = nuevo;
			aux.pop();
		}

		// Descomentar para ver el plan en el mapa
		VisualizaPlan(origen, plan);

		return true;
	}

	return false;
}

Action ComportamientoJugador::think(Sensores sensores) {
  if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	cout << "Destino fil: " << sensores.destinoF << "  col: " << sensores.destinoC << endl;

	if (sensores.reset){
		hayPlan = false;
	}

	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != ultPosF or sensores.destinoC != ultPosC)){
		cout << "El destino ha cambiado\n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (!hayPlan){
		estado origen, destino;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    hayPlan = pathFinding(origen,destino,plan);

		ultPosF = sensores.destinoF;
		ultPosC = sensores.destinoC;
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size()>0){
		sigAccion = plan.front();
		if(sigAccion == actFORWARD && sensores.superficie[2] == 'a')
		{
			sigAccion = actIDLE;
		}
		else
			plan.erase(plan.begin());
	}
	else {
		sigAccion = actIDLE;
	}

	ultimaAccion = sigAccion;
	switch (sigAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula){
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
	}
	return sigAccion;
}


void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
