#include <map>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>

const int TOP = 10;
const int MAX_WORDS = 1000;

std::string prepare(std::string& s)
{
	int size = s.size() - 1;
	//std::cout << s.size() << "\n";
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	if (s[size] == '.' || s[size] == ',' || s[size] == ':') {
		s.pop_back();
	}
	//std::cout << s << "\n";
	return s;
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
		while (!file.eof()) {
			file >> word;
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
	
	for (auto it:m)
	{
		std::cout<< it.first << "" << it.second << "\n";
	}
	
	std::sort(s.begin(), s.end(), compare);

	for (i = 0; i < TOP; i++) {
		std::cout << i + 1 << ") " << s[i].word << " - " << s[i].count << "\n";
	}

	return 0;
}
