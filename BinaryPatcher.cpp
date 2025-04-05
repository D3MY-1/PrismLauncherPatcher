#include "BinaryPatcher.h"
#include <fstream>
#include <iostream>



BinaryPatcher::BinaryPatcher(std::string filename) :
    input_filename(std::move(filename)), is_patched(false), applied_patch_offset(0), applied_patch_name("None")
{
    readFile(); // Load file content on creation, throws on error
}

void BinaryPatcher::readFile() {
    std::ifstream file(input_filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw PatchError("Error opening input file: " + input_filename);
    }

    std::streamsize size = file.tellg();
    if (size < 0) {
        file.close();
        throw PatchError("Error getting file size for: " + input_filename);
    }
    file.seekg(0, std::ios::beg);

    file_buffer.resize(static_cast<size_t>(size)); // Resize buffer appropriately
    if (size > 0 && !file.read(reinterpret_cast<char*>(file_buffer.data()), size)) {
        file.close();
        throw PatchError("Error reading file content from: " + input_filename);
    }
    file.close(); // Close file promptly after reading
}


std::optional<size_t> BinaryPatcher::findSignature(const std::vector<uint8_t>& pattern, const std::vector<bool>& mask) const {
    size_t pattern_size = pattern.size();
    if (pattern_size == 0 || file_buffer.size() < pattern_size) {
        return std::nullopt; // Cannot find if pattern is empty or larger than buffer
    }

    for (size_t i = 0; i <= file_buffer.size() - pattern_size; ++i) {
        bool match = true;
        for (size_t j = 0; j < pattern_size; ++j) {
            if (mask[j] && file_buffer[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i; // Return the offset
        }
    }
    return std::nullopt; // Not found
}


void BinaryPatcher::applyPatch(size_t offset, const std::vector<uint8_t>& patch_bytes) {
    if (patch_bytes.empty()) {
        std::cerr << "Warning: Attempting to apply an empty patch." << std::endl;
        return;
    }
    // Boundary check
    if (offset + patch_bytes.size() > file_buffer.size()) {
        throw PatchError("Patch application out of bounds. Offset: " + std::to_string(offset)
            + ", Patch Size: " + std::to_string(patch_bytes.size())
            + ", File Size: " + std::to_string(file_buffer.size()));
    }

    for (size_t i = 0; i < patch_bytes.size(); ++i) {
        file_buffer[offset + i] = patch_bytes[i];
    }
    is_patched = true;
    applied_patch_offset = offset; // Store info about the applied patch
}




void BinaryPatcher::addPatch(const std::string& name, const std::string& sig_str, const std::vector<uint8_t>& patch_bytes) {
    try {
        available_patches.emplace_back(name, sig_str, patch_bytes);
    }
    catch (const PatchError& e) {
        // Re-throw or handle signature parsing errors during addition
        throw PatchError("Error adding patch '" + name + "': " + e.what());
    }
}

// Attempts to find and apply the first matching patch
// Returns true if a patch was applied, false otherwise
bool BinaryPatcher::tryPatch() {
    if (is_patched) {
        std::cerr << "Warning: File has already been patched." << std::endl;
        return true; // Already done
    }

    for (const auto& patch_info : available_patches) {
        std::optional<size_t> offset_opt = findSignature(patch_info.pattern, patch_info.mask);

        if (offset_opt.has_value()) {
            size_t offset = offset_opt.value();
            std::cout << "Signature for patch '" << patch_info.name << "' found at offset: 0x"
                << std::hex << offset << std::dec << std::endl;
            try {
                applyPatch(offset, patch_info.patch_bytes);
                applied_patch_name = patch_info.name; // Record which patch was applied
                std::cout << "Applied patch '" << applied_patch_name << "' successfully." << std::endl;
                return true; // Patch applied, stop searching
            }
            catch (const PatchError& e) {
                // Handle potential errors during patch application (e.g., bounds)
                throw PatchError("Error applying patch '" + patch_info.name + "': " + e.what());
            }
        }
    }

    // If loop finishes, no applicable signature was found
    std::cout << "No applicable signature found in the file." << std::endl;
    return false;
}

// Saves the (potentially modified) buffer to a new file
void BinaryPatcher::savePatchedFile(const std::string& output_filename) const {
    if (!is_patched) {
        std::cout << "No patch was applied. Saving skipped." << std::endl;
        // Or optionally, save the original content anyway if desired
        // throw PatchError("Cannot save: File was not patched.");
        return;
    }

    std::ofstream pFile(output_filename, std::ios::binary | std::ios::trunc); // Trunc ensures overwriting if file exists
    if (!pFile.is_open()) {
        throw PatchError("Failed to create or open output file: " + output_filename);
    }

    if (!pFile.write(reinterpret_cast<const char*>(file_buffer.data()), file_buffer.size())) {
        pFile.close();
        throw PatchError("Failed to write patched data to file: " + output_filename);
    }

    pFile.close();
    std::cout << "Successfully saved patched file as " << output_filename << '\n';
}

std::string BinaryPatcher::generateOutputFilename() const {
    std::string output_filename = input_filename;
    std::string tag = is_patched ? "_patched" : "_unpached";
    size_t dot_pos = output_filename.rfind(".exe");

    if (dot_pos != std::string::npos) {
        output_filename.insert(dot_pos, tag);
    }
    else {
        output_filename += tag;
    }
    return output_filename;
}

bool BinaryPatcher::wasPatched() const { return is_patched; }
const std::string& BinaryPatcher::getAppliedPatchName() const { return applied_patch_name; }
size_t BinaryPatcher::getAppliedPatchOffset() const { return applied_patch_offset; }

