#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>

#include "Chromosome.hpp"
#include "eval_expr.hpp"

// Fitness function used in selection (only when result != target)
float getFitness(int result, int target)
{
	return 1.f / std::abs(result - target);
}

// Returns a random target number in the required range
int getTarget(int min, int max)
{
	return rand() % (min + max) + min;
}

// Starts the algorithm with the required parameters
void startGenerations(int targetNumber, uint maxGenerations, uint genesMinNumber, uint genesMaxNumber, uint logThreshold)
{
	bool solutionFound = false;
	int generation = 0;
	Chromosome *fittestChromosome = NULL;
	float bestFitness = 0;
	
	// Run the simulation until a solution has been found or the maximum number of generations has been reached
	for (unsigned int i = 0; !solutionFound && (maxGenerations == 0 || i < maxGenerations); ++i)
	{
		bool writeLog = logThreshold && (generation % logThreshold == 0);
		
		if (writeLog)
			std::cout << "Generation " << generation << '\n'; 
		
		Chromosome *newChromosome = NULL;
		
		if (fittestChromosome)
			newChromosome = new Chromosome(*fittestChromosome);
		else
			newChromosome = new Chromosome(genesMinNumber, genesMaxNumber);
		
		int result = 0;
		eval_expr(newChromosome->getSanitized(), result);
		if (writeLog)
			std::cout << "Chromosome result: " << result << '\n';
		
		if (result != targetNumber) // Solution not yet found
		{
			float fitness = getFitness(result, targetNumber);
			
			if (writeLog)
				std::cout << "Chromosome fitness: " << fitness;
			
			if (fitness > bestFitness)
			{
				if (writeLog)
					std::cout << " -> selected\n";
				
				if (fittestChromosome)
					delete fittestChromosome;
				fittestChromosome = newChromosome;
				bestFitness = fitness;
			}
			else
			{
				if (writeLog)
					std::cout << " -> rejected\n";
				delete newChromosome;
			}
		}
		else // Solution found
		{
			if (fittestChromosome)
				delete fittestChromosome;
			fittestChromosome = newChromosome;
			
			solutionFound = true;
		}
		
		if (writeLog)
			std::cout << std::endl;
		
		++generation;
	}
	
	std::cout << "Target number: " << targetNumber << '\n'
		<< "Solution " << (solutionFound ? "" : "not ") << "found after " << generation << " generations" << std::endl;
	
	if (fittestChromosome)
	{
		if (solutionFound)
			fittestChromosome->displayInfo();
		
		delete fittestChromosome;
	}
}

void runSimulation()
{
	int genesMinNumber = 15;
	int genesMaxNumber = 19;
	int maxGenerations = 1000000; // 0 -> no limit
	int targetNumber = getTarget(1, 500);
	int logThreshold = 10000;
	
	std::cout << "Target number: " << targetNumber << std::endl;
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	
	startGenerations(targetNumber, maxGenerations, genesMinNumber, genesMaxNumber, logThreshold);
	
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(t2 - t1);
	
	std::cout << "\nDuration: " << ms.count() << " milliseconds" << std::endl;
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	runSimulation();
	
	return 0;
}
