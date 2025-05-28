#ifndef KECCAK_H
#define KECCAK_H

#include <string>
#include <vector>

std::vector<uint8_t> keccak256(const std::vector<uint8_t>& input);
std::string keccak256Hex(const std::string& input);

#endif // KECCAK_H
