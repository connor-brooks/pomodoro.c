#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>

enum draw_mode{GREY, COLOR};

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
chime(int fd, int* pattern, int tone_dur)
{
  for(int i = 0; i < 3; i++) 
    tone(fd, pattern[i], tone_dur);
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
  int mode_durs[2] = {25, 5};
  char* mode_labels[2] =  {"\e[1mWork\e[0m", "Break"};
  int times_chimes[2][3] = {{1000, 1500, 3000}, {3000, 1500, 1000}};
  int tone_dur = 50;
  int cur_mode = 0;
  int cur_min = 0;
  int fd = open("/dev/console", O_WRONLY);

  draw(mode_labels[cur_mode], GREY, mode_durs[cur_mode]);

    while(1) {
      draw(mode_labels[cur_mode], COLOR, cur_min);

      if(cur_min == mode_durs[cur_mode]) {
        cur_mode = (cur_mode)? 0 : 1;
        cur_min = 0;
        chime(fd, times_chimes[cur_mode], tone_dur);
        printf("\n");
        draw(mode_labels[cur_mode], GREY, mode_durs[cur_mode]);
      }
      sleep(60);
      cur_min++;
    }
  close(fd);
}
