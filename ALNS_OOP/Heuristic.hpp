#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"


class Heuristic
{
public:
	
	// Score da heurística, atualizado dinamicamente no algoritmo
	double score {0};
	
	// Peso (probabilidade de escolha) da heurística, atualizado dinamicamente no algoritmo
	double peso {0};
	
	// Número de iterações pelas quais a heurística passou: talvez inicializar como 1
	int n_it {0};
	
	// Número total de iterações pelas quais a heurística passou: talvez inicializar como 1
	int n_it_total {0};
	
	// Início das alterações!
	
	// Métodos
	
	// Método "padrão" para os objetos Heuristic
	virtual Sol aplicar(Sol &S);
	
	// Método para inicialização dos operadores de remoção (calcula porcentagem de "n" a ser removida da solução)
	virtual int inicializarMetodo(Sol &S) = 0;
	
	// Método para inicialização dos operadores de inserção (placeholder)
	virtual void inicializarMetodo() = 0;
	
	// Método específico para cada heurística
	virtual Sol aplicarMetodoEspecifico(Sol &S)  = 0;
	
	virtual ~Heuristic() {} // Adicionamos um destrutor virtual
	
	protected:
	// Construtor protegido para evitar instanciar a classe Heuristic diretamente
	Heuristic() {}
	
	
};

// Removal heuristic object

class RemovalHeuristic : public Heuristic {
public:
	
	// Sobrescrita do método "specificApply" para a RemovalHeuristic
	int inicializarMetodo(Sol &S) override;
	
	void inicializarMetodo() override {};
	
};

// Removal heuristics inherited objects

class RandomRemoval : public RemovalHeuristic {
	
	Sol aplicarMetodoEspecifico(Sol &S) override;
	
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
	
	
	Sol aplicarMetodoEspecifico(Sol &S) override;
	
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
	
	
	Sol aplicarMetodoEspecifico(Sol &S) override;
	
};

// Insertion heuristic object

class InsertionHeuristic : public Heuristic {
public:
	
	// Sobrescrita do método "specificApply" para a RemovalHeuristic
    int inicializarMetodo(Sol &S) override {};
	
	void inicializarMetodo() override;
};

class GreedyInsertion : public InsertionHeuristic {
	
public:
	
	// Parâmetro para controlar grau de aleatorização no cálculo do incremento da FO
	double eta {};
	
	// Constructor:
	GreedyInsertion(double eta_value){
		eta = eta_value;
	};
	
	Sol aplicarMetodoEspecifico(Sol &S) override;
	
};

class RegretInsertion : public InsertionHeuristic {
	
public:

	Sol aplicarMetodoEspecifico(Sol &S) override;
	
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

#endif // _HEURISTIC_H_
