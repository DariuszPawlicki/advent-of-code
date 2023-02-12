#include <string> 
#include <vector>
#include <ranges>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>


std::vector<std::vector<int>> parseData(const std::string& file_path) {
	std::vector<std::vector<int>> all_elves_calories;
	std::vector<int> elf_calories;

	std::ifstream data{ file_path };

	std::string line;

	while (std::getline(data, line)) {
		if (line != "") {
			elf_calories.push_back(std::stoi(line));
		}
		else {
			all_elves_calories.push_back(elf_calories);
			elf_calories.clear();
		}
	}

	all_elves_calories.push_back(elf_calories);

	return all_elves_calories;
}

int findMaxCalories(std::vector<std::vector<int>> all_elves_calories)
{
	return std::ranges::max(
		all_elves_calories | std::views::transform([](auto& cals) {
			return std::accumulate(cals.begin(), cals.end(), 0);
			})
	);
}

int main() {
	auto all_elves_calories = parseData("./data.txt");

	std::cout << findMaxCalories(all_elves_calories) << std::endl;

	return 0;
}