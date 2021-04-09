#include <iostream>
#include <vector>
#include <algorithm>
#include <float.h>
#include <string>
#include <fstream>
#include <time.h>
using namespace std;
class point
{
private:
	double X, Y;

public:
	//defualt constructor 
	point() { X = 0; Y = 0; }

	point(double a, double b) {
		setPoint(a, b);
	}
	//setter function
	void setPoint(double a, double b)
	{
		X = a;
		Y = b;
	}
	
	//getter functions
	double getX(void)
	{
		return X;
	}
	double getY(void)
	{
		return Y;
	}
	friend std::ostream& operator<< (std::ostream& out, const point& point);
	
	static double distance(const point& a, const point& b) {
		return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
	}

};
std::ostream& operator<< (std::ostream& out, const point& point_)
{
	// Поскольку operator<< является другом класса Point, то мы имеем прямой доступ к членам Point
	out << "(" << point_.X << ", " << point_.Y << ")";

	return out;
}
class city {
	
public :
	vector<double> distances;
	vector<int> closeCities;
	point location;
	int number;
	city(double x, double y) : distances(vector<double>()),closeCities(vector<int>()) {
		location = point(x, y);
	}
	city(double x, double y,int _number) : distances(vector<double>()), closeCities(vector<int>()) {
		location = point(x, y);
		number = _number;
	}
	void find_closest_cities(int numberOfCloseCities) {
		double shortestDistance;
		int shortestCity = 0;
		vector<double> dist = vector<double>(distances.size());

		copy(distances.begin(),distances.end(),dist.begin());
		if (numberOfCloseCities > distances.size() - 1)
		{
			numberOfCloseCities = distances.size() - 1;
		}

		closeCities.clear();

		for (int i = 0; i < numberOfCloseCities; i++)
		{
			shortestDistance = DBL_MAX;
			for (int cityNum = 0; cityNum < distances.size(); cityNum++)
			{
				if (dist[cityNum] < shortestDistance)
				{
					shortestDistance = dist[cityNum];
					shortestCity = cityNum;
				}
			}
			closeCities.push_back(shortestCity);
			dist[shortestCity] = DBL_MAX;
		}
	}
};
class link {
public:

	int connection1;

	int connection2;
	
};
class cities
{
private:
public:
	vector<city> _cities;

	void CalculateCityDistances(int numberOfCloseCities)
	{
		for (city& _city : _cities)
		{
			_city.distances.clear();

			for (int i = 0; i < _cities.size(); i++)
			{
				_city.distances.push_back(
					sqrt(pow((double)(_city.location.getX() - _cities[i].location.getX()), 2) +
						pow((double)(_city.location.getY() - _cities[i].location.getY()), 2))
				);
			}
		}

		for (city& _city : _cities)
		{
			_city.find_closest_cities(numberOfCloseCities);
		}
	}

