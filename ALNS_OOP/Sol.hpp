#ifndef _SOL_H
#define _SOL_H

#include <iostream>
#include <algorithm>
#include "Instance.hpp"

class Sol
{
	
public:
	
	// Rotas da solução:
	std::vector<std::vector<double>> rotas {};
	
	// Vetor de cargas
	std::vector<std::vector<double>> cargas {};
	
	// Vetor de tempos de visita
	std::vector<std::vector<double>> tempos_de_visita {};
	
	// Pedidos não atendidos na solução (chamados aqui de "L")
	std::vector<double> L {};
	
	// Pedidos atendidos na solução (chamados aqui de "A")
	std::vector<double> A {};
	
	// Tamanho das rotas na solução
	std::vector<int> rotas_size {};
	
	// Tamanho do vetor de pedidos não atendidos
	int L_size {};
	
	// Tamanho do vetor de pedidos atendidos
	int A_size {};
	
	// Posição do pedido na solução
	std::map<int, std::vector<int>> posicoes_pedidos {};
	
	// Instância da solução
	Instance inst;
	
	// Constructor/Destructor
	
	// Constructor com objeto instance:
	Sol(Instance &dados_instancia);
	
	// Atribuição direta
	Sol();
	
	// Destructor
	~Sol();
	
	// Métodos
	
	// Método para inserção de um pedido
	void inserirPedido(double &pedido, int index_rota, int pos_no_pickup, int pos_no_delivery); // Somente posições factíveis?
	
	// Método para remoção de um pedido
	void removerPedido(double &pedido);
	
	// Método para remoção de uma rota (usado na heurística de remoção de rotas)
	void removerRota(int index_rota);
	
	// Cálculo da função objetivo:
	double calcularFO();
	
	// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
	double calcularVariacaoFO(double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery);
	
	// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
	double calcularVariacaoFO(double &pedido);
	
	// Checagem se uma solução é factível ou não:
	bool checarFactibilidadeSolucao();
	
	// Novo método de checagem de factibilidade, que usa vetor de cargas e tempos de visita
	bool checarFactibilidadeInsercao(double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery);
	
	// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes a um dado pedido
	std::vector<double> calcularDadosMelhorInsercao(double &pedido);
	
	// Delta melhor inserção considerando rota (regret insertion):
	// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
	std::vector<double> calcularDadosMelhorInsercao(double &pedido, int &index_rota);
	
	// Executa a melhor inserção considerando todas as rotas:
	void executarMelhorInsercao(double &pedido);
	
	// Método para printar solução
	void printSol();
	
};

#endif // _SOL_H
