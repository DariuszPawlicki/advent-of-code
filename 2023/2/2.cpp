#include "Utils.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


struct GameCubesCount {
	std::size_t red;
	std::size_t green;
	std::size_t blue;
};

template<typename Container>
class GamesParser : public LineParser<Container> {
public:
	void parseLine(const std::string& line) override {
		auto colon_pos{ line.find(':') };

		auto cubes_subsets{ line.substr(colon_pos + 2, line.size()) };

		auto semicolons_positions{ findAllSemicolonsPositions(cubes_subsets) };
		auto subgames_parts{ getSubgamesParts(cubes_subsets, semicolons_positions) };
	}

private:
	std::vector<std::size_t> findAllSemicolonsPositions(const std::string& cubes_subsets) const {
		std::vector<std::size_t> semicolons_positions;

		auto i{ 0 };
		std::size_t semicolon_pos{ 0 };
		while (i < cubes_subsets.size()) {
			semicolon_pos = cubes_subsets.find(';', i);

			if (semicolon_pos != std::string::npos) {
				semicolons_positions.push_back(semicolon_pos);
				i = semicolon_pos + 1;
			}
			else {
				break;
			}
		}

		return semicolons_positions;
	}

	std::vector<std::string> getSubgamesParts(const std::string& cubes_subsets,
		const std::vector<std::size_t>& semicolons_positions) const {
		std::vector<std::string> subgames_parts;

		std::size_t cur_game_begin{ 0 };
		for (const auto& semicolon_pos : semicolons_positions) {
			subgames_parts.push_back(cubes_subsets.substr(cur_game_begin, semicolon_pos - cur_game_begin));
			cur_game_begin = semicolon_pos + 2;
		}

		subgames_parts.push_back(cubes_subsets.substr(cur_game_begin, cubes_subsets.size()));

		return subgames_parts;
	}

	GameCubesCount countCubesInAllSubgames(const std::vector<std::string>& subgames_parts) const {
		GameCubesCount cubes_count;


	}

};


int main() {
	using ParsedDataContainer = std::vector<GameCubesCount>;

	std::unique_ptr<LineParser<ParsedDataContainer>> lines_parser{
		std::make_unique<GamesParser<ParsedDataContainer>>()
	};

	parseData<ParsedDataContainer>("./data.txt", std::make_unique<GamesParser<ParsedDataContainer>>());
}