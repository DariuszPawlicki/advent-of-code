#include <format>
#include <memory>
#include <ranges>
#include <utility>
#include <fstream>
#include <iostream>
#include <unordered_map>


enum class Shapes {
	Rock = 1,
	Paper = 2,
	Scissors = 3
};


enum class Results {
	Lose = 0,
	Draw = 3,
	Win = 6
};


std::unordered_map<char, Shapes> letters_to_moves{
		{ 'A', Shapes::Rock },
		{ 'X', Shapes::Rock },
		{ 'B', Shapes::Paper },
		{ 'Y', Shapes::Paper },
		{ 'C', Shapes::Scissors },
		{ 'Z', Shapes::Scissors }
};


std::unordered_map<char, Results> letters_to_results{
	{'X', Results::Lose},
	{'Y', Results::Draw},
	{'Z', Results::Win}
};


std::unordered_map<Shapes, Shapes> winning_moves{
	{Shapes::Rock, Shapes::Scissors},
	{Shapes::Paper, Shapes::Rock},
	{Shapes::Scissors, Shapes::Paper}
};


std::unique_ptr<std::vector<std::pair<char, char>>> parseData(const std::string& file_path)
{
	std::unique_ptr<std::vector<std::pair<char, char>>> moves = std::make_unique<std::vector<std::pair<char, char>>>();

	std::ifstream data{ file_path };

	size_t space_pos;
	std::string line;
	while (std::getline(data, line))
	{
		space_pos = line.find(' ');

		if (line.length() > 0)
			moves->push_back({ line.substr(0, space_pos)[0],
					line.substr(space_pos + 1, line.length() - 1)[0] });
	}

	return std::move(moves);
}


int calculatePointsForMovePartI(const std::pair<char, char>& move)
{
	Shapes opponent = letters_to_moves[move.first];
	Shapes player = letters_to_moves[move.second];

	int result_points{ 0 };

	if (winning_moves[player] == opponent)
		result_points = static_cast<int>(Results::Win);
	else if (player == opponent)
		result_points = static_cast<int>(Results::Draw);
	else
		result_points = static_cast<int>(Results::Lose);

	return static_cast<int>(player) + result_points;
}


int calculatePointsForMovePartII(const std::pair<char, char>& move)
{
	Results result = letters_to_results[move.second];

	Shapes opponent = letters_to_moves[move.first];

	int points_from_shape{ 0 };

	if (result == Results::Lose)
		points_from_shape = static_cast<int>(winning_moves[opponent]);
	else if (result == Results::Draw)
		points_from_shape = static_cast<int>(opponent);
	else
		points_from_shape = static_cast<int>(std::ranges::find_if(winning_moves,
			[&opponent = opponent](auto& entry) {
				return entry.second == opponent;
			}
	)->first);

	return points_from_shape + static_cast<int>(result);
}


int main()
{
	auto moves = parseData("./data.txt");

	int total_score_part_1{ 0 };
	int total_score_part_2{ 0 };

	for (const auto& move : *moves)
	{
		total_score_part_1 += calculatePointsForMovePartI(move);
		total_score_part_2 += calculatePointsForMovePartII(move);
	}

	std::cout << std::format("Total score part I: {}\n", total_score_part_1);
	std::cout << std::format("Total score part II: {}\n", total_score_part_2);
}