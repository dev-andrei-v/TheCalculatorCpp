#pragma once
#include <vector>
#include "AdvancedCalculator.h"

class AppMenu {
private:
	AdvancedCalculator calc;
	const vector<string> options{ "Read equation from keyboard",			    //1
						   "Read equation(s) from text file",					//2
						   "Choose where to display results",					//3
						   "Save last result in a variable (or type save)",		//4
						   "Show or delete saved variable(s)",					//5
						   "Delete all saved variable(s)",						//6
						   "Change output precision",							//7
						   "Help (or type help)",								//8
						   "Exit (or type exit)" };								//9
	enum class optionType {
		readEq = 1,
		readEqFile = 2,
		ch2display = 3,
		saveVar = 4,
		showDelVar = 5,
		delAllVar = 6,
		chPrecision = 7,
		help = 8,
		exit = 9
	};
	int precision;
	bool saveInFile = false;
public:
	AppMenu();
	void help();
	void showOptions();
	void option(int opt);
	bool setPrecision(int p);
	void show();

};
