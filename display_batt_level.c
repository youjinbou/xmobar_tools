#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* lean and mean system property display */

char *filename_ac       = "/sys/bus/acpi/drivers/ac/ACPI0003:00/power_supply/ACAD/online";
char *filename_bat0     = "/sys/devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0A:00/power_supply/BAT0/charge_now";
char *filename_maxbat0  = "/sys/devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0A:00/power_supply/BAT0/charge_full"; 

int maxvalue        = 0;
int minvalue        = 0;


const int grey   = 0x7f7f7f;
const int red    = 0xff0000;
const int green  = 0x00ff00;
const int blue   = 0x0000ff;
const int orange = 0xff9000;
const int purple = 0xff00ff;

int read_bool(int fd){
  char b;
  int len = read(fd,&b,1);
  return (len == 1) && (b != '0');
}

int read_bool_prop(const char *filename_value) {
  int fd;
  int value;
  fd = open(filename_value, O_RDONLY);
  value = read_bool(fd);
  close(fd);
  return value;
}

int read_int(int fd){
  char buff[21];
  int len = read(fd,buff,20);
  buff[len] = 0;
  return atoi(buff);
}

int read_int_prop(const char *filename_value) {
  int fd;
  int value;
  fd = open(filename_value, O_RDONLY);
  value = read_int(fd);
  close(fd);
  return value;
}

int main (int argc, char **argv){
  float battcurr;
  int   battfull; 
  int color;
  int ac_on;

  const char *txt;

  /* display whether on Battery or AC */
  ac_on = read_bool_prop(filename_ac);
  if (ac_on) {  
    printf("<fc=#%06x>AC</fc> - ", blue);
  }
  else {
    printf("<fc=#%06x>Batt</fc> - ", purple);
  }
  /* display battery level */
  battcurr  = (float) read_int_prop(filename_bat0); /* current battery level */
  if (battcurr > 0) {
    battfull  = read_int_prop(filename_maxbat0); /* max battery level */
    battcurr /= battfull;
    battcurr *= 100.0f;
    if (battcurr > 75.0) 
      color = green;
    else if (battcurr < 25.0)
      color = red;
    else 
      color = orange;
    printf("<fc=#%06x>%03.1f%%</fc>", color, battcurr);
  }
  else {
    printf("<fc=#%06x>error</fc>", grey, battcurr);
  }
  return 0;
}
