#include "fileinfo.h"

bool FileInfo::is_dir() {
    return _fileinfo.fattrib & AM_DIR;
}

std::string FileInfo::name() {
    return std::string(_fileinfo.fname);
}

uint32_t FileInfo::size() {
    return _fileinfo.fsize;
}

FileInfo::FileInfo() {
}

FileInfo::~FileInfo() {
}