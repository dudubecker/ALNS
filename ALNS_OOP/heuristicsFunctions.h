#ifndef HEURISTICS_FUNCTIONS_H_INCLUDED
#define HEURISTICS_FUNCTIONS_H_INCLUDED

#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"
#include <algorithm>

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double delta_FO_ins(Sol &S_ins, double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery);


#endif