#include <iostream>
#include <random>
#include <string>
#include <chrono>

inline bool is_number(const char & input);
int read_until(const std::string & input, size_t & cursor);

//init random generators
std::mt19937 rand_engine(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
std::random_device device;
std::mt19937 generator(device());
std::uniform_int_distribution<size_t> big_range(1, 100);
std::uniform_int_distribution<size_t> medium_range(1, 20);
std::uniform_int_distribution<size_t> small_range(1, 8);


struct Roll{
	size_t dice_count = 0;
	size_t dice_size = 0;
	int roll_mod = 0;
	bool valid_roll = false;

	void process_token(std::string token){
		size_t cursor = 0;
		dice_count = read_until(token, cursor);
		dice_size = read_until(token, cursor);
		roll_mod = read_until(token, cursor);
		valid_roll = (dice_count > 0 && dice_size > 0 && dice_size <= 100);
	}

	void do_roll(){
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

	void reset_roll(){
		dice_count = 0;
		dice_size = 0;
		roll_mod = 0;
		valid_roll = false;
	}

	inline bool new_roll(){
		return (dice_count == 0 && dice_size == 0 && roll_mod == 0 && valid_roll == false);
	}
};

int main(){
	printf("Ready to roll! ('x' to exit, '?' for help, return to reroll)\nr: ");

	bool time_to_exit = false;
	std::string token = "";
	Roll current;

	while (!time_to_exit){
		std::getline(std::cin, token);
		if (token == "x" || token == "X"){
			time_to_exit = true;
		}
		else if (token == "h" || token == "H" || token == "?"){
			std::cout << "\nEnter rolls as XdY or XdY+Z, where:\n     X is the number of dice,\n"
				<< "     Y is the number of sides those dice have, and\n     Z is the modifier, if any.\n\n"
				<< "You may use '.' in place of 'd' if you'd like to use the numpad.\n\nYou may press return to reroll, or 'x' to quit.\n\nr:";
			std::cout.flush();
		}
		else if (token.size() == 0){
			current.do_roll(); //reroll
		}
		else{
			current.reset_roll();
			current.process_token(token);
			if (current.valid_roll)
				current.do_roll();
			else{
				std::cout << "   = error\nr: ";
				std::cout.flush();
			}
		}
	}
}

inline bool is_number(const char & input){
	return (static_cast<int>(input) >= 48 && static_cast<int>(input) <= 57);
}

int read_until(const std::string & input, size_t & cursor){
	std::string chunk = "";
	while (cursor < input.size() && is_number(input[cursor])){
		chunk += input[cursor];
		++cursor;
	}
	++cursor; //skip this char
	if (chunk == "") return 0; //invalid format
	return std::stoi(chunk);
}