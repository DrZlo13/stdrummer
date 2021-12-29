#include "dir.h"
#include "path_holder.h"
#include "filesystem.h"

Dir::Dir() {
}

Dir::~Dir() {
}

bool Dir::open(std::string_view _path) {
    std::string normalized_path(_path);
    normalize_path(normalized_path);

    do {
        if(PathHolder::contain(&this->path)) {
            _error = FR_LOCKED;
            break;
        }

        path = normalized_path;
        PathHolder::insert(&this->path);

        _error = f_opendir(&_dir, path.c_str());
    } while(false);

    return (_error == FR_OK);
}

bool Dir::close() {
    if(PathHolder::contain(&this->path)) {
        PathHolder::erase(&this->path);
        _error = f_closedir(&_dir);
    } else {
        _error = FR_LOCKED;
    }

    return (_error == FR_OK);
}

bool Dir::read() {
    FILINFO _fileinfo;
    _error = f_readdir(&_dir, &_fileinfo);
    return (_error == FR_OK && _fileinfo.fname[0] != 0);
}

bool Dir::read(FileInfo& fileinfo) {
    _error = f_readdir(&_dir, &fileinfo._fileinfo);
    return (_error == FR_OK && fileinfo._fileinfo.fname[0] != 0);
}

uint32_t Dir::error() {
    return _error;
}

const char* Dir::error_text() {
    return fs_error_text(_error);
}