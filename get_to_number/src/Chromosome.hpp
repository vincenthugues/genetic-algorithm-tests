#include <string>

#ifndef CHROMOSOME_HPP
# define CHROMOSOME_HPP

static const char *geneSequences[15] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", NULL };
static const char genes[15] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '\0' };

class Chromosome {
	public:
		Chromosome(int geneCountMin = 1, int geneCountMax = 11);
		Chromosome(const Chromosome &other);
		~Chromosome();
		
		void displayInfo() const;
		
		const uint& getGeneCount() const;
		const std::string& getEncoded() const;
		const std::string& getSanitized() const;
		
	protected:
		void init(const std::string* otherEncoded = NULL);
		
		// Creates random gene sequences from scratch
		void encode();
		
		// Generates gene sequences starting from a previously generated chromosome, with possible random mutations
		void encodeNextGen(const std::string& prevChromosome);
		
		// Generates a string of decoded gene characters from an encoded chromosome
		void decode();
		
		// Generates a usable mathematical expression
		void sanitize();
		
		// Returns the gene character (e.g. '3') corresponding to a gene sequence (e.g. "0011")
		char getGeneChar(const std::string& geneSequence);
		
	protected:
		uint m_geneCount;
		std::string m_encodedStr;
		std::string m_decodedStr;
		std::string m_sanitizedStr;
};

#endif
