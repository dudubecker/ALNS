#include "ALNS.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <cmath>
#include <ctime>
#include <time.h>
#include <chrono>


// Inicialização do objeto ALNS

ALNS::ALNS(Sol S_inicial,
		double w_value, double c_value, double sigma_1_value, double sigma_2_value,
		double sigma_3_value, double r_value, double eta_value, double delta_value){
	
	
	// Atribuindo valores aos atributos da ALNS
	w = w_value;
	c = c_value;
	sigma_1 = sigma_1_value;
	sigma_2 = sigma_2_value;
	sigma_3 = sigma_3_value;
	r = r_value;
	
	// Atribuindo valores aos atributos das heurísticas (eta e delta)
	eta = eta_value;
	delta = delta_value;
	
	// Definindo soluções incumbente e melhor solução como a solução construída inicialmente
	S_p = S_inicial;
	
	S_i = S_inicial;
	
	// Definindo teomperatura inicial:
	Temperature = (S_i.FO())*((0.3)/log(0.5));
	
	// Inicializando heurísticas
	
	// Random Removal
	
	RandomRemoval* H_r = new RandomRemoval;
	
	// Shaws Removal e derivadas
	
	ShawsRemoval* H_s = new ShawsRemoval(0.3, 0.4, 0.3, delta);
	
	ShawsRemoval* H_s_TTR = new ShawsRemoval(1, 0, 0, delta);
	
	ShawsRemoval* H_s_STR = new ShawsRemoval(0, 1, 0, delta);
	
	ShawsRemoval* H_s_DER = new ShawsRemoval(0, 0, 1, delta);
	
	// Worst Removal
	
		// Sem random noise
	WorstRemoval* H_w = new WorstRemoval(0, delta);
	
		// Com random noise
	WorstRemoval* H_w_random = new WorstRemoval(eta, delta);
	
	// Populando vetores de heurísticas de remoção
	
	removal_heuristics = {H_r, H_s, H_w, H_w_random, H_s_TTR, H_s_STR, H_s_DER};
	
	// Greedy Insertion
	
		// Sem random noise
	GreedyInsertion* H_g = new GreedyInsertion(0.0);
	
		// Com random noise
	GreedyInsertion* H_g_random = new GreedyInsertion(eta);
	
	insertion_heuristics = {H_g, H_g_random};
	
	// Regret insertion - k vai ser igual ao número de rotas da solução construída subtraído de 2 unidades
	// Isso porque não lembro de haver, em nenhuma instância, a remoção de mais de uma rota com a heurística de remoção de rotas!
	
	int numero_de_rotas = S_inicial.Rotas.size();
	
	int max_k = std::max(1, numero_de_rotas - 2);
	
	for (int k = 1; k <= max_k; k++){
		
		RegretInsertion* H_a = new RegretInsertion(k, 0);
		insertion_heuristics.push_back(H_a);
		
		RegretInsertion* H_a_random = new RegretInsertion(k, eta);
		insertion_heuristics.push_back(H_a_random);
		
	}
	
}


ALNS::ALNS()
{
}

ALNS::~ALNS()
{
}

// Método para atualização de pesos, realizado no início de cada segmento



