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

using namespace std;

// Função para printar vetores: debug

void printDouble(std::vector<double> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}

void printInt(std::vector<int> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}

// Função para printar soluções: debug

void print_sol(Sol S)
{
	
	for (auto &rota:S.Rotas){
		
		for (auto node: rota){
			
			cout << node << " ";
		}
		
		cout << "\n";
		
	}
	
	cout << "\n";
	
}


int main(){
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	
	// Para controlar a seed
	srand(125);
	
	
	/*
	
	// String com instância
	std::string instancia = "instances/AA20";
	
	// Objeto Instance
	Instance inst(instancia);
	
	// Objeto Sol, inicializado a partir do objeto Instance
	Sol S(inst);
	
	std::cout << "\n\nSolucao apos heuristica construtiva: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	// Inicializando heurísticas:
	
	RandomRemoval H_r {};
	
	WorstRemoval H_w {};
	
	ShawsRemoval H_s(0.3, 0.4, 0.3);
	
	GreedyInsertion H_g {};
	
	RegretInsertion H_a(2);
	
	// Aplicando remoção aleatória:
	
	H_r.apply(S);
	
	std::cout << "\nSolucao apos remocao aleatoria: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	// Aplicando inserção gulosa:
	
	H_g.apply(S);
	
	std::cout << "\nSolucao apos regret: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	
	
	
	// Objetos Heuristic
	
	// Worst removal:
	Heuristic H_w('W');
	
	// Random removal:
	Heuristic H_r('R');
	
	// Greedy insertion:
	Heuristic H_g('G');
	
	// Regret insertion
	Heuristic H_a('A');
	
	
	// Aplicando worst removal:
	
	std::cout << "\nSolucao apos worst removal: \n";
	
	H_w.apply(S);
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	
	// Aplicando inserção gulosa:
	
	H_g.apply(S);
	
	std::cout << "\nSolucao apos insercao gulosa: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	
	
	// Aplicando remoção aleatória:
	H_r.apply(S);
	
	std::cout << "\nSolucao apos remocao aleatoria: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	
	// Aplicando inserção por arrependimento:
	H_a.apply(S);
	
	std::cout << "\nSolucao apos regret insertion: \n";
	
	S.print_sol();
	
	std::cout << "\n FO: " << std::setprecision(7) << S.FO() << std::endl;
	
	*/
	
	
	// std::vector<std::string> instancias = {"instances/AA25","instances/CC20","instances/CC25","instances/DD25","instances/DD30"};
	
	
	// /*
	std::vector<std::string> instancias = {"instances/AA60"};
	
	for (auto instancia: instancias){
		
		for (auto i {0}; i < 2; i++){
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			// Objeto ALNS
			ALNS meta;
			
			// Objeto instância
			Instance inst(instancia);
			
			std::cout << "\n\n" << instancia << "\n\n" << std::endl;
			
			// Inicializando objeto solução;
			Sol s(inst);
			
			// Random removal:
			RandomRemoval* H_r = new RandomRemoval;

			// Worst removal:
			WorstRemoval* H_w = new WorstRemoval;

			// Shaw's removal:
			ShawsRemoval* H_s = new ShawsRemoval(0.3, 0.4, 0.3);

			// Shaw's removal:
			ShawsRemoval* H_s_TTR = new ShawsRemoval(1, 0, 0);

			// Shaw's removal:
			ShawsRemoval* H_s_STR = new ShawsRemoval(0, 1, 0);

			// Shaw's removal:
			ShawsRemoval* H_s_DER = new ShawsRemoval(0, 0, 1);

			// Greedy insertion:
			GreedyInsertion* H_g = new GreedyInsertion;

			// Regret insertion 1:
			RegretInsertion* H_a_1 = new RegretInsertion(1);

			// Regret insertion 2:
			RegretInsertion* H_a_2 = new RegretInsertion(2);

			// Regret insertion 3:
			RegretInsertion* H_a_3 = new RegretInsertion(3);

			// Regret insertion 4:
			RegretInsertion* H_a_4 = new RegretInsertion(4);

			// Parâmetros da ALNS:

			// Definindo soluções incumbente e melhor solução como a solução construída inicialmente
			meta.S_p = s;

			meta.S_i = s;

			meta.removal_heuristics = {H_r, H_s, H_w, H_s_TTR, H_s_STR, H_s_DER};
			meta.insertion_heuristics = {H_a_1, H_a_2, H_g};
			
			// Definindo teomperatura inicial:
			double T_inicial = (meta.S_i.FO())*((0.3)/log(0.5));
			
			meta.Temperature = T_inicial;
			
			meta.algo(2000, 1000, 600);
			
			meta.S_p.print_sol();
			
			cout << "\n FO: " << std::setprecision(7) << meta.S_p.FO() << endl;
			
			cout << "\n Factibilidade:  " << meta.S_p.isFeasible() << endl;
			
			cout << "\n Cargas: \n" ;
			
			for (auto vec: meta.S_p.Cargas){
			
				printDouble(vec);
			
			}
			
			cout << "\n Tempos de visita: \n" ;
			
			for (auto vec: meta.S_p.TemposDeVisita){
			
				printDouble(vec);
			
			}
			
			
			//for (auto heuristic: meta.removal_heuristics){
				
			//	std::cout << heuristic.name << ": " << heuristic.processing_time << std::endl;
				
			//}
			
			//for (auto heuristic: meta.insertion_heuristics){
				
			//	std::cout << heuristic.name << ": " << heuristic.processing_time << std::endl;
				
			//}
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
		}
	}
	
	
	// */
	
	
	return 0;
	
	
	
	
}