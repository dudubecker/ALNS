#include "Heuristic.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include "heuristicsFunctions.h"

//Heuristic::Heuristic()
//{
//}

Heuristic::~Heuristic()
{
}

/*
Sol Heuristic::apply(Sol &S){
	
    // std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas

    // Chamada para o método "specificApply" específico da classe derivada
    // Isso garantirá que o comportamento específico da classe derivada seja chamado após o código base.
	specificApply(S);
	
	return S;
	
}

//// RemovalHeuristic objects implementation

int RemovalHeuristic::initializeMethod(Sol &S) {
	
	// Valor mínimo de mi (10% da quantidade de pedidos)
	int low_mi = round(S.inst.n*0.1);
			
	// Valor máximo de mi (40% da quantidade de pedidos)
	int high_mi = round(S.inst.n*0.4);
			
	int mi = low_mi + rand()%(high_mi - low_mi + 1);
	
	return mi;
	
	
}

Sol RandomRemoval::specificApply(Sol &S){
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
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
	
}

Sol WorstRemoval::specificApply(Sol &S) {
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
	// Parâmetro "delta" para controle da aleatoriedade 
	const int delta {6};
	
	// Pedidos contidos na solução: complemento de L!
	std::vector<double> pedidos = S.A;
	
	// Variável com quantidade de pedidos removidos
	int n_pedidos_removidos {0};
	
	// Início do algoritmo
	while (n_pedidos_removidos < mi){
		
		// Custos incrementais de cada pedido (não-ordenados)
		std::vector<double> delta_custos {};
		
		for (auto pedido: pedidos){
			
			// Calculando variação no custo da rota com a remoção do pedido
			double delta = S.delta_FO_rem(pedido);
			
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
		
		n_pedidos_removidos += 1;
		
		
	}
	
	// Fim da heurística worst removal
	
}

Sol ShawsRemoval::specificApply(Sol &S) {
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
	// Parâmetro "delta" para controle da aleatoriedade 
	const int delta {6};
	
	// Obtendo tempos de visita (T_i) de cada nó "i" na solução S
	
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
	
}

//// InsertionHeuristic objects implementation

void InsertionHeuristic::initializeMethod() {
	
	;
	
}

Sol GreedyInsertion::specificApply(Sol &S) {
	
	// Initialization of insertion:
	
	InsertionHeuristic::initializeMethod();
	
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
			std::vector<double> dados_melhor_insercao = S.delta_melhor_insercao(pedido);
			
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
	
	
}

Sol RegretInsertion::specificApply(Sol &S) {
	
	// std::cout << "Solucao apos regret insertion" << std::endl;
			
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
				
				std::vector<double> dados_melhor_insercao_rota = S.delta_melhor_insercao(pedido, index_rota);
				
				double delta_rota = dados_melhor_insercao_rota.at(0);
				
				// Se houver uma posição de inserção factível para a rota (delta_min > 0, valor retornado pela função caso não tenha se achado posições factíveis)
				if (delta_rota > 0){
					
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
					
					if (delta_min < delta_min_pedido_min_insercoes){
						
						pedido_min_insercoes = pedido;
						delta_min_pedido_min_insercoes = delta_min;
						
						dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
					}
				
				// Caso o número de inserções seja menor do que o mínimo já encontrado, atualizam-se todos os valores!
				} else if (qtd_insercoes_factiveis_pedido < qtd_insercoes_factiveis_pedido_min){
					
					delta_min_pedido_min_insercoes = delta_min;
					
					pedido_min_insercoes = pedido;
					
					qtd_insercoes_factiveis_pedido_min = qtd_insercoes_factiveis_pedido;
					
					dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
					
				}
				
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
		
		
		// Tratando exceção: bug raro de quando não são retornados dados para melhor inserção. A operação, é, então, interrompida
		if (dados_melhor_insercao_min.size() == 0){
			
			break;
			
		}
		
		
		// Inserções: avaliando se houve ao menos uma posição de inserção factível encontrada:
		
		if (qtd_insercoes_factiveis_total > 0){
			
			// Caso nenhum pedido tenha tido menos do que "m - k + 1" posições de inserção factíveis, insere-se o que possui o máximo regret value!
			if (pedido_min_insercoes == 0){
				
				S.inserir_pedido(pedido_max_regret, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
				
				qtd_inseridos += 1;
			
			// Caso contrário, isso significará que deve-se optar pelo pedido com o mínimo de posições de inserção encontradas!
			} else {
				
				S.inserir_pedido(pedido_min_insercoes, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
				qtd_inseridos += 1;
				
			}
		
		}
		
		if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis_total == 0)){
				
				break;
		}
	}
	
	
	
	for (auto &pedido: S.L){
		
		// S = melhor_insercao(S, pedido);
		S.executar_melhor_insercao(pedido);
		
	}
	
	// Fim da heurística de regret insertion
	
}

*/


