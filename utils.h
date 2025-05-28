#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::vector<uint8_t> hexStrToBytes(const std::string& hex);
std::string bytesToHexStr(const std::vector<uint8_t>& bytes);
std::vector<uint8_t> generatePrivateKey();
std::string toLower(const std::string& input);
std::string publicKeyToAddress(const std::vector<uint8_t>& pubKey);

#endif // UTILS_H
