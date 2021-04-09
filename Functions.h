#pragma once
#include"Structs.h"

int Path_Betw_2(city c1, city c2);
int cost(int* path, Cities* cities);
void Random(int* path);
void Swap(int* p, int l, int r);
void Two_Opt(int* path, int* new_path, int i,
             int k);
void LS(Solution& best, Cities* cities, int limit_LS);
bool Acceptance_Criterion(int* path, int* old_path,
                          Cities* cities);
void Double_Bridge_Move(int* path, int* new_path);
void Perturbation(Cities* cities, Solution& best_solution,
                  Solution& current_solution);
void ILS(Solution& best_solution, Cities* cities, int number_iter,
         int limit_LS);
void file_scanner(Cities* x);
