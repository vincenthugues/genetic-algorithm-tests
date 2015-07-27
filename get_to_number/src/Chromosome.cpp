#include <iostream>

#include "Chromosome.hpp"

Chromosome::Chromosome(int genesMinNumber, int genesMaxNumber)
{
	m_genesNumber = rand() % (genesMaxNumber - genesMinNumber + 1) + genesMinNumber;
	init();
}

Chromosome::Chromosome(const Chromosome &other)
{
	m_genesNumber = other.getGenesNumber();
	std::string otherEncoded = other.getEncoded();
	init(&otherEncoded);
}

Chromosome::~Chromosome()
{
	
}

void Chromosome::displayInfo() const
{
	std::cout << "Genes number: " << m_genesNumber
		<< "\nEncoded chromosome: " << m_encodedStr
		<< "\nDecoded chromosome: " << m_decodedStr
		<< "\nSanitized expression: " << m_sanitizedStr << std::endl;
}

const uint& Chromosome::getGenesNumber() const
{
	return m_genesNumber;
}

const std::string& Chromosome::getEncoded() const
{
	return m_encodedStr;
}

const std::string& Chromosome::getSanitized() const
{
	return m_sanitizedStr;
}

void Chromosome::init(const std::string* otherEncoded)
{
	if (otherEncoded)
		this->encodeNextGen(*otherEncoded);
	else
		this->encode();
	
	this->decode();
	this->sanitize();
}

void Chromosome::encode()
{
	for (uint i = 0; i < 4 * m_genesNumber; ++i)
		m_encodedStr += '0' + rand() % 2;
}

void Chromosome::encodeNextGen(const std::string& prevChromosome)
{
	const float mutationRate = 0.001;
	int mutationsNumber = 0;
	
	for (uint i = 0; i < prevChromosome.size(); ++i)
	{
		char bit = prevChromosome[i];
		if (rand() % 101 <= (mutationRate * 100))
		{
			bit = (bit == '0' ? '1' : '0');
			++mutationsNumber;
		}
		m_encodedStr += bit;
	}
	// std::cout << "Mutations: " << mutationsNumber << '\n';
}

void Chromosome::decode()
{
	for (uint i = 0; i < m_encodedStr.size(); ) {
		std::string geneBits;
		for (int j = 0; j < 4; ++j)
			geneBits += m_encodedStr[i++];
		char geneChar = this->getGeneChar(geneBits);
		m_decodedStr += geneChar ? geneChar : '?';
	}
}

void Chromosome::sanitize()
{
	static const std::string operands = "0123456789";
	static const std::string operators = "+-*/";
	
	for (uint i = 0; i < m_decodedStr.size(); i++) {
		char geneChar = m_decodedStr[i];
		
		bool isOperand = (operands.find(geneChar) != std::string::npos);
		bool isOperator = !isOperand && (operators.find(geneChar) != std::string::npos);
		
		bool isLastOperand = (operands.find(m_sanitizedStr.back()) != std::string::npos);
		bool isLastOperator = (operators.find(m_sanitizedStr.back()) != std::string::npos);
		
		if ((isOperand && (m_sanitizedStr.size() == 0 || (isLastOperator && !(m_sanitizedStr.back() == '/' && geneChar == '0')))) // Operand as first character or after an operator (if not division by zero)
			|| (isOperator && isLastOperand)) // Operator after an operand
			m_sanitizedStr += geneChar;
	}
	
	if (operators.find(m_sanitizedStr.back()) != std::string::npos) // if last character is an operator, remove it
		m_sanitizedStr.pop_back();
	else if (m_sanitizedStr.size() == 0)
		m_sanitizedStr = "0";
}

char Chromosome::getGeneChar(const std::string& geneSequence)
{
	for (int i = 0; geneSequences[i]; ++i)
		if (geneSequences[i] == geneSequence)
			return genes[i];
	return '\0';
}
