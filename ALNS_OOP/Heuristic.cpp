#include "Heuristic.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>

//Heuristic::Heuristic()
//{
//}

Heuristic::~Heuristic()
{
}

// Funções utilizadas ao longo da implementação das heurísticas:

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double delta_FO_ins(Sol &S_ins, double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S_ins.inst.n};
	
	// Objeto para a rota que potencialmente sofrerá a inserção
	std::vector<double> novaRota = S_ins.Rotas[index_rota];
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		double delta = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 1])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_delivery - 1])]);
		
		return delta;
		
	}else{
		
		
		// Variação pela inserção do nó de pickup
		double delta_pickup = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(novaRota[pos_no_pickup])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		double delta_delivery = (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 2])]) - (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(novaRota[pos_no_delivery - 2])]);
		
		// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
double delta_FO_rem(Sol &S_rem, double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S_rem.inst.n};
	
	// Achando índice da rota que contém o pedido
	int index_rota {0};
	
	for (auto &rota: S_rem.Rotas){
		
		if (count(rota.begin(), rota.end(), no_pickup)){
			
			break;
			
		}else{
			index_rota += 1;
		}
		
	}
	
	// Objeto para a rota que potencialmente sofrerá a remoção
	std::vector<double> novaRota = S_rem.Rotas[index_rota];
	
	// Índice (posição) do nó de pickup na rota
	double pos_no_pickup = std::find(novaRota.begin(),novaRota.end(), no_pickup) - novaRota.begin();
	
	// Índice (posição) do nó de delivery na rota
	double pos_no_delivery = std::find(novaRota.begin(),novaRota.end(), no_delivery) - novaRota.begin();
	
	// Calculando decréscimo nos custos
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// std::cout << novaRota[pos_no_pickup - 1] << " " << no_pickup << " " << novaRota[pos_no_delivery + 1] << " " << no_delivery << "\n";
		
		// Variação total
		double delta = (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(novaRota[pos_no_delivery + 1])]) - (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_delivery + 1])]);
		
		return delta;
		
	}else{
		
		// std::cout << novaRota[pos_no_pickup - 1] << " " << no_pickup << " " << novaRota[pos_no_pickup + 1] << " " << novaRota[pos_no_delivery -1] << " " << no_delivery << " " << novaRota[pos_no_delivery + 1] << "\n";
		//// Variação pela inserção do nó de pickup
		double delta_pickup = (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(novaRota[pos_no_pickup + 1])]) - (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_pickup + 1])]);
		
		//// Variação pela inserção do nó de delivery
		double delta_delivery = (S_rem.inst.t[(novaRota[pos_no_delivery - 1])][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(novaRota[pos_no_delivery + 1])]) - (S_rem.inst.t[(novaRota[pos_no_delivery - 1])][(novaRota[pos_no_delivery + 1])]);
		
		//// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

double delta_melhor_insercao(Sol &S,double &pedido, int &index_rota){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Realizando inserções
		
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
				Sol S_teste = S;
				
				// Inserindo nós na rota, nas posições da iteração
				S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
				
				if (S_teste.isFeasible(index_rota)){
					
					num_rotas_factiveis += 1;
					
					double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (delta_S < delta_min){
						
						delta_min = delta_S;
						
					}
					
					
				}
				
			}
		}		
	}			
	
	if (num_rotas_factiveis > 0){
		
		return delta_min;
		
	} else {
		
		// Possível problema em retornar um valor grande: se não for factível, como busca-se maximizar (regret), vai dar problema!
		// return 99999;
		return 0;
		
	}
	
}

double delta_melhor_insercao(Sol &S, double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
					Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (S_teste.isFeasible(index_rota)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
						}
						
						
					}
					
				}
			}		
		}			
	}
	
	if (num_rotas_factiveis > 0){
		
		return delta_min;
		
	} else {
		
		return 99999;
		
	}
	
	//return delta_min;
	
}

// Tentativa: retornar, em vez de apenas o delta, um vetor com o delta, rota e posições e melhor inserção!

std::vector<double> delta_melhor_insercao(Sol &S, double &pedido, char teste){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
					Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (S_teste.isFeasible(index_rota)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
							
						}
						
						
					}
					
				}
			}		
		}			
	}
	
	//return_vector = {delta_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min, index_rota_min};
	
	if (num_rotas_factiveis > 0){
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
	} else {
		
		return_vector = {99999, 0, 0,0};
		
		//return 99999;
		
	}
	
	return return_vector;
	
	//return delta_min;
	
}

