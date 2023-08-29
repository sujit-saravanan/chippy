#pragma once
#include <string>

namespace chippy {
        [[nodiscard]] std::string readFile(const char* filepath) noexcept;
}