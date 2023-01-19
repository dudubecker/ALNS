#include <ALNS.hpp>
#include <Sol.hpp>
#include <Heuristic.hpp>
#include <cmath>
#include <ctime>
#include <time.h>

//ALNS::ALNS()
//{
//}

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
			
			heuristic.weight = novo_W_removal;
			
		}
		
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: insertion_heuristics){
			
			heuristic.weight = novo_W_insertion;
			
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
			heuristic.weight = heuristic.weight*(1-r) + r*((heuristic.score)/(heuristic.n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_W_rem += heuristic.weight;
			
		}
		// Atualizando pesos para as heurísticas de inserção:
		for (auto &heuristic: insertion_heuristics){
			
			heuristic.weight = heuristic.weight*(1-r) + r*((heuristic.score)/(heuristic.n_it));
			
			// Incrementando nova soma de pesos:
			nova_soma_W_ins += heuristic.weight;
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
	std::vector<Heuristic> heuristics_vector {};
	
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
	for (Heuristic &heuristic: heuristics_vector){
		
		// Probabilidade de escolha da heurística:
		double prob_heuristica = heuristic.weight/soma_W;
		
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
		
		removal_heuristics[index_h_rem].score += sigma_1;
		insertion_heuristics[index_h_ins].score += sigma_1;
		
		//std::cout << "Case 1" << std::endl;
		
	} else {
		
		
		if ((CA) && (!count(S_encontradas.begin(), S_encontradas.end(), FO))){
			
			if (FO < S_i.FO()){
				
				removal_heuristics[index_h_rem].score += sigma_2;
				insertion_heuristics[index_h_ins].score += sigma_2;
				
				//std::cout << "Case 2" << std::endl;
				
			}else{
				
				removal_heuristics[index_h_rem].score += sigma_3;
				insertion_heuristics[index_h_ins].score += sigma_3;
				
				//std::cout << "Case 3" << std::endl;
				
			}
			
		}
	}
	// Incrementando a quantidade de iterações da heurística no segmento:
	
	removal_heuristics[index_h_rem].n_it += 1;
	insertion_heuristics[index_h_ins].n_it += 1;
	
	// Incrementando a quantidade total de iterações da heurística (debug):
	
	removal_heuristics[index_h_rem].n_it_total += 1;
	insertion_heuristics[index_h_ins].n_it_total += 1;
	
	
}

void ALNS::zerar_pontuacoes(){
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de remoção:
	
	for (auto &heuristic: removal_heuristics){
		
		heuristic.score = 0;
		heuristic.n_it = 0;
		
	}
	
	// Zerando pontuações e número de iterações no segmento para heurísticas de inserção:
	
	for (auto &heuristic: insertion_heuristics){
		
		heuristic.score = 0;
		heuristic.n_it = 0;
		
	}
	
}

Sol ALNS::routeReductionHeuristic(Sol &S_i, int max_it_RRH){
	
	// Criando uma cópia da rota:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	Sol BKS = S_i;
	
	// Para gerar números aleatórios
	srand(time(NULL));
	
	// Variável para o número de iterações:
	int n_it {0};
	
	while (n_it < max_it_RRH){
		
		std::cout << "Iteracao " << n_it << " para remocao de rotas" << std::endl;
		
		// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
		if (S.L.size() == 0){
			
			BKS = S;
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que será removida
			double index_rota = rand()%(m);
			
			std::vector<double> Rota = S.Rotas.at(index_rota);
			
			// Removendo nós da solução
			for (auto &node: Rota){
				
				// Caso seja um nó correspondente a um pedido de pickup:
				if ((node > 0) && (node <= S.inst.n)){
					
					S.remover_pedido(node);
					
				}
				
			}
			
			// Removendo rota vazia da solução
			S.Rotas.erase(S.Rotas.begin() + index_rota);
		
		}
		
		// Escolhendo heurísticas e aplicando-as à solução
		
		int index_h_rem = escolher_heuristica('R');
		int index_h_ins = escolher_heuristica('I');
		
		S = removal_heuristics.at(index_h_rem).apply(S);
		
		S = insertion_heuristics.at(index_h_ins).apply(S);
		
		n_it += 1;
	}
	
	return BKS;
	
}


// Método do algoritmo em si

void ALNS::algo(int max_it_ALNS, int max_it_RRH){
	
	S_i = routeReductionHeuristic(S_i, max_it_RRH);
	
	S_i.print_sol();
	
	// Variável para o número de iterações:
	int n_it {};
	
	while (n_it < max_it_ALNS){
		
		std::cout << "Iteracao " << n_it << " da ALNS" << std::endl;
		
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
		
		S = removal_heuristics.at(index_h_rem).apply(S);
		
		S = insertion_heuristics.at(index_h_ins).apply(S);
		
		
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
		
	}
	
}
