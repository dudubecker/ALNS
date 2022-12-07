#ifndef _SOL_H
#define _SOL_H

#include <iostream>
#include <algorithm>
#include "Instance.hpp"

class Sol
{
	//// Atributos:
	//private:
public:
	
	// Rotas da solução:
	std::vector<std::vector<double>> Rotas {};
	
	// Pedidos não atendidos na solução (chamados aqui de "L")
	std::vector<double> L {};
	
	// Pedidos atendidos na solução (chamados aqui de "A")
	std::vector<double> A {};
	
	// Instância da solução
	Instance inst;
	
	// Constructor/Destructor
	// Constructor com objeto instance:
	//Sol(Instance inst_val){
	//	inst = inst_val;
	//};
	// Adicionado na última atualização:
	Sol();
	
	// Criando objeto solução a partir de inst, Rotas, L e A
	Sol(std::vector<std::vector<double>>& Rotas_init, std::vector<double>& L_init, std::vector<double>& A_init, Instance& inst_init)
	{
		Rotas = Rotas_init;
		inst = inst_init;
		L = L_init;
		A = A_init;
	}
	
	
	// Criando objeto solução a partir de outro
	Sol(const Sol& S)
	{
		Rotas = S.Rotas;
		inst = S.inst;
		L = S.L;
		A = S.A;
	}
	
	~Sol();
	
	// Methods
	
	// Cálculo da função objetivo:
	double FO();
	
	// Cálculo da função objetivo penalizada:
	double FO_W();
	
	// Método para inserção de um pedido
	void inserir_pedido(double &pedido, int index_rota, int pos_no_pickup, int pos_no_delivery); // Somente posições factíveis?
	
	// Método para remoção de um pedido
	void remover_pedido(double &pedido);
	
	// Checagem se uma solução é factível ou não:
	bool isFeasible();
	
	// Checagem se uma rota da solução é factível ou não:
	bool isFeasible(int &index_rota);
	
	// Método para printar solução
	void print_sol();
	
};

#endif // _SOL_H
