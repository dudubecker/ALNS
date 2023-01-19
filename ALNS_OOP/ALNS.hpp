#ifndef _ALNS_H
#define _ALNS_H
#include <iostream>
#include <Sol.hpp>
#include <Instance.hpp>
#include <Heuristic.hpp>

class ALNS
{
public:
	
	// Atributos
	
	// Vetor que guardará as heurísticas de remoção já inicializadas 
	std::vector<Heuristic> removal_heuristics {};
	
	// Vetor que guardará as heurísticas de inserção já inicializadas 
	std::vector<Heuristic> insertion_heuristics {};
	
	// Vetor com os pesos de cada heurística de remoção -> Talvez mais simples do que cada heurística ter um atributo "peso"!
	// std::vector<double> W_rem {};
	
	// Vetor com os pesos de cada heurística de inserção -> Talvez mais simples do que cada heurística ter um atributo "peso"!
	// std::vector<double> W_ins {};
	
	// OBS: Achei mais adequado fazer a soma dos pesos um atributo, para não ter que calcular esse valor em todas as iterações (na etapa de escolha das heurísticas);
	
	// Soma dos pesos das heurísticas de remoção:
	double soma_W_rem {0};
	
	// Soma dos pesos das heurísticas de inserção:
	double soma_W_ins {0};
	
	// Instância
	Instance inst;
	
	// Melhor solução encontrada pelo algoritmo:
	Sol S_p;
	
	// Solução incumbente, a ser alterada iterativamente:
	Sol S_i;
	
	// Soluções conhecidas: vetor com funções-objetivo já encontradas pelo algoritmo, usado na atualização dos scores!
	std::vector<double> S_encontradas {};
	
	// Atributos para o critério de aceitação:
	// Caso 1: as heurísticas resultaram um novo valor ótimo global
	int sigma_1 {27};
	
	// Caso 2: as heurísticas resultaram um valor que passou pelo critério de aceitação, ainda não foram aceitas anteriormente e S* < S
	int sigma_2 {30};
	
	// Caso 3: as heurísticas resultaram um valor que passou pelo critério de aceitação, ainda não foram aceitas anteriormente e S* > S
	int sigma_3 {22};
	
	// Fator de reação: utilizado para a atualização dos pesos
	double r {0.1};
	
	// Temperatura: utilizada no critério de aceitação, necessita um cálculo para seu valor inicial
	double Temperature {};
	
	// Taxa de resfriamento
	double c {0.9997};
	//double c {0.8};
	
	// Atributo "w": utilizado no critério de aceitação, para o cálculo da temperatura inicial
	double w {1.3};
	
	// Constructor/Destructor
	//ALNS();
	~ALNS();
	
	// Methods
	
	// Método para atualização do vetor de pesos, a partir do vetor de heurísticas
	void atualizar_pesos();
	
	// Método para atualização de pontuações: necessário saber se a heurística passou pelo critério de aceitação, valor da melhor solução global, solução incumbente e solução recém-alterada (S);
	void atualizar_pontuacoes(bool &CA, bool &BKS, int &index_h_rem, int &index_h_ins, double &FO);
	
	// Método para zerar pontuações
	void zerar_pontuacoes();
	
	// Método para critério de aceitação: retorna valor booleano caso o critério tenha sido aceito ou não
	bool criterio_aceitacao(Sol &S);
	
	// Método para escolher heurísticas: retorna o índice de uma heurística a partir de uma escolha aleatória considerando os pesos
	int escolher_heuristica(char type);
	
	// Método para o algoritmo geral, que utiliza todos os métodos acima e retorna a melhor função encontrada
	void algo(int max_it_ALNS, int max_it_RRH);
	
	// Método de inicialização para redução de rotas
	Sol routeReductionHeuristic(Sol &S_i, int max_it_RRH);
	

};

#endif // _ALNS_H
