#include"Functions.h"
#include"Structs.h"
#include<math.h>
#include<vector>
#include<string>
#include<fstream>
#include<iomanip>
#include<iostream>

using namespace std;

Cities cities[amout_of_cities];

int Path_Betw_2(city c1, city c2) {
  int p = sqrt(
      (double)((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y)));
  return p;
}

int cost(int* path, Cities* cities) {
  int sum_cost = 0;
  int x, y;

  for (int i = 0; i < amout_of_cities; i++) {
    x = path[i];

    if (i == amout_of_cities - 1) {
      y = path[0];

    } else {
      y = path[i + 1];
    }

    sum_cost += Path_Betw_2(cities[x], cities[y]);
  }

  return sum_cost;
}


void Random(int* city_way) {
  int i, r, temp;
  for (i = 0; i < amout_of_cities; i++) {
    city_way[i] = i;
  }

  for (i = 0; i < amout_of_cities; i++) {
    r = rand() % (amout_of_cities - i) + i;
    temp = city_way[i];
    city_way[i] = city_way[r];
    city_way[r] = temp;
  }
}

void Swap(int* p, int l, int r) {
  int t;
  while (l < r) {
    t = p[l];
    p[l] = p[r];
    p[r] = t;
    l++;
    r--;
  }
}

void Two_Opt(int* path, int* new_path, int i_,
             int k) {
  for (int i = 0; i < amout_of_cities; i++) {
    new_path[i] = path[i];
  }

  Swap(new_path, i_, k);
}

void LS(Solution& best_solution, Cities* cities, int limit_LS) {
  Solution* current_solution = new Solution;
  int inital_cost = best_solution.cost;

   int i, k;
  int h = 0;
  
   while (h <= limit_LS) {
    for (i = 0; i < amout_of_cities - 1; i++) {
      for (k = i + 1; k < amout_of_cities; k++) {
        Two_Opt(best_solution.path, current_solution->path, i, k);
        current_solution->cost = cost(current_solution->path, cities);
        if (current_solution->cost < best_solution.cost) {
          h = 0;
          for (int j = 0; j < amout_of_cities; j++) {
            best_solution.path[j] = current_solution->path[j];
          }
          best_solution.cost = current_solution->cost;
          inital_cost = best_solution.cost;
        }
      }
    }
    h++;
  }
}

bool Acceptance_Criterion(int* path_new, int* path_old, Cities* cities) {
  int acceptance = 300;
  int old_cost = cost(path_old, cities);
  int new_cost = cost(path_new, cities);

  if ((new_cost <= (old_cost + acceptance)) ||
      (new_cost >= (old_cost - acceptance))) {
    return true;
  }

  return false;
}

void Double_Bridge_Move(int* path_current, int* path_new) {
  int tmp[amout_of_cities];

  int p1 = 1 + rand() % (amout_of_cities / 4);
  int p2 = p1 + 1 + rand() % (amout_of_cities / 4);
  int p3 = p2 + 1 + rand() % (amout_of_cities / 4);
  vector<int> array;
  int i;

  for (i = 0; i < p1; i++) {
    array.push_back(path_current[i]);
  }

  for (i = p3; i < amout_of_cities; i++) {
    array.push_back(path_current[i]);
  }

  for (i = p2; i < p3; i++) {
    array.push_back(path_current[i]);
  }

  for (i = p1; i < p2; i++) {
    array.push_back(path_current[i]);
  }

  for (i = 0; i < (int)array.size(); i++) {
    tmp[i] = array[i];
  }

  if (Acceptance_Criterion(path_current, tmp, cities)) {
    memcpy(path_new, tmp, sizeof(tmp));
  }
}

void Perturbation(Cities* cities, Solution& best_solution,
                  Solution& current_solution) {
  Double_Bridge_Move(best_solution.path, current_solution.path);
  current_solution.cost = cost(current_solution.path, cities);
}

void ILS(Solution& best_solution, Cities* cities, int number_iter,
         int limit_LS) {
  Solution* current_solution = new Solution;

  Random(best_solution.path);
  best_solution.cost = cost(best_solution.path, cities);
  LS(best_solution, cities, limit_LS);
  int i = 0;

  while (i != number_iter) {
    Perturbation(cities, best_solution, *current_solution);
    LS(*current_solution, cities, limit_LS);

    if (current_solution->cost < best_solution.cost) {
      for (int j = 0; j < amout_of_cities; j++) {
        best_solution.path[j] = current_solution->path[j];
      }
      best_solution.cost = current_solution->cost;
    }
    cout << setw(13) << setiosflags(ios::left) << "iterations " << i << "\t"
         << "best solution now " << best_solution.cost << " curent solution "
         << current_solution->cost << endl;
    i++;
  }
}

void file_scanner(Cities* x) {
  std::string line;
  string k;
  int number;
  int number1;
  int number2;
  int i = 0;

  std::ifstream file(
      "C:\\Users\\михаил\\Desktop\\instances\\random_1.txt");  // окрываем файл
                                                                // для чтения
  if (file.is_open()) {
    while (getline(file, line)) {
      // std::cout << line << std::endl;
      while (i != line.size()) {
        while (line[i] != ' ') {
          k += line[i];
          i++;
        }
        number = stoi(k);
        k.clear();
        i++;
        while (line[i] != ' ') {
          k += line[i];
          i++;
        }
        number1 = stoi(k);
        k.clear();
        i++;
        while (i != line.size()) {
          k += line[i];
          i++;
        }
        number2 = stoi(k);
        k.clear();
        x[number - 1].x = number1;
        x[number - 1].y = number2;
      }
      i = 0;
    }
    file.close();  // закрываем файл
  }
}
