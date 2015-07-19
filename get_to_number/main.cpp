#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "eval_expr.cpp"

const char *geneSequences[15] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", NULL };
const char genes[15] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '\0' };

// Returns the gene character (e.g. '3') corresponding to a gene sequence (e.g. "0011")
char getGeneChar(const std::string& geneSequence)
{
	for (int i = 0; geneSequences[i]; ++i)
		if (geneSequences[i] == geneSequence)
			return genes[i];
	return '\0';
}

// Fitness function used in selection
float getFitness(int result, int target)
{
	return 1.f / std::abs(result - target);
}

// Returns a random target number in the required range
int getTarget(int min, int max)
{
	return rand() % (min + max) + min;
}

// Creates a random chromosome (gene sequences) from scratch
void encodeChromosome(std::string& encodedChromosome, int genesNumber)
{
	for (int i = 0; i < 4 * genesNumber; ++i)
		encodedChromosome += '0' + rand() % 2;
}

// Creates a chromosome (gene sequences) starting from a previously generated chromosome, with possible random mutations
void encodeNextGenChromosome(std::string& encodedChromosome, std::string& prevChromosome)
{
	const float mutationRate = 0.001;
	int mutationsNumber = 0;
	
	for (int i = 0; i < prevChromosome.size(); ++i)
	{
		char bit = prevChromosome[i];
		if (rand() % 101 <= (mutationRate * 100))
		{
			bit = (bit == '0' ? '1' : '0');
			++mutationsNumber;
		}
		encodedChromosome += bit;
	}
	std::cout << "Mutations: " << mutationsNumber << std::endl;
}

// Creates a string of decoded gene characters from an encoded chromosome
void decodeChromosome(std::string& decodedChromosome, const std::string& encodedChromosome)
{
	for (int i = 0; i < encodedChromosome.size(); ) {
		std::string geneBits;
		for (int j = 0; j < 4; ++j)
			geneBits += encodedChromosome[i++];
		char geneChar = getGeneChar(geneBits);
		decodedChromosome += geneChar ? geneChar : '?';
	}
}

// Creates a usable mathematical expression in a string
void sanitizeExpression(std::string& sanitizedExpr, const std::string& decodedChromosome)
{
	static const std::string operands = "0123456789";
	static const std::string operators = "+-*/";
	
	for (int i = 0; i < decodedChromosome.size(); i++) {
		char geneChar = decodedChromosome[i];
		
		bool isOperand = (operands.find(geneChar) != string::npos);
		bool isOperator = !isOperand && (operators.find(geneChar) != string::npos);
		
		bool isLastOperand = (operands.find(sanitizedExpr.back()) != string::npos);
		bool isLastOperator = (operators.find(sanitizedExpr.back()) != string::npos);
		
		if ((isOperand && (sanitizedExpr.size() == 0 || (isLastOperator && !(sanitizedExpr.back() == '/' && geneChar == '0')))) // Operand as first character or after an operator (if not division by zero)
			|| (isOperator && isLastOperand)) // Operator after an operand
			sanitizedExpr += geneChar;
	}
	
	if (operators.find(sanitizedExpr.back()) != string::npos) // if last character is an operator, remove it
		sanitizedExpr.pop_back();
	else if (sanitizedExpr.size() == 0)
		sanitizedExpr = "0";
}

// Starts the algorithm with the required parameters
void startGenerations(int targetNumber, int maxGenerations, int genesMinNumber, int genesMaxNumber)
{
	bool solutionFound = false;
	int generation = 0;
	int genesNumber = rand() % (genesMaxNumber - genesMinNumber + 1) + genesMinNumber;
	
	std::cout << "Genes number: " << genesNumber << std::endl << std::endl;
	
	std::string prevChromosome;
	float prevFitness = 0;
	for (unsigned int i = 0; !solutionFound && (maxGenerations == 0 || i < maxGenerations); ++i)
	{
		std::string encodedChromosome;
		
		std::cout << "Generation " << generation + 1 << std::endl; 
		
		if (generation == 0)
			encodeChromosome(encodedChromosome, genesNumber);
		else
			encodeNextGenChromosome(encodedChromosome, prevChromosome);
		std::cout << "Encoded chromosome: " << encodedChromosome << std::endl;
		
		std::string decodedChromosome;
		decodeChromosome(decodedChromosome, encodedChromosome);
		std::cout << "Decoded chromosome: " << decodedChromosome << std::endl;
		
		std::string sanitizedExpression;
		sanitizeExpression(sanitizedExpression, decodedChromosome);
		std::cout << "Sanitized expression: " << sanitizedExpression << std::endl;
		
		int result = 0;
		eval_expr(sanitizedExpression, result);
		std::cout << "Chromosome result: " << result << std::endl;
		
		if (result != targetNumber) {
			float fitness = getFitness(result, targetNumber);
			std::cout << "Chromosome fitness: " << fitness;
			++generation;
			if (fitness > prevFitness) {
				std::cout << " -> selected" << std::endl;
				prevChromosome = encodedChromosome;
				prevFitness = fitness;
			}
			else
				std::cout << " -> rejected" << std::endl;
		}
		else
			solutionFound = true;
		
		std::cout << std::endl;
	}
	
	std::cout << "Target number: " << targetNumber << std::endl
		<< "Solution " << (solutionFound ? "" : "not ") << "found after " << generation + 1 << " generations" << std::endl;
}

// Sets up the parameters
void geneticExpressions()
{
	int genesMinNumber = 7;
	int genesMaxNumber = 11;
	int maxGenerations = 0; // 0 -> no limit
	
	int targetNumber = getTarget(1, 100);
	std::cout << "Target number: " << targetNumber << std::endl;
	
	startGenerations(targetNumber, maxGenerations, genesMinNumber, genesMaxNumber);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	geneticExpressions();
	
	return 0;
}
