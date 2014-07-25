#include <iostream>
#include "../src/confslice.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 2) {
    ConfSlice cs;
    int status = cs.analyze(argv[1]);
    if (!status) {
      Configuration *conf = cs.configuration();
      if (conf) cout << "OK\n";
      else cout << "ERROR\n";
    } else {
      cout << "ERROR\n";
    }
    return status;
  } else {
    cout << "No input file.\n";
    return 1;
  }
}
