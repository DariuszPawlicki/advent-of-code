#include <span>
#include <stack>
#include <ranges>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <utility>
#include <iterator>
#include <iostream>


struct CratesMovement
{
	uint32_t how_many;
	uint32_t from;
	uint32_t to;
};


using CratesStacks = std::vector<std::stack<char>>;
using CratesMovements = std::vector<CratesMovement>;


CratesStacks parseLinesToCratesStacks(std::span<std::string> lines)
{
	uint32_t stacks_count = (lines[0].length() + 1) / 4;

	CratesStacks crates_stacks(stacks_count);

	for (auto& line : lines | std::views::reverse | std::views::drop(1))
	{
		for (size_t i = 1; i < line.length(); i += 4)
		{
			if (line[i] >= 'A' && line[i] <= 'Z')
			{
				crates_stacks[(i - 1) / 4].push(line[i]);
			}
		}
	}

	return crates_stacks;
}


std::vector<CratesMovement> parseLinesToCratesMovements(std::span<std::string> lines)
{
	std::vector<CratesMovement> crates_movements;

	for (auto& line : lines)
	{
		uint32_t how_many, from, to;

		std::sscanf(line.c_str(), "move %d from %d to %d", &how_many, &from, &to);

		crates_movements.emplace_back(how_many, from - 1, to - 1);
	}

	return crates_movements;
}


void whatCratesOnTopAfterRearrangePartI(CratesStacks crates_stacks, const CratesMovements& crates_movements)
{
	for (auto& movement : crates_movements)
	{
		for (size_t i = 0; i < movement.how_many; ++i)
		{
			char crate_item = crates_stacks[movement.from].top();

			crates_stacks[movement.from].pop();

			crates_stacks[movement.to].push(crate_item);
		}
	}

	for (auto& crate : crates_stacks)
		std::cout << crate.top();

	std::cout << std::endl;
}


void whatCratesOnTopAfterRearrangePartII(CratesStacks crates_stacks, const CratesMovements& crates_movements)
{
	for (auto& movement : crates_movements)
	{
		std::vector<char> crate_items;

		for (size_t i = 0; i < movement.how_many; ++i)
		{
			crate_items.push_back(crates_stacks[movement.from].top());

			crates_stacks[movement.from].pop();
		}

		for (char item : crate_items | std::views::reverse)
			crates_stacks[movement.to].push(item);
	}

	for (auto& crate : crates_stacks)
		std::cout << crate.top();

	std::cout << std::endl;
}


std::pair<CratesStacks, CratesMovements> parseData(const std::string& file_path)
{
	std::ifstream data{ file_path };

	std::vector<std::string> lines;

	std::string line;
	while (std::getline(data, line))
	{
		lines.push_back(line);
	}

	auto section_separate_iterator = std::ranges::find_if(lines, [](const std::string& line) {
		return line == "";
		});

	auto crates_lines = std::span(lines.begin(), section_separate_iterator);
	auto moves_lines = std::span(section_separate_iterator + 1, lines.end());

	CratesStacks crates_stacks = parseLinesToCratesStacks(crates_lines);
	std::vector<CratesMovement> crates_movements = parseLinesToCratesMovements(moves_lines);

	return { crates_stacks, crates_movements };
}


int main()
{
	auto [crates_stacks, crates_movements] = parseData("./data.txt");

	whatCratesOnTopAfterRearrangePartI(crates_stacks, crates_movements);
	whatCratesOnTopAfterRearrangePartII(crates_stacks, crates_movements);
}