#include <array>
#include <vector>
#include <string>
#include <ranges>
#include <fstream>
#include <iostream>
#include <algorithm>


std::unique_ptr<std::vector<std::string>> parseData(const std::string& file_path)
{
	auto rucksacks = std::make_unique<std::vector<std::string>>();

	std::ifstream data{ file_path };

	std::string line;
	while (std::getline(data, line))
	{
		rucksacks->push_back(line);
	}

	return std::move(rucksacks);
}


std::array<int, 52> countItemsInCompartment(const std::string& compartment)
{
	std::array<int, 52> items_in_compartment{ 0 };

	auto change_character_capitality = [](auto c) {
		return std::islower(c) ? std::toupper(c) : std::tolower(c);
	};

	int item_num{ 0 };
	for (auto item : compartment | std::views::transform(change_character_capitality))
	{
		item_num = std::islower(item) ? item - 'A' - 6 : item - 'A';

		items_in_compartment[item_num] += 1;
	}

	return items_in_compartment;
}


int calculatePriorityPointsPartI(const std::string& rucksack_items)
{
	size_t half_of_items = rucksack_items.length() / 2;

	std::array<int, 52> first_comp = countItemsInCompartment(rucksack_items.substr(0, half_of_items));
	std::array<int, 52> second_comp = countItemsInCompartment(rucksack_items.substr(half_of_items));

	for (size_t i = 0; i < 52; ++i)
	{
		if (first_comp[i] >= 1 && second_comp[i] >= 1)
			return i + 1;
	}
}


int calculatePriorityPointsPartII(std::vector<std::string> rucksacks_group)
{
	std::array<int, 52> first_elf = countItemsInCompartment(rucksacks_group[0]);
	std::array<int, 52> second_elf = countItemsInCompartment(rucksacks_group[1]);
	std::array<int, 52> third_elf = countItemsInCompartment(rucksacks_group[2]);

	for (size_t i = 0; i < 52; ++i)
	{
		if (first_elf[i] >= 1 && second_elf[i] >= 1 && third_elf[i] >= 1)
			return i + 1;
	}
}


int main()
{
	auto rucksacks = parseData("./data.txt");

	int total_priority_part_1{ 0 };

	for (const auto& rucksack : *rucksacks)
		total_priority_part_1 += calculatePriorityPointsPartI(rucksack);

	int total_priority_part_2{ 0 };
	auto rucksacks_group_start = rucksacks->begin();
	auto rucksacks_group_end = rucksacks_group_start;
	while (true)
	{
		rucksacks_group_end = rucksacks_group_start;
		std::advance(rucksacks_group_end, 3);

		total_priority_part_2 += calculatePriorityPointsPartII({ rucksacks_group_start, rucksacks_group_end });

		if (rucksacks_group_start + 3 == rucksacks->end())
			break;

		rucksacks_group_start = rucksacks_group_end;
	}

	std::cout << "Total priority part I: " << total_priority_part_1 << std::endl;
	std::cout << "Total priority part II: " << total_priority_part_2 << std::endl;

	return 0;
}