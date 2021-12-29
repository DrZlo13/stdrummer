#pragma once
#include <string>
#include <fatfs.h>
#include <helper.h>
#include "fileinfo.h"

class Dir {
private:
    DISALLOW_COPY_AND_ASSIGN(Dir);
    DIR _dir;
    uint32_t _error;
    std::string path;

public:
    Dir();
    ~Dir();

    bool open(std::string_view path);
    bool close();
    bool read();
    bool read(FileInfo& fileinfo);
    bool read_filter_by_ext(FileInfo& fileinfo, std::string_view ext);
    uint32_t error();
    const char* error_text();
};