// Delta melhor inserção considerando rota (regret insertion):

std::vector<double> delta_melhor_insercao(Sol &S, double &pedido, int &index_rota, char teste){
	
	// Criando uma cópia do objeto solução:
	// Sol S(Rotas, L, A, inst);
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Realizando inserções
		
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
				Sol S_teste = S;
				
				// Inserindo nós na rota, nas posições da iteração
				S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
				
				if (S_teste.isFeasible(index_rota)){
					
					num_rotas_factiveis += 1;
					
					double delta_S = S.delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (delta_S < delta_min){
						
						delta_min = delta_S;
						
						pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
						pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
						index_rota_min = index_rota;
						
						
					}
					
					
				}
				
			}
		}		
	}			
	
	if (num_rotas_factiveis > 0){
		
		// return delta_min;
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
		
	} else {
		
		// Possível problema em retornar um valor grande: se não for factível, como busca-se maximizar, vai dar problema!
		// return 0;
		
		return_vector = {0, 0, 0,0};
		
		
	}
	
	return return_vector;
	
}


Sol melhor_insercao(Sol &S,double &pedido){
	
	// Criando uma cópia do objeto solução:
	// Sol S(Rotas, L, A, inst);
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção
					Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (S_teste.isFeasible(index_rota)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
							
						}
						
						
					}
					
				}
			}		
		}			
	}
	
	if (num_rotas_factiveis > 0){
		
		S.inserir_pedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
	return S;
	
}

// Fim das funções utilizadas ao longo da implementação das heurísticas

// Método principal das heurísticas

