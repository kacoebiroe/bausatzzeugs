#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <SerialStream.h>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <map>
#include <float.h>

using namespace std;
using namespace LibSerial;

#include "LibSerialHelper.h"


#include "Analyser.h"


int main(int argc, char * argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " device baudrate" << endl;
    return 0;
  }
  Analyser myAnalyser;
  myAnalyser.setup(argv[1], argv[2]);
  myAnalyser.logging();
  return 0;

}
