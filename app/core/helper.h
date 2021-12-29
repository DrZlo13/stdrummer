#include <stdint.h>
#include <string>

uint8_t get_first_index_of_one(uint16_t value);

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)

#define COUNT_OF(array) (sizeof(array) / sizeof(array[0]))

void replace_all(std::string& source, const std::string& from, const std::string& to);

void normalize_path(std::string& path);