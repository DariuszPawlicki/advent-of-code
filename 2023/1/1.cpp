#include <array>
#include <string>
#include <ranges>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>


constexpr std::array<const char*, 10> digit_words{
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

std::vector<std::string> parseData(const std::filesystem::path& filepath) {
	std::ifstream data{ filepath };
	std::vector<std::string> calibration_lines;

	std::string line;
	while (std::getline(data, line)) {
		calibration_lines.push_back(line);
	}

	return calibration_lines;
}

std::vector<unsigned int> extractDigitsFromLine(const std::string& line) {
	std::vector<unsigned int> digits;

	for (char c : line) {
		if (std::isdigit(c)) {
			digits.push_back(c - '0');
		}
	}

	return digits;
}

std::unordered_map<unsigned int, std::pair<std::size_t, std::size_t>> findAllDigitsWordsPositions(const std::string& line) {
	std::unordered_map<unsigned int, std::pair<std::size_t, std::size_t>> digits_words_positions;

	std::size_t lowest_pos{ std::numeric_limits<std::size_t>::max() };
	std::size_t highest_pos{ 0 };

	for (auto i{ 0 }; i < 2; ++i) {
		for (auto j{ 0 }; j < digit_words.size(); ++j) {
			std::size_t found_digit_pos;

			if (i == 0) {
				found_digit_pos = line.find(digit_words[j]);

				if (found_digit_pos != std::string::npos) {
					digits_words_positions[j].first = found_digit_pos;
				}
			}
			else {
				found_digit_pos = line.rfind(digit_words[j]);

				if (found_digit_pos != std::string::npos) {
					digits_words_positions[j].second = found_digit_pos;
				}
			}
		}
	}

	return digits_words_positions;
}

std::pair<unsigned int, unsigned int> getFirstAndLastDigitFromListOfExtractedDigitsWords(
	const std::unordered_map<unsigned int, std::pair<std::size_t, std::size_t>>& digits_words_positions) {
	auto digits_words_first_entry{ digits_words_positions.begin() };

	std::pair<unsigned int, std::size_t> digit_lowest_pos{
		digits_words_first_entry->first,
		digits_words_first_entry->second.first
	};

	std::pair<unsigned int, std::size_t> digit_highest_pos{
		digits_words_first_entry->first,
		digits_words_first_entry->second.first
	};

	for (auto it{ ++digits_words_positions.begin() }; it != digits_words_positions.end(); ++it) {
		auto digit_pos{ it->second };

		if (digit_pos.first < digit_lowest_pos.second) {
			digit_lowest_pos.first = it->first;
			digit_lowest_pos.second = digit_pos.first;
		}

		if (digit_pos.second > digit_highest_pos.second) {
			digit_highest_pos.first = it->first;
			digit_highest_pos.second = digit_pos.second;
		}
	}

	return { digit_lowest_pos.first, digit_highest_pos.first };
}

unsigned int extractFirstAndLastDigitsSumFromCalibrationLine(const std::string& calibration_line) {
	auto digits_in_line{ extractDigitsFromLine(calibration_line) };
	return (*digits_in_line.begin() * 10) + *(digits_in_line.end() - 1);
}

int main() {
	auto calibration_lines{ parseData("./data.txt") };

	unsigned int total_sum{ 0 };
	for (const auto& calibration_line : calibration_lines) {
		auto extracted{ extractFirstAndLastDigitsSumFromCalibrationLine(calibration_line) };
		auto pos{ findAllDigitsWordsPositions(calibration_line) };
		auto asdf{ getFirstAndLastDigitFromListOfExtractedDigitsWords(pos) };
		total_sum += extracted;
	}

	std::cout << "Total calibration values sum: " << total_sum << '\n';
}