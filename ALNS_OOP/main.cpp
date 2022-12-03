#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <Heuristic.hpp>
#include <ALNS.hpp>

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
	
	cout << "Hello world!" << endl;
	
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
	
	// Regret insertion:
	Heuristic H_a('A', 2);
	
	// Construindo solução:
	s = H_c.apply(s);
	
	s.print_sol();
	
	// Parâmetros da ALNS:
	
	// Definindo soluções incumbente e melhor solução como a solução construída inicialmente
	meta.S_p = s;
	
	meta.S_i = s;
	
	// Definindo vetores de heurísticas
	meta.insertion_heuristics = {H_g, H_a};
	
	//for (){
		
	//}
	
	meta.removal_heuristics = {H_r,H_s,H_w, H_s_TTR, H_s_STR, H_s_DER};
	
	// Definindo temperatura inicial:
	double T_inicial = (meta.S_i.FO())*((0.3)/log(0.5));
	
	meta.Temperature = T_inicial;
	
	meta.algo(100);
	
	meta.S_p.print_sol();
	meta.S_p.FO();
	
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
		
		cout << heuristic.name << ": " << heuristic.processing_time/soma_tempos << "%" << endl;
		// soma_tempos += heuristic.processing_time;
		
		
	}
	
		
	for (auto &heuristic: meta.insertion_heuristics){
		
		cout << heuristic.name << ": " << heuristic.processing_time/soma_tempos << "%" << endl;
		// soma_tempos += heuristic.processing_time;
		
	}
	
	cout << "\n\nTempo total: " << soma_tempos << endl;
	
	/*
	
	cout << "Solucao construida: " << endl;
	
	print_sol(s);
	
	cout << s.FO() << endl;
	
	print(s.A);
	
	cout << "\n\n\n";
	
	Sol BKS = s;
	
	for (int i {0}; i < 100; i++){
		
		// cout << i << endl;
		
		if ((i%2==0) && (i%3!=0)){
			
			s = H_r.apply(s);
			cout << "Solucao apos random removal: " << endl;
			
		} if ((i%2!=0) && (i%3!=0)) {
			
			s = H_w.apply(s);
		
			cout << "Solucao apos worst removal: " << endl;
			
		} if ((i%2!=0) && (i%3==0)){
			
			s = H_s.apply(s);
			
			cout << "Solucao apos shaws removal: " << endl;
			
		}
		
		cout << "L: ";

		print(s.L);

		cout << "A: ";

		print(s.A);

		cout << "S: " << endl;
		
		print_sol(s);
		
		

		if (i%2 == 0){
			
			s = H_g.apply(s);
			
			cout << "Solucao apos greedy insertion: " << endl;
			
		}
		
		else {
			
			s = H_a.apply(s);
			
			cout << "Solucao apos regret insertion: " << endl;
			
		}
		
		cout << "L: ";

		print(s.L);

		cout << "A: ";

		print(s.A);

		cout << "S: " << endl;
		
		print_sol(s);
		
		cout << s.FO() << endl;
		
		cout << boolalpha;
		cout << s.isFeasible() << endl;
		
		if (s.FO() < BKS.FO()){
			
			BKS = s;
			
		}
	
	}
	
	cout << "\n\n\nBKS:" << endl;
	
	cout << "L: ";

	print(BKS.L);

	cout << "A: ";

	print(BKS.A);

	cout << "S: " << endl;
	
	print_sol(BKS);
	
	cout << BKS.isFeasible() << endl;
	cout << BKS.FO() << endl;
	
	// Temperatura inicial
	
	// double T_inicial = (S_i.FO())*((0.3)/log(0.5))
	
	// Adequar função objetivo
	
	// Aplicar ruído na inserção
	
	
	*/
	return 0;
}