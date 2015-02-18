/*
 roll.h
 DiCLI
 
 Created by Hans Behrens on 2015-02-18.
 Copyright (c) 2015 Hans Behrens. All rights reserved.
 
 This file is part of DiCLI.
 
 DiCLI is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 DiCLI is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with DiCLI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROLL_H
#define ROLL_H

#include <random>
#include <chrono>
#include <iostream>
#include <string>

inline bool is_number(const char & input);
int extract(const std::string & input, size_t & cursor);

//init random generators
std::mt19937 rand_engine(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
std::random_device device;
std::mt19937 generator(device());
std::uniform_int_distribution<size_t> big_range(1, 100);
std::uniform_int_distribution<size_t> medium_range(1, 20);
std::uniform_int_distribution<size_t> small_range(1, 8);

class Roll{
    size_t dice_count = 0;
    size_t dice_size = 0;
    int roll_mod = 0;
    bool valid_roll = false;
    
public:
    void process_token(std::string token);
    void do_roll();
    inline bool is_valid(){return valid_roll;}
};

void Roll::process_token(std::string token){
    size_t cursor = 0;
    dice_count = extract(token, cursor);
    dice_size = extract(token, cursor);
    roll_mod = extract(token, cursor);
    valid_roll = (dice_count > 0 && dice_size >= 2 && dice_size <= 100);
}

void Roll::do_roll(){
    int result = 0;
    size_t rand = 0;
    size_t remaining_count = dice_count;
    while (remaining_count > 0){
        if (dice_size <= 8)
            rand = small_range(generator);
        else if (dice_size <= 20)
            rand = medium_range(generator);
        else
            rand = big_range(generator);
        if (rand >= 1 && rand <= dice_size){
            result += rand;
            --remaining_count;
        }
    }
    
    std::cout << "   = " << (result + roll_mod);
    
    //some special cases get extra labels
    if (dice_count == 1 && dice_size == 20){
        //only single 20s can crit or fumble
        if (rand == 20)
            std::cout << " (CRIT)";
        else if (rand == 1)
            std::cout << " (FUMBLE)";
    }
    else{
        if (dice_size == 100)
            std::cout << "%";
        if ((dice_count * dice_size) == rand)
            std::cout << " (MAX)";
        else if (rand == dice_count)
            std::cout << " (MIN)";
    }
    
    std::cout << "\nr: ";
    std::cout.flush();
}




inline bool is_number(const char & input){
    return (static_cast<int>(input) >= 48 && static_cast<int>(input) <= 57);
}

int extract(const std::string & input, size_t & cursor){
    std::string chunk = "";
    while (cursor < input.size() && is_number(input[cursor])){
        chunk += input[cursor];
        ++cursor;
    }
    ++cursor; //skip this char
    if (chunk == "") return 0; //invalid format
    return std::stoi(chunk);
}

#endif //ROLL_H
