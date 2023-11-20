#include <iostream>
#include <vector>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include "ALNS.hpp"
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>

int main(){
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	// Para controlar a seed
	srand(125);
	
	
	// /*
	std::vector<std::string> instancias = {"instances/CC50","instances/DD50"};
	
	// std::vector<std::string> instancias = {"instances/li_lim/400/LC1_4_1.txt"};
	
	// std::vector<std::string> instancias = {"instances/real_instances/R_75.txt"};
	
	for (auto instancia: instancias){
		
		for (auto i {0}; i < 1; i++){
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			// Objeto ALNS
			ALNS meta;
			
			// Objeto instância
			Instance inst(instancia);
			
			// std::cout << "\n\n" << instancia << "\n\n" << std::endl;
			
			// Inicializando objeto solução;
			Sol s(inst);
			
			// Random removal:
			RandomRemoval* H_r = new RandomRemoval;

			// Worst removal:
			WorstRemoval* H_w = new WorstRemoval(0.02, 6);

			// Shaw's removal:
			ShawsRemoval* H_s = new ShawsRemoval(0.3, 0.4, 0.3, 6);

			// Shaw's removal:
			ShawsRemoval* H_s_TTR = new ShawsRemoval(1, 0, 0, 6);

			// Shaw's removal:
			ShawsRemoval* H_s_STR = new ShawsRemoval(0, 1, 0, 6);

			// Shaw's removal:
			ShawsRemoval* H_s_DER = new ShawsRemoval(0, 0, 1, 6);

			// Greedy insertion:
			GreedyInsertion* H_g_random = new GreedyInsertion(0.02);
			
			GreedyInsertion* H_g = new GreedyInsertion(0.0);

			// Regret insertion 1:
			RegretInsertion* H_a_1 = new RegretInsertion(1, 0.0);

			// Regret insertion 2:
			RegretInsertion* H_a_2 = new RegretInsertion(2, 0.0);

			// Regret insertion 3:
			RegretInsertion* H_a_3 = new RegretInsertion(3, 0.0);
			
			// Regret insertion 1:
			RegretInsertion* H_a_1_random = new RegretInsertion(1, 0.02);

			// Regret insertion 2:
			RegretInsertion* H_a_2_random = new RegretInsertion(2, 0.02);

			// Regret insertion 3:
			RegretInsertion* H_a_3_random = new RegretInsertion(3, 0.02);

			// Regret insertion 4:
			RegretInsertion* H_a_4 = new RegretInsertion(4, 0.02);
			
			// Parâmetros da ALNS:
			
			// Definindo soluções incumbente e melhor solução como a solução construída inicialmente
			meta.S_p = s;
			
			meta.S_i = s;
			
			meta.removal_heuristics = {H_r, H_s, H_w, H_s_TTR, H_s_STR, H_s_DER};
			meta.insertion_heuristics = {H_a_1, H_a_2, H_g,
										 H_a_1_random, H_a_2_random, H_g_random};
			
			// Definindo teomperatura inicial:
			double T_inicial = (meta.S_i.FO())*((0.3)/log(0.5));
			
			meta.Temperature = T_inicial;
			
			meta.algo(10000, 1000, 100);
			
			// meta.S_p.print_sol();
			
			std::cout << "\n FO: " << std::setprecision(7) << meta.S_p.FO() << std::endl;
			
			std::cout << "\n Factibilidade:  " << meta.S_p.isFeasible() << std::endl;
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
		}
	}
	
	
	// */
	
	/*
	// std::vector<std::string> instancias = {"instances/BB50"};
	
	std::vector<std::string> instancias = {"instances/li_lim/200/LC1_2_1.txt"};
	
	int iteracoes_por_instancia = 10;
	
	for (auto instancia: instancias){
		
		for (auto i {0}; i < iteracoes_por_instancia; i++){
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			// Objeto instância
			Instance inst(instancia);
			
			std::cout << "\n\n" << instancia << "\n\n" << std::endl;
			
			// Inicializando objeto solução;
			Sol s(inst);
			
			
			// Objeto ALNS
			ALNS meta(
			
			s, // Solução inicial
			1.3, // w
			0.9997, // c
			27, 30, 22, // sigma 1, sigma 2, sigma 3
			0.1, // r
			0.02, // eta
			6 // delta
			
			);
			
			meta.algo(
			
			1000, // Iterações
			300, // Iterações RRH
			600 // Tempo máximo
			
			);
			
			meta.S_p.print_sol();
			
			std::cout << "\n FO: " << std::setprecision(7) << meta.S_p.FO() << std::endl;
			
			std::cout << "\n Factibilidade:  " << meta.S_p.isFeasible() << std::endl;
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
		}
	}
	
	
	*/
	
	
	
	
	return 0;
	
	
	
	
}