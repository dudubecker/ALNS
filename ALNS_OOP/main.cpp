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
#include <unordered_map>

// init para rodar IRACE

/*

int main(int argc, char *argv[]){
	
	// Controle dos critérios de parada do algoritmo
	
	int max_it = 2000; // Número máximo de iterações do algoritmo
	int max_it_no_improv = 2000; // Número máximo de iterações sem melhoria
	int it_RRH = 500; // Número iterações da route reduction heuristic a cada intervalo
	int max_t = 600; // Tempo máximo de execução do algoritmo
	
	// Verifica se o número correto de argumentos foi fornecido
	if (argc != 21) {
		std::cerr << "Uso: seu_executavel <id_configuration> <id_instance> <seed> <instancia> --w <w> --c <c> --sigma1 <sigma1> --sigma2 <sigma2> --sigma3 <sigma3> --r <r> --eta <eta> --delta <delta>" << std::endl;
		return 1; // Retorna código de erro
	}
	
	// Mapeia as flags para os valores correspondentes
	std::unordered_map<std::string, std::string> args;
	for (int i = 5; i < argc; i += 2) {
		args[argv[i]] = argv[i + 1];
	}
	
	// Lendo argumentos e atribuindo variáveis
	long long seed = std::stoll(argv[3]);
	std::string instancia = argv[4];
	double w = std::stod(args["--w"]);
	double c = std::stod(args["--c"]);
	int sigma1 = std::stoi(args["--sigma1"]);
	int sigma2 = std::stoi(args["--sigma2"]);
	int sigma3 = std::stod(args["--sigma3"]);
	double r = std::stod(args["--r"]);
	double eta = std::stod(args["--eta"]);
	double delta = std::stod(args["--delta"]);
	
	// // Parâmetros:
	
	// instancia: Diretório para a instância (em relação ao diretório onde está o executável) e nome
	// w: Utilizado no critério de aceitação, para o cálculo da temperatura inicial
	// c: Taxa de resfriamento
	// sigma1: Atualização de scores das heurísticas - Caso 1: as heurísticas resultaram um novo valor ótimo global
	// sigma2: Atualização de scores das heurísticas - Caso 2: as heurísticas resultaram um valor que passou pelo critério de aceitação, ainda não foram aceitas anteriormente e S* < S
	// sigma3: Atualização de scores das heurísticas - // Caso 3: as heurísticas resultaram um valor que passou pelo critério de aceitação, ainda não foram aceitas anteriormente e S* > S
	// r: Fator de reação: utilizado para a atualização dos pesos
	// eta: Utilizado no cálculo do ruído aleatório
	// delta: Utilizado em worst e shaw's removal, para determinar posição de retirada na lista ordenada
	
	// Código principal
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	// Para ter controle sobre os outputs
	srand(seed);
	
	// Executando algoritmo
	
	// Objeto instância
	Instance inst(instancia);
	
	// Inicializando objeto solução;
	Sol S(inst);
	
	// Inicializando objeto da ALNS
	
	ALNS ALNSObject(S, // Solução inicial
							w, // w
							c, // c
							sigma1, sigma2, sigma3, // sigma 1, sigma 2, sigma 3
							r, // r
							eta, // eta
							delta // delta
	);
	
	// Executando algoritmo
	ALNSObject.executarALNS(
					
					max_it,
					max_it_no_improv,
					it_RRH,
					max_t
					
	);
	
	// Printando saída do algoritmo, para IRACE
	std::cout << ALNSObject.S_p.calcularFO() << std::endl;
	
}

*/

// Init normal, para ensaios computacionais

// /*

int main(){
	
	// Parâmetros de controle dos critérios de parada
	
	int max_it = 3000; // Número máximo de iterações do algoritmo
	int max_it_no_improv = 2000; // Número máximo de iterações sem melhoria
	int it_RRH = 5000; // Número iterações da route reduction heuristic a cada intervalo
	int max_t = 600; // Tempo máximo de execução do algoritmo
	int iteracoes_por_instancia = 3; 
	
	// Parâmetros da meta-heurística
	
	long long seed = 1382364237;
	double w = 0.94;
	double c = 0.997;
	int sigma1 = 29;
	int sigma2 = 32;
	int sigma3 = 25;
	double r = 0.35;
	double eta = 0.04;
	double delta = 1;
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	// Para controlar a seed
	srand(seed);
	
	// Instâncias
	
		std::vector<std::string> instancias = {
		
		//"instances/AA5",
		//"instances/AA10",
		//"instances/AA15",
		//"instances/AA20",
		//"instances/AA25",
		//"instances/AA30",
		//"instances/AA35",
		//"instances/AA40",
		//"instances/AA45",
		//"instances/AA50",
		//"instances/AA55",
		//"instances/AA60",
		//"instances/AA65",
		//"instances/AA70",
		//"instances/AA75",
		//"instances/BB5",
		//"instances/BB10",
		//"instances/BB15",
		//"instances/BB20",
		//"instances/BB25",
		//"instances/BB30",
		//"instances/BB35",
		//"instances/BB40",
		//"instances/BB45",
		"instances/BB50",
		//"instances/BB55",
		//"instances/BB60",
		//"instances/BB65",
		//"instances/BB70",
		//"instances/BB75",
		//"instances/CC5",
		//"instances/CC10",
		//"instances/CC15",
		//"instances/CC20",
		//"instances/CC25",
		//"instances/CC30",
		//"instances/CC35",
		//"instances/CC40",
		//"instances/CC45",
		//"instances/CC50",
		//"instances/CC55",
		//"instances/CC60",
		//"instances/CC65",
		//"instances/CC70",
		//"instances/CC75",
		//"instances/DD5",
		//"instances/DD10",
		//"instances/DD15",
		//"instances/DD20",
		//"instances/DD25",
		//"instances/DD30",
		//"instances/DD35",
		//"instances/DD40",
		//"instances/DD45",
		//"instances/DD50",
		//"instances/DD55",
		//"instances/DD60",
		//"instances/DD65",
		//"instances/DD70",
		//"instances/DD75",
		//"instances/real_instances/R50.txt",
		//"instances/li_lim/100/lc101.txt"
	};
	
	
	// Executando algoritmo
	
	for (auto instancia: instancias){
		
		for (auto i {0}; i < iteracoes_por_instancia; i++){
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			// Objeto instância
			Instance inst(instancia);
			
			std::cout << "\n" << instancia << std::endl;
			
			// Inicializando objeto solução;
			Sol s(inst);
			
			// Objeto ALNS
			ALNS ALNSObject(
			
			s, // Solução inicial
			w, // w
			c, // c
			sigma1, sigma2, sigma3, // sigma 1, sigma 2, sigma 3
			r, // r
			eta, // eta
			delta // delta
			);
			
			ALNSObject.executarALNS(
			
			max_it, // Iterações
			max_it_no_improv, // Número máximo de iterações sem melhorias
			it_RRH, // Iterações RRH
			max_t // Tempo máximo
			
			);
			
			// ALNSObject.S_p.printSol();
			
			std::cout << "\nFO: " << std::setprecision(7) << ALNSObject.S_p.calcularFO() << std::endl;
			
			std::cout << "\nFactibilidade:  " << ALNSObject.S_p.checarFactibilidadeSolucao() << std::endl;
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
		}
	}
	
	return 0;
	
}

// */