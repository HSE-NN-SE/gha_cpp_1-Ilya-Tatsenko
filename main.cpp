#include<iostream>
#include<time.h>
#include"Functions.h"
#include"Structs.h"

using namespace std;


int main()
{
  srand(time(0));
  Cities goroda[amout_of_cities];

    int number_iter = 9;
    int limit_LS = 19;

    file_scanner(goroda);
    Solution best_solution;

    ILS(best_solution, goroda, number_iter, limit_LS);

    cout << endl << endl <<"search is finished! optimal marshrut = " << best_solution.cost << endl;
    cout << "The best order to visit the city is as follows：";
    for (int i = 0; i < amout_of_cities; i++)
    {
        cout << best_solution.path[i] + 1 << " ";
    }

    cout << endl << endl; 
    return 0;
}
