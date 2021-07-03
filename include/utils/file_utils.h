//
// @description: FileUtils
// @author: von.wu
// @date: 2021-06-27 10:24:45
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif
    // your code

    inline bool existsByIfStream(const std::string &name)
    {
        std::ifstream f(name.c_str());
        return f.good();
    }

    inline bool existsByFileOpen(const std::string &name)
    {
        if (FILE *file = fopen(name.c_str(), "r"))
        {
            fclose(file);
            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool existsByAccess(const std::string &name)
    {
        return (access(name.c_str(), F_OK) != -1);
    }

    inline bool existsByStat(const std::string &name)
    {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }

    inline void getFilePath(char const *path, char *returnPath)
    {
        memset(returnPath, 0, 256);
        strcat(returnPath, "./");
        strcat(returnPath, path);
        std::cout << "start check file exist: " << returnPath << std::endl;
        if (!existsByStat(returnPath))
        {
            memset(returnPath, 0, 256);
            strcat(returnPath, "../");
            strcat(returnPath, path);
            std::cout << "start check file exist: " << returnPath << std::endl;
            if (!existsByStat(returnPath))
            {
                std::cout << "load file failed !" << std::endl;
                returnPath = NULL;
            }
        }
    }

#ifdef __cplusplus
}
#endif
#endif // FILE_UTILS_H