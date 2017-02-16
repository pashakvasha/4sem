#include <map>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#define TOP 10
#define MAX_WORDS 1000

char punctiation[] = {'.', ',', '!', ':', ';', '?'};

bool is_punctuation(char last_symbol) {
	for (int j = 0; j < sizeof(punctiation) / sizeof(char); j++) 
	{
		if (last_symbol == punctiation[j])
			return true;
	}
	return false;
}

std::string prepare(const std::string& s)
{
	std::string result = s;
	int last_symbol = s.size() - 1;
	std::transform(s.begin(), s.end(), result.begin(), ::tolower);
	if ( is_punctuation( result[last_symbol] ) ) {
		result.pop_back();
	}
	return result;
}


struct Statistics {
	int count;
	std::string word;
};
	
bool compare(const Statistics& a, const Statistics& b) {
	if (a.count != b.count) {
		return a.count > b.count;
	}
	return a.word < b.word;
}

int main()
{
	std::ifstream file("text.txt");
	std::map<std::string, int> m;
	if (file.is_open())
	{
		std::string word;

		while ( !file.eof() )
		{
			file >> word;
			// Почему-то иногда дублируется последнее слово в тексте. Как это исправить?
			//std::cout << word << "\n";
			word = prepare(word);
			m[word] += 1;
		}
	}
	
	file.close();
	
	std::vector<Statistics> s;
	s.resize(MAX_WORDS);
	
	int i = 0;
	
	for (auto it:m)
	{
		s[i].word = it.first;
		s[i].count = it.second;
		i++;
	}
	
	std::sort(s.begin(), s.end(), compare);

	for (i = 0; i < TOP; i++) {
		std::cout << i + 1 << ") " << s[i].word << " - " << s[i].count << "\n";
	}
	
	return 0;
}
