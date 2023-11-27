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
	
	
	// Arquivo de saída
	std::string output = "output.txt";
	
	std::ofstream output_file(output, std::ios::app);
	
	// Escrevendo parâmetros
	
	output_file << "/****Parametros de controle dos criterios de parada****/\n\n";
	
	output_file << "max_it : " << max_it << std::endl;
	output_file << "max_it_no_improv : " << max_it_no_improv << std::endl;
	output_file << "it_RRH : " << it_RRH << std::endl;
	
	output_file << "\n\n/****Parametros da meta-heuristica****/\n\n";
	
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
	std::string path = "instances/";
	
		std::vector<std::string> instancias = {
		
		"AA5",
		"AA10",
		"AA15",
		//"AA20",
		//"AA25",
		//"AA30",
		//"AA35",
		//"AA40",
		//"AA45",
		//"AA50",
		//"AA55",
		//"AA60",
		//"AA65",
		//"AA70",
		//"AA75",
		"BB5",
		"BB10",
		"BB15",
		//"BB20",
		//"BB25",
		//"BB30",
		//"BB35",
		//"BB40",
		//"BB45",
		//"BB50",
		//"BB55",
		//"BB60",
		//"BB65",
		//"BB70",
		//"BB75",
		"CC5",
		"CC10",
		"CC15",
		//"CC20",
		//"CC25",
		//"CC30",
		//"CC35",
		//"CC40",
		//"CC45",
		//"CC50",
		"CC55",
		//"CC60",
		//"CC65",
		//"CC70",
		//"CC75",
		"DD5",
		"DD10",
		"DD15",
		"DD20",
		//"DD25",
		//"DD30",
		//"DD35",
		//"DD40",
		//"DD45",
		//"DD50",
		//"DD55",
		//"DD60",
		//"DD65",
		//"DD70",
		//"DD75",
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
			
			// ALNSObject.S_p.printSol();
			
			//std::cout << "\nFO: " << std::setprecision(7) << ALNSObject.S_p.calcularFO() << std::endl;
			
			//std::cout << "\nFactibilidade:  " << ALNSObject.S_p.checarFactibilidadeSolucao() << std::endl;
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			//std::cout << "Tempo de execucao: " << elapsed.count() * 1e-9 << "segundos. " << std::endl;
			
			// Printando output
			std::cout << instancia << ";" << std::setprecision(7) << ALNSObject.S_p.calcularFO() << ";" << ALNSObject.S_p.checarFactibilidadeSolucao() << ";" << elapsed.count() * 1e-9 << std::endl;
			
			// Escrevendo output no arquivo
			output_file << instancia << ";" << std::setprecision(7) << ALNSObject.S_p.calcularFO() << ";" << ALNSObject.S_p.checarFactibilidadeSolucao() << ";" << elapsed.count() * 1e-9 << std::endl;
			
			// Fechando arquivo
			output_file.close();
			
		}
	}
	
	return 0;
	
}

// */