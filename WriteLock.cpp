/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include "WriteLock.h"

#include <string.h>

namespace utils
{
    namespace fs
    {
        WriteLock::WriteLock(int _fd)
        :   fd(_fd)
        {
            memset (&lock, 0, sizeof(lock));
            lock.l_type = F_WRLCK;
            fcntl (fd, F_SETLKW, &lock);
        }

        WriteLock::~WriteLock()
        {
            lock.l_type = F_UNLCK;
            fcntl (fd, F_SETLKW, &lock);
        }
    }
}