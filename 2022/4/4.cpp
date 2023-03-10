#include <vector>
#include <ranges>
#include <format>
#include <fstream>
#include <iostream>
#include <filesystem>


using Range = std::pair<int, int>;
using RangesPair = std::pair<Range, Range>;


std::vector<RangesPair> parseData(const std::filesystem::path& path)
{
	std::vector<RangesPair> pairs;

	std::ifstream data{ path };

	std::string line;
	while (std::getline(data, line))
	{
		Range first_range;
		Range second_range;

		std::sscanf(line.c_str(), "%d-%d,%d-%d", &first_range.first, &first_range.second,
			&second_range.first, &second_range.second);

		pairs.emplace_back(first_range, second_range);
	}

	return pairs;
}


int areRangesFullyOverlappedPartI(const RangesPair& pair)
{
	auto [first_range, second_range] = pair;

	return (first_range.first <= second_range.first && first_range.second >= second_range.second) ||
		(second_range.first <= first_range.first && second_range.second >= first_range.second);
}


int areRangesOverlappedAtAllPartII(const RangesPair& pair)
{
	auto [first_range, second_range] = pair;

	return first_range.first <= second_range.second && second_range.first <= first_range.second;
}


int main()
{
	auto pairs = parseData("./data.txt");

	std::cout << std::format("Totally overlapped ranges count: {}\n", std::ranges::count_if(pairs, areRangesFullyOverlappedPartI));
	std::cout << std::format("Partially overlapped ranges count: {}\n", std::ranges::count_if(pairs, areRangesOverlappedAtAllPartII));

	return 0;
}