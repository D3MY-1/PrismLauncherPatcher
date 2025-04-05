#include "PatchInfo.h"
#include <sstream>


PatchInfo::PatchInfo(std::string n, std::string sig, std::vector<uint8_t> patch)
    : name(std::move(n)), patch_bytes(std::move(patch))
{
    parseSignature(sig); // Parse signature upon creation
}


void PatchInfo::parseSignature(std::string signature_str) {
    pattern.clear();
    mask.clear();
    std::stringstream ss(signature_str);
    std::string byte_str;

    while (getline(ss, byte_str, ' ')) {
        if (byte_str.empty()) continue; // Handle potential extra spaces

        if (byte_str == "??") {
            pattern.push_back(0x00);
            mask.push_back(false);
        }
        else {
            try {
                unsigned long value_ul = std::stoul(byte_str, nullptr, 16);
                if (value_ul > 0xFF) {
                    throw std::out_of_range("Byte value out of range (0-255)");
                }
                pattern.push_back(static_cast<uint8_t>(value_ul));
                mask.push_back(true);
            }
            catch (const std::invalid_argument& e) {
                throw PatchError("Invalid hex byte format in signature '" + signature_str + "': " + byte_str);
            }
            catch (const std::out_of_range& e) {
                throw PatchError("Hex byte value out of range in signature '" + signature_str + "': " + byte_str);
            }
        }
    }
    if (pattern.empty()) {
        throw PatchError("Empty signature generated from string: " + signature_str);
    }
}