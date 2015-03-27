#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* lean and mean system property display */

char *filename_value = "/sys/bus/pci/drivers/k10temp/0000:00:18.3/temp1_input";
char *filename_maxvalue = "/sys/bus/pci/drivers/k10temp/0000:00:18.3/temp1_max";
const char *unit  = "°C";

int min_value = 50;
int max_value = 100;
float scale  = 0.001;

int low_color = 0x0000ff;  // blue
int med_color = 0xff9000;  // orange
int high_color = 0xff0000; // red


int read_int(int fd){
  char buff[21];
  int len = read(fd,buff,20);
  buff[len] = 0;
  return atoi(buff);
}

int read_int_value(const char *filename_value) {
  int fd;
  int value;
  fd = open(filename_value, O_RDONLY);
  value = read_int(fd);
  close(fd);
  return value;
}

int main (int argc, char **argv){
  int value;
  int color;

  color = med_color;

  value     = read_int_value(filename_value);
  max_value = read_int_value(filename_maxvalue);
  min_value /= scale;
  
  if (value < min_value) {
    color = low_color;
  }
  else if (value > max_value) {
    color = high_color;
  }
  printf("<fc=#%06x>%03.2f%s</fc>", color, (float)value * scale, unit);
  return 0;
}
