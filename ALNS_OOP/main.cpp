#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <Heuristic.hpp>
#include <ALNS.hpp>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>

using namespace std;

// Função para printar vetores: debug

void print(std::vector<double> const &input)
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
	
	// Objeto ALNS
	ALNS meta;
	
	// Objeto instância
	Instance inst;
	inst.read("BB30");
	
	// Inicializando objeto solução;
	//Sol s(inst);
	Sol s;
	s.inst = inst;
	
	// Construtiva:
	Heuristic H_c('C');
	
	// Random removal:
	Heuristic H_r('R');
	
	// Worst removal:
	Heuristic H_w('W');
	
	// Shaw's removal:
	Heuristic H_s('S',0.3,0.4,0.3);
	
	// Shaw's removal:
	Heuristic H_s_TTR('S',1,0,0);
	
	// Shaw's removal:
	Heuristic H_s_STR('S',0,1,0);
	
	// Shaw's removal:
	Heuristic H_s_DER('S',0,0,1);
	
	// Greedy insertion:
	Heuristic H_g('G');
	
	// Regret insertion 1:
	Heuristic H_a_1('A', 1);
	
	// Regret insertion 2:
	Heuristic H_a_2('A', 2);
	
	// Regret insertion 3:
	Heuristic H_a_3('A', 3);
	
	// Regret insertion 4:
	Heuristic H_a_4('A', 4);
	
	// Construindo solução:
	s = H_c.apply(s);
	
	s.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << s.FO() << endl;
	
	// Parâmetros da ALNS:
	
	
	
	// Definindo soluções incumbente e melhor solução como a solução construída inicialmente
	meta.S_p = s;
	
	meta.S_i = s;
	
	// Definindo vetores de heurísticas
	// meta.insertion_heuristics = {H_g};
	
	// meta.insertion_heuristics = {H_g, H_a_1};
	
	meta.removal_heuristics = {H_r,H_s,H_w, H_s_TTR, H_s_STR, H_s_DER};
	
	meta.insertion_heuristics = {H_a_1, H_a_2 ,H_g};
	
	// meta.removal_heuristics = {H_r};
	
	// Definindo teomperatura inicial:
	double T_inicial = (meta.S_i.FO())*((0.3)/log(0.5));
	
	meta.Temperature = T_inicial;
	
	meta.algo(5000, 30);
	
	meta.S_p.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << meta.S_p.FO() << endl;
	
	cout << "\n Factibilidade:  " << meta.S_p.isFeasible() << endl;
	
	auto soma_tempos {0};
	
	for (auto &heuristic: meta.removal_heuristics){
		
		// cout << heuristic.name << ": " << heuristic.processing_time << endl;
		soma_tempos += heuristic.processing_time;
		
		
	}
	
		
	for (auto &heuristic: meta.insertion_heuristics){
		
		// cout << heuristic.name << ": " << heuristic.processing_time << endl;
		soma_tempos += heuristic.processing_time;
		
	}
	
	for (auto &heuristic: meta.removal_heuristics){
		
		cout << heuristic.name << ": " << heuristic.processing_time/soma_tempos << "% " << heuristic.n_it_total << endl;
		// soma_tempos += heuristic.processing_time;
		
		
	}
	
		
	for (auto &heuristic: meta.insertion_heuristics){
		
		cout << heuristic.name << ": " << heuristic.processing_time/soma_tempos << "% " << heuristic.n_it_total << endl;
		// soma_tempos += heuristic.processing_time;
		
	}
	
	std::cout << soma_tempos << std::endl;
	
	return 0;
}