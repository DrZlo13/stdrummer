#include "file.h"
#include "path_holder.h"
#include "filesystem.h"

File::File() {
}

File::~File() {
    close();
}

bool File::open(std::string_view _path, Access _access, Mode _mode) {
    std::string normalized_path(_path);
    normalize_path(normalized_path);

    do {
        if(PathHolder::contain(&normalized_path)) {
            _error = FR_LOCKED;
            break;
        }

        path = normalized_path;
        PathHolder::insert(&this->path);

        uint8_t mode = (uint8_t)_access | (uint8_t)_mode;
        _error = f_open(&_file, path.c_str(), mode);
    } while(false);

    return (_error == FR_OK);
}

bool File::fastseek_enable(uint32_t cache_size) {
    _cluster_link_map = static_cast<DWORD*>(malloc(sizeof(DWORD) * cache_size));
    _cluster_link_map[0] = cache_size;
    _file.cltbl = _cluster_link_map;
    _error = f_lseek(&_file, CREATE_LINKMAP);
    return (_error == FR_OK);
}

bool File::close() {
    if(PathHolder::contain(&this->path)) {
        PathHolder::erase(&this->path);
        _error = f_close(&_file);
        if(_cluster_link_map != NULL) free(_cluster_link_map);
        _cluster_link_map = NULL;
    } else {
        _error = FR_LOCKED;
    }

    return (_error == FR_OK);
}

bool File::opened() {
    return PathHolder::contain(&this->path);
}

uint32_t File::error() {
    return _error;
}

const char* File::error_text() {
    return fs_error_text(_error);
}

uint32_t File::read(uint8_t* data, uint32_t data_size) {
    UINT was_read = 0;

    _error = f_read(&_file, data, data_size, &was_read);
    return was_read;
}

uint32_t File::write(const uint8_t* data, uint32_t data_size) {
    UINT was_written = 0;
    _error = f_write(&_file, data, data_size, &was_written);
    return was_written;
}

bool File::seek(int64_t offset, bool from_start) {
    if(from_start) {
        _error = f_lseek(&_file, offset);
    } else {
        uint64_t position = tell();
        _error = f_lseek(&_file, position + offset);
    }

    return (_error == FR_OK);
}

uint32_t File::tell() {
    return f_tell(&_file);
}

bool File::eof() {
    return f_eof(&_file);
}