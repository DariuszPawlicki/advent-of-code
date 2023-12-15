#include "Utils.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>


struct GameCubesSubset {
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
};


int main() {
	using ParsedDataContainer = std::vector<GameCubesSubset>;

	std::unique_ptr<LineParser<ParsedDataContainer>> lines_parser{ 
		std::make_unique<GamesParser<ParsedDataContainer>>() 
	};

	parseData<ParsedDataContainer>("./data.txt", std::make_unique<GamesParser<ParsedDataContainer>>());
}