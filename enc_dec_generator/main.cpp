#include <Windows.h>
#include <iostream>
#include <fstream>
#include <random>
#include <boost/random.hpp>

#include "bitvise.h"

typedef struct large_integer
{
    unsigned int lower;
    unsigned int upper;

    large_integer(unsigned int lower, unsigned int upper)
    {
        this->lower = lower;
        this->upper = upper;
    }

    auto get() -> uintptr_t
    {
        return *(uintptr_t*)(this);
    }
};

bool write_file_text(const std::string& path, std::string& in_buffer)
{
    std::ofstream file_stream(path, std::ofstream::out);
    if (!file_stream)
        return false;

    file_stream << in_buffer;
    file_stream.close();
    return true;
}

void get_new_routine(int id, std::string& out_encoding, std::string& out_decoding, bool x64)
{
    const auto rand_number = []() -> unsigned int
    {
        std::random_device dev;
        boost::random::mt19937 rng(dev());
        boost::random::uniform_int_distribution<> f(INT_MAX / 2, INT_MAX);
        return f(rng);
    };

    const auto rand_number_between = [](int min, int max) -> int
    {
        std::random_device dev;
        boost::random::mt19937 rng(dev());
        boost::random::uniform_int_distribution<> f(min, max);
        return f(rng);
    };

    auto A = large_integer(rand_number(), rand_number()).get();
    auto B = large_integer(rand_number(), rand_number()).get();
    auto C = rand_number_between(INT_MIN, INT_MAX);
    auto roll_left = static_cast<bool>(rand_number_between(0, 1));
    //std::printf("A: 0x%p, B: 0x%p, C: 0x%X \n", A, B, C);

    char buffer_encoding[255];
    sprintf_s(buffer_encoding, "#define ENC_%s_%d(x) (__RO%s%s__(x + (0x%p ^ 0x%p), 0x%X))", 
        x64 ? "64" : "32",
        id, 
        roll_left ? "L" : "R", 
        x64 ? "8" : "4", 
        A, B, C);

    char buffer_decoding[255];
    sprintf_s(buffer_decoding, "#define DEC_%s_%d(x) (__RO%s%s__(x, 0x%X) - (0x%p ^ 0x%p))", 
        x64 ? "64" : "32",
        id, 
        roll_left ? "R" : "L", 
        x64 ? "8" : "4", 
        C, A, B);

    out_encoding = buffer_encoding;
    out_decoding = buffer_decoding;
}

int main()
{
    // Determine the path of the file we'll be writing to
    const auto new_file_path = []() -> std::string
    {
        char current_path[MAX_PATH];
        GetModuleFileNameA(NULL, current_path, sizeof(current_path));
        return std::string(current_path).substr(0, std::string(current_path).find_last_of("/\\")) + "\\encryption.h";
    };

    // Buffer to store the file content
    std::string buffer;

    // Default content
    buffer.append("#pragma once\n");
    buffer.append("#include \"stdafx.h\"\n\n");

    // Generate content for x86
    for (auto i = 1; i < 20; i++)
    {
        std::string routine_encoding, routine_decoding;
        get_new_routine(i, routine_encoding, routine_decoding, false);

        buffer.append(routine_encoding + "\n");
        buffer.append(routine_decoding + "\n\n");
    }

    // Generate content for x64
    for (auto i = 1; i < 20; i++)
    {
        std::string routine_encoding, routine_decoding;
        get_new_routine(i, routine_encoding, routine_decoding, true);

        buffer.append(routine_encoding + "\n");
        buffer.append(routine_decoding + "\n\n");
    }

    // Find file path
    auto path = new_file_path();
    std::printf("writing to %s\n", path.c_str());

    // Write to disk
    if (!write_file_text(path, buffer))
    {
        std::printf("unable to write to disk\n");
        std::cin.get();
        return -1;
    }

    // Ok
    std::printf("file written to disk successfully\n");
    std::cin.get();
    return 0;
}