// Método principal das heurísticas

// Método de aplicação das heurísticas
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol Heuristic::apply(Sol &S){
	
	// Medindo tempo
	auto begin = std::chrono::high_resolution_clock::now();
	
	// Valor mínimo de mi (10% da quantidade de pedidos)
	int low_mi = round(S.inst.n*0.1);
			
	// Valor máximo de mi (40% da quantidade de pedidos)
	int high_mi = round(S.inst.n*0.4);
			
	int mi = low_mi + rand()%(high_mi - low_mi + 1);
	
	switch (name){
		
		// Random removal
		case 'R':{
			
			// std::cout << "Solucao apos random removal" << std::endl;
			
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
				
				// std::cout << "\nPedido: " << pedido << std::endl;
				
				// S.print_sol();
				
				// double a;
				
				// std::cin >> a;
				
				S.remover_pedido(pedido);
				
			}
			
			
			
			// Fim da heurística random removal
			break;
		}
		
		// Worst removal
		case 'W':{
			
			// std::cout << "Solucao apos worst removal" << std::endl;
			
			// srand(time(NULL));
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			// Pedidos contidos na solução: complemento de L!
			std::vector<double> pedidos = S.A;
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Início do algoritmo
			while (n_pedidos_removidos < mi){
				
				// Custos incrementais de cada pedido (não-ordenados)
				std::vector<double> delta_custos {};
				
				for (auto pedido: pedidos){
					
					// Calculando variação no custo da rota com a remoção do pedido
					double delta = S.delta_FO_rem(pedido);
					
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
				
				n_pedidos_removidos += 1;
				
				
			}
			
			// Fim da heurística worst removal
			break;
		}
		
		// Shaws removal
		case 'S':{
			
			// std::cout << "Solucao apos shaws removal" << std::endl;
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			// Obtendo tempos de visita (T_i) de cada nó "i" na solução S
			
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
			
			// std::cout << "Solucao apos greedy insertion" << std::endl;
			
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
					std::vector<double> dados_melhor_insercao = S.delta_melhor_insercao(pedido);
					
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
			
			// std::cout << "Solucao apos regret insertion" << std::endl;
			
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
				
				std::vector<double> pedidos_nao_atendidos = S.L;
				
				
				for (auto &pedido: pedidos_nao_atendidos){
					
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
						
						std::vector<double> dados_melhor_insercao_rota = S.delta_melhor_insercao(pedido, index_rota);
						
						double delta_rota = dados_melhor_insercao_rota.at(0);
						
						// Se houver uma posição de inserção factível para a rota (delta_min > 0, valor retornado pela função caso não tenha se achado posições factíveis)
						if (delta_rota > 0){
							
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
							
							if (delta_min < delta_min_pedido_min_insercoes){
								
								pedido_min_insercoes = pedido;
								delta_min_pedido_min_insercoes = delta_min;
								
								dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
							}
						
						// Caso o número de inserções seja menor do que o mínimo já encontrado, atualizam-se todos os valores!
						} else if (qtd_insercoes_factiveis_pedido < qtd_insercoes_factiveis_pedido_min){
							
							delta_min_pedido_min_insercoes = delta_min;
							
							pedido_min_insercoes = pedido;
							
							qtd_insercoes_factiveis_pedido_min = qtd_insercoes_factiveis_pedido;
							
							dados_melhor_insercao_min = dados_melhor_insercao_pedido_min;
							
						}
						
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
				
				
				// Tratando exceção: bug raro de quando não são retornados dados para melhor inserção. A operação, é, então, interrompida
				if (dados_melhor_insercao_min.size() == 0){
					
					break;
					
				}
				
				
				// Inserções: avaliando se houve ao menos uma posição de inserção factível encontrada:
				
				if (qtd_insercoes_factiveis_total > 0){
					
					// Caso nenhum pedido tenha tido menos do que "m - k + 1" posições de inserção factíveis, insere-se o que possui o máximo regret value!
					if (pedido_min_insercoes == 0){
						
						S.inserir_pedido(pedido_max_regret, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
						
						qtd_inseridos += 1;
					
					// Caso contrário, isso significará que deve-se optar pelo pedido com o mínimo de posições de inserção encontradas!
					} else {
						
						S.inserir_pedido(pedido_min_insercoes, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
						qtd_inseridos += 1;
						
					}
				
				}
				
				if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis_total == 0)){
						
						break;
				}
			}
			
			std::vector<double> pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido);
				
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




