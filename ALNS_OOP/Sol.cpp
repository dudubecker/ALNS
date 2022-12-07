#include "Sol.hpp"
#include "Instance.hpp"


Sol::Sol()
{
	
	
}

Sol::~Sol()
{
}

// Método de cálculo da função objetivo

double Sol::FO(){
	
	// Calculando custos da solução: ainda segundo método convencional, não utilizando FO do artigo!
	
	// Custo de cada veículo da instância:
	int custo_veiculo = 10000;
	
	// Custos totais
	double custos_totais = 0;
	
	for (auto &rota: Rotas){
		
		custos_totais += custo_veiculo;
		
		for (unsigned index_no {0}; index_no < rota.size() - 1; index_no++){
			
			custos_totais += inst.t[rota[index_no]][rota[index_no + 1]];
			
		}
	}
	
	return (double) custos_totais;
}

// Método de cálculo da função objetivo penalizada, como em Gasque 2022

/*
double Sol::FO_W(){
	
	// Custos totais
	double custos_totais {};
	
	for (unsigned index_rota {0}; index_rota < Rotas.size(); index_rota++){
		
		//custos_totais += custo_veiculo;
		//std::vector Rota = Rotas[index_rota];
		
		for (unsigned index_no {0}; index_no < Rota.size() - 1; index_no++){
			
			custos_totais += inst.t[Rota[index_no]][Rota[index_no + 1]];
			
		}
	}
	
}

*/

void Sol::print_sol(){
	
	std::cout << "S: " << std::endl;
	
	for (auto &rota: Rotas){
		
		for (auto &node: rota){
			
			std::cout << node << " ";
		}
		
		std::cout << "\n";
		
	}
	
	std::cout << "\n";
	
	std::cout << "L: " << std::endl;
	
	for (auto pedido: L){
			
			std::cout << pedido << " ";
	}
	
	std::cout << "\n";
	
	std::cout << "A: " << std::endl;
	
	for (auto pedido: A){
			
			std::cout << pedido << " ";
	}
	
	std::cout << "\n";
	
}

// Método para inserção de um pedido
void Sol::inserir_pedido(double &pedido, int index_rota, int pos_no_pickup, int pos_no_delivery){
	
	// Vetor que sofrerá inserção
	//std::vector novaRota = Rotas.at(index_rota);
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Inserindo nós na rota, nas determinadas
	
	Rotas.at(index_rota).insert(Rotas.at(index_rota).begin() + pos_no_pickup, no_pickup);
	Rotas.at(index_rota).insert(Rotas.at(index_rota).begin() + pos_no_delivery, no_delivery);
	
	//Rotas.at(index_rota) = novaRota;
	
	// Retirando pedido da lista de pedidos não atendidos
	
	L.erase(std::remove_if(L.begin(), L.end(), [&pedido](double value) -> bool { return value == pedido; }), L.end());
	
	// Adicionando pedido da lista de pedidos atendidos
	
	A.push_back(pedido);
	
	
}


// Método para remoção de um pedido
void Sol::remover_pedido(double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// "Procurando" pedido (no_pickup) na solução:
	
	for (auto &rota: Rotas){
		
		// Caso o nó esteja contido na rota
		if (count(rota.begin(), rota.end(), no_pickup)){
			
			// Removendo nó de pickup
			
			rota.erase(std::remove_if(rota.begin(), rota.end(), [&no_pickup](int value) -> bool { return value == no_pickup; }), rota.end());
			
			// Removendo nó de delivery
			
			rota.erase(std::remove_if(rota.begin(), rota.end(), [&no_delivery](int value) -> bool { return value == no_delivery; }), rota.end());
			
			break;
			
		}
	}
	
	// Adicionando pedido ao conjunto L
	L.push_back(pedido);
	
	// Removendo pedido do conjunto A
	A.erase(std::remove_if(A.begin(), A.end(), [&pedido](double value) -> bool { return value == pedido; }), A.end());
	
	
}

