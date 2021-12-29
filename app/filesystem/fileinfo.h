#pragma once
#include <fatfs.h>
#include <string>

class FileInfo {
private:
    friend class Dir;
    FILINFO _fileinfo;

public:
    bool is_dir();
    std::string name();
    uint32_t size();

    FileInfo();
    ~FileInfo();
};