// SPDX-FileCopyrightText: Copyright (c) 2022 The ObjCrypto Project Authors
// SPDX-License-Identifier: BSD-2-Clause

#include <objCrypto/objCrypto.h>

#include <cassert>
#include <chrono>
#include <iostream>

using namespace ObjCrypto;

int main(/* int argc, char *argv[] */) {
  ObjCryptoErr err;

  ObjCryptor cryptor;
  KeyID keyId = 1;

  std::vector<uint8_t> plainTextIn = {0x0A, 0x0B, 0x0C, 0x0D, 0x0A, 0x0B,
                                      0x0C, 0x0D, 0x0A, 0x0B, 0x0C, 0x0D,
                                      0x0A, 0x0B, 0x0C, 0x0D};
  std::vector<uint8_t> authData = {0x01, 0x02, 0x03, 0x00};

  std::vector<uint8_t> cipherText(plainTextIn.size());
  std::vector<uint8_t> plainTextOut(plainTextIn.size());

  Key128 key128 = {0};

  KeyInfo keyInfo(ObjCryptoAlg::AES_128_GCM_128, key128);

  Nonce nonce = {0};

  std::vector<uint8_t> tag(128 / 8);
  assert(tag.size() == 128 / 8);

  err = cryptor.addKey(keyId, keyInfo);
  assert(err == ObjCryptoErr::None);

  auto startTime = std::chrono::high_resolution_clock::now();

  const long loops = 1 * 1000 * 1000;
  for (int i = 0; i < loops; i++) {
    err = cryptor.seal(keyId, nonce, plainTextIn, authData, tag, cipherText);
    assert(err == ObjCryptoErr::None);
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  auto elapsedMS = std::chrono::duration_cast<std::chrono::microseconds>(
      endTime - startTime);
  float seconds = (float)(elapsedMS.count()) * 1e-6f;
  const long bytesProcessed = loops * (long)plainTextIn.size();
  std::cout << "mbps of AES128-GCM: "
            << (float)(bytesProcessed)*8.0 / seconds / 1.0e6 << std::endl;
  std::cout << "Kbytes of AES128-GCM: "
            << (float)(bytesProcessed) / seconds / 1.0e3 << std::endl;

  // err = cryptor.unseal( keyId, nonce, cipherText, authData, tag, plainTextOut
  // ); assert( err == ObjCryptoErr::None);

  return 0;
}
