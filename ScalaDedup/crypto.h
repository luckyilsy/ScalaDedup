#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <pbc/pbc.h>
#include <math.h>
#include <iomanip>
#include <memory>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/aes.h>
#include <openssl/rand.h> 

#include "openssl/evp.h"
#include "openssl/x509.h"


using namespace std;

const int SHORTHASH_LENGTH = 2;
const int MD5_LENGTH = 16;
const int SHA256_LENGTH = 32;


// SHORT HASH - MD5
unsigned char* getShortHash(string filepath);
unsigned char* getShortHash(unsigned char* md5);

// Hash Function - MD5
unsigned char* getStringMD5(const char* str);
unsigned char* getFileMD5(string filepath);

// Hash Function - SHA256
unsigned char* getStringSHA256(const char* str);
unsigned char* getFileSHA256(string filepath);
unsigned char* getFileSHA256(string filepath, unsigned char* random);

// Generate Random Bit String
unsigned char* genRandom256Bit();
unsigned char* genRandom128Bit();
unsigned char* genRandom64Bit();

//XOR
unsigned char* XOR(unsigned char* data1,unsigned char* data2,int length);
// AES Encryption
int AESEncFile(unsigned char* key, string filepath);
int AESDecFile(unsigned char* key, string filepath);


// PBC random element
element_s* genRandomZpElement(pairing_s* e);
element_s* genRandomG1Element(pairing_s* e);
element_s* genRandomG2Element(pairing_s* e);
element_s* genRandomGTElement(pairing_s* e);
element_s* intToZp(int n, pairing_s* e);

// Shamir Key Share - PBC
class ShamirKey{
public:
    element_s* x = nullptr;
    element_s* y = nullptr;
    element_s* ki = nullptr; 
    int minKeyCount = 0;
};
vector<ShamirKey*>* genShamirKeyShare(element_s* s, int min, int n, pairing_s* e);
element_s* recoverShamirKeyShare(vector<ShamirKey*>* ski, pairing_s* e);



#endif