	void OpenCityList(string fileName) {
		std::ifstream in(fileName);
		
		if (in.is_open())
		{
			string first_line;
			in >> first_line;
			int number;
			double x, y;
			char c;
			while (in >>number>>c>> x >>c>> y)
			{
				_cities.push_back(city(x,y,number));
			}
		}
		in.close();
	}
};
class random {
public:
	random() {
		srand(time(0));
	}
	static int Next(int maxValue) {
		return rand() % maxValue;
	}
};
class tour{
private:
	void resetTour(int numberOfCities)
	{
		links.clear();

		link link_;
		for (int i = 0; i < numberOfCities; i++)
		{
			link_ = link();
			link_.connection1 = -1;
			link_.connection2 = -1;
			links.push_back(link_);
		}
	}
	static int findNextCity(tour& parent, tour& child, cities& cityList, vector<int>& cityUsage, int city)
	{
		if (testConnectionValid(child, cityList, cityUsage, city, parent.links[city].connection1))
		{
			return parent.links[city].connection1;
		}
		else if (testConnectionValid(child, cityList, cityUsage, city, parent.links[city].connection2))
		{
			return parent.links[city].connection2;
		}

		return -1;
	}
	static bool testConnectionValid(tour& tour, cities& cityList, vector<int>& cityUsage, int city1, int city2)
	{
		// Quick check to see if cities already connected or if either already has 2 links
		if ((city1 == city2) || (cityUsage[city1] == 2) || (cityUsage[city2] == 2))
		{
			return false;
		}

		// A quick check to save CPU. If haven't been to either city, connection must be valid.
		if ((cityUsage[city1] == 0) || (cityUsage[city2] == 0))
		{
			return true;
		}

		// Have to see if the cities are connected by going in each direction.
		for (int direction = 0; direction < 2; direction++)
		{
			int lastCity = city1;
			int currentCity;
			if (direction == 0)
			{
				currentCity = tour.links[city1].connection1;  // on first pass, use the first connection
			}
			else
			{
				currentCity = tour.links[city1].connection2;  // on second pass, use the other connection
			}
			int tourLength = 0;
			while ((currentCity != -1) && (currentCity != city2) && (tourLength < cityList._cities.size() - 2))
			{
				tourLength++;
				// figure out if the next city in the list is [0] or [1]
				if (lastCity != tour.links[currentCity].connection1)
				{
					lastCity = currentCity;
					currentCity = tour.links[currentCity].connection1;
				}
				else
				{
					lastCity = currentCity;
					currentCity = tour.links[currentCity].connection2;
				}
			}

			// if cities are connected, but it goes through every city in the list, then OK to join.
			if (tourLength >= cityList._cities.size() - 2)
			{
				return true;
			}

			// if the cities are connected without going through all the cities, it is NOT OK to join.
			if (currentCity == city2)
			{
				return false;
			}
		}

		// if cities weren't connected going in either direction, we are OK to join them
		return true;
	}
public:
	vector<link> links;
	double fitness;
	tour() {
		links = vector<link>();
	}
	tour(int capacity)
	{
		links = vector<link>(capacity);
		resetTour(capacity);
	}

