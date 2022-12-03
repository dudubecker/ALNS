#ifndef _INSTANCE_H_
#define _INSTANCE_H_
// Quais "bibliotecas" incluir aqui?
#include <iostream>
#include <vector>
#include <cmath>

class Instance
{
	//// Atributos:
	//private:
	public:
	
	// Coordenadas x de cada nó i da instância
	std::vector <double> x {};
	
	// Coordenadas y de cada nó i da instância
	std::vector <double> y {};
	
	// Tempo de serviço de cada nó i da instância
	std::vector <double> d {};
	
	// Demanda de cada nó i da instância
	std::vector <double> q {};
	
	// Início da janela de tempo de cada nó i da instância
	std::vector <double> e {};
	
	// Fim da janela de tempo de cada nó i da instância
	std::vector <double> l {};
	
	// Número de pedidos
	int n {};
	
	// Capacidade de cada veículo
	int Cap {};
	
	// Definindo vetor com "t_ij", para cada par de nós i j:
	
	std::vector<std::vector<double>> t {};
	
	// Dados normalizados para Shaw's removal: demandas (q) e tempos de viagem (t)
	
	std::vector <double> q_norm {};
	
	std::vector<std::vector<double>> t_norm {};
	
	// Constructor/Destructor
	Instance();
	
	~Instance();
	
	// Methods
	
	void read(std::string file_name);
	
};

#endif // _INSTANCE_H_
