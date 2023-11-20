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

// init para rodar IRACE

/*

int main(int argc, char *argv[]){
	
		// Verifica se o número correto de argumentos foi fornecido
	if (argc != 17) {
		std::cerr << "Uso: seu_executavel <id_configuration> <id_instance> <seed> <instancia> --eta <eta> --kappa <kappa> --Gamma <Gamma> --d_b <d_b> --noise <noise> --alpha <alpha>" << std::endl;
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
	double eta = std::stod(args["--eta"]);
	double kappa = std::stod(args["--kappa"]);
	int Gamma = std::stoi(args["--gamma"]);
	int d_b = std::stoi(args["--d_b"]);
	double noise = std::stod(args["--noise"]);
	double alpha = std::stod(args["--alpha"]);
	
	// // Parâmetros:
	
	// instancia: Diretório para a instância (em relação ao diretório onde está o executável) e nome
	// eta: Determina b_UP no critério de aceitação
	// kappa: Porcentagem de soluções aceitas
	// Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
	// d_b: Distância de referência ("ideal") entre soluções
	// eta_noise: Utilizado no cálculo do ruído aleatório
	// alpha: Probabilidade de aplicação do ruído aleatório
	
	
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
							1.3, // w
							0.9997, // c
							27, 30, 22, // sigma 1, sigma 2, sigma 3
							0.1, // r
							0.02, // eta
							6 // delta
	);
	
	// Executando algoritmo
	ALNSObject.executeALNS(
					
					2000, // max_it: Número máximo de iterações do algoritmo
					2000, // max_it_no_improv: Número máximo de iterações sem melhoria
					500, // it_RRH: Número iterações da route reduction heuristic a cada intervalo
					600 // max_t :Tempo máximo de execução do algoritmo
					
	);
	
	// Printando saída do algoritmo, para IRACE
	std::cout << ALNSObject.S_p.FO() << std::endl;
	
}
*/

// Init normal, para ensaios computacionais

// /*

int main(){
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	// Para controlar a seed
	srand(125);
	
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
		//"instances/BB50",
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
		"instances/DD10",
		"instances/DD15",
		"instances/DD20",
		"instances/DD25",
		"instances/DD30",
		"instances/DD35",
		"instances/DD40",
		"instances/DD45",
		"instances/DD50",
		"instances/DD55",
		"instances/DD60",
		"instances/DD65",
		"instances/DD70",
		"instances/DD75",
	};
	
	// std::vector<std::string> instancias = {"instances/li_lim/200/LC1_2_1.txt"};
	
	int iteracoes_por_instancia = 3;
	
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
			1.3, // w
			0.9997, // c
			27, 30, 22, // sigma 1, sigma 2, sigma 3
			0.1, // r
			0.02, // eta
			6 // delta
			
			);
			
			ALNSObject.executarALNS(
			
			10000, // Iterações
			5000, // Número máximo de iterações sem melhorias
			500, // Iterações RRH
			600 // Tempo máximo
			
			);
			
			// ALNSObject.S_p.print_sol();
			
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