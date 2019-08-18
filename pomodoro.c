#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

void
tomato(int color)
{
  if(color)
    printf("\033[0;32m>\033[0;31m@ ");
  else
    printf("\033[1;30m>@ ");
}

void
tomatoes(int color, int count)
{
  for(int i = 0; i < count; i++)
    tomato(color);
  fflush(stdout);
}

void
tone(int fd, int freq, int dur)
{
  ioctl(fd, KIOCSOUND, (int)(1193180/freq));
  usleep(1000 * dur);
  ioctl(fd, KIOCSOUND, 0);
}

void
chime(int fd, int freq, int dur)
{
  tone(fd, freq, dur);
  tone(fd, freq * 1.5, dur);
  tone(fd, freq * 2, dur);
}

void
label(char* text)
{
  printf("\r\033[0m");
  printf("%s\t", text);
}

void
draw(char* text, int color, int count)
{
  label(text);
  tomatoes(color, count);
}

int main()
{
  int times[2] = {25, 5};
  char* times_label[2] =  {"\e[1mWork\e[0m", "Break"};
  int timer_mode = 0;
  int cur_min = 0;
  int freq = 1000;
  int len = 50;
  int fd = open("/dev/console", O_WRONLY);

  draw(times_label[timer_mode], 0, times[timer_mode]);

    while(1) {
      draw(times_label[timer_mode], 1, cur_min);

      if(cur_min == times[timer_mode]) {
        timer_mode = (timer_mode)? 0 : 1;
        cur_min = 0;
        chime(fd, freq, len);
        printf("\n");
        draw(times_label[timer_mode], 0, times[timer_mode]);
      }
      sleep(60);
      cur_min++;
    }
  close(fd);
}
