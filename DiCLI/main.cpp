/*
 main.cpp
 DiCLI
 
 Created by Hans Behrens on 2015-02-17.
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

#include "roll.h"

static const std::string help_message = "\nEnter rolls as XdY or XdY+Z, where:\n     X is the number of dice,\n     Y is the number of sides those dice have, and\n     Z is the modifier, if any.\n\nEx: 1d20, 2d4+2, 1d100, etc.\n";
static const std::string intro_message = "Welcome to DiCLI!\n('?' for help, return to reroll last roll, 'X' to exit)";
static const std::string capstone = "\nr: ";

int main(){
    printf("%s\nr: ",intro_message.c_str());
    
    bool time_to_exit = false;
    std::string token = "";
    Roll current = Roll();
    
    while (!time_to_exit){
        std::getline(std::cin, token);
        if (token[0] == 'x' || token[0] == 'X'){
            time_to_exit = true;
        }
        else if (token[0] == 'h' || token[0] == 'H' || token[0] == '?'){
            std::cout << help_message << capstone;
            std::cout.flush();
        }
        else if (token.size() == 0 && current.is_valid()){
            current.do_roll(); //use most recent valid token
        }
        else{
            current = Roll(); //prep for new token
            current.process_token(token);
            if (current.is_valid())
                current.do_roll();
            else{
                current = Roll(); //discard invalid token
                std::cout << "   = [invalid roll]" << capstone;
                std::cout.flush();
            }
        }
    }
}