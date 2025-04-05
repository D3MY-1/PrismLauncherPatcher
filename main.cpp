#include "BinaryPatcher.h"
#include <sstream>  
#include <iostream>



int main() {
    const std::string target_executable = "prismlauncher.exe";

    const std::string sig_vA = "0F 84 ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 89 ?? 45 31 ?? FF ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 89"; 
    const std::vector<uint8_t> patch_vA = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; 

    const std::string sig_vB = "75 ?? 4C ?? ?? ?? ?? ?? ?? 48 8D ?? ?? FF ?? ?? ?? ?? ?? 90 89";
    const std::vector<uint8_t> patch_vB = { 0xEB };                       

    // --- Patching Process ---
    try {
        BinaryPatcher patcher(target_executable);
        std::cout << "Loaded file: " << target_executable << std::endl;

        patcher.addPatch("PrismPortablePatch", sig_vA, patch_vA);
        patcher.addPatch("PrismInstalledPatch", sig_vB, patch_vB);

        // Attempt to find and apply ONE matching patch
        if (patcher.tryPatch()) {
            std::string output_filename = patcher.generateOutputFilename();
            patcher.savePatchedFile(output_filename);
            std::cout << "Patching process completed successfully for patch: " << patcher.getAppliedPatchName() << std::endl;
        }
        else {
            std::cout << "Patching failed: No known signature was found in the file." << std::endl;
            std::cin.get();
            return 1;
        }

    }
    catch (const PatchError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press enter to exit...";
        std::cin.get();
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "An unexpected standard error occurred: " << e.what() << std::endl;
        std::cout << "Press enter to exit...";
        std::cin.get();
        return 1;
    }
    catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        std::cout << "Press enter to exit...";
        std::cin.get();
        return 1;
    }


    std::cout << "Press enter to continue...";
    std::cin.get();
    return 0;
}