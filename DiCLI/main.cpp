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

int main(){
    printf("Ready to roll! ('x' to exit, '?' for help, return to reroll)\nr: ");
    
    bool time_to_exit = false;
    std::string token = "";
    Roll current = Roll();
    
    while (!time_to_exit){
        std::getline(std::cin, token);
        if (token[0] == 'x' || token[0] == 'X'){
            time_to_exit = true;
        }
        else if (token[0] == 'h' || token[0] == 'H' || token[0] == '?'){
            std::cout << "\nEnter rolls as XdY or XdY+Z, where:\n"
            <<"     X is the number of dice,\n"
            << "     Y is the number of sides those dice have, and\n"
            <<"     Z is the modifier, if any.\n\n"
            << "You may use '.' in place of 'd' if you'd like to use the numpad."
            <<"\n\nYou may press return to reroll, or 'x' to quit.\n\nr:";
            std::cout.flush();
        }
        else if (token.size() == 0){
            current.do_roll(); //reroll
        }
        else{
            current = Roll();
            current.process_token(token);
            if (current.is_valid())
                current.do_roll();
            else{
                std::cout << "   = error\nr: ";
                std::cout.flush();
            }
        }
    }
}