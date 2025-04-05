
#include "PatchInfo.h"
#include <optional>

// --- Binary Patcher Class ---
class BinaryPatcher {
private:
    std::string input_filename;
    std::vector<uint8_t> file_buffer;
    std::vector<PatchInfo> available_patches;
    bool is_patched;
    size_t applied_patch_offset;
    std::string applied_patch_name;

    void readFile();

    std::optional<size_t> findSignature(const std::vector<uint8_t>& pattern, const std::vector<bool>& mask) const;

    void applyPatch(size_t offset, const std::vector<uint8_t>& patch_bytes);

public:
    BinaryPatcher(std::string filename);

    void addPatch(const std::string& name, const std::string& sig_str, const std::vector<uint8_t>& patch_bytes);

    bool tryPatch();

    void savePatchedFile(const std::string& output_filename) const;

    std::string generateOutputFilename() const;

    bool wasPatched() const;
    const std::string& getAppliedPatchName() const;
    size_t getAppliedPatchOffset() const;


}; 
