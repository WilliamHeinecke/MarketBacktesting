//
// Created by willi on 5/26/2025.
//

#ifndef GETENV_H
#define GETENV_H
#include <iostream>
#include <bits/ostream.tcc>

#endif //GETENV_H
#include <string>


inline std::string getEnvVar( std::string const & key )
{
    std::cout << "Getting env var " << key << std::endl;
    char * val = getenv( key.c_str() );
    return val == NULL ? std::string("") : std::string(val);
}