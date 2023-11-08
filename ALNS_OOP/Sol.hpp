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
	
	// Vetor de cargas
	std::vector<std::vector<double>> Cargas {};
	
	// Vetor de tempos de visita
	std::vector<std::vector<double>> TemposDeVisita {};
	
	// Tamanho das rotas na solução
	std::vector<int> RotasSize {};
	
	// Pedidos não atendidos na solução (chamados aqui de "L")
	std::vector<double> L {};
	
	// Tamanho do vetor de pedidos não atendidos
	int LSize {};
	
	// Pedidos atendidos na solução (chamados aqui de "A")
	std::vector<double> A {};
	
	// Tamanho do vetor de pedidos atendidos
	int ASize {};
	
	// Posição do pedido na solução
	std::map<int, std::vector<int>> request_positions {};
	
	// Instância da solução
	Instance inst;
	
	// Constructor/Destructor
	// Constructor com objeto instance:
	Sol(Instance &inst_data);
	
	// Adicionado na última atualização:
	Sol();
	
	~Sol();
	
	// Methods
	
	// Cálculo da função objetivo:
	double FO();
	
	// Cálculo da função objetivo penalizada:
	// double FO_W();
	
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
	
	// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
	double delta_FO_ins(double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery);
	
	// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
	double delta_FO_rem(double &pedido);
	
	// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
	std::vector<double> delta_melhor_insercao(double &pedido);
	
	// Delta melhor inserção considerando rota (regret insertion):
	// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
	std::vector<double> delta_melhor_insercao(double &pedido, int &index_rota);
	
	// Executa a melhor inserção considerando todas as rotas:
	void executar_melhor_insercao(double &pedido);
	
	// Avaliando factibilidade da inserção de um pedido (sem realizar a inserção!)
	bool isInsertionFeasible(double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery);
	
	// Novo método de checagem de factibilidade, que usa vetor de cargas e tempos de visita
	bool checar_factibilidade(double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery);
	
};

#endif // _SOL_H