// Método para checar factibilidade
bool Sol::isFeasible(){
	
	// *** Checando factibilidade da rota ***
	// *OBS: não é necessário testar pairing e precedence, porque os pedidos são inseridos em pares nas rotas e as posições de inserção já asseguram precedência
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Para cada rota da solução:
	
	for (auto &rota: Rotas){
		
		// Capacidade atual do veículo na rota (inicia como 0)
		double cap_atual {0};
		
		// Tempo atual da rota (inicia como 0)
		double t_atual {0};
		
		
		for (unsigned index_no {1}; index_no < rota.size(); index_no++){
		
			// Variável que guarda o nó atual considerado na checagem de factibilidade
			int no_atual {rota.at(index_no - 1)};
			
			// Variável que guarda o nó seguinte considerado na checagem de factibilidade
			int no_seguinte {rota.at(index_no)};
			
			// Checando se ir do no atual para o nó seguinte irá violar as restrições de capacidade e time window
			if ((cap_atual + inst.q.at(no_seguinte) > inst.Cap) || (inst.l.at(no_seguinte) < t_atual + inst.t.at(no_atual).at(no_seguinte))){
				
				// Atribuindo valor falso para a variável de factibilidade e quebrando o laço for
				factivel = false;
				break;
				
			// Caso seja possível, os valores são atualizados
			} else {
				// Atualizando valores
				
				// Capacidade
				cap_atual += inst.q.at(no_seguinte);
				
				// Tempo
				// Caso haja adiantamento (tempo de chegada menor que a janela de tempo de abertura)
				if (t_atual + inst.t.at(no_atual).at(no_seguinte) < inst.e.at(no_seguinte)){
					t_atual = inst.e.at(no_seguinte);
					
				} else {
					t_atual += inst.t.at(no_atual).at(no_seguinte);
					
				}
			}
		}
		
		// Caso alguma rota da solução tenha sido infactível, a busca termina
		
		if (!factivel){
			break;
		}
	}
	
	return factivel;
	
}

// Método para checar factibilidade da rota
bool Sol::isFeasible(int &index_rota){
	
	// *** Checando factibilidade da rota ***
	// *OBS: não é necessário testar pairing e precedence, porque os pedidos são inseridos em pares nas rotas e as posições de inserção já asseguram precedência
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Criando objeto para armazenar a rota de índice correspondente
	// auto rota = Rotas.at(index_rota);
	
	// Capacidade atual do veículo na rota (inicia como 0)
	double cap_atual {0};
	
	// Tempo atual da rota (inicia como 0)
	double t_atual {0};
	
	for (unsigned index_no {1}; index_no < Rotas.at(index_rota).size(); index_no++){
	
		// Variável que guarda o nó atual considerado na checagem de factibilidade
		int no_atual {Rotas.at(index_rota).at(index_no - 1)};
		
		// Variável que guarda o nó seguinte considerado na checagem de factibilidade
		int no_seguinte {Rotas.at(index_rota).at(index_no)};
		
		// Checando se ir do no atual para o nó seguinte irá violar as restrições de capacidade e time window
		if ((cap_atual + inst.q.at(no_seguinte) > inst.Cap) || (inst.l.at(no_seguinte) < t_atual + inst.t.at(no_atual).at(no_seguinte))){
			
			// Atribuindo valor falso para a variável de factibilidade e quebrando o laço for
			factivel = false;
			break;
			
		// Caso seja possível, os valores são atualizados
		} else {
			// Atualizando valores
			
			// Capacidade
			cap_atual += inst.q.at(no_seguinte);
			
			// Tempo
			// Caso haja adiantamento (tempo de chegada menor que a janela de tempo de abertura)
			if (t_atual + inst.t.at(no_atual).at(no_seguinte) < inst.e.at(no_seguinte)){
				t_atual = inst.e.at(no_seguinte);
				
			} else {
				t_atual += inst.t.at(no_atual).at(no_seguinte);
				
			}
		}
	}
	
	// Caso alguma rota da solução tenha sido infactível, a busca termina
	
	return factivel;
	
}


