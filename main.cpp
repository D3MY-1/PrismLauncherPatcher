#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

const char* sig = "0F 84 50 01 00 00 C7 44";

const char* name = "prismlauncher.exe";

unsigned char path[] = { 0x90,0x90,0x90,0x90,0x90,0x90 };

using namespace std;

vector<uint8_t> pattern;
vector<bool> mask;

void parse_signature(const char* sig) {
    stringstream ss(sig);
    string byte;

    while (getline(ss, byte, ' ')) {
        if (byte == "??") {
            pattern.push_back(0x00);
            mask.push_back(false);
        }
        else {
            uint8_t value = static_cast<uint8_t>(strtoul(byte.c_str(), nullptr, 16));
            pattern.push_back(value);
            mask.push_back(true);
        }
    }
}

vector<uint8_t> read_file(const char* filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return {};
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        cout << "Error reading file: " << filename << endl;
        return {};
    }
    return buffer;
}



intptr_t find_signature(const vector<uint8_t>& buffer) {
    size_t pattern_size = pattern.size();
    if (pattern_size == 0 || buffer.size() < pattern_size) {
        return -1;
    }

    for (size_t i = 0; i <= buffer.size() - pattern_size; ++i) {
        bool match = true;

        for (size_t j = 0; j < pattern_size; ++j) {
            if (mask[j] && buffer[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            return i;
        }
    }
    return -1;
}


void Patch(vector<uint8_t>& binary,intptr_t offset, uint8_t* patch,size_t patchSize)
{
    if (patchSize <= 0) { cout << "Patch size < 0/n"; return; }

    for (int i = 0; i < patchSize; i++)
    {
        binary[offset + i] = patch[i];
    }
    return;
}

int main() {

    parse_signature(sig);
    vector<uint8_t> file_buffer = read_file(name);

    if (file_buffer.empty()) {
        cin.get();
        return 1;
    }

    intptr_t offset = find_signature(file_buffer);
    if (offset != -1) {
        cout << "Signature found at offset: 0x" << hex << offset << endl;
    }
    else {
        cout << "Signature not found" << endl;
    }

    Patch(file_buffer, offset, path, 6);


    string str(name);
    str.replace(str.find(".exe"), 4, "_patched.exe");

    
    ofstream pFile(str,std::ios::binary);

    if (pFile.is_open())
    {
        pFile.write(reinterpret_cast<char*>(file_buffer.data()), file_buffer.size());

        pFile.close();
        cout << "Successfully created file " << str << '/n';
    }
    else
        cout << "Failed to create file /n";

    cout << "Press enter to continue...";

    std::cin.get();

    return 0;
}