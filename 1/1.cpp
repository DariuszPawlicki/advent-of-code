#include <string> 
#include <vector>
#include <ranges>
#include <format>
#include <fstream>
#include <numeric>
#include <iostream>


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

// Part 1

void printMaxCalories(std::vector<std::vector<int>> all_elves_calories)
{
	std::cout << std::format("Max calories: {}\n", std::ranges::max(
		all_elves_calories | std::views::transform([](auto& cals) {
			return std::accumulate(cals.begin(), cals.end(), 0);
			})
	));
}

// Part 2

void printFirstNMaxCalories(size_t n, const std::vector<std::vector<int>>& all_elves_calories)
{
	std::vector<int> summed_calories(all_elves_calories.size());

	std::ranges::transform(all_elves_calories, summed_calories.begin(), [](auto& cals) {
		return std::accumulate(cals.begin(), cals.end(), 0);
		});

	std::ranges::sort(summed_calories, std::greater());

	int sum{ 0 };
	for (auto cals : summed_calories | std::views::take(3))
		sum += cals;

	std::cout << std::format("Top 3 calories sum: {}\n", sum);
}

int main() {
	auto all_elves_calories = parseData("./data.txt");

	printMaxCalories(all_elves_calories);

	printFirstNMaxCalories(3, all_elves_calories);

	return 0;
}