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
	
	// Adição: penalizando número de pedidos não atendidos:
	for (auto &pedido: L){
		
		custos_totais += 10000;
		
	}
	
	
	return (double) custos_totais;
}

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

double Sol::delta_FO_ins(double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Objeto para a rota que potencialmente sofrerá a inserção
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		
		double delta = (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(no_delivery)]) + (inst.t[(no_delivery)][(Rotas[index_rota][pos_no_delivery - 1])]) - (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(Rotas[index_rota][pos_no_delivery - 1])]);
		
		return delta;
		
	}else{
		
		// Variação pela inserção do nó de pickup
		double delta_pickup = (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(Rotas[index_rota][pos_no_pickup])]) - (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(Rotas[index_rota][pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		double delta_delivery = (inst.t[(Rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (inst.t[(no_delivery)][(Rotas[index_rota][pos_no_delivery - 2])]) - (inst.t[(Rotas[index_rota][pos_no_delivery - 1])][(Rotas[index_rota][pos_no_delivery - 2])]);
		
		// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

double Sol::delta_FO_rem(double &pedido){
	
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Achando índice da rota que contém o pedido
	int index_rota {0};
	
	for (auto &rota: Rotas){
		
		if (count(rota.begin(), rota.end(), no_pickup)){
			
			break;
			
		}else{
			index_rota += 1;
		}
		
	}
	
	
	// Índice (posição) do nó de pickup na rota
	double pos_no_pickup = std::find(Rotas[index_rota].begin(),Rotas[index_rota].end(), no_pickup) - Rotas[index_rota].begin();
	
	// Índice (posição) do nó de delivery na rota
	double pos_no_delivery = std::find(Rotas[index_rota].begin(),Rotas[index_rota].end(), no_delivery) - Rotas[index_rota].begin();
	
	
	// Calculando decréscimo nos custos
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		
		double delta = (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(no_delivery)]) + (inst.t[(no_delivery)][(Rotas[index_rota][pos_no_delivery + 1])]) - (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(Rotas[index_rota][pos_no_delivery + 1])]);
		
		return delta;
		
	}else{
		
		
		//// Variação pela inserção do nó de pickup
		double delta_pickup = (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(Rotas[index_rota][pos_no_pickup + 1])]) - (inst.t[(Rotas[index_rota][pos_no_pickup - 1])][(Rotas[index_rota][pos_no_pickup + 1])]);
		
		//// Variação pela inserção do nó de delivery
		double delta_delivery = (inst.t[(Rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (inst.t[(no_delivery)][(Rotas[index_rota][pos_no_delivery + 1])]) - (inst.t[(Rotas[index_rota][pos_no_delivery - 1])][(Rotas[index_rota][pos_no_delivery + 1])]);
		
		//// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

bool Sol::isInsertionFeasible(double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Capacidade atual do veículo na rota (inicia como 0)
	double cap_atual {0};
	
	// Tempo atual da rota (inicia como 0)
	double t_atual {0};
	
	for (auto index_no {0}; index_no < Rotas.at(index_rota).size() + 1; index_no++){
		
		// Variável que guarda o nó atual considerado na checagem de factibilidade
		int no_atual {};
		
		// Variável que guarda o nó seguinte considerado na checagem de factibilidade
		int no_seguinte {};
		
		// Se as posições de inserção são consecutivas (pos_no_delivery = pos_no_pickup + 1)
		if (pos_no_delivery == pos_no_pickup + 1){
			
			if (index_no < pos_no_pickup - 1){
				
				no_atual = Rotas.at(index_rota).at(index_no);
				
				no_seguinte = Rotas.at(index_rota).at(index_no + 1);
				
			} else if (index_no == pos_no_pickup - 1){
				
				no_atual = Rotas.at(index_rota).at(index_no);
				
				no_seguinte = no_pickup;
				
			} else if (index_no == pos_no_pickup){
				
				no_atual = no_pickup;
				
				no_seguinte = no_delivery;
				
			} else if (index_no == pos_no_pickup + 1){
				
				no_atual = no_delivery;
				
				no_seguinte = Rotas.at(index_rota).at(index_no - 1);
				
				
			} else {
				
				no_atual = Rotas.at(index_rota).at(index_no - 2);
				
				no_seguinte = Rotas.at(index_rota).at(index_no - 1);
				
			}
			
			
			
		// Se as posições de inserção não são consecutivas (pos_no_delivery > pos_no_pickup + 1)
		} else if (pos_no_delivery > pos_no_pickup + 1){
			
			if (index_no < pos_no_pickup - 1){
				
				no_atual = Rotas.at(index_rota).at(index_no);
				
				no_seguinte = Rotas.at(index_rota).at(index_no + 1);
				
			} else if (index_no == pos_no_pickup - 1){
				
				no_atual = Rotas.at(index_rota).at(index_no);
				
				no_seguinte = no_pickup;
				
			} else if (index_no == pos_no_pickup){
				
				no_atual = no_pickup;
				
				no_seguinte = Rotas.at(index_rota).at(index_no);
				
			} else if ((index_no > pos_no_pickup) && (index_no < pos_no_delivery - 1)){
				
				no_atual = Rotas.at(index_rota).at(index_no - 1);
				
				no_seguinte = Rotas.at(index_rota).at(index_no);
				
			} else if (index_no == pos_no_delivery - 1){
				
				no_atual = Rotas.at(index_rota).at(index_no - 1);
				
				no_seguinte = no_delivery;
				
			} else if (index_no == pos_no_delivery){
				
				no_atual = no_delivery;
				
				no_seguinte = Rotas.at(index_rota).at(index_no - 1);
				
				
			} else {
				
				no_atual = Rotas.at(index_rota).at(index_no - 2);
				
				no_seguinte = Rotas.at(index_rota).at(index_no - 1);
				
			}
			
			
		}
		
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
	
	return factivel;
	
}

std::vector<double> Sol::delta_melhor_insercao(double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
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
	for (auto index_rota {0}; index_rota < Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
					// Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					// S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					// if (S_teste.isFeasible(index_rota)){
					if (isInsertionFeasible(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
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
		
	}
	
	return return_vector;
	
	
}

std::vector<double> Sol::delta_melhor_insercao(double &pedido, int &index_rota){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
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
		
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
				// Sol S_teste = S;
				
				// Inserindo nós na rota, nas posições da iteração
				// S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
				
				// if (S_teste.isFeasible(index_rota)){
				if (isInsertionFeasible(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
					
					num_rotas_factiveis += 1;
					
					// double delta_S = S.delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					double delta_S = delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
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
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
		
	} else {
		
		// Possível problema em retornar um valor grande: se não for factível, como busca-se maximizar (regret), vai dar problema!
		
		return_vector = {0, 0, 0,0};
		
	}
	
	return return_vector;
	
}


void Sol::executar_melhor_insercao(double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
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
	
	// Avaliando inserções
	for (auto index_rota {0}; index_rota < Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção
					// Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					// S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					//if (S_teste.isFeasible(index_rota)){
					if (isInsertionFeasible(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
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
		
		inserir_pedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
}



