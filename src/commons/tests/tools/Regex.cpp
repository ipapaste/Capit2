#include <iostream>

#include "../../tools/String.hpp"

int main()
{
    if(String::regexMatch("haHello world", "^(Hel)"))
    	cout << "It did match !" << endl;

    return 0;
}
