#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


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

	for (auto i{ 0 }; i < line.size(); ++i) {
		if (std::isdigit(line[i])) {
			digits.push_back(line[i] - '0');
		}
		else {
			for (auto j{ 0 }; j < digit_words.size(); ++j) {
				auto digit_word{ digit_words[j] };
				auto digit_word_length{ std::strlen(digit_word) };

				if (line.substr(i, digit_word_length) == digit_word) {
					digits.push_back(j);
					i += digit_word_length - 2; // In case of strings like "twone"
											   //  next iteration would begin from one before last letter of word
				}
			}
		}
	}

	return digits;
}

unsigned int extractFirstAndLastDigitsSumFromCalibrationLine(const std::string& calibration_line) {
	auto digits_in_line{ extractDigitsFromLine(calibration_line) };
	return (*digits_in_line.begin() * 10) + *(digits_in_line.end() - 1);
}

int main() {
	auto calibration_lines{ parseData("./data.txt") };

	unsigned int total_sum{ 0 };
	for (const auto& calibration_line : calibration_lines) {
		total_sum += extractFirstAndLastDigitsSumFromCalibrationLine(calibration_line);
	}

	std::cout << "Total calibration values sum: " << total_sum << '\n';
}