	void DetermineFitness(cities& cities)
	{
		fitness = 0;

		int lastCity = 0;
		int nextCity = links[0].connection1;

		for(link link : links)
		{
			fitness += cities._cities[lastCity].distances[nextCity];

			// figure out if the next city in the list is [0] or [1]
			if (lastCity != links[nextCity].connection1)
			{
				lastCity = nextCity;
				nextCity = links[nextCity].connection1;
			}
			else
			{
				lastCity = nextCity;
				nextCity = links[nextCity].connection2;
			}
		}
	}
	static void joinCities(tour& tour, vector<int>& cityUsage, int city1, int city2)
	{
		// Determine if the [0] or [1] link is available in the tour to make this link.
		if (tour.links[city1].connection1 == -1)
		{
			tour.links[city1].connection1 = city2;
		}
		else
		{
			tour.links[city1].connection2 = city2;
		}

		if (tour.links[city2].connection1 == -1)
		{
			tour.links[city2].connection1 = city1;
		}
		else
		{
			tour.links[city2].connection2 = city1;
		}

		cityUsage[city1]++;
		cityUsage[city2]++;
	}
	static tour Crossover(tour& parent1, tour& parent2, cities& cityList, random rand)
	{

		tour child = tour(cityList._cities.size());      // the new tour we are making
		vector<int> cityUsage = vector<int>(cityList._cities.size());  // how many links 0-2 that connect to this city
		int city;                                   // for loop variable
		int nextCity;                               // the other city in this link

		for (city = 0; city < cityList._cities.size(); city++)
		{
			cityUsage[city] = 0;
		}

		// Take all links that both parents agree on and put them in the child
		for (city = 0; city < cityList._cities.size(); city++)
		{
			if (cityUsage[city] < 2)
			{
				if (parent1.links[city].connection1 == parent2.links[city].connection1)
				{
					nextCity = parent1.links[city].connection1;
					if (testConnectionValid(child, cityList, cityUsage, city, nextCity))
					{
						joinCities(child, cityUsage, city, nextCity);
					}
				}
				if (parent1.links[city].connection2 == parent2.links[city].connection2)
				{
					nextCity = parent1.links[city].connection2;
					if (testConnectionValid(child, cityList, cityUsage, city, nextCity))
					{
						joinCities(child, cityUsage, city, nextCity);

					}
				}
				if (parent1.links[city].connection1 == parent2.links[city].connection2)
				{
					nextCity = parent1.links[city].connection1;
					if (testConnectionValid(child, cityList, cityUsage, city, nextCity))
					{
						joinCities(child, cityUsage, city, nextCity);
					}
				}
				if (parent1.links[city].connection2 == parent2.links[city].connection1)
				{
					nextCity = parent1.links[city].connection2;
					if (testConnectionValid(child, cityList, cityUsage, city, nextCity))
					{
						joinCities(child, cityUsage, city, nextCity);
					}
				}
			}
		}

		// The parents don't agree on whats left, so we will alternate between using
		// links from parent 1 and then parent 2.

		for (city = 0; city < cityList._cities.size(); city++)
		{
			if (cityUsage[city] < 2)
			{
				if (city % 2 == 1)  // we prefer to use parent 1 on odd cities
				{
					nextCity = findNextCity(parent1, child, cityList, cityUsage, city);
					if (nextCity == -1) // but if thats not possible we still go with parent 2
					{
						nextCity = findNextCity(parent2, child, cityList, cityUsage, city); ;
					}
				}
				else // use parent 2 instead
				{
					nextCity = findNextCity(parent2, child, cityList, cityUsage, city);
					if (nextCity == -1)
					{
						nextCity = findNextCity(parent1, child, cityList, cityUsage, city);
					}
				}

				if (nextCity != -1)
				{
					joinCities(child, cityUsage, city, nextCity);

					// not done yet. must have been 0 in above case.
					if (cityUsage[city] == 1)
					{
						if (city % 2 != 1)  // use parent 1 on even cities
						{
							nextCity = findNextCity(parent1, child, cityList, cityUsage, city);
							if (nextCity == -1) // use parent 2 instead
							{
								nextCity = findNextCity(parent2, child, cityList, cityUsage, city);
							}
						}
						else // use parent 2
						{
							nextCity = findNextCity(parent2, child, cityList, cityUsage, city);
							if (nextCity == -1)
							{
								nextCity = findNextCity(parent1, child, cityList, cityUsage, city);
							}
						}

						if (nextCity != -1)
						{
							joinCities(child, cityUsage, city, nextCity);
						}
					}
				}
			}
		}

		// Remaining links must be completely random.
		// Parent's links would cause multiple disconnected loops.
		for (city = 0; city < cityList._cities.size(); city++)
		{
			while (cityUsage[city] < 2)
			{
				do
				{
					nextCity = rand.Next(cityList._cities.size());  // pick a random city, until we find one we can link to
				} while (!testConnectionValid(child, cityList, cityUsage, city, nextCity));

				joinCities(child, cityUsage, city, nextCity);
			}
		}

		return child;
	}
	void Mutate(random rand)
	{
		int cityNumber = rand.Next(links.size());
		link link_ = links[cityNumber];
		int tmpCityNumber;

		// Find which 2 cities connect to cityNumber, and then connect them directly
		if (links[link_.connection1].connection1 == cityNumber)   // Conn 1 on Conn 1 link points back to us.
		{
			if (links[link_.connection2].connection1 == cityNumber)// Conn 1 on Conn 2 link points back to us.
			{
				tmpCityNumber = link_.connection2;
				links[link_.connection2].connection1 = link_.connection1;
				links[link_.connection1].connection1 = tmpCityNumber;
			}
			else                                                // Conn 2 on Conn 2 link points back to us.
			{
				tmpCityNumber = link_.connection2;
				links[link_.connection2].connection2 = link_.connection1;
				links[link_.connection1].connection1 = tmpCityNumber;
			}
		}
		else                                                    // Conn 2 on Conn 1 link points back to us.
		{
			if (links[link_.connection2].connection1 == cityNumber)// Conn 1 on Conn 2 link points back to us.
			{
				tmpCityNumber = link_.connection2;
				links[link_.connection2].connection1 = link_.connection1;
				links[link_.connection1].connection2 = tmpCityNumber;
			}
			else                                                // Conn 2 on Conn 2 link points back to us.
			{
				tmpCityNumber = link_.connection2;
				links[link_.connection2].connection2 = link_.connection1;
				links[link_.connection1].connection2 = tmpCityNumber;
			}

		}

		int replaceCityNumber = -1;
		do
		{
			replaceCityNumber = rand.Next(links.size());
		} while (replaceCityNumber == cityNumber);
		link replaceLink = links[replaceCityNumber];

		// Now we have to reinsert that city back into the tour at a random location
		tmpCityNumber = replaceLink.connection2;
		link_.connection2 = replaceLink.connection2;
		link_.connection1 = replaceCityNumber;
		replaceLink.connection2 = cityNumber;

		if (links[tmpCityNumber].connection1 == replaceCityNumber)
		{
			links[tmpCityNumber].connection1 = cityNumber;
		}
		else
		{
			links[tmpCityNumber].connection2 = cityNumber;
		}
	}
};
class population {
private:
	
public:
	tour BestTour;
	vector<tour> tours;
	void CreateRandomPopulation(int populationSize, cities& cityList, random rand, int chanceToUseCloseCity)
	{
		int firstCity, lastCity, nextCity;

		for (int tourCount = 0; tourCount < populationSize; tourCount++)
		{
			tour tour_ = tour(cityList._cities.size());

			// Create a starting point for this tour
			firstCity = rand.Next(cityList._cities.size());
			lastCity = firstCity;

			for (int city = 0; city < cityList._cities.size() - 1; city++)
			{
				do
				{
					// Keep picking random cities for the next city, until we find one we haven't been to.
					if ((rand.Next(100) < chanceToUseCloseCity) && (cityList._cities[city].closeCities.size() > 0))
					{
						// 75% chance will will pick a city that is close to this one
						nextCity = cityList._cities[city].closeCities[rand.Next(cityList._cities[city].closeCities.size())];
					}
					else
					{
						// Otherwise, pick a completely random city.
						nextCity = rand.Next(cityList._cities.size());
					}
					// Make sure we haven't been here, and make sure it isn't where we are at now.
				} while ((tour_.links[nextCity].connection2 != -1) || (nextCity == lastCity));

				// When going from city A to B, [1] on A = B and [1] on city B = A
				tour_.links[lastCity].connection2 = nextCity;
				tour_.links[nextCity].connection1 = lastCity;
				lastCity = nextCity;
			}

			// Connect the last 2 cities.
			tour_.links[lastCity].connection2 = firstCity;
			tour_.links[firstCity].connection1 = lastCity;

			tour_.DetermineFitness(cityList);

			tours.push_back(tour_);
			if (BestTour.links.empty() || (tour_.fitness < BestTour.fitness))
			{
				BestTour = tour_;
			}
		}
	}

};
class tsp {
public:
	random rand;

