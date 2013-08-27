
class Analyser {
  public:

    Analyser() {
    }

    /**
     * Analyse the measurements sent by the uC
     */
    void analyse(string& package) {
      stringstream pack(package);

      int cycles_intervall = 0;
      pack >> std::hex >> cycles_intervall;

      const double timer_limit = 65536;
      const double cpu_freq = 16000000;
      const double timer_overflows = 16;
      const double time_intervall = (double) cycles_intervall / cpu_freq;

      const double frequency = timer_limit * timer_overflows * 2.0 / time_intervall;

      if (frequency > 1e8) {
        cout << "Error: frequency too high, read " << package << endl;
        return;
      }

      cout << setprecision(8) << "Got 16 cycles in " << time_intervall << " seconds, avr frequency is " << frequency << " Hz" << endl;
      measurements.push_back(frequency);
      statistics(measurements);
    }

    void statistics(vector<double> measurements) {
      if (measurements.size() < 3) {
        return;
      }
      sort(measurements.begin(), measurements.end());
      double sum = 0;
      fstream out;
      out.open("distribution.data", fstream::out);
      for (size_t i = 0; i < measurements.size(); i++) {
        sum += measurements[i];
        out << measurements[i] << "\t" << (double)i/measurements.size() << endl;
      }
      out.close();
      average = sum / measurements.size();
      sum = 0;
      for (size_t i = 0; i < measurements.size(); i++) {
        sum += (measurements[i]-average)*(measurements[i]-average);
      }
      variance = sum / measurements.size();
      median = measurements[measurements.size()/2];
      cout << "Min:    " << measurements[0] << endl
        << "Max:    " << measurements[measurements.size()-1] << endl
        << "Avg:    " << average << endl
        << "Median: " << median << endl
        << "Stddev: " << sqrt(variance) << endl << endl;



      fstream gnuplot;
      gnuplot.open("plot.gpl", fstream::out);
      gnuplot << "set term svg" << endl
        << "set output 'distribution.svg'" << endl
        << "plot 'distribution.data' with lines" << endl;
      system("gnuplot plot.gpl");
    }

    double get_freq() {
      measurements = vector<double>();

      string package;
      int analyse_counter = 0;
      while(true) {
        // Check for new data from the uC
        while(usb0.rdbuf()->in_avail() > 0){
          char c;
          usb0.get(c);
          //cout << c;
          package += c;
          if (c == '\n') {
            if (analyse_counter > 0) {
                analyse(package);
            }
            package = "";
            if (measurements.size() > 2) {
              return median;
            }
            analyse_counter++;
          }
        }
        usleep(1000);
      }
    }

    /**
     * This method opens a connection to a serial device and listens to the data sent by the uC
     *
     * @param device Path of the device, e.g. /dev/ttyUSB0
     * @param baudrate Baudrate used by the uC, e.g. 38400
     */
    void setup(const char * device, const char * baudrate) {

      LibSerialHelper helper;
      stringstream tmp(baudrate);
      int baud = 0;
      tmp >> baud;
      helper.Open(usb0, device, baud);
    }

    int logging() {
      string package;
      while(true) {
        // Check for new data from the uC
        while(usb0.rdbuf()->in_avail() > 0){
          char c;
          usb0.get(c);
          //cout << c;
          package += c;
          if (c == '\n') {
            analyse(package);
            package = "";
          }
        }
        usleep(1000);
      }

      return 0;
    }

  private:
    vector<double> measurements;
    double average, median, variance;
    SerialStream usb0;
};

