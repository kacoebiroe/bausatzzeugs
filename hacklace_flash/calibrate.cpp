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

const double target_freq = 4e6;

int statistics(map<int,double> measurements) {
    if (!measurements.empty()) {
      int best_osccal = measurements.begin()->first;
      double best_dist = fabs(target_freq - measurements.begin()->second);
      fstream out;
      out.open("my_little_log", fstream::out | fstream::app);
      for (map<int, double>::iterator it = measurements.begin(); it != measurements.end(); ++it) {
        cout << "OSCCAL value of " << it->first << " maps to frequency " << it->second << endl;
        if (fabs(it->second - target_freq) < best_dist) {
            best_dist = fabs(it->second - target_freq);
            best_osccal = it->first;
        }
        out << it->first << " " << it->second << " ";
      }
      out << endl;
      cout << "Best OSCCAL: " << best_osccal << endl
           << "Best Freq:   " << measurements[best_osccal] << endl
           << "Error:       " << fabs(target_freq - measurements[best_osccal]) << endl;
      cout << endl;
      return best_osccal;
    }
    return 0;
}

void total_log(Analyser& myAnalyser) {
    fstream log;
    log.open("total_log", fstream::out);
    for (int i = 0; i < 128; i++) {
      fstream out;
      out.open("firmware/calibration-data.h", fstream::out);
      out << "#define EXT_OSCCAL " << i << endl;
      out.close();
      system("cd firmware; make clean > /dev/null; make > /dev/null; make flash > /dev/null");
      log << i << "\t" << myAnalyser.get_freq() << endl;
    }
}

int main(int argc, char * argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " device baudrate" << endl;
    return 0;
  }
  Analyser myAnalyser;
  myAnalyser.setup(argv[1], argv[2]);

  //total_log(myAnalyser);

  int upper = 127;
  int lower = 0;
  int mid = (upper+lower)/2;

  fstream log;
  log.open("log.txt", fstream::out | fstream::app);


  map<int, double> measurements;
  while (true) {
    if (measurements.find(mid) != measurements.end()) {
      const int osccal = statistics(measurements);
      log << osccal << endl;
      fstream out;
      out.open("Hacklace/calibration-data.h", fstream::out);
      out << "#define EXT_OSCCAL " << osccal << endl;
      system("cd Hacklace; make clean > /dev/null; make > /dev/null; make flashall > /dev/null");
      return 0;
    }
    fstream out;
    out.open("firmware/calibration-data.h", fstream::out);
    out << "#define EXT_OSCCAL " << mid << endl;
    out.close();
    system("cd firmware; make clean > /dev/null; make > /dev/null; make flash > /dev/null");
    //statistics(measurements);
    const double freq = myAnalyser.get_freq();
    measurements[mid] = freq;
    if (freq < target_freq) {
      lower = mid;
    }
    else {
      upper = mid;
    }
    mid = (lower+upper)/2;
  }


  return 0;

}
