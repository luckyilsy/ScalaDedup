#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <set>
#include <unordered_map>
#include <pbc/pbc.h>
#include "io.h"
#include "crypto.h"

using namespace std;

#pragma warning(disable:4996)

const int MAX_LEGHTH = 1024;

class Subkey{
public:
    ShamirKey* kt;
    ShamirKey* kr;
};

class Package{
public:
    unsigned char* r = nullptr;
    vector<unsigned char*> CT, h_CF;
};
class PackageNew{
public:
    vector<unsigned char*> IF;
};
class Result_Package{
public:
    vector<unsigned char*> IF;
    int dr;
};
class Client{
public:
    Client();
    Client(int id_);
    set<int> genSequence(int t, int KS_COUNT, unsigned char* h);
public:
    int id;
    unsigned char* sh;//短哈希
    unsigned char* hF;//文件hash
};

class CloudServer{
public:
    CloudServer();
    CloudServer(pair<int, int> threshold_, int KS_COUNT_);
    void print();
    bool duplicateCheck(unsigned char* sh, Package* pPackage);
    void duplicateCheck(string sh, Result_Package* result_P);
public:
    pair<int, int> threshold;
    int KS_COUNT;
    element_s* sk = nullptr;
    element_s* g = nullptr;
    element_s* pk = nullptr;
    pairing_s* pairing = nullptr;
    unordered_map<unsigned char*, Package*> data;
    unordered_map<string, PackageNew*> dataNew;
};


class KeyServer{
public:
public:
    element_s* pk;
    pairing_s* pairing;
    unordered_map<string, Subkey*> list;
};


#endif