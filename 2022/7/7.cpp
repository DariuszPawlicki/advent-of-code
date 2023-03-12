#include <ranges>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>


struct File
{
	size_t size;
	std::string name;
};


struct Directory
{
	std::string dir_name;
	size_t dir_size;
	std::shared_ptr<Directory> parent_dir;
	std::vector<std::shared_ptr<Directory>> directories_in_dir;
	std::vector<File> files_in_dir;

	Directory(const std::string& dir_name) : dir_name(dir_name), dir_size(0) {};

	Directory(Directory&& other) = default;

	size_t calculateDirSize() const
	{
		size_t total_size{ 0 };

		auto static sumFilesSize = [](size_t acc, const File& file) {
			return acc + file.size;
		};

		for (auto dir : directories_in_dir)
			total_size += dir->calculateDirSize();

		total_size += std::accumulate(files_in_dir.begin(), files_in_dir.end(), 0, sumFilesSize);

		return total_size;
	}

	std::vector<size_t> getAllSizesOfDirsInDir()
	{
		std::vector<size_t> sizes;
		sizes.push_back(this->calculateDirSize());

		for (auto dir : directories_in_dir)
		{
			auto sizes_of_dir_in_dir = dir->getAllSizesOfDirsInDir();

			sizes.insert_range(sizes.end(), sizes_of_dir_in_dir);
		}

		return sizes;
	}

	auto getSearchedDirIterator(const std::string& dir_name) const
	{
		auto dir_iter = std::ranges::find_if(directories_in_dir, [&dir_name](std::shared_ptr<Directory> dir)
			{
				return dir->dir_name == dir_name;
			});

		return dir_iter;
	}

	bool dirExistsInCurrent(const std::string& dir_name)
	{
		return getSearchedDirIterator(dir_name) != directories_in_dir.end();
	}

	std::shared_ptr<Directory> getDirectory(const std::string& dir_name)
	{
		auto dir_iter = getSearchedDirIterator(dir_name);

		return dir_iter != directories_in_dir.end() ? *dir_iter : nullptr;
	}

	void addDirectory(const std::string& dir_name, std::shared_ptr<Directory> previous_dir)
	{
		if (!dirExistsInCurrent(dir_name))
		{
			std::shared_ptr<Directory> new_dir = std::make_shared<Directory>(dir_name);
			new_dir->parent_dir = previous_dir;

			directories_in_dir.push_back(new_dir);
		}
	}

	void addDirectory(std::shared_ptr<Directory> dir)
	{
		if (!dirExistsInCurrent(dir->dir_name))
			directories_in_dir.push_back(dir);
	}

	void addFile(File&& file)
	{
		files_in_dir.push_back(std::move(file));
		dir_size += file.size;
	}
};


struct FileSystem
{
	std::shared_ptr<Directory> root;
	std::shared_ptr<Directory> working_directory;

	const static size_t total_disk_space{ 70000000 };

	FileSystem() : root(std::make_shared<Directory>("/")),
		working_directory(root)
	{
		root->parent_dir = root;
	};

	void totalSizeOfDirs()
	{
		auto sizes = root->getAllSizesOfDirsInDir();

		auto filtered_sizes = sizes | std::views::filter([](size_t size) {
			return size <= 100000;
			});

		std::cout << "Total size of directories which size is less than 100000: "
			<< std::accumulate(filtered_sizes.begin(), filtered_sizes.end(), 0) << std::endl;

	}

	void sizeOfDirToDelete()
	{
		auto sizes = root->getAllSizesOfDirsInDir();
		std::ranges::sort(sizes);

		size_t available_space = total_disk_space - sizes.back();

		size_t dir_to_delete_size = *std::ranges::find_if(sizes, [available_space](size_t size) {
			return (available_space + size) >= 30000000;
			});

		std::cout << "Directory to delete size: " << dir_to_delete_size << std::endl;
	}

	void changeDirectory(const std::string& dir_name)
	{
		if (dir_name == "/" || dir_name == ".")
			return;

		if (dir_name == "..")
		{
			working_directory = working_directory->parent_dir;

			return;
		}

		auto new_working_dir = working_directory->getDirectory(dir_name);

		if (new_working_dir != nullptr)
		{
			working_directory = new_working_dir;
		}
		else
			std::cout << "Cannot change directory to unexistent one!" << std::endl;
	}

	void addDirectory(const std::string& dir_name)
	{
		working_directory->addDirectory(dir_name, working_directory);
	}

	void addFileToDirectory(File&& file)
	{
		working_directory->addFile(std::move(file));
	}

	void printFileSystemSize()
	{
		std::cout << "Size of a filesystem: " << root->calculateDirSize() << std::endl;
	}
};


struct System
{
public:

	inline static FileSystem fs;

	static void executeCommands(const std::string& file_path)
	{
		std::vector<std::string> lines = loadCommands(file_path);

		for (auto line_it = lines.begin(); line_it != lines.end(); ++line_it)
		{
			std::string cur_line = *line_it;

			if (cur_line[0] == '$')
			{
				if (cur_line.find("cd") != std::string::npos)
				{
					char arg[100];
					std::sscanf(cur_line.c_str(), "$ cd %s", &arg);

					executeCommand("cd", arg);
				}
				else if (cur_line.find("ls") != std::string::npos)
				{
					++line_it;

					while (true)
					{
						executeCommand("ls", *line_it);

						if ((line_it + 1) == lines.end() || (*(line_it + 1))[0] == '$')
							break;

						++line_it;
					}
				}
			}
		}
	}

	static void printFileSystemSize()
	{
		fs.printFileSystemSize();
	}

	static void totalSizeOfDirsPartI()
	{
		fs.totalSizeOfDirs();
	}

	static void sizeOfDirToDeletePartII()
	{
		fs.sizeOfDirToDelete();
	}

private:

	static std::vector<std::string> loadCommands(const std::string& file_path)
	{
		std::vector<std::string> lines;

		std::ifstream data{ file_path };

		std::string line;
		while (std::getline(data, line))
		{
			lines.push_back(std::move(line));
		}

		return lines;
	}

	static void executeCommand(const std::string& cmd, const std::string& arg)
	{
		if (cmd == "cd")
		{
			fs.changeDirectory(arg);
		}
		else if (cmd == "ls")
		{
			if (arg.find("dir") != std::string::npos)
			{
				char dir_name[100];
				std::sscanf(arg.c_str(), "dir %s", dir_name);

				fs.addDirectory(dir_name);
			}
			else
			{
				uint32_t file_size;
				char filename[100];
				std::sscanf(arg.c_str(), "%d %s", &file_size, filename);

				fs.addFileToDirectory({ static_cast<size_t>(file_size), filename });
			}
		}
	}
};


int main()
{
	System::executeCommands("./data.txt");

	System::totalSizeOfDirsPartI();
	System::sizeOfDirToDeletePartII();

	return 1;
}