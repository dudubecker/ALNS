#ifndef HEURISTICS_FUNCTIONS_H_INCLUDED
#define HEURISTICS_FUNCTIONS_H_INCLUDED

#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"
#include <algorithm>

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double delta_FO_ins(Sol &S_ins, double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery);

// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
double delta_FO_rem(Sol &S_rem, double &pedido);

// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
std::vector<double> delta_melhor_insercao(Sol &S, double &pedido);

// Delta melhor inserção considerando rota (regret insertion):
// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
std::vector<double> delta_melhor_insercao(Sol &S, double &pedido, int &index_rota);

// Melhor inserção considerando todas as rotas:
Sol melhor_insercao(Sol &S_in,double &pedido);

// Avaliando factibilidade da inserção de um pedido (sem realizar a inserção!)
bool isInsertionFeasible(Sol &S, double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery);

#endif