void ALNS::atualizar_pesos(){
	
	// Caso seja a primeira atualização dos pesos, as heurísticas terão o mesmo peso
	// A soma dos pesos inicial será arbitrariamente igual a 100, e cada heurística terá um peso igual a 100 dividido pelo número de heurísticas
	
	if (soma_W_rem == 0){
		
		double novo_W_removal = (double) 100/(removal_heuristics.size());
		
		double novo_W_insertion = (double) 100/(insertion_heuristics.size());
		
		// Atualizando pesos para as heurísticas de remoção:
		for (auto &heuristic: removal_heuristics){
			
			heuristic->weight = novo_W_removal;
			
		}
		
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: insertion_heuristics){
			
			heuristic->weight = novo_W_insertion;
			
		}
		
		soma_W_rem = 100;
		soma_W_ins = 100;
		
	// Caso contrário, a atualização levará em consideração os pesos pré-estabelecidos, scores e número de iterações de cada heurística
	} else {
		
		// Variáveis que armazenarão a nova soma de pesos:
		
		double nova_soma_W_rem {};
		
		double nova_soma_W_ins {};
		
		// Atualizando pesos para as heurísticas de remoção:
		for (auto &heuristic: removal_heuristics){
			
			// Atualizando peso
			heuristic->weight = heuristic->weight*(1-r) + r*((heuristic->score)/(heuristic->n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_W_rem += heuristic->weight;
			
		}
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: insertion_heuristics){
			
			heuristic->weight = heuristic->weight*(1-r) + r*((heuristic->score)/(heuristic->n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_W_ins += heuristic->weight;
		}
		
		// Atualizando valores de soma de pesos:
		soma_W_rem = nova_soma_W_rem;
		
		soma_W_ins = nova_soma_W_ins;
	
	}
	
}



// Método para critério de aceitação:

bool ALNS::criterio_aceitacao(Sol &S){
	
	// Para gerar números aleatórios:
	// srand(time(NULL));
	
	// Variável para retornar resultado do critério de aceitação
	bool CA = true;
	
	// Caso todos os pedidos tenham sido atendidos, o cálculo é feito (Gasque)
	if (S.L.size() == 0){
		
		// Calculando probabilidade de aceitação:
		
		// Valor "e":
		const double e = std::exp(1.0);
		
		// Valor do expoente:
		double expoente = (double) (S.FO() - S_i.FO())/Temperature;
		
		// Cálculo da probabilidade
		double prob = pow(e, (-1)*expoente);
		
		// Gerando número aleatório entre 0 e 1:
		double random_number = {};
		random_number = ((double) std::rand() / (RAND_MAX));
		
		// Caso o número gerado seja maior que a probabilidade, o critério de aceitação não é aceito!
		if (random_number > prob){
			
			CA = false;
			
		}
		
	// Caso haja algum pedido faltante, a solução não é aceita
	} else {
		
		CA = false;
		
	}
	
	
	// Atualizando valor da temperatura
	Temperature = Temperature*c;
	
	return CA;
	
}



	// Método para escolher heurísticas: retorna o índice de uma heurística ou a própria heurística?
	
	// OBS: ver um jeito melhor para não criar código redundante! Possivelmente passar o vetor de heurísticas como um argumento
	// Possível melhoria: identificar inserção e remoção não por meio de atributos diferentes, mas por meio de um atributo do próprio objeto heurística!
	
int ALNS::escolher_heuristica(char type){
	
	// Vetor de heurísticas (inicialmente vazio)
	std::vector<Heuristic*> heuristics_vector {};
	
	// Soma dos pesos das heurísticas 
	double soma_W {};
	
	// Caso se queira escolher uma heurística de remoção
	if (type == 'R'){
		
		heuristics_vector = removal_heuristics;
		soma_W = soma_W_rem;
		
	// Caso se queira escolher uma heurística de inserção
	} else {
		
		heuristics_vector = insertion_heuristics;
		soma_W = soma_W_ins;
		
	}
	
	
	// Probabilidade acumulada
	double prob_acumulada {0};
	
	// Valor aleatório gerado entre 0 e 1:
	double random_number = {};
	random_number = ((double) std::rand() / (RAND_MAX));
	
	// Contador para determinar o índice da heurística
	int index_heuristica {0};
	
	// Determinando índice da heurística de remoção:
	for (Heuristic* &heuristic: heuristics_vector){
		
		// Probabilidade de escolha da heurística:
		double prob_heuristica = heuristic->weight/soma_W;
		
		prob_acumulada += prob_heuristica;
		
		// Checar se o número aleatório é maior do que a probabilidade acumulada
		if (random_number > prob_acumulada){
			
			// incrementar probabilidade acumulada com probabilidade de escolha da heurística
			index_heuristica += 1;
			
		} else {
			
			break;
			
		}
		
	}
	
	return index_heuristica;
	
}


void ALNS::atualizar_pontuacoes(bool &CA, bool &BKS,int &index_h_rem, int &index_h_ins, double &FO){
	
	//std::cout << FO << std::endl;
	
	if (BKS){
		
		removal_heuristics[index_h_rem]->score += sigma_1;
		insertion_heuristics[index_h_ins]->score += sigma_1;
		
		//std::cout << "Case 1" << std::endl;
		
	} else {
		
		
		if ((CA) && (!count(S_encontradas.begin(), S_encontradas.end(), FO))){
			
			if (FO < S_i.FO()){
				
				removal_heuristics[index_h_rem]->score += sigma_2;
				insertion_heuristics[index_h_ins]->score += sigma_2;
				
				//std::cout << "Case 2" << std::endl;
				
			}else{
				
				removal_heuristics[index_h_rem]->score += sigma_3;
				insertion_heuristics[index_h_ins]->score += sigma_3;
				
				//std::cout << "Case 3" << std::endl;
				
			}
			
		}
	}
	// Incrementando a quantidade de iterações da heurística no segmento:
	
	removal_heuristics[index_h_rem]->n_it += 1;
	insertion_heuristics[index_h_ins]->n_it += 1;
	
	// Incrementando a quantidade total de iterações da heurística (debug):
	
	removal_heuristics[index_h_rem]->n_it_total += 1;
	insertion_heuristics[index_h_ins]->n_it_total += 1;
	
	
}



void ALNS::zerar_pontuacoes(){
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de remoção:
	
	for (auto &heuristic: removal_heuristics){
		
		heuristic->score = 0;
		heuristic->n_it = 0;
		
	}
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de inserção:
	
	for (auto &heuristic: insertion_heuristics){
		
		heuristic->score = 0;
		heuristic->n_it = 0;
		
	}
	
}



Sol ALNS::routeReductionHeuristic(Sol &S_i, int max_it_RRH){
	
	// Criando uma cópia da rota:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	Sol BKS = S_i;
	
	// Para gerar números aleatórios
	// srand(time(NULL));
	
	// Variável para o número de iterações:
	int n_it {0};
	
	while (n_it < max_it_RRH){
		
		if (n_it%100 == 0){
			
		//std::cout << "Iteracao RR: " << n_it << std::endl;
			;
		}
		
		
		// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
		if (S.L.size() == 0){
			
			BKS = S;
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que será removida
			double index_rota = rand()%(m);
			
			// Removendo rota
			S.remover_rota(index_rota);
			
		}
		
		// Escolhendo heurísticas e aplicando-as à solução
		
		int index_h_rem = escolher_heuristica('R');
		int index_h_ins = escolher_heuristica('I');
		
		removal_heuristics.at(index_h_rem)->apply(S);
		
		insertion_heuristics.at(index_h_ins)->apply(S);
		
		// S.print_sol();
		
		n_it += 1;
		
	}
	
	return BKS;
	
}


// Método do algoritmo em si

void ALNS::algo(int max_it_ALNS, int max_it_RRH, double max_t_ALNS){
	
	S_i = routeReductionHeuristic(S_i, max_it_RRH);
	
	// S_i.print_sol();
	
	// Variável para o número de iterações:
	int n_it {};
	
	// Variável para o tempo de execução
	double t_ALNS {0};
	
	while ((n_it < max_it_ALNS) && (t_ALNS < max_t_ALNS)){
		
		// std::cout << t_ALNS << std::endl;
		
		if (n_it%100 == 0){
			
			// std::cout << "Iteracao ALNS: " << n_it << std::endl;
			;
		}
		
		// Medindo tempo
		auto begin = std::chrono::high_resolution_clock::now();
		
		// std::cout << "Iteracao " << n_it << " da ALNS" << std::endl;
		
		// Modificando a solução incumbente
		Sol S = S_i;
		
		// Atualizando pesos, scores e número de iterações para cada heurística no início de cada segmento
		if (n_it % 100 == 0){
			
			atualizar_pesos();
			zerar_pontuacoes();
			
		}
		
		// Escolhendo e aplicando heurísticas
		
		int index_h_rem = escolher_heuristica('R');
		int index_h_ins = escolher_heuristica('I');
		
		removal_heuristics.at(index_h_rem)->apply(S);
		
		insertion_heuristics.at(index_h_ins)->apply(S);
		
		// Adição -> para remover rotas vazias
		if (S.LSize == 0){
			
			for (int index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
				
				if (S.RotasSize.at(index_rota) == 2){
					
					S.remover_rota(index_rota);
					
				}
				
				
			}
			
		}
		
		
		// Analisando se a solução é a melhor já encontrada e armazenando seu valor na região de soluções encontradas
		
		// Variável para armazenar função objetivo
		double FO = (double) S.FO();
		
		// std::cout << "FO: " << FO << "\n\n";
		
		bool BKS = false;
		
		if (FO < S_p.FO()){
			
			if (S.L.size() == 0){
				
				S_p = S;
			}
			
			S_p = S;
			BKS = true;
			
		}
		
		// Avaliando se a solução passa pelo critério de aceitação
		
		bool CA = criterio_aceitacao(S);
		
		// Atualizando pontuações das respectivas heurísticas, de acordo com os resultados da iteração
		atualizar_pontuacoes(CA, BKS, index_h_rem, index_h_ins, FO);
		
		if (CA){
			
			S_i = S;
			
		}
		
		// Armazenando função objetivo nas soluções já encontradas, para ter o controle delas
		S_encontradas.push_back(FO);
		
		n_it += 1;
		
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		
		t_ALNS += elapsed.count() * 1e-9;
		
	}
	
}
/*
*/