// CHUONG TRINH TINH CHECKSUM MOT FILE (CHAY TREN LINUX X86)
#include <ios>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
using namespace std;


std::vector<char> ReadAllBytes(char const* filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Thieu tham so dong lenh: ten tap tin" << endl;
    return -1;
  }

  vector<char> buffer = ReadAllBytes(argv[1]);

  unsigned short c0 = 0;
  unsigned short c1 = 0;
  long numBytesRead = 0;
  for (int i = 0; i < buffer.size(); i++) { 
    c0 = (c0 + (int)buffer[i]) % 255;
    c1 = (c1 + c0) % 255;
    numBytesRead++;
  }
  cout << "Fletcher-16 checksum: " << ((c1 << 8) | c0) << "; " << numBytesRead << " bytes checked."<< endl;
}
