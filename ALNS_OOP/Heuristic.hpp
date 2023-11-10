#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"


/*
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
	
	// Mudar de lugar!
	
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
*/

// /*
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
	
	// Tempo computacional empregado na heurística:
	double processing_time {0};
	
	// Início das alterações!
	
	// Métodos
	
	// Método "padrão" para os objetos Heuristic
	virtual Sol apply(Sol &S);
	
	// Método para inicialização dos operadores de remoção (calcula porcentagem de "n" a ser removida da solução)
	virtual int initializeMethod(Sol &S) = 0;
	
	// Método para inicialização dos operadores de inserção (placeholder)
	virtual void initializeMethod() = 0;
	
	// Método específico para cada heurística
	virtual Sol specificApply(Sol &S)  = 0;
	
	virtual ~Heuristic() {} // Adicionamos um destrutor virtual
	
	protected:
	// Construtor protegido para evitar instanciar a classe Heuristic diretamente
	Heuristic() {}
	
	
};

// Removal heuristic object

class RemovalHeuristic : public Heuristic {
public:
	
	// Sobrescrita do método "specificApply" para a RemovalHeuristic
	int initializeMethod(Sol &S) override;
	
	void initializeMethod() override {};
	
};

// Removal heuristics inherited objects

class RandomRemoval : public RemovalHeuristic {
	
	Sol specificApply(Sol &S) override;
	
};

class WorstRemoval : public RemovalHeuristic {
	
public:
	
	// Parâmetro para controlar grau de aleatorização no cálculo do incremento da FO
	double eta {};
	
	// Parâmetro para controle de aleatoriedade na escolha dos pedidos
	double delta {};
	
	// Constructor:
	WorstRemoval(double eta_value, double delta_value){
		eta = eta_value;
		delta = delta_value;
	};
	
	
	Sol specificApply(Sol &S) override;
	
};

class ShawsRemoval : public RemovalHeuristic {
	
public:
	// Shaw's Removal:
	double phi {};
	double chi {};
	double psi {};
	
	// Parâmetro para controle de aleatoriedade na escolha dos pedidos
	double delta {};
	
	// Constructor:
	ShawsRemoval(double phi_value, double chi_value, double psi_value, double delta_value){
		phi = phi_value;
		chi = chi_value;
		psi = psi_value;
		delta = delta_value;
	};
	
	
	Sol specificApply(Sol &S) override;
	
};

// Insertion heuristic object

class InsertionHeuristic : public Heuristic {
public:
	
	// Sobrescrita do método "specificApply" para a RemovalHeuristic
    int initializeMethod(Sol &S) override {};
	
	void initializeMethod() override;
};

class GreedyInsertion : public InsertionHeuristic {
	
public:
	
	// Parâmetro para controlar grau de aleatorização no cálculo do incremento da FO
	double eta {};
	
	// Constructor:
	GreedyInsertion(double eta_value){
		eta = eta_value;
	};
	
	Sol specificApply(Sol &S) override;
	
};

class RegretInsertion : public InsertionHeuristic {
	
public:

	Sol specificApply(Sol &S) override;
	
	// Parâmetro da regret
	double k {};
	
	// Parâmetro para controlar grau de aleatorização no cálculo do incremento da FO
	double eta {};
	
	// Constructor:
	RegretInsertion(double k_value, double eta_value){
		k = k_value;
		eta = eta_value;
	};
	
	
};
// */


#endif // _HEURISTIC_H_
