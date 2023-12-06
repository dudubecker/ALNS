#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include "ALNS.hpp"
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
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
	int max_it_no_improv = 1000; // Número máximo de iterações sem melhoria
	int it_RRH = 800; // Número iterações da route reduction heuristic a cada intervalo
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
	
	int max_it = 2000; // Número máximo de iterações do algoritmo
	int max_it_no_improv = 1200; // Número máximo de iterações sem melhoria
	int it_RRH = 750; // Número iterações da route reduction heuristic a cada intervalo
	int max_t = 3600; // Tempo máximo de execução do algoritmo
	int iteracoes_por_instancia = 3;
	
	// Parâmetros da meta-heurística
	
	long long seed = 1382364237;
	double w = 1.25;
	double c = 0.9997;
	int sigma1 = 31;
	int sigma2 = 6;
	int sigma3 = 26;
	double r = 0.38;
	double eta = 0.04;
	double delta = 3;
	
	// Para gerar valores aleatórios, tirar comentário:
	// srand(time(NULL));
	
	// Para controlar a seed
	srand(seed);
	
	
	// Arquivo de saída
	std::string output = "output.txt";
	
	std::ofstream output_file(output, std::ios::app);
	
	// Escrevendo parâmetros
	
	output_file << "***Parametros de controle dos criterios de parada***\n\n";
	
	output_file << "max_it : " << max_it << std::endl;
	output_file << "max_it_no_improv : " << max_it_no_improv << std::endl;
	output_file << "it_RRH : " << it_RRH << std::endl;
	
	output_file << "\n\n***Parametros da meta-heuristica***\n\n";
	
	output_file << "seed : " << seed << std::endl;
	output_file << "w : " << w << std::endl;
	output_file << "c : " << c << std::endl;
	output_file << "sigma1 : " << sigma1 << std::endl;
	output_file << "sigma2 : " << sigma2 << std::endl;
	output_file << "sigma3 : " << sigma3 << std::endl;
	output_file << "r : " << r << std::endl;
	output_file << "eta : " << eta << std::endl;
	output_file << "delta : " << delta << "\n\n\n";
	
	
	// Instâncias
	
	// Caminho para as instância:
	std::string path = "instances/li_lim/200/";
	
		std::vector<std::string> instancias = {
		
		"LC1_2_1.txt",
		"LC1_2_10.txt",
		"LC1_2_2.txt",
		"LC1_2_3.txt",
		"LC1_2_4.txt",
		"LC1_2_5.txt",
		"LC1_2_6.txt",
		"LC1_2_7.txt",
		"LC1_2_8.txt",
		"LC1_2_9.txt",
		"LC2_2_1.txt",
		"LC2_2_10.txt",
		"LC2_2_2.txt",
		"LC2_2_3.txt",
		"LC2_2_4.txt",
		"LC2_2_5.txt",
		"LC2_2_6.txt",
		"LC2_2_7.txt",
		"LC2_2_8.txt",
		"LC2_2_9.txt",
		"LR1_2_1.txt",
		"LR1_2_10.txt",
		"LR1_2_2.txt",
		"LR1_2_3.txt",
		"LR1_2_4.txt",
		"LR1_2_5.txt",
		"LR1_2_6.txt",
		"LR1_2_7.txt",
		"LR1_2_8.txt",
		"LR1_2_9.txt",
		"LR2_2_1.txt",
		"LR2_2_10.txt",
		"LR2_2_2.txt",
		"LR2_2_3.txt",
		"LR2_2_4.txt",
		"LR2_2_5.txt",
		"LR2_2_6.txt",
		"LR2_2_7.txt",
		"LR2_2_8.txt",
		"LR2_2_9.txt",
		"LRC1_2_1.txt",
		"LRC1_2_10.txt",
		"LRC1_2_2.txt",
		"LRC1_2_3.txt",
		"LRC1_2_4.txt",
		"LRC1_2_5.txt",
		"LRC1_2_6.txt",
		"LRC1_2_7.txt",
		"LRC1_2_8.txt",
		"LRC1_2_9.txt",
		"LRC2_2_1.txt",
		"LRC2_2_10.txt",
		"LRC2_2_2.txt",
		"LRC2_2_3.txt",
		"LRC2_2_4.txt",
		"LRC2_2_5.txt",
		"LRC2_2_6.txt",
		"LRC2_2_7.txt",
		"LRC2_2_8.txt",
		"LRC2_2_9.txt"


	};
	
	
	// Executando algoritmo
	
	for (auto instancia: instancias){

		for (auto i {0}; i < iteracoes_por_instancia; i++){

			// Abrindo arquivo de saída
			// Modo append
			std::ofstream output_file(output, std::ios::app);
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			std::string path_to_instance = path + instancia;

			// Objeto instância
			Instance inst(path_to_instance);
			
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
			
			//ALNSObject.S_p.printSol();
			
			//std::cout << "\nFO: " << std::setprecision(7) << ALNSObject.S_p.calcularFO() << std::endl;
			
			//std::cout << "\nFactibilidade:  " << ALNSObject.S_p.checarFactibilidadeSolucao() << std::endl;
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			//std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
			
			// Printando output
			std::cout << instancia << ";" << std::setprecision(7) << ALNSObject.S_p.calcularFO() << ";" << ALNSObject.S_p.checarFactibilidadeSolucao() << ";" << ALNSObject.S_p.rotas.size() << ";" << elapsed.count() * 1e-9 << std::endl;

			// ALNSObject.S_p.printSol();
			
			// Escrevendo output no arquivo
			output_file << instancia << ";" << std::setprecision(7) << ALNSObject.S_p.calcularFO() << ";" << ALNSObject.S_p.checarFactibilidadeSolucao() << ";" << ALNSObject.S_p.rotas.size() << ";" << elapsed.count() * 1e-9 << std::endl;

			// Fechando arquivo
			output_file.close();
			
		}
	}
	
	return 0;
	
}

// */