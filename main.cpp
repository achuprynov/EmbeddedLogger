/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include <iostream>
#include <sstream>
#include "Logger.h"

int main (int argc, char* argv[])
{
    try
    {
        char* file = argv[1];
        char* message = argv[2];

        utils::fs::Logger logger(file, 1024);

//        for (int i = 0; i < 100; i++)
//        {
//            std::stringstream ss;
//            ss << i;
//            logger.Write(ss.str());
//        }
        
        for (int i = 0; i < 10; i++)
        {
            std::stringstream ss;
            ss << i;
            logger.DateWrite(ss.str());
        }
        
        auto text = logger.Read();
        std::cout << text;
    }
    catch (std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
    return 0;
}


