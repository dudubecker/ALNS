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
	temperatura = (S_i.calcularFO())*((0.3)/log(0.5));
	
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
	
	heuristicas_remocao = {H_r, H_s, H_w, H_w_random, H_s_TTR, H_s_STR, H_s_DER};
	qtd_heuristicas_remocao = heuristicas_remocao.size();
	
	// Greedy Insertion
	
		// Sem random noise
	GreedyInsertion* H_g = new GreedyInsertion(0.0);
	
		// Com random noise
	GreedyInsertion* H_g_random = new GreedyInsertion(eta);
	
	heuristicas_insercao = {H_g, H_g_random};
	
	// Regret insertion - k vai ser limitado por 3
	
	int numero_de_rotas = S_inicial.rotas.size();
	
	// Regret só faz sentido para soluções com mais de 1 rota!
	
	if (numero_de_rotas > 1){
		
		int max_k = std::min(4, numero_de_rotas - 1);
		
		for (int k = 1; k <= max_k; k++){
			
			RegretInsertion* H_a = new RegretInsertion(k, 0);
			heuristicas_insercao.push_back(H_a);
			
			RegretInsertion* H_a_random = new RegretInsertion(k, eta);
			heuristicas_insercao.push_back(H_a_random);
			
		}
	}
	
	qtd_heuristicas_insercao = heuristicas_insercao.size();
	
}

// Destructor
ALNS::~ALNS()
{
}

