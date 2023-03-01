#include "../include/Chip8.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>


//---------------------------TEST FUNCTION------------------------------------------
void summer(char const* filename){
    std::cout << "*********SUMMER**********\n";
    uint8_t memory[4096]{};
    // Open the file as a stream of binary and move the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    std::cout << file.is_open() <<"\n";
    if (file.is_open()){

        // Get size of file and allocate a buffer to hold the contents
        file.seekg (0, file.end);
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        // Go back to the beginning of the file and fill the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Load the ROM contents into the Chip8's memory, starting at 0x200
        for (long i = 0; i < size; ++i){
            memory[0x200 + i] = buffer[i];
        }

        // Free the buffer
        delete[] buffer;
    }
}
//----------------------------------------------------------------------------------

//CONSTANTS
const unsigned int START_ADDRESS = 0x200; // Program memory start adress
const unsigned int FONTSET_SIZE  = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	// Initialize PC
	pc = START_ADDRESS;

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    //initialize RNG
    randByte = std::uniform_int_distribution<uint8_t>(0,255U);
}

void Chip8::LoadROM(char const* filename){
    
    // Open the file as a stream of binary and move the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()){

        // Get size of file and allocate a buffer to hold the contents
        file.seekg (0, file.end); //tellg() isn't reliable to get size of the file try better approach that is why we first go to the end (file.end)
        std::streampos size = file.tellg(); 
        char* buffer = new char[size];

        // Go back to the beginning of the file and fill the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Load the ROM contents into the Chip8's memory, starting at 0x200
        for (long i = 0; i < size; ++i){
            memory[START_ADDRESS + i] = buffer[i];
        }

        // Free the buffer
        delete[] buffer;
    }
}
