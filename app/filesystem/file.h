#pragma once
#include <string>
#include <fatfs.h>
#include <helper.h>

class File {
private:
    DISALLOW_COPY_AND_ASSIGN(File);
    FIL _file;
    uint32_t _error;
    std::string path;
    DWORD* _cluster_link_map = NULL;

public:
    enum class Access : uint8_t {
        Write = FA_WRITE,
        Read = FA_READ,
    };

    enum class Mode : uint8_t {
        CreateNew = FA_CREATE_NEW,
        CreateAlways = FA_CREATE_ALWAYS,
        OpenExisting = FA_OPEN_EXISTING,
        OpenAlways = FA_OPEN_ALWAYS,
        OpenAppend = FA_OPEN_APPEND,
    };

    File();
    ~File();

    bool open(std::string_view path, Access access, Mode mode);
    bool fastseek_enable(uint32_t cache_size);

    bool close();
    bool opened();
    uint32_t error();
    const char* error_text();

    uint32_t read(uint8_t* data, uint32_t data_size);
    uint32_t write(const uint8_t* data, uint32_t data_size);
    bool seek(int64_t offset, bool from_start);
    uint32_t tell();
};