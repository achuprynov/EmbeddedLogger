/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#pragma once

#define make_str(x) (((std::stringstream::__stringbuf_type*)(std::stringstream() << x).rdbuf())->str())


