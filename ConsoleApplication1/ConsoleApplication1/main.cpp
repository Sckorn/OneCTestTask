#include <iostream>
#include <boost/program_options.hpp>

#include <map>
#include <fstream>

std::map<std::string, int> countWords(const std::string &file_path);
std::string trimWord(std::string word);

int main(int argc, char * argv[])
{
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("input,i", po::value<std::string>(), "input file with text")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}


	if (!vm.count("input"))
	{
		std::cerr << "No input file specified!" << std::endl;
		return 1;
	}

	try
	{
		auto words_map = countWords(vm["input"].as<std::string>());
		for (const auto &word : words_map)
		{
			std::cout << word.first << " - " << word.second << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception thrwon: " << e.what() << std::endl;
	}

	return 0;
}

std::string trimWord(std::string word)
{
	constexpr char to_trim[] = { ',', '.', ':', ';', '-', '!', '?', '\'' };
	for (auto c : to_trim)
	{
		auto pos = word.find(c);

		if (pos == std::string::npos)
			continue;

		if (c == '\'' && (pos != word.length() - 1))
			continue;

		word = word.erase(pos, 1);
	}

	return word;
}

std::map<std::string, int> countWords(const std::string &file_path)
{
	std::map<std::string, int> ret;

	std::ifstream f(file_path);
	if (!f)
		throw std::runtime_error("File doesn't exist or is malformed!");

	while (!f.eof())
	{
		std::string word;
		f >> word;
		word = trimWord(word);
		if (ret.count(word))
			++ret[word];
		else
			ret.emplace(word, 1);
	}

	return ret;
}