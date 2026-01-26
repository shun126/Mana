/*!
mana (test)

@author	Shun Moriya
@date	2024-
*/

#include "../runner/ProgramImage.h"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	bool Contains(const std::vector<std::string_view>& values, const std::string_view target)
	{
		return std::find(values.begin(), values.end(), target) != values.end();
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "usage: ProgramImageTest <program.mx>" << std::endl;
		return 1;
	}

	const std::string path = argv[1];
	std::ifstream input(path, std::ios::binary);
	if (!input.is_open())
	{
		std::cerr << "failed to open program: " << path << std::endl;
		return 1;
	}

	input.seekg(0, std::ios::end);
	const std::streamsize size = input.tellg();
	input.seekg(0, std::ios::beg);
	if (size <= 0)
	{
		std::cerr << "program file is empty" << std::endl;
		return 1;
	}

	std::shared_ptr<std::byte[]> programBuffer(new std::byte[static_cast<size_t>(size)]);
	if (!input.read(reinterpret_cast<char*>(programBuffer.get()), size))
	{
		std::cerr << "failed to read program" << std::endl;
		return 1;
	}

	std::shared_ptr<const void> program(programBuffer, programBuffer.get());

	mana::ProgramImage image;
	if (!image.LoadProgram(program, static_cast<size_t>(size)))
	{
		std::cerr << "failed to load program: " << image.GetLastError() << std::endl;
		return 1;
	}

	int result = 0;
	const auto actorNames = image.GetActorNames();
	if (!Contains(actorNames, "TestActor"))
	{
		std::cerr << "missing TestActor" << std::endl;
		result = 1;
	}
	if (image.HasActor("MissingActor"))
	{
		std::cerr << "unexpected actor exists" << std::endl;
		result = 1;
	}
	if (!image.HasActorAction("TestActor", "init") || !image.HasActorAction("TestActor", "main"))
	{
		std::cerr << "missing TestActor actions" << std::endl;
		result = 1;
	}

	const auto phantomActions = image.GetPhantomActionNames("TestPhantom");
	if (!image.HasPhantom("TestPhantom") || !Contains(phantomActions, "init"))
	{
		std::cerr << "missing TestPhantom" << std::endl;
		result = 1;
	}
	if (image.HasPhantom("MissingPhantom"))
	{
		std::cerr << "unexpected phantom exists" << std::endl;
		result = 1;
	}

	{
		std::shared_ptr<std::byte[]> corruptBuffer(new std::byte[static_cast<size_t>(size)]);
		std::memcpy(corruptBuffer.get(), programBuffer.get(), static_cast<size_t>(size));
		corruptBuffer.get()[0] = std::byte{ 0 };
		std::shared_ptr<const void> corruptProgram(corruptBuffer, corruptBuffer.get());

		mana::ProgramImage corruptImage;
		if (corruptImage.LoadProgram(corruptProgram, static_cast<size_t>(size)))
		{
			std::cerr << "corrupt program should fail" << std::endl;
			result = 1;
		}
		if (corruptImage.GetLastError().empty())
		{
			std::cerr << "corrupt program missing error" << std::endl;
			result = 1;
		}
	}

	{
		mana::ProgramImage truncatedImage;
		if (truncatedImage.LoadProgram(program, 1))
		{
			std::cerr << "truncated program should fail" << std::endl;
			result = 1;
		}
		if (truncatedImage.GetLastError().empty())
		{
			std::cerr << "truncated program missing error" << std::endl;
			result = 1;
		}
	}

	return result;
}
