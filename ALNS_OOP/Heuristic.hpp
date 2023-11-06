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
	
	/*
	
	// Início das alterações!
	
	// Métodos
	
	// Main method for applying heuristic
	virtual Sol apply(Sol &S);
	
	// Method for initialization of removal methods
	virtual int initializeMethod(Sol &S) = 0;
	
	// Method for initialization of insertion methods
	virtual void initializeMethod() = 0;
	
	// Specific method for each heuristic
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
	
	Sol specificApply(Sol &S) override;
	
};

class ShawsRemoval : public RemovalHeuristic {
	
	// Shaw's Removal:
	double phi {};
	double chi {};
	double psi {};
	
	// Constructor:
	ShawsRemoval(int phi_value, int chi_value, int psi_value){
		phi = phi_value;
		chi = chi_value;
		psi = psi_value;
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
	
	Sol specificApply(Sol &S) override;
	
};

class RegretInsertion : public InsertionHeuristic {
	
	Sol specificApply(Sol &S) override;
	
	// Parâmetro da regret
	double k {};
	
	// Constructor:
	RegretInsertion(int k_value){
		k = k_value;
	};
	
	
*/
};

#endif // _HEURISTIC_H_
