#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class PatchError : public std::runtime_error {
public:
    PatchError(const std::string& message) : std::runtime_error(message) {}
};

class PatchInfo {
public:
    std::string name;
    std::vector<uint8_t> patch_bytes;
    std::vector<uint8_t> pattern;
    std::vector<bool> mask;


    PatchInfo(std::string n, std::string sig, std::vector<uint8_t> patch);

    void parseSignature(std::string signature_str);
};
