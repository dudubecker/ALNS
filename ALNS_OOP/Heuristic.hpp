#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"


class Heuristic
{
public:
	
	// Nome da heurística: Cons, Shaw, Worst, Random, Greedy, Regret
	char name {};
	
	// Score da heurística, atualizado dinamicamente no algoritmo
	double score {0};
	
	// Peso (probabilidade de escolha) da heurística, atualizado dinamicamente no algoritmo
	double weight {0};
	
	// Número de iterações pelas quais a heurística passou: talvez inicializar como 1
	int n_it {0};
	
	// Número total de iterações pelas quais a heurística passou: talvez inicializar como 1
	int n_it_total {0};
	
	// ... Demais parâmetros específicos de cada heurística
	
	// Shaw's Removal:
	double phi {};
	double chi {};
	double psi {};
	
	// Regret insertion:
	double k {};
	
	// Tempo computacional empregado na heurística:
	double processing_time {};
	
	
	// Constructor/Destructor
	
	// Inicialização padrão, apenas com o character
	Heuristic(char heuristic_name){
		
		name = heuristic_name;
	}
	
	// Inicialização do Shaw's Removal
	Heuristic(char heuristic_name, double phi_value, double chi_value, double psi_value){
		
		name = heuristic_name;
		
		phi = phi_value;
		chi = chi_value;
		psi = psi_value;
		
	}
	
	// Inicialização do Regret insertion
	Heuristic(char heuristic_name, double k_value){
		
		name = heuristic_name;
		
		k = k_value;
		
	}
	
	~Heuristic();
	
	// Methods
	
	// Método de aplicação da heurística: tem como entrada uma solução e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S);
	
	

};

#endif // _HEURISTIC_H_