	cities cityList;

	population population_;

	bool halt = false;
	tsp() {

	}
	void Begin(int populationSize, int maxGenerations, int groupSize, int mutation, int seed, int chanceToUseCloseCity, cities& cityList)
	{

		this->cityList = cityList;

		population_ = population();
		population_.CreateRandomPopulation(populationSize, cityList, rand, chanceToUseCloseCity);

		//разкомментируй если хочешь видеть первое поколение
		//displayTour(population_.BestTour, 0, false);

		bool foundNewBestTour = false;
		int generation;
		for (generation = 0; generation < maxGenerations; generation++)
		{
			
			foundNewBestTour = makeChildren(groupSize, mutation);

			if (foundNewBestTour)
			{
				//разкомментируй если хочешь видеть как работает алгоритм
				//displayTour(population_.BestTour, generation, false);
				cout <<generation<<")"<< population_.BestTour.fitness << endl;
				
			}
		}

		displayTour(population_.BestTour, generation, true);
	}

	
	bool makeChildren(int groupSize, int mutation)
	{
		vector<int> tourGroup = vector<int>(groupSize);
		int tourCount, i, topTour, childPosition, tempTour;

		// pick random tours to be in the neighborhood city group
		// we allow for the same tour to be included twice
		for (tourCount = 0; tourCount < groupSize; tourCount++)
		{
			tourGroup[tourCount] = rand.Next(population_.tours.size());
		}

		// bubble sort on the neighborhood city group
		for (tourCount = 0; tourCount < groupSize - 1; tourCount++)
		{
			topTour = tourCount;
			for (i = topTour + 1; i < groupSize; i++)
			{
				if (population_.tours[tourGroup[i]].fitness < population_.tours[tourGroup[topTour]].fitness)
				{
					topTour = i;
				}
			}

			if (topTour != tourCount)
			{
				tempTour = tourGroup[tourCount];
				tourGroup[tourCount] = tourGroup[topTour];
				tourGroup[topTour] = tempTour;
			}
		}

		bool foundNewBestTour = false;

		// take the best 2 tours, do crossover, and replace the worst tour with it
		childPosition = tourGroup[groupSize - 1];
		population_.tours[childPosition] = tour::Crossover(population_.tours[tourGroup[0]], 
			population_.tours[tourGroup[1]], cityList, rand);
		if (rand.Next(100) < mutation)
		{
			population_.tours[childPosition].Mutate(rand);
		}
		population_.tours[childPosition].DetermineFitness(cityList);

		// now see if the first new tour has the best fitness
		if (population_.tours[childPosition].fitness < population_.BestTour.fitness)
		{
			population_.BestTour = population_.tours[childPosition];
			foundNewBestTour = true;
		}

		// take the best 2 tours (opposite order), do crossover, and replace the 2nd worst tour with it
		childPosition = tourGroup[groupSize - 2];
		population_.tours[childPosition] = tour::Crossover(population_.tours[tourGroup[1]], 
			population_.tours[tourGroup[0]], cityList, rand);
		if (rand.Next(100) < mutation)
		{
			population_.tours[childPosition].Mutate(rand);
		}
		population_.tours[childPosition].DetermineFitness(cityList);

		// now see if the second new tour has the best fitness
		if (population_.tours[childPosition].fitness < population_.BestTour.fitness)
		{
			population_.BestTour = population_.tours[childPosition];
			foundNewBestTour = true;
		}

		return foundNewBestTour;
	}
	void displayTour(tour& bestTour, int generationNumber, bool complete) {
	
		int lastCity = 0;
		int nextCity = bestTour.links[0].connection1;
		cout <<"("<<generationNumber<<")"<< cityList._cities[lastCity].number;
		for (const city& city_ : cityList._cities) {
			//Printing......
			//cout << "<-" << cityList._cities[nextCity].location;
			
			if (lastCity != bestTour.links[nextCity].connection1)
			{
				lastCity = nextCity;
				nextCity = bestTour.links[nextCity].connection1;
			}
			else
			{
				lastCity = nextCity;
				nextCity = bestTour.links[nextCity].connection2;
			}
			cout << " " << lastCity;
		}
		
		

	}
	
	

};
int main()
{
	cities cityList = cities();
	tsp tsp_;
	int populationSize = 9;
	int maxGenerations = 21;
	int mutation = 3;
	int groupSize = 6;
	int seed = 5;
	int numberOfCloseCities =10;
	int chanceUseCloseCity = 100;
	cityList.OpenCityList("cities.txt");
	cout << "city locations :" << endl;
	for (city c:cityList._cities)
	{
		cout << c.location.getX() << " " << c.location.getY() << endl;
	}
	cityList.CalculateCityDistances(numberOfCloseCities);
	tsp_ = tsp();
	tsp_.Begin(populationSize, maxGenerations, groupSize, mutation, seed, chanceUseCloseCity, cityList);
}