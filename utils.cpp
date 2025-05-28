#include "utils.h"
#include "keccak.h"
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <sstream>
#include <iomanip>
#include <random>

std::vector<uint8_t> hexStrToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);
        uint8_t byte = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string bytesToHexStr(const std::vector<uint8_t>& bytes) {
    std::ostringstream oss;
    for (uint8_t byte : bytes)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return oss.str();
}

std::string toLower(const std::string& input) {
    std::string result = input;
    for (char& c : result)
        c = std::tolower(c);
    return result;
}

std::vector<uint8_t> generatePrivateKey() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    std::vector<uint8_t> privKey(32);
    for (int i = 0; i < 4; ++i) {
        uint64_t part = dis(gen);
        for (int j = 0; j < 8; ++j)
            privKey[i * 8 + j] = (part >> (56 - j * 8)) & 0xFF;
    }

    return privKey;
}

std::string publicKeyToAddress(const std::vector<uint8_t>& pubKey) {
    std::vector<uint8_t> hash = keccak256(std::vector<uint8_t>(pubKey.begin() + 1, pubKey.end()));
    std::vector<uint8_t> address(hash.end() - 20, hash.end());
    return "0x" + bytesToHexStr(address);
}
