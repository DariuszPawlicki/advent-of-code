#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <fstream>
#include <iostream>
#include <algorithm>


std::vector<std::vector<uint32_t>> parseData(const std::string& file_path)
{
	std::vector<std::vector<uint32_t>> tree_grid;

	std::fstream data{ file_path };

	std::string line;
	while (std::getline(data, line))
	{
		std::vector<uint32_t> tree_line;

		std::ranges::for_each(line, [&tree_line](char c) {
			tree_line.push_back(c - '0');
			});

		tree_grid.push_back(std::move(tree_line));
	}

	return tree_grid;
}


bool checkIfVisibleInLine(const std::vector<uint32_t>& tree_line, int tree_idx)
{
	for (int i = 0; i < tree_line.size(); ++i)
	{
		if (i == tree_idx)
			return true;

		if (tree_line[i] >= tree_line[tree_idx])
			break;
	}

	for (int i = tree_line.size() - 1; i >= tree_idx; --i)
	{
		if (i == tree_idx)
			return true;

		if (tree_line[i] >= tree_line[tree_idx])
			return false;
	}
}


std::vector<uint32_t> getColumnFromGrid(const std::vector<std::vector<uint32_t>>& tree_grid, int col)
{
	std::vector<uint32_t> tree_column;

	for (int i = 0; i < tree_grid.size(); ++i)
		tree_column.push_back(tree_grid[i][col]);

	return tree_column;
}


size_t getScenicViewForLine(const std::vector<uint32_t>& tree_line, int tree_idx)
{
	size_t scenic_left{ 0 };

	for (int i = tree_idx - 1; i >= 0; --i)
	{
		++scenic_left;

		if (tree_line[i] >= tree_line[tree_idx])
			break;
	}

	size_t scenic_right{ 0 };

	for (int i = tree_idx + 1; i < tree_line.size(); ++i)
	{
		++scenic_right;

		if (tree_line[i] >= tree_line[tree_idx])
			break;
	}

	return scenic_left * scenic_right;
}


size_t countVisibleTreesPartI(const std::vector<std::vector<uint32_t>>& tree_grid)
{
	size_t visible_trees_count{ 0 };

	for (int i = 1; i < tree_grid.size() - 1; ++i)
	{
		for (int j = 1; j < tree_grid[0].size() - 1; ++j)
		{
			if (checkIfVisibleInLine(tree_grid[i], j) || checkIfVisibleInLine(getColumnFromGrid(tree_grid, j), i))
				++visible_trees_count;
		}
	}

	size_t trees_on_edges = tree_grid.size() * 2;
	trees_on_edges += (tree_grid[0].size() * 2) - 4;

	return visible_trees_count + trees_on_edges;
}


size_t calculateHighestScenicScorePartII(const std::vector<std::vector<uint32_t>>& tree_grid)
{
	size_t highest_scenic{ 0 };

	for (int i = 1; i < tree_grid.size() - 1; ++i)
	{
		for (int j = 1; j < tree_grid[0].size() - 1; ++j)
		{
			size_t horizontal_scenic = getScenicViewForLine(tree_grid[i], j);
			size_t vertical_scenic = getScenicViewForLine(getColumnFromGrid(tree_grid, j), i);
			size_t total_scenic = horizontal_scenic * vertical_scenic;

			if (total_scenic > highest_scenic)
				highest_scenic = total_scenic;
		}
	}

	return highest_scenic;
}


int main()
{
	auto tree_grid = parseData("./data.txt");

	std::cout << std::format("Visible trees in grid: {}\n", countVisibleTreesPartI(tree_grid));
	std::cout << std::format("Highest scenic in grid: {}\n", calculateHighestScenicScorePartII(tree_grid));

	return 0;
}