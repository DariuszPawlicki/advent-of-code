#pragma once

#include <memory>
#include <string>
#include <filesystem>


template<typename Container>
class LineParser {
public:
	LineParser() = default;
	virtual ~LineParser() = default;

	virtual void parseLine(const std::string& line) = 0;

	Container getParsedData() const {
		return parsed_data;
	}

protected:
	Container parsed_data;
};

template<typename Container>
Container parseData(const std::filesystem::path& filepath,
	const std::unique_ptr<LineParser<Container>>& line_parser) {

	std::ifstream data{ filepath };

	std::string line;
	while (std::getline(data, line)) {
		line_parser->parseLine(line);
	}

	return line_parser->getParsedData();
}