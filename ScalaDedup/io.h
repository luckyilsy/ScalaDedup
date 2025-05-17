#ifndef _IO_H_
#define _IO_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include "struct.h"
#include <pbc/pbc.h>
#include <math.h>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void write128BitUnsignedChar(string filepath, unsigned char* data);

unsigned char* read128BitUnsignedChar(string filepath);

string charEncode(char* data);

char* charDecode(string str);

string unsignedCharEncode(unsigned char* data, int len);

unsigned char* unsignedCharDecode(string str);

unsigned char* unsignedCharDecode(string str, unsigned char* data);

bool writeStringAppend(string filepath, string str);

void unsignedCharCopy(unsigned char* str1, unsigned char* str2);

string readString(string filepath);

bool writeString(string filepath, string str);

string getElementString(element_t& elem);

string getElementString(element_s* elem);

unsigned char* getElementBytes(element_s* elem);

string getElementString(element_t& elem, bool isCompressed);

string getZpString(element_s* elem);

string getGString(element_s* elem);

string getElementStringCompressed(element_t& elem);

unsigned char* getElementByte(element_s* elem);

void printElement(string name, element_s* elem);

void printElement(element_s* elem);

int genRandomElementInZr(string filepath);

int genRandomElementInG(string filepath);

bool sendFile(const std::string& filename, const std::string& serverIP, int port) ;

// std::string element_to_hex_string(element_s* elem) ;

// element_s* hex_string_to_element(const std::string& hex_str,CloudServer *pCloud);

int get_elem_length(element_s* elem);

element_s* getStringElement(string elem_s,pairing_s *pairing);

unsigned char* genKey128Bit(element_s *kf_e);

using namespace std;

#endif