// Método para atualização de pesos, realizado no início de cada segmento
void ALNS::atualizarPesos(){
	
	// Caso seja a primeira atualização dos pesos, as heurísticas terão o mesmo peso
	// A soma dos pesos inicial será arbitrariamente igual a 100, e cada heurística terá um peso igual a 100 dividido pelo número de heurísticas
	
	if (soma_peso_rem == 0){
		
		double novo_peso_remocao = (double) 100/(heuristicas_remocao.size());
		
		double novo_peso_insercao = (double) 100/(heuristicas_insercao.size());
		
		// Atualizando pesos para as heurísticas de remoção:
		for (auto &heuristic: heuristicas_remocao){
			
			heuristic->peso = novo_peso_remocao;
			
		}
		
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: heuristicas_insercao){
			
			heuristic->peso = novo_peso_insercao;
			
		}
		
		
		soma_peso_rem = 100;
		soma_peso_ins = 100;
		
	// Caso contrário, a atualização levará em consideração os pesos pré-estabelecidos, scores e número de iterações de cada heurística
	} else {
		
		// Variáveis que armazenarão a nova soma de pesos:
		
		double nova_soma_peso_rem {};
		
		double nova_soma_peso_ins {};
		
		// Atualizando pesos para as heurísticas de remoção:
		for (auto &heuristic: heuristicas_remocao){
			
			// Atualizando peso
			heuristic->peso = heuristic->peso*(1-r) + r*((heuristic->score)/(heuristic->n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_peso_rem += heuristic->peso;
			
		}
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: heuristicas_insercao){
			
			heuristic->peso = heuristic->peso*(1-r) + r*((heuristic->score)/(heuristic->n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_peso_ins += heuristic->peso;
		}
		
		// Atualizando valores de soma de pesos:
		soma_peso_rem = nova_soma_peso_rem;
		
		soma_peso_ins = nova_soma_peso_ins;
	
	}
}

// Método para critério de aceitação:
bool ALNS::avaliarCriterioDeAceitacao(Sol &S){
	
	// Variável para retornar resultado do critério de aceitação
	bool CA = true;
	
	// Caso todos os pedidos tenham sido atendidos, o cálculo é feito (Gasque)
	if (S.L_size == 0){
		
		// Calculando probabilidade de aceitação:
		
		// Valor "e":
		const double e = std::exp(1.0);
		
		// Valor do expoente:
		double expoente = (double) (S.calcularFO() - S_i.calcularFO())/temperatura;
		
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
	temperatura = temperatura*c;
	
	return CA;
	
}

// Método para escolher heurísticas: retorna o índice de uma heurística ou a própria heurística?
int ALNS::escolherHeuristica(char type){
	
	// Vetor de heurísticas (inicialmente vazio)
	std::vector<Heuristic*> heuristicas {};
	
	// Soma dos pesos das heurísticas 
	double soma_peso {};
	
	// Caso se queira escolher uma heurística de remoção
	if (type == 'R'){
		
		heuristicas = heuristicas_remocao;
		soma_peso = soma_peso_rem;
		
	// Caso se queira escolher uma heurística de inserção
	} else {
		
		heuristicas = heuristicas_insercao;
		soma_peso = soma_peso_ins;
		
	}
	
	
	// Probabilidade acumulada
	double prob_acumulada {0};
	
	// Valor aleatório gerado entre 0 e 1:
	double random_number = {};
	random_number = ((double) std::rand() / (RAND_MAX));
	
	// Contador para determinar o índice da heurística
	int index_heuristica {0};
	
	// Determinando índice da heurística de remoção:
	for (Heuristic* &heuristic: heuristicas){
		
		// Probabilidade de escolha da heurística:
		double prob_heuristica = heuristic->peso/soma_peso;
		
		prob_acumulada += prob_heuristica;
		
		// Checar se o número aleatório é maior do que a probabilidade acumulada
		if (random_number > prob_acumulada){
			
			// incrementar probabilidade acumulada com probabilidade de escolha da heurística
			index_heuristica += 1;
			
		} else {
			
			break;
			
		}
		
	}
	
	// Corrigindo bug muito raro de escolher um índice acima do tamanho do vetor
	if ((type == 'R') and (index_heuristica == qtd_heuristicas_remocao)){
		
		index_heuristica -= 1;
		
	} else if ((type == 'I') and (index_heuristica == qtd_heuristicas_insercao)) {
		
		index_heuristica -= 1;
		
	}
	
	
	return index_heuristica;
	
}

// Método para atualização de pontuações: necessário saber se a heurística passou pelo critério de aceitação, valor da melhor solução global, solução incumbente e solução recém-alterada (S);
void ALNS::atualizarPontuacoes(bool &CA, bool &BKS, int &index_h_rem, int &index_h_ins, double &FO){
	
	if (BKS){
		
		heuristicas_remocao[index_h_rem]->score += sigma_1;
		heuristicas_insercao[index_h_ins]->score += sigma_1;
		
	} else {
		
		
		if ((CA) && (!count(solucoes_encontradas.begin(), solucoes_encontradas.end(), FO))){
			
			if (FO < S_i.calcularFO()){
				
				heuristicas_remocao[index_h_rem]->score += sigma_2;
				heuristicas_insercao[index_h_ins]->score += sigma_2;
				
			}else{
				
				heuristicas_remocao[index_h_rem]->score += sigma_3;
				heuristicas_insercao[index_h_ins]->score += sigma_3;
				
			}
			
		}
	}
	
	// Incrementando a quantidade de iterações da heurística no segmento:
	
	heuristicas_remocao[index_h_rem]->n_it += 1;
	heuristicas_insercao[index_h_ins]->n_it += 1;
	
	// Incrementando a quantidade total de iterações da heurística (debug):
	
	heuristicas_remocao[index_h_rem]->n_it_total += 1;
	heuristicas_insercao[index_h_ins]->n_it_total += 1;
	
}

// Método para zerar pontuações
void ALNS::zerarPontuacoes(){
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de remoção:
	
	for (auto &heuristic: heuristicas_remocao){
		
		heuristic->score = 0;
		heuristic->n_it = 0;
		
	}
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de inserção:
	
	for (auto &heuristic: heuristicas_insercao){
		
		heuristic->score = 0;
		heuristic->n_it = 0;
		
	}
}

// Método de inicialização para redução de rotas
Sol ALNS::reduzirRotas(Sol &S_i, int it_RRH){
	
	// Criando uma cópia da rota:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	Sol BKS = S_i;
	
	// Variável para o número de iterações:
	int n_it {0};
	
	while (n_it < it_RRH){
		
		//if (n_it%5 == 0){
			
		//	std::cout << "Iteracao RR: " << n_it << std::endl;
			
		//}
		
		// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
		if (S.L_size == 0){
			
			BKS = S;
			
			// Quantidade "m" de rotas na solução:
			int m = S.rotas.size();
			
			// Escolhendo índice da rota que será removida
			double index_rota = rand()%(m);
			
			// Removendo rota
			S.removerRota(index_rota);
			
		}
		
		// Escolhendo heurísticas e aplicando-as à solução
		
		int index_h_rem = escolherHeuristica('R');
		int index_h_ins = escolherHeuristica('I');
		
		heuristicas_remocao.at(index_h_rem)->aplicar(S);
		heuristicas_insercao.at(index_h_ins)->aplicar(S);
		
		n_it += 1;
		
	}
	
	return BKS;
}


// Método do algoritmo em si
void ALNS::executarALNS(int max_it, int max_it_sem_melhoria, int it_RRH, double max_t){
	
	// Código para redução de rotas
	
	if (S_i.rotas.size() > 1){
		
		S_i = reduzirRotas(S_i, it_RRH);
		
	}
	
	// Código principal
	
	// Variável para o número de iterações:
	int n_it {};
	
	// Variável para o tempo de execução
	double t_ALNS {0};
	
	// Variável para o número de iterações sem melhoria:
	int it_sem_melhoria = 0;
	
	while ((n_it < max_it) && (t_ALNS < max_t)){
		
		//if (n_it%100 == 0){
			
		//	std::cout << "Iteracao ALNS: " << n_it << std::endl;
			
		//}
		
		// Medindo tempo
		auto begin = std::chrono::high_resolution_clock::now();
		
		// Interrompendo execução no caso de o algoritmo ter chegado no número máximo de iterações sem melhoria
		
		if (it_sem_melhoria == max_it_sem_melhoria){
			
			break;
			
		}
		
		// Modificando a solução incumbente
		Sol S = S_i;
		
		// Atualizando pesos, scores e número de iterações para cada heurística no início de cada segmento (100 iterações)
		if (n_it % 100 == 0){
			
			atualizarPesos();
			zerarPontuacoes();
			
		}
		
		
		// Escolhendo e aplicando heurísticas
		
		int index_h_rem = escolherHeuristica('R');
		int index_h_ins = escolherHeuristica('I');
		
		heuristicas_remocao.at(index_h_rem)->aplicar(S);
		heuristicas_insercao.at(index_h_ins)->aplicar(S);
		
		
		// Analisando se a solução é a melhor já encontrada e armazenando seu valor na região de soluções encontradas
		
		// Variável para armazenar função objetivo
		double FO = (double) S.calcularFO();
		
		bool BKS = false;
		
		if (FO < S_p.calcularFO()){
			
			if ((S.L_size == 0)){
				
				S_p = S;
				BKS = true;
				it_sem_melhoria = 0;
				
			} else {
			
				it_sem_melhoria += 1;
			
			}
			
		}
		
		// Avaliando se a solução passa pelo critério de aceitação
		
		bool CA = avaliarCriterioDeAceitacao(S);
		
		// Atualizando pontuações das respectivas heurísticas, de acordo com os resultados da iteração
		atualizarPontuacoes(CA, BKS, index_h_rem, index_h_ins, FO);
		
		if (CA){
			
			S_i = S;
			
		}
		
		// Armazenando função objetivo nas soluções já encontradas, para ter o controle delas
		solucoes_encontradas.push_back(FO);
		
		n_it += 1;
		
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
		
		t_ALNS += elapsed.count() * 1e-9;
		
	}
}
