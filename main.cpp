#include <iostream>
#include <string>
#include <fstream>
#include "Util.h"

using namespace std;

void process(ifstream* in, ostream* out, unsigned long long* remaining, bool enc);
void encrypt(char* buffer, int bufferSize);
void decrypt(char* buffer, int bufferSize);
bool endsWith(string str1, string str2);
unsigned int getIntFromStr(string* str);
void encryptMode(ifstream* in, string* path);
void decryptMode(ifstream* in, string* path);

string extension;
int maxBufferLengthMb = 16;
const int mb2Byte = 1048576;
int encPaddingMb = 0;
short enc;

int main() {
    //get in file
    cout << "Enter file path : ";
    string filePath;
    cin >> filePath;
    cout << endl;

    //get key
    cout << "Enter key : ";
    string strKey;
    cin >> strKey;
    cout << endl;

    //get mode
    cout << "Enter 1 for encrypt, 0 for decrypt, 3 for expand : ";
    cin >> enc;
    cout << endl;

    //get padding
    if (enc) {
        cout << "Enter padding in MB to add to file : ";
        cin >> encPaddingMb;
        cout << endl;
    }

    //get megabyte chunk size
    cout << "Enter MB chunk size (more = faster but more ram) : ";
    int chunkSize;
    cin >> chunkSize;
    if (chunkSize > 0) maxBufferLengthMb = chunkSize;
    cout << endl;

    //get extension
    cout << "Enter file extension : ";
    cin >> extension;
    cout << endl;

    //Add up in value of each char to get key
    unsigned int key = getIntFromStr(&strKey);
    srand(key);
    ifstream inFile(filePath, ios::binary);
    if (enc) {
        encryptMode(&inFile, &filePath);
    } else {
        decryptMode(&inFile, &filePath);
    }
    return 0;
}

void encryptMode(ifstream* in, string* path) {
    ofstream outFile(*path  + extension, ios::binary);

    //Get length of file
    in->seekg(0, std::ios::end);
    size_t size = in->tellg();
    in->seekg(0, std::ios::beg);
    unsigned long long remaining = (long long) size + (long long) encPaddingMb * (long long) mb2Byte;

    Util::print(to_string(remaining));
    Util::print("Processing File...");
    while (remaining > 0) {
        process(in, &outFile, &remaining, true);
    }
    in->close();
    outFile.close();
}

void decryptMode(ifstream* in, string* path) {

    //remove file extension
    string fileName = *path;
    if (endsWith(fileName, extension)) fileName = fileName.substr(0, fileName.length() - extension.length());

    ofstream outFile(fileName, ios::binary);

    //Get length of file
    in->seekg(0, std::ios::end);
    size_t size = in->tellg();
    in->seekg(0, std::ios::beg);

    unsigned long long remaining = (long long) size;
    Util::print("Processing File...");
    while (remaining > 0) {
        process(in, &outFile, &remaining, false);
    }
    in->close();
    outFile.close();
}

void process(ifstream* in, ostream* out, unsigned long long* remaining, bool enc) {
    int arraySize = min((long) maxBufferLengthMb * (long) mb2Byte, (long) *remaining);
    Util::print(to_string(arraySize));
    //if (arraySize == -1) return;
    char* buffer = new char[arraySize] {};
    in->read(buffer, arraySize);
    if (enc) {
        encrypt(buffer, arraySize);
    } else {
        decrypt(buffer, arraySize);
    }
    out->write(buffer, arraySize);
    *remaining -= arraySize;
    delete [] buffer;
}

void encrypt(char* buffer, int bufferSize) {
    if (enc == 3) return;
    for (int i  = 0; i < bufferSize; i++) {
        buffer[i] = buffer[i] + rand();
    }
}

void decrypt(char* buffer, int bufferSize) {
    for (int i  = 0; i < bufferSize; i++) {
        buffer[i] = buffer[i] - rand();
    }
}

unsigned int getIntFromStr(string* str) {
    unsigned int sum = 0;
    for (char& c : *str) {
        sum += c;
    }
    return sum;
}

//string.ends_swith stopped working for some reason so I had to do this
bool endsWith(string str1, string str2) {
    str1 = str1.substr(str1.length() - str2.length());
    return str1 == str2;
}