/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include "Logger.h"
#include "WriteLock.h"
#include "macros.h"

#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include <ctime>

namespace utils
{
    namespace fs
    {
        Logger::Logger(const std::string & vPath, size_t vSize)
        :   fileSize(vSize)
        {
            fd = open(vPath.c_str(), O_RDWR);
            if (fd == -1)
            {
                fd = open(vPath.c_str(), O_RDWR | O_CREAT);

                WriteLock lock(fd);

                AllocateFile();

                PutWritePosition(headerSize);
                PutReadPosition (headerSize);
            }
        }

        Logger::~Logger() 
        {
            close(fd);
        }

        std::string Logger::Read()
        {
            WriteLock lock(fd);

            auto readPosition  = GetReadPosition();
            auto writePosition = GetWritePosition();

            std::string buffer;

            if (writePosition > readPosition)
            {
                auto length = writePosition - readPosition;
                buffer.resize(length);

                readPosition = ReadInternal(buffer, 0, length, readPosition);
            }
            else if (writePosition < readPosition)
            {
                auto lengthTail = fileSize - readPosition;
                auto lengthHead = writePosition - headerSize;
                auto length = lengthTail + lengthHead;
                buffer.resize(length);

                readPosition = ReadInternal(buffer, 0, lengthTail, readPosition);

                readPosition = ReadInternal(buffer, lengthTail, lengthHead, headerSize);
            }
            else
            {
                return "";
            }

            PutReadPosition(readPosition);

            return std::move(buffer);
        }

        void Logger::Write(const std::string & vText)
        {
            WriteLock lock(fd);

            auto position = GetWritePosition();

            position = WriteComplex(vText, position);

            position = WriteComplex("\n", position);

            PutWritePosition(position);
        }

        void Logger::DateWrite(const std::string & vText)
        {
            std::time_t epochTime = std::time(nullptr);

            WriteLock lock(fd);

            auto position = GetWritePosition();

            position = WriteComplex(make_str(epochTime << " "), position);

            position = WriteComplex(vText, position);

            position = WriteComplex("\n", position);

            PutWritePosition(position);
        }

        size_t Logger::ReadInternal(std::string & buffer, size_t offset, size_t length, size_t position)
        {
            auto readBytes = pread(fd, (void*)(buffer.data() + offset), length, position);
            if (readBytes != length)
            {
                throw std::ios_base::failure(make_str(
                    "Read failed with error code " << errno));
            }

            return position + readBytes;
        }

        size_t Logger::WriteComplex(const std::string & vText, size_t position)
        {
            auto length = vText.length();

            if (position + length < fileSize)
            {
                return WriteInternal(vText.c_str(), 0, length, position);
            }
            else
            {
                auto lengthTail = fileSize - position;
                auto lengthHead = length - lengthTail;

                WriteInternal(vText.c_str(), 0, lengthTail, position);

                return WriteInternal(vText.c_str(), lengthTail, lengthHead, headerSize);
            }
        }

        size_t Logger::WriteInternal(const std::string & vText, size_t offset, size_t length, size_t position)
        {
            auto writeBytes = pwrite(fd, vText.c_str() + offset, length, position);
            if (writeBytes != length)
            {
                throw std::ios_base::failure(make_str(
                    "Write failed with error code " << errno));
            }

            return position + length;
        }

        void Logger::AllocateFile()
        {
            auto errCode = posix_fallocate(fd, 0, fileSize);
            if (errCode != 0)
            {
                throw std::overflow_error(make_str(
                    "Can't allocate disk space " << fileSize << ". Error code " << errCode));
            }
        }

        void Logger::PutWritePosition(size_t position)
        {
            auto written = pwrite(fd, &position, sizeof(position), 0);
            if (written != sizeof(position))
            {
                throw std::ios_base::failure(make_str(
                    "Can't write WritePosition. Error code " << errno));
            }
        }

        void Logger::PutReadPosition (size_t position)
        {
            auto written = pwrite(fd, &position, sizeof(position), sizeof(position));
            if (written != sizeof(position))
            {
                throw std::ios_base::failure(make_str(
                    "Can't write WritePosition. Error code " << errno));
            }
        }

        size_t Logger::GetWritePosition()
        {
            size_t position(0);

            auto lenRead = pread(fd, &position, sizeof(position), 0);
            if (lenRead != sizeof(position))
            {
                throw std::ios_base::failure(make_str(
                    "Can't read WritePosition. Error code " << errno));
            }

            return position;
        }

        size_t Logger::GetReadPosition()
        {
            size_t position(0);

            auto lenRead = pread(fd, &position, sizeof(position), sizeof(position));
            if (lenRead != sizeof(position))
            {
                throw std::ios_base::failure(make_str(
                    "Can't read ReadPosition. Error code " << errno));
            }

            return position;
        }
    }
}