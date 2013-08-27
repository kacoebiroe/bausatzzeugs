#!/usr/bin/python

import sys
import os

def main(argv):
  b1,b2,b3,b4 = os.popen('cat firmware/blink1.c | grep \'uint8_t ee_serialnum\' | cut -d \' \' -f9-12 | sed \'s/\s//g\'').read().split(',')
  byte1 = int(b1, 16) & 0xff
  byte2 = int(b2, 16) & 0xff
  byte3 = int(b3, 16) & 0xff
  byte4 = int(b4, 16) & 0xff
  print "Old serial: %02X%02X%02X%02X" % (byte1,byte2,byte3,byte4)
  # make a common long from them.
  total = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4)
  total += 1
  serial = "New serial: %02X%02X%02X%02X" % ( (total & 0xff000000) >> 24, (total & 0xff0000) >> 16, (total & 0xff00) >> 8, total & 0xff )
  print serial
  os.system('sed -i \'s/uint8_t ee_serialnum\[4\]    EEMEM = .*/uint8_t ee_serialnum\[4\]    EEMEM = \{ 0x%02X, 0x%02X, 0x%02X, 0x%02X \};/g\' firmware/blink1.c' % ( (total & 0xff000000) >> 24, (total & 0xff0000) >> 16, (total & 0xff00) >> 8, total & 0xff ) )
  os.system('cd firmware; make clean; make; make fuse; make flash');
  print serial
  output = '=' * 34 + "\n"
  output += ' ' * 9 + "RZLblink V 2.1a\n"
  output += '=' * 34 + "\n"
  output += "\n" + "Serial: %02X%02X%02X%02X\n" % ( (total & 0xff000000) >> 24, (total & 0xff0000) >> 16, (total & 0xff00) >> 8, total & 0xff )
  output += "Wiki: http://rzl.so/wiki/RZLblink\n" 
  output += "\nRZLblink is based on blink(1) by\nThingM (thingm.com) licensed under\nCreative Commons Attribution Share-Alike 3.0 License\n\n"
  output += "Common mistakes:\n* R1 and R2 are vertical!\n  All other resistors and capaci-\n  tors are horizontal!"
  os.system('echo \'%s\' | lpr -o DocCutType=1PartialCutDoc -o cpi=12 -P "Star_TSP143_(STR_T-001)"' % (output))


main(sys.argv) 
