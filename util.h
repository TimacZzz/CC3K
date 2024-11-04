#ifndef UTIL
#define UTIL

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <functional>
#include <sstream>


using namespace std;

class Util{
    public:
        // set random seed for the program, default seed is the current time
        void setRandomSeed(int seed = time(NULL));

        // generate random number between min and max
        int getRandomNumber(int min, int max);

        // request input, keep asking until the input is valid (validator returns true)
        // if reading failed, a whole line will be discarded
        template <typename T> void requestInput(istream &in, ostream &out, T &input, 
            const string &prompt, function<bool(T)> validator = [](T){return true;},
            const string error_message = "Invalid input");
};

template<typename T>
void Util::requestInput(istream &in, ostream &out, T &input, const string &prompt, 
    function<bool(T)> validator, const string error_message){

    while(true){
        out << prompt;
        in >> input;
        if(in.fail()){
            out << error_message << endl;
            in.clear();
            in.ignore(1000, '\n'); // discard 1000 characters until '\n' is encountered (discard a line)
            continue;
        }
        if(validator(input)){
            out << input << " selected" << endl;
            return;
        }else{
            out << error_message << endl;
        }
    }
}


#endif
