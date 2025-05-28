#include <iostream>
#include <iomanip>
#include <openssl/rand.h>
#include "utils.h"

int main() {
    std::string privateKey = generatePrivateKey();
    std::string publicKey = privateKeyToPublicKey(privateKey);
    std::string ethAddress = publicKeyToEthAddress(publicKey);

    double simulatedBalance = simulateBalance();

    std::cout << "[+] Private Key : " << privateKey << "\n";
    std::cout << "[+] ETH Address : 0x" << ethAddress << "\n";
    std::cout << "[+] Status      : Simulasi | Saldo: " 
              << std::fixed << std::setprecision(3) 
              << simulatedBalance << " ETH\n";

    return 0;
}