// Método de aplicação das heurísticas
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol Heuristic::apply(Sol &S){
	
	//cout << "Hello I'm waiting...." << endl;
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000) );
	//cout << "Waited 20000 ms\n";
	
	// Cálculo de "mi": possivelmente "mi" pode ser um parâmetro do método "apply()"!
	// srand(time(NULL));
	// Gerando aqui o número aleatório de pedidos "mi": possivelmente será feito fora!
	
	// Medindo tempo
	auto begin = std::chrono::high_resolution_clock::now();
	
	// Valor mínimo de mi (10% da quantidade de pedidos)
	int low_mi = round(S.inst.n*0.1);
			
	// Valor máximo de mi (40% da quantidade de pedidos)
	int high_mi = round(S.inst.n*0.4);
			
	int mi = low_mi + rand()%(high_mi - low_mi + 1);
	
	
	switch (name){
		
		// Heurística construtiva
		case 'C':{
			
			std::cout << "Solucao apos heuristica construtiva" << std::endl;
		
			/* Criando variáveis com valores atualizados a cada inserção */
			// Inserindo no conjunto L os pedidos não atendidos
			
			for(double value = 1; value < S.inst.n + 1; value++){
				
				S.L.push_back(value);
			}
			
			// Criando uma rota inicial vazia para a solução:
			S.Rotas.push_back({0, 2*(S.inst.n) + 1});
			
			// Quantidade de requests atendidos (inicia-se em 0)
			int qtd_atendidos {0};
			
			// Início do algoritmo de inserção:
			while (qtd_atendidos < S.inst.n){
				
				// std::cout << "A" << std::endl;
				
				// Variável que abrigará o request a ser inserido na iteração
				double pedido {S.L.at(0)};
				
				// std::cout << "A" << std::endl;
				
				//Sol *S_best = new Sol(S);
				
				// Objeto solução, que tentará fazer melhor inserção
				Sol S_best = S.melhor_insercao(pedido);
				
				// Caso o pedido tenha sido inserido:
				if (S_best.L.size() != S.L.size()){
					
					S = S_best;
					
				
				
				
				// Caso não seja possível fazer a inserção, isso significará que não foram encontradas posições de inserção factíveis para as rotas em questão
				// fazendo-se necessária uma nova rota
				
				} else {
					
					// Rota vazia com os nós da iteração
					std::vector <double> nova_rota {0, pedido, pedido + S.inst.n, 2*(S.inst.n) + 1};
					S.Rotas.push_back(nova_rota);
					
					// Removendo pedido de L
					S.L.erase(S.L.begin());
					
					// Adicionando pedido em A
					S.A.push_back(pedido);
					
				}
				
				// Atualizando quantidade de pedidos atendidos
				qtd_atendidos += 1;
				
				
			}
			// Fim da heurística construtiva
			
			break;
		}
		
		// Random removal
		case 'R':{
			
			std::cout << "Solucao apos random removal" << std::endl;
			
			// srand(time(NULL));
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Vetor com pedidos a serem removidos
			std::vector<double> pedidos_a_remover {};
			
			while (n_pedidos_removidos < mi){
				
				double pedido_aleatorio = rand()%S.inst.n + 1;
				
				// Se o pedido não estiver no vetor de pedidos a serem removidos e nem nos pedidos ausentes (L), ele pode ser escolhido
				if ((!count(pedidos_a_remover.begin(), pedidos_a_remover.end(), pedido_aleatorio)) && (!count(S.L.begin(), S.L.end(), pedido_aleatorio))){
					
					pedidos_a_remover.push_back(pedido_aleatorio);
					n_pedidos_removidos += 1;
				}
				
			}
			
			// Retirando pedidos da solução:
			for (auto pedido: pedidos_a_remover){
				
				S.remover_pedido(pedido);
				
			}
			
			// Fim da heurística random removal
			break;
		}
		
		// Worst removal
		case 'W':{
			
			std::cout << "Solucao apos worst removal" << std::endl;
			
			// srand(time(NULL));
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			// Pedidos contidos na solução: complemento de L!
			std::vector<double> pedidos = S.A;
			
			// Pedidos contidos na solução
			//for (auto pedido {1}; pedido < S.inst.n + 1; pedido++){
				
				//// Se o pedido não estiver no conjunto L, é porque ele está na solução!
				//if (!count(S.L.begin(), S.L.end(), pedido)){
				//	pedidos.push_back(pedido);
				//}
			//}
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Início do algoritmo
			while (n_pedidos_removidos < mi){
				
				// Custos incrementais de cada pedido (não-ordenados)
				std::vector<double> delta_custos {};
				
				for (auto pedido: pedidos){
					
					// Calculando variação no custo da rota com a remoção do pedido
					double delta = delta_FO_rem(S, pedido);
					
					delta_custos.push_back(delta);
					
				}
				
				// Ordenando lista
				
				// Vetor "delta_custos_sorted", com ordenação dos custos incrementais em ordem decrescente
				std::vector<double> delta_custos_sorted = delta_custos;
				sort(delta_custos_sorted.begin(), delta_custos_sorted.end(), std::greater<double>());
				
				// Vetor "pedidos_sorted", com ordenação dos pedidos de acordo com custos de remoção
				std::vector<double> pedidos_sorted {};
				
				// Iterando no vetor de deltas ordenados, buscando índice correspondente no vetor de deltas não ordenados e trazendo pedido correspondente
				for (auto delta: delta_custos_sorted){
				
					// Índice no vetor não ordenado
					
					// Obtendo posição do valor da lista de semelhanças ordenada na lista de semelhanças não-ordenada
					auto index_delta = std::find(delta_custos.begin(), delta_custos.end(), delta);
					
					int index_pedido = std::distance(delta_custos.begin(), index_delta);
					
					// Guardando pedido correspondente na lista ordenada
					
					pedidos_sorted.push_back(pedidos.at(index_pedido));
					
				
				} 
				
				// Escolhendo pedido a ser removido
				
				// Valor aleatório p entre 0 e 1
				
				double p = rand()/RAND_MAX;
				
				// Posição na lista ordenada de pedidos do pedido a ser retirado da solução
				
				// Obs: "-1" é necessário porque, caso o valor p seja muito próximo de 1, a posição de retirada será igual ao tamanho do vetor e isso resultará em um erro de índice!
				double pos_retirada = round(pow(p, delta)*((pedidos_sorted).size()-1));
				
				// Pedido retirado
				
				double pedido_retirado = pedidos_sorted.at(pos_retirada);
				
				// Retirando pedido do vetor "pedidos"
				
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_retirado](double value) -> bool { return value == pedido_retirado; }), pedidos.end());
				
				// Retirando pedido da solução
				
				S.remover_pedido(pedido_retirado);
				
				
				//for (auto delta: delta_custos_sorted){
				
				//	std::cout << delta << " ";
				
				//}
				
				//std::cout << "\n";
				
				n_pedidos_removidos += 1;
				
				
			}
			
			// Fim da heurística worst removal
			break;
		}
		
		// Shaws removal
		case 'S':{
			
			std::cout << "Solucao apos shaws removal" << std::endl;
			
			// Parâmetros: possivelmente alterados na inicialização da heurística!
			
			// Parâmetro "phi", que multiplica o termo das distâncias (ropke 2006, eq 17)
			// const double phi {0.3};
			
			// Parâmetro "chi", que multiplica o termo dos tempos de visita (ropke 2006, eq 17)
			// const double chi {0.4};
			
			// Parâmetro "psi", que multiplica o termo das demandas (ropke 2006, eq 17)
			// const double psi {0.3};
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			//// Obtendo tempos de visita (T_i) de cada nó "i" na solução S
			
			std::vector <double> T (2*(S.inst.n) + 2, 0);
			
			// Iterando para cada rota na solução 
			for (auto &rota: S.Rotas){
				
				// Inicializando tempo da rota
				int tempo_rota {0};
				
				// Iterando para cada nó na rota 
				for(unsigned index_no {0}; index_no < rota.size() - 1; index_no++){
					
					// Nó atual
					int no_atual = rota.at(index_no);
					
					// Nó seguinte
					int no_seguinte = rota.at(index_no+1);
					
					// Atualizando valor de tempo de visita no vetor T
					T[no_atual] = tempo_rota;
					
					// Incrementando tempo da rota
					tempo_rota += S.inst.t.at(no_atual).at(no_seguinte);
					
				}
			}
			
			// Tempos de visita (T):
			// Vetor com tempos de visita normalizados
			std::vector <double> T_norm (2*(S.inst.n) + 2, 0);
			
			// Valor mínimo para tempos de visita
			double min_T = *min_element(T.begin(), T.end());
			// Valor máximo para tempos de visita
			double max_T = *max_element(T.begin(), T.end());
			
			// Normalização de T
			
			for (int i {0}; i < 2*(S.inst.n) + 2; i++){
				
				// Normalizando valores de tempos de visita
				
				T_norm[i] = (T[i] - min_T)/(max_T - min_T);
				
			}
			
			// Obtendo grau de semelhança R entre pedidos da instância
			// Pontos importantes: entre nós iguais, esse grau de semelhança será igual a 0! É necessário tratar isso!
			// Além disso, R não estará definido para o depósito central, já que esses nós não poderão ser retirados!
			
			// Vetor com graus de semelhança
			std::vector<std::vector<double>> R(S.inst.n + 1, std::vector<double> (S.inst.n + 1, 9999));
			
			for (unsigned i {1}; i < S.inst.n + 1; i++){
				for (unsigned j {1}; j < S.inst.n + 1; j++){
					
					 // Calculando valor de R_ij
					
					// Caso o pedido não esteja contido na solução, seu grau de proximidade com qualquer outro pedido mantem-se em 9999
					if ((count(S.L.begin(), S.L.end(), i)) || (count(S.L.begin(), S.L.end(), j))){
						;
					}else{
						R[i][j] = phi*(S.inst.t_norm[i][j] + S.inst.t_norm[i+S.inst.n][j+S.inst.n]) + chi*(abs(T_norm[i] - T_norm[j]) + abs(T_norm[i+S.inst.n] - T_norm[j+S.inst.n])) + psi*(abs(S.inst.q_norm[i] - S.inst.q_norm[j]));
					}
					
				}
			}
			
			// Printar R_ij para debug:
			
			//for (unsigned i {0}; i < S.inst.n + 1; i++){
				//for (unsigned j {0}; j < S.inst.n + 1; j++){
					
					//std::cout << ceil(R[i][j]*100)/100 << " ";
					
				//}
				
				//std::cout << "\n";
			//}
			
			//////////////// debugado até aqui!
			
			//// Início do algoritmo
			
			// Escolhendo um pedido aleatório para inicialização, que não esteja no conjunto L
			// Seria viável escolher pedidos do conjunto L? Acho que provavelmente sim, porque teria uma sinergia entre worst e shaw's removal!
			// Um pedido difícil de ser inserido ficará "preso" no conjunto L, e a Shaw's removal pode ajudar a inserir esse pedido ao tirar pedidos
			// parecidos com ele
			
			
			// Vetor com pedidos disponíveis para remoção
			std::vector<double> pedidos = S.A;
			
			// Criando vetor D com pedidos removidos, inicializado pelo primeiro pedido escolhido
			std::vector <double> D {};
			
			if (S.L.size() == 0){
				
				double r = S.A.at(rand()%((S.A).size()));
				D.push_back(r);
				
				// Removendo pedido da lista de pedidos disponíveis para remoção
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&r](double value) -> bool { return value == r; }), pedidos.end());
				
				
			} else {
				
				double r = S.L.at(rand()%((S.L).size()));
				D.push_back(r);
			}
			
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			while (n_pedidos_removidos < mi){
				
				// Pedido r em "D" a ser levado em conta:
				double r = D.at(rand()%D.size());
				
				// Criando objeto com graus de semelhança para os pedidos em A
				std::vector<double> R_r {};
				
				for (auto &pedido: pedidos){
					
					R_r.push_back(R[r][pedido]);
					
					}
				
				// Criando objeto com graus de semelhança ordenados para os pedidos em A
				std::vector<double> R_r_sorted (R_r);
				sort(R_r_sorted.begin(), R_r_sorted.end());
				
				// Vetor "pedidos_sorted", com ordenação dos pedidos de acordo com custos de remoção
				std::vector<double> pedidos_sorted {};
				
				// Iterando no vetor de graus de semelhança ordenados, buscando índice correspondente no vetor de graus não ordenados e trazendo pedido correspondente
				for (auto &R: R_r_sorted){
				
					// Índice no vetor não ordenado
					
					// Obtendo posição do valor da lista de semelhanças ordenada na lista de semelhanças não-ordenada
					auto index_R = std::find(R_r.begin(), R_r.end(), R);
					
					int index_pedido = std::distance(R_r.begin(), index_R);
					
					// Guardando pedido correspondente na lista ordenada
					
					pedidos_sorted.push_back(pedidos.at(index_pedido));
					
				
				} 
				
				// Retirando pedido:
				
				double p = (double) rand()/RAND_MAX;
				
				// Posição na lista ordenada de pedidos do pedido a ser retirado da solução
				double pos_retirada = round(pow(p, delta)*((pedidos_sorted).size()-1));
				
				// Pedido retirado
				double pedido_retirado = pedidos_sorted[pos_retirada];
				
				D.push_back(pedido_retirado);
				
				// Removendo pedido da lista de pedidos disponíveis para remoção
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_retirado](double value) -> bool { return value == pedido_retirado; }), pedidos.end());
				
				n_pedidos_removidos += 1;
			}
			
			// Retirando pedidos em D do objeto de solução
			
			// Caso o algoritmo não tenha sido inicializado com um pedido em L:
			
			if (S.L.size() == 0){
			
				for (auto &pedido: D){
					
					S.remover_pedido(pedido);
					
				}
			
			} else {
				
				// Inicia-se por 1, pois o pedido de índice 0 é o pedido que já está em L!
				
				for (int index {1}; index < D.size() ; index++){
					
					S.remover_pedido(D.at(index));
					
				}
				
			}
			
			// Fim da heurística shaws removal
			break;
		}
		
		case 'G':{
			
			std::cout << "Solucao apos greedy insertion" << std::endl;
			
			// Variável que controlará o número de pedidos inseridos pelo algoritmo
			int qtd_inseridos {0};
			
			// Quantidade de pedidos não atendidos na solução 
			int qtd_pedidos_nao_atendidos {S.L.size()};
			
			// Condição de parada do algoritmo: caso não tenham sido encontradas posições factíveis de inserção ou caso todos os pedidos tenham sido inseridos
			while (true){
				
				// Variável que controlará o número de inserções factíveis encontradas
				int qtd_insercoes_factiveis {0};
				
				// Menor delta de inserção encontrado
				double delta_min {9999};
				
				// Pedido correspondente ao menor delta de inserção
				double pedido_min {};
				
				// Dados de rota e posição dos nós para o pedido de menor delta:
				std::vector<double> dados_melhor_insercao_min {};
				
				for (auto &pedido: S.L){
					
					// Obtendo dados para melhor insercao do pedido (chamando função com parâmetro char)
					std::vector<double> dados_melhor_insercao = delta_melhor_insercao(S, pedido, 'A');
					
					// Variação na função objetivo dada pela melhor inserção possível do pedido em S:
					// double delta = delta_melhor_insercao(S, pedido);
					
					// Delta FO -> Primeiro dado do vetor retornado pela função
					double delta = dados_melhor_insercao.at(0);
					
					if (delta < delta_min){
						
						delta_min = delta;
						pedido_min = pedido;
						
						dados_melhor_insercao_min = dados_melhor_insercao;
						
						// A função retornará um valor maior do que 9999 para delta! Por isso a contagem de inserções factíveis está aqui!
						qtd_insercoes_factiveis += 1;
						
					}
					
				}
				
				// Caso se tenha achado pelo menos uma posição de inserção factível (delta_min < 9999)
				if (delta_min < 9999){
					
					// S = melhor_insercao(S, pedido_min);
					// Inserindo pedido de delta mínimo, com dados do vetor retornado pela função
					S.inserir_pedido(pedido_min, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
					
					qtd_inseridos += 1;
					
				// Caso não tenha se achado pelo menos uma posição de inserção factível (delta_min == 99999, valor retornado pela função)
				} else {
					
					qtd_insercoes_factiveis == 0;
					
				}
				
				if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis == 0)){
					
					break;
					
				}
				
			}
			
			// Fim da heurística de greedy insertion
			break;
			
		}
		
		
		// Regret insertion
		case 'A':{
			
			std::cout << "Solucao apos regret insertion" << std::endl;
			
			// Definindo o valor "k" (por enquanto, só igual a 2);
			// double k = 2;
			
			// Variável que controlará o número de pedidos inseridos pelo algoritmo
			int qtd_inseridos {0};
			
			// Quantidade de pedidos não atendidos na solução 
			int qtd_pedidos_nao_atendidos {S.L.size()};
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Condição de parada do algoritmo: caso não tenham sido encontradas posições factíveis de inserção ou caso todos os pedidos tenham sido inseridos
			while (true){
				
				// Variável que controlará o número total de inserções factíveis encontradas na iteração
				int qtd_insercoes_factiveis_total {0};
				
				// Regret value máximo 
				double max_regret {0};
				
				// Pedido correspondente
				double pedido_max_regret {0};
				
				// Quantidade mínima de inserções achada para um pedido com menos de "m - k + 1" inserções factíveis
				int qtd_insercoes_factiveis_pedido_min {9999};
				
				// Pedido correspondente à quantidade mínima de inserções
				double pedido_min_insercoes {0};
				
				// Delta de inserção mínimo correspondente ao pedido com o número mínimo de inserções factíveis
				double delta_min_pedido_min_insercoes {9999};
				
				// Dados de rota e posição dos nós para o pedido de menor delta
				std::vector<double> dados_melhor_insercao_min {};
				
				// Iterando para cada pedido não atendido no objeto solução
				for (auto &pedido: S.L){
					
					// Vetor de deltas para a melhor inserção em cada rota de cada pedido
					std::vector<double> delta_min_por_rota {};
					
					// Variável que controlará o número de inserções factíveis encontradas pelo pedido da iteração (para ter controle do pedido com mínimo número de inserções factíveis)
					int qtd_insercoes_factiveis_pedido {0};
					
					// Dados de rota e posição dos nós para o pedido de menor delta:
					std::vector<double> dados_melhor_insercao_pedido_min {};
					
					// Delta correspondente à posição de mínima inserção para cada rota (inicia-se alto)
					double delta_min {9999};
					
					// Para cada rota da solução, será determinado o delta da melhor inserção
					for (auto index_rota {0}; index_rota < m; index_rota++){
						
						// double delta_min = delta_melhor_insercao(S, pedido, index_rota);
						
						std::vector<double> dados_melhor_insercao_rota = delta_melhor_insercao(S, pedido, index_rota,'A');
						
						double delta_rota = dados_melhor_insercao_rota.at(0);
						
						// Se houver uma posição de inserção factível para a rota (delta_min > 0, valor retornado pela função caso não tenha se achado posições factíveis)
						if (delta_rota > 0){
						//if (delta_min > 0){
							
							// delta_min_por_rota.push_back(delta_min);
							delta_min_por_rota.push_back(delta_rota);
							
							qtd_insercoes_factiveis_total += 1;
							
							qtd_insercoes_factiveis_pedido += 1;
							
							if (delta_rota < delta_min){
								
								delta_min = delta_rota;
								dados_melhor_insercao_pedido_min = dados_melhor_insercao_rota;
								
							}
							
						}
						
					}
					
					// Há dois casos possíveis: o pedido ter mais de " m - k + 1 " inserções factíveis considerando cada rota ou o oposto disso
					
					// Caso não haja ao menos " m - k + 1 " posições de inserção factíveis por rota para o pedido, deve-se ter controle do pedido correspondente e da quantidade de posições,
					// pois será escolhido aquele que tiver a quantidade mínima
					
					if (qtd_insercoes_factiveis_pedido < (m - k + 1)){
						
						// Avaliando se esse será o pedido com o mínimo de inserções factíveis da iteração
						
						// Caso o número de inserções seja igual ao mínimo já encontrado, atualizam-se os valores apenas se o delta for menor do que o já encontrado!
						if (qtd_insercoes_factiveis_pedido == qtd_insercoes_factiveis_pedido_min){
							
							// double delta_min = delta_melhor_insercao(S, pedido);
							
							if (delta_min < delta_min_pedido_min_insercoes){
								
								pedido_min_insercoes = pedido;
								delta_min_pedido_min_insercoes = delta_min;
								
								dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
							}
						
						// Caso o número de inserções seja menor do que o mínimo já encontrado, atualizam-se todos os valores!
						} else if (qtd_insercoes_factiveis_pedido < qtd_insercoes_factiveis_pedido_min){
							
							double delta_min = delta_melhor_insercao(S, pedido);
							
							delta_min_pedido_min_insercoes = delta_min;
							
							pedido_min_insercoes = pedido;
							
							qtd_insercoes_factiveis_pedido_min = qtd_insercoes_factiveis_pedido;
							
							dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
							
						}
						
						// std::cout << qtd_insercoes_factiveis_pedido_min << std::endl;
						
					// Caso contrário, o cálculo do regret value é realizado
					
					} else {
						
						// Só fará sentido continuar calculando regret values caso o pedido tenha tido mais do que "m - k + 1" posições de inserção factíveis!
						if (pedido_min_insercoes == 0){
							
							// Ordenando lista de deltas:
							sort(delta_min_por_rota.begin(), delta_min_por_rota.end());
							
							// Calculando regret value para o pedido:
							
							// Necessidade de calcular a posição a partir do valor mínimo entre "k-1" e o tamanho da lista de deltas
							
							double delta_len = delta_min_por_rota.size();
							
							int pos = std::min(delta_len - 1, k-1);
							
							double regret_k = delta_min_por_rota.at(pos) - delta_min_por_rota.at(0);
							
							
							if (regret_k >= max_regret){
								
								max_regret = regret_k;
								
								pedido_max_regret = pedido;
								
								dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
							}
						}
					}
				}
				
				// Inserções: avaliando se houve ao menos uma posição de inserção factível encontrada:
				
				if (qtd_insercoes_factiveis_total > 0){
					
					// Caso nenhum pedido tenha tido menos do que "m - k + 1" posições de inserção factíveis, insere-se o que possui o máximo regret value!
					if (pedido_min_insercoes == 0){
						
						//std::cout << pedido_max_regret << "\n\n" << std::endl;
						
						// S = melhor_insercao(S, pedido_max_regret);
						S.inserir_pedido(pedido_max_regret, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
						
						qtd_inseridos += 1;
					
					// Caso contrário, isso significará que deve-se optar pelo pedido com o mínimo de posições de inserção encontradas!
					} else {
						
						// S = melhor_insercao(S, pedido_min_insercoes);
						S.inserir_pedido(pedido_min_insercoes, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
						qtd_inseridos += 1;
						
					}
				
				}
				
				if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis_total == 0)){
						
						break;
				}
			}
			
			// Fim da heurística de regret insertion
			break;
			
			
		}
		
		default:
		std::cout << "Invalido" << std::endl;
		
	}
	
	
	// S.print_sol();
	
	auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	
	processing_time += elapsed.count() * 1e-9;
	
	
	return S;
	
}





