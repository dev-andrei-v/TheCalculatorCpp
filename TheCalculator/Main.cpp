#include "AppMenu.h"

int main(int argc, char* argv[])
{
	bool isCmd = false;
	if (argc == 2) {
		//console eq run
		//argv[0] => program name/path
		//argv[1] => input
		string cmd(argv[1]);
		try {
			Calculator c(cmd);
			cout << c << endl;
			isCmd = true;
		}
		catch (exception e) {
			isCmd = true;
			string msg = e.what();
			cout << "Error: " << msg << endl << endl;
		}
	}
	else if (argc >= 2)
	{
		cout << "Usage: " << argv[0] << " <math input>";
		isCmd = true;
	}

	if (!isCmd) {
		AppMenu menu;
		while (true) {
			try {
				menu.showOptions();
			}
			catch (exception e) {
				cout << "Error: " << e.what() << endl;
			}
		}
	}
}