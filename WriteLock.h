/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#pragma once

#include <fcntl.h>

namespace utils
{
    namespace fs
    {
        class WriteLock
        {
        public:
            WriteLock(int _fd);
            virtual ~WriteLock();
        private:
            int fd;
            struct flock lock;
        };
    }
}
