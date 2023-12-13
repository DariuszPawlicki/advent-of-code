#include <set>
#include <ranges>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <functional>
#include <unordered_map>


using Movement = std::pair<char, size_t>;


struct Position
{
	int x{ 0 };
	int y{ 0 };

	Position operator+(const Position& rhs) const {
		return { x + rhs.x, y + rhs.y };
	}

	Position& operator+=(const Position& rhs) {
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Position operator-(const Position& rhs) const {
		return { x - rhs.x, y - rhs.y };
	}
};


std::vector<Movement> parseData(const std::string& file_path) {
	std::ifstream data{ file_path };

	std::vector<Movement> movements;

	std::string line;
	while (std::getline(data, line)) {
		movements.push_back({ line[0], std::stoi(line.substr(2)) });
	}

	return movements;
}


void moveTailToNewPositionIfPossible(const Position& head, Position& tail) {

	Position distance{ head - tail };



}


size_t countFieldsVisitedPartI(const std::vector<Movement>& movements) {
	std::set<Position> fields_visited;

	static std::unordered_map <char, std::function<void(Position&, int)>> movement_handlers{
		{'U',[](Position& pos, int steps) { pos.y += steps; }},
		{'D', [](Position& pos, int steps) { pos.y -= steps; }},
		{'R', [](Position& pos, int steps) {pos.x += steps; }},
		{'L', [](Position& pos, int steps) {pos.x -= steps; }}
	};

	Position head_position{};
	Position tail_position{};

	for (const auto& movement : movements) {
		movement_handlers[movement.first](head_position, movement.second);

		fields_visited.insert(head_position);
	}

	return fields_visited.size();
}


int main()
{
	// auto movements = parseData("./data.txt");
	auto movements = parseData(R"(C:\Users\Darek\Desktop\Programowanie\advent-of-code\2022\9\test.txt)");

	std::cout << "Fields visited at least once: " << countFieldsVisitedPartI(movements) << std::endl;

	return 0;
}