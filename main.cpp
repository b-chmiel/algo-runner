#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const std::string compiler_path = "/usr/bin/clang++";
const std::string flags = "-Wall -Wextra -pedantic -std=c++17 -O2 -Wshadow -Wformat=2 -Wfloat-equal "
                          "-Wconversion -Wshift-overflow "
                          "-Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC "
                          "-D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined "
                          "-fno-sanitize-recover -fstack-protector";

std::string exec(const std::string& cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
        pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

void compile_source(const std::string& source_name)
{
    std::stringstream cmd;
    cmd << compiler_path << ' ' << flags << ' ' << source_name;
    std::cout << exec(cmd.str());
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr
            << "Too few arguments. Required: [source file .cpp] [...in] [..out]"
            << std::endl;
        return 1;
    }

    compile_source(argv[1]);

    const int in_out_count = argc - 2;

    if (in_out_count % 2 != 0)
    {
        std::cerr << "Number of in and out files must be even." << std::endl;
        return 1;
    }

    for (auto i = 2; i + (in_out_count / 2) < argc; i += 2)
    {
        const std::string in = argv[i];
        const std::string out = argv[i + (in_out_count / 2)];

        std::cout << exec("./a.out < " + in + " | diff -c " + out + " -");
    }

    return 0;
}
