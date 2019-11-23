/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#pragma once

#include <string>

namespace utils
{
    namespace fs
    {
        class Logger 
        {
        public:
            explicit Logger(const std::string & vPath, size_t vSize);
            Logger(const Logger& orig) = delete;
            virtual ~Logger();

            std::string Read();

            void Write    (const std::string & vText);
            void DateWrite(const std::string & vText);

        private:
            size_t ReadInternal(std::string & buffer, size_t offset, size_t length, size_t readPosition);

            size_t WriteComplex (const std::string & vText, size_t position);
            size_t WriteInternal(const std::string & vText, size_t offset, size_t length, size_t position);

            void AllocateFile();

            void PutWritePosition(size_t position);
            void PutReadPosition (size_t position);

            size_t GetWritePosition();
            size_t GetReadPosition();

        private:
            int fd;
            size_t fileSize;
            const int headerSize = sizeof(size_t) * 2;
        };
    }
}