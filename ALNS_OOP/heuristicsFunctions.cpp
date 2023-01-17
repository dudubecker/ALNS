#include "heuristicsFunctions.h"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include <numeric>

// Funções utilizadas ao longo da implementação das heurísticas:

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double delta_FO_ins(Sol &S_ins, double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S_ins.inst.n};
	
	// Objeto para a rota que potencialmente sofrerá a inserção
	// std::vector<double> novaRota = S_ins.Rotas[index_rota];
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		// double delta = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 1])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_delivery - 1])]);
		double delta = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(S_ins.Rotas[index_rota][pos_no_delivery - 1])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(S_ins.Rotas[index_rota][pos_no_delivery - 1])]);
		
		return delta;
		
	}else{
		
		
		// Variação pela inserção do nó de pickup
		// double delta_pickup = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(novaRota[pos_no_pickup])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		// double delta_delivery = (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 2])]) - (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(novaRota[pos_no_delivery - 2])]);
		
		// Variação pela inserção do nó de pickup
		double delta_pickup = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(S_ins.Rotas[index_rota][pos_no_pickup])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(S_ins.Rotas[index_rota][pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		double delta_delivery = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(S_ins.Rotas[index_rota][pos_no_delivery - 2])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_delivery - 1])][(S_ins.Rotas[index_rota][pos_no_delivery - 2])]);
		
		// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}