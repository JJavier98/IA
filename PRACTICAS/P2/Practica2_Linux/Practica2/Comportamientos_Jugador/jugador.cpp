#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"


#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <utility>
#include <stack>
#include <cstdlib>
#include <cassert>

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

int ComportamientoJugador::veoK(Sensores sensores)
{
	bool hayK = false;
	int i;

	for (i = 0 ; !hayK && i < sensores.terreno.size(); ++i)
	{
		if(sensores.terreno[i] == 'K')
			hayK = true;
	}
	if(!hayK)
		i =0;
	return --i;
}

void ComportamientoJugador::pintarMapa(Sensores sensores)
{
	assert(fil != -1);

	switch(brujula)
	{
		case 0: 
			mapaResultado[fil][col] = sensores.terreno[0];
			mapaResultado[fil-1][col-1] = sensores.terreno[1];
			mapaResultado[fil-1][col] = sensores.terreno[2];
			mapaResultado[fil-1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col-2] = sensores.terreno[4];
			mapaResultado[fil-2][col-1] = sensores.terreno[5];
			mapaResultado[fil-2][col] = sensores.terreno[6];
			mapaResultado[fil-2][col+1] = sensores.terreno[7];
			mapaResultado[fil-2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col-3] = sensores.terreno[9];
			mapaResultado[fil-3][col-2] = sensores.terreno[10];
			mapaResultado[fil-3][col-1] = sensores.terreno[11];
			mapaResultado[fil-3][col] = sensores.terreno[12];
			mapaResultado[fil-3][col+1] = sensores.terreno[13];
			mapaResultado[fil-3][col+2] = sensores.terreno[14];
			mapaResultado[fil-3][col+3] = sensores.terreno[15];
		break;
		case 1: 
			mapaResultado[fil][col] = sensores.terreno[0];
			mapaResultado[fil-1][col+1] = sensores.terreno[1];
			mapaResultado[fil][col+1] = sensores.terreno[2];
			mapaResultado[fil+1][col+1] = sensores.terreno[3];
			mapaResultado[fil-2][col+2] = sensores.terreno[4];
			mapaResultado[fil-1][col+2] = sensores.terreno[5];
			mapaResultado[fil][col+2] = sensores.terreno[6];
			mapaResultado[fil+1][col+2] = sensores.terreno[7];
			mapaResultado[fil+2][col+2] = sensores.terreno[8];
			mapaResultado[fil-3][col+3] = sensores.terreno[9];
			mapaResultado[fil-2][col+3] = sensores.terreno[10];
			mapaResultado[fil-1][col+3] = sensores.terreno[11];
			mapaResultado[fil][col+3] = sensores.terreno[12];
			mapaResultado[fil+1][col+3] = sensores.terreno[13];
			mapaResultado[fil+2][col+3] = sensores.terreno[14];
			mapaResultado[fil+3][col+3] = sensores.terreno[15];
		break;
		case 2:
			mapaResultado[fil][col] = sensores.terreno[0];
			mapaResultado[fil+1][col+1] = sensores.terreno[1];
			mapaResultado[fil+1][col] = sensores.terreno[2];
			mapaResultado[fil+1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col+2] = sensores.terreno[4];
			mapaResultado[fil+2][col+1] = sensores.terreno[5];
			mapaResultado[fil+2][col] = sensores.terreno[6];
			mapaResultado[fil+2][col-1] = sensores.terreno[7];
			mapaResultado[fil+2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col+3] = sensores.terreno[9];
			mapaResultado[fil+3][col+2] = sensores.terreno[10];
			mapaResultado[fil+3][col+1] = sensores.terreno[11];
			mapaResultado[fil+3][col] = sensores.terreno[12];
			mapaResultado[fil+3][col-1] = sensores.terreno[13];
			mapaResultado[fil+3][col-2] = sensores.terreno[14];
			mapaResultado[fil+3][col-3] = sensores.terreno[15];
		break;
		case 3:
			mapaResultado[fil][col] = sensores.terreno[0];
			mapaResultado[fil+1][col-1] = sensores.terreno[1];
			mapaResultado[fil][col-1] = sensores.terreno[2];
			mapaResultado[fil-1][col-1] = sensores.terreno[3];
			mapaResultado[fil+2][col-2] = sensores.terreno[4];
			mapaResultado[fil+1][col-2] = sensores.terreno[5];
			mapaResultado[fil][col-2] = sensores.terreno[6];
			mapaResultado[fil-1][col-2] = sensores.terreno[7];
			mapaResultado[fil-2][col-2] = sensores.terreno[8];
			mapaResultado[fil+3][col-3] = sensores.terreno[9];
			mapaResultado[fil+2][col-3] = sensores.terreno[10];
			mapaResultado[fil+1][col-3] = sensores.terreno[11];
			mapaResultado[fil][col-3] = sensores.terreno[12];
			mapaResultado[fil-1][col-3] = sensores.terreno[13];
			mapaResultado[fil-2][col-3] = sensores.terreno[14];
			mapaResultado[fil-3][col-3] = sensores.terreno[15];
		break;
	}
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan, Sensores sensores) {
	//Borro la lista

	plan.clear();
	int posK, filaK, columnaK;
	estado objetivo;
	bool mini = false,
		 hayObjetivo = false;
	vector<vector<unsigned char> > miniMatriz;

	int dx[4] = {-1, 0, 1, 0};
	int dy[4] = {0, 1, 0, -1};
	if(!seDondeEstoy)
	{
		mini = true;
		posK = veoK(sensores);
		miniMatriz.resize(sensores.terreno.size());

		for(int i = 0 ; i < sensores.terreno.size() ; ++i)
		{
			miniMatriz[i].resize(sensores.terreno.size());
			for (int j = 0; j < sensores.terreno.size(); ++j)
				miniMatriz[i][j] = 'P';
		}

			miniMatriz[0][0] = sensores.terreno[9];
			miniMatriz[0][1] = sensores.terreno[10];
			miniMatriz[0][2] = sensores.terreno[11];
			miniMatriz[0][3] = sensores.terreno[12];
			miniMatriz[0][4] = sensores.terreno[13];
			miniMatriz[0][5] = sensores.terreno[14];
			miniMatriz[0][6] = sensores.terreno[15];
			miniMatriz[1][1] = sensores.terreno[4];
			miniMatriz[1][2] = sensores.terreno[5];
			miniMatriz[1][3] = sensores.terreno[6];
			miniMatriz[1][4] = sensores.terreno[7];
			miniMatriz[1][5] = sensores.terreno[8];
			miniMatriz[2][2] = sensores.terreno[1];
			miniMatriz[2][3] = sensores.terreno[2];
			miniMatriz[2][4] = sensores.terreno[3];
			miniMatriz[3][3] = sensores.terreno[0];

		if(posK == -1)
		{

			if(girosIzda < 2)
			{
				
				girosIzda++;
				repeticionesTotales++;
				if(girosIzda == 2)
					girosDcha = 0;

				if(sensores.terreno[4] != 'B' && sensores.terreno[4] != 'A' &&
				 sensores.terreno[4] != 'P' && sensores.terreno[4] != 'M' && sensores.terreno[4] != 'D')
				{
					objetivo.fila = 1;
					objetivo.columna = 1;
					objetivo.orientacion = 0;

					hayObjetivo = true;


				}
			}
			else if(girosDcha < 2)
			{
				girosDcha++;
				if(repeticionesTotales == 20)
				{
					girosDcha--;
					repeticionesTotales = 0;
				}
				if(girosDcha == 2)
					girosIzda = 0;

				if(sensores.terreno[8] != 'B' && sensores.terreno[8] != 'A' &&
				 sensores.terreno[8] != 'P' && sensores.terreno[8] != 'M' && sensores.terreno[8] != 'D')
				{
					objetivo.fila = 1;
					objetivo.columna = 5;
					objetivo.orientacion = 0;

					hayObjetivo = true;


				}
			}
		}
		else
		{
			switch(posK)
			{
				case 0:	filaK = 3; columnaK =3;	break;

				case 1:	filaK = 2; columnaK =2; break;

				case 2:	filaK = 2; columnaK =3;	break;

				case 3:	filaK = 2; columnaK =4; break;

				case 4:	filaK = 1; columnaK =1;	break;

				case 5:	filaK = 1; columnaK =2; break;

				case 6:	filaK = 1; columnaK =3; break;

				case 7:	filaK = 1; columnaK =4; break;

				case 8:	filaK = 1; columnaK =5; break;

				case 9:	filaK = 0; columnaK =0; break;

				case 10: filaK = 0;	columnaK =1; break;

				case 11: filaK = 0;	columnaK =2; break;

				case 12: filaK = 0;	columnaK =3; break;

				case 13: filaK = 0; columnaK =4; break;

				case 14: filaK = 0;	columnaK =5; break;

				case 15: filaK = 0; columnaK =6; break;
			}
			objetivo.fila = filaK;
			objetivo.columna = columnaK;
			objetivo.orientacion = 0;

			hayObjetivo = true;
		}
	}
	else
	{
		objetivo.fila = destino.fila;
		objetivo.columna = destino.columna;
		objetivo.orientacion = destino.orientacion;

		hayObjetivo = true;
		mini = false;
	}
		

///////////////////////////////////////////////////////////////////NIVEL 2////////////////////////////////////////////////////////////////////////////////////////////////////

	bool encontrado = false;

	vector <vector<bool> > visitado;		// casillas visitadas
	vector<vector<estado> > previo;			// casilla anterior por la que se ha pasado
	vector<vector<unsigned char> > mapaFinal;
	estado inicial;				// casilla de origen y orientación


	if(mini)
	{
		mapaFinal.resize(miniMatriz.size());

		for(int i = 0 ; i < miniMatriz.size() ; ++i)
		{
			mapaFinal[i].resize(miniMatriz.size());
			mapaFinal[i] = miniMatriz[i];
		}
		inicial.fila = 3;
		inicial.columna = 3;
		inicial.orientacion = 0;


	}
	else
	{
		mapaFinal.resize(mapaResultado.size());
		for(int i = 0 ; i < mapaResultado.size() ; ++i)
		{
			mapaFinal[i].resize(mapaResultado.size());
			mapaFinal[i] = mapaResultado[i];
		}
		inicial = origen;


	}


	visitado.resize(mapaFinal.size());
	previo.resize(mapaFinal.size());

	estado a;
	a.fila = -3;
	a.columna = -3;
	a.orientacion = -3;
	for(int i = 0 ; i < mapaFinal.size() ; ++i)
	{
		visitado[i].resize(mapaFinal.size());
		previo[i].resize(mapaFinal.size());
		for (int j = 0; j < mapaFinal.size(); ++j)
		{
			visitado[i][j] = false;
			previo[i][j] = a;

		}
	}

	queue<estado> Q;						// Todos los posibles estados por los que paso hasta destino

	Q.push(inicial);
	visitado[inicial.fila][inicial.columna] = true;

	

	while( !Q.empty() and hayObjetivo)								//Mientras cola no este vacia
	{
		estado actual = Q.front();         			//Obtengo de la cola el estado actual, en un comienzo será el inicial
		Q.pop();                           			//Saco el elemento de la cola

		for( int i = 0; i < 4 && !encontrado; i++)					//Recorremos hasta 4 porque tenemos 4 posibles adyacentes
		{
			int nx, ny;
			if(dx[i]+actual.fila < mapaFinal.size() and dy[i]+actual.columna < mapaFinal.size())
			{
				nx = dx[i] + actual.fila;      		//nx y ny tendran la coordenada adyacente
				ny = dy[i] + actual.columna;


			

				if(mapaFinal[nx][ny] != 'B' && mapaFinal[nx][ny] != 'A' &&
				 mapaFinal[nx][ny] != 'P' && mapaFinal[nx][ny] != 'M' && mapaFinal[nx][ny] != 'D' && !visitado[nx][ny])
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


					if( (adyacente.fila == objetivo.fila) && (adyacente.columna == objetivo.columna))			//Si se llego al objetivo (punto final)
					{
						encontrado = true;
					}
					previo[nx][ny] = actual;
				}
			}
		}
	}

	if(encontrado)
	{

		int i = objetivo.fila;
		int j = objetivo.columna;
		stack<estado> aux;
		estado nuevo, dest;
		
		dest.fila = objetivo.fila;
		dest.columna = objetivo.columna;
		if(previo[i][j].fila < objetivo.fila)
			dest.orientacion = 2;
		else if(previo[i][j].fila > objetivo.fila)
			dest.orientacion = 0;
		else if(previo[i][j].columna > objetivo.columna)
			dest.orientacion = 3;
		else if(previo[i][j].columna < objetivo.columna)
			dest.orientacion = 1;

		
		aux.push(dest);

		while(i!=inicial.fila or j!=inicial.columna)
		{
			estado pre = previo[i][j];
			aux.push(pre);
			i=pre.fila;
			j=pre.columna;
		}
		
		aux.pop();
		while(!aux.empty())
		{
			nuevo = aux.top();
			
			//if(mini)
			//	inicial.orientacion =brujula;
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
		if(!mini)
			VisualizaPlan(origen, plan);
		

	}
	else if(mini)
	{
		
		if(sensores.terreno[2] != 'B' && sensores.terreno[2] != 'A' &&
			sensores.terreno[2] != 'P' && sensores.terreno[2] != 'M' && sensores.terreno[2] != 'D')
		{
			plan.push_back(actFORWARD);

		}
		else
		{
			srand( time( NULL ) );
			if(rand() % 2 == 1)
				plan.push_back(actTURN_R);
			else
				plan.push_back(actTURN_L);
		}
	}

	
	return true;
}

Action ComportamientoJugador::think(Sensores sensores) {
  	if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		seDondeEstoy = true;
	}
	if(seDondeEstoy)
		pintarMapa(sensores);

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

    	hayPlan = pathFinding(origen,destino,plan, sensores);

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
		hayPlan = false;
		sigAccion = actIDLE;
	}

	if((sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A' or
		sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M' or sensores.terreno[2] == 'D') and sigAccion == actFORWARD)
	{
		sigAccion = actIDLE;
		hayPlan = false;
	}
	else
	{
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
