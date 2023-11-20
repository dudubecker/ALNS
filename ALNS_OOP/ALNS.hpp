#ifndef _ALNS_H
#define _ALNS_H
#include <iostream>
#include <Sol.hpp>
#include "Instance.hpp"
#include "Heuristic.hpp"


class ALNS
{
public:
	
	// Atributos
	
	// Vetor que guardará as heurísticas de remoção já inicializadas 
	std::vector<Heuristic*> heuristicas_remocao {};
	
	// Vetor que guardará as heurísticas de inserção já inicializadas 
	std::vector<Heuristic*> heuristicas_insercao {};
	
	// Soma dos pesos das heurísticas de remoção:
	double soma_peso_rem {0};
	
	// Soma dos pesos das heurísticas de inserção:
	double soma_peso_ins {0};
	
	// Melhor solução encontrada pelo algoritmo:
	Sol S_p;
	
	// Solução incumbente, a ser alterada iterativamente:
	Sol S_i;
	
	// Soluções conhecidas: vetor com funções-objetivo já encontradas pelo algoritmo, usado na atualização dos scores!
	std::vector<double> solucoes_encontradas {};
	
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
	double temperatura {};
	
	// Taxa de resfriamento
	double c {0.9997};
	
	// Atributo "w": utilizado no critério de aceitação, para o cálculo da temperatura inicial
	double w {1.3};
	
	// Valores aos atributos das heurísticas (eta e delta)
	
	// Parâmetro para controle de aleatoriedade na escolha dos pedidos (Shaws e Worst)
	double delta {};
	
	// Parâmetro para controlar grau de aleatorização no cálculo do incremento da FO
	double eta {};
	
	// Inicialização do objeto ALNS
	ALNS(Sol S_inicial, double w_value, double c_value, double sigma_1_value, double sigma_2_value, double sigma_3_value, double r_value, double eta_value, double delta_value);
	
	// Destructor
	~ALNS();
	
	// Methods
	
	// Método para atualização do vetor de pesos, a partir do vetor de heurísticas
	void atualizarPesos();
	
	// Método para critério de aceitação: retorna valor booleano caso o critério tenha sido aceito ou não
	bool avaliarCriterioDeAceitacao(Sol &S);
	
	// Método para escolher heurísticas: retorna o índice de uma heurística a partir de uma escolha aleatória considerando os pesos
	int escolherHeuristica(char type);
	
	// Método para atualização de pontuações: necessário saber se a heurística passou pelo critério de aceitação, valor da melhor solução global, solução incumbente e solução recém-alterada (S);
	void atualizarPontuacoes(bool &CA, bool &BKS, int &index_h_rem, int &index_h_ins, double &FO);
	
	// Método para zerar pontuações
	void zerarPontuacoes();
	
	// Método de inicialização para redução de rotas
	Sol reduzirRotas(Sol &S_i, int it_RRH);
	
	// Método para o algoritmo geral, que utiliza todos os métodos acima e retorna a melhor função encontrada
	void executarALNS(int max_it, int max_it_sem_melhoria, int it_RRH, double max_t);
	
};

#endif // _ALNS_H
