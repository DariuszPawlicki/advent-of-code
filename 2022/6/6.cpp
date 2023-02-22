#include <set>
#include <format>
#include <string>
#include <fstream>
#include <iostream>


std::string loadData(const std::string& file_path)
{
	std::ifstream file{ file_path };

	std::string signal;
	std::getline(file, signal);

	return signal;
}


uint32_t findStartMarkerPositionPartIAndPartII(const std::string& signal, uint32_t marker_size)
{
	auto cur_begin = signal.begin();

	while (cur_begin < signal.end())
	{
		auto cur_end = cur_begin + marker_size;

		std::string cur_marker{ cur_begin, cur_end };

		if (std::set<char>{cur_begin, cur_end}.size() == marker_size)
			return std::distance(signal.begin(), cur_end);

		cur_begin += 1;
	}
}


int main()
{
	std::string signal = loadData("./data.txt");

	std::cout << std::format("Start Marker part I: {}", findStartMarkerPositionPartIAndPartII(signal, 4)) << std::endl;
	std::cout << std::format("Message Start Marker part II: {}", findStartMarkerPositionPartIAndPartII(signal, 14)) << std::endl;
}