#pragma once

#include "hardware.h"
#include <fstream>
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>


using namespace std;

long long file_size(FILE *file)
{
    if (!file)
    {
        return 0;
    }

    fpos_t original;

    if (fgetpos(file, &original) != 0)
    {
        cout << "fgetpos() failed: " << errno << endl;
        return 0;
    }

    fseek(file, 0, SEEK_END);

    long long out = ftell(file);

    if (fsetpos(file, &original) != 0)
    {
        cout << "fsetpos() failed: " << errno << endl;
    }

    return out;
}

void Chip8::loadrom(char *filename)
{
    ifstream file(filename, ios::binary); // to use tellg(), use ios::ate

    if (file.is_open())
    {
        FILE *f = fopen(filename, "r");
        long long size = file_size(f); // could have used streampos size = file.tellg()
        char *buffer = new char[size];

        file.seekg(0, ios::beg);
        file.read(buffer, size);
        file.close();

        for (long long i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }

    else
    {
        cout << "ifstream() failed: " << errno << endl;
    }
}