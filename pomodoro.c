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
    printf("\033[1;32m>\033[0;31m@ ");
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
  printf("\r\033[0m%s\t", text);
}

void
draw(char* text, int color, int count)
{
  label(text);
  tomatoes(color, count);
}

int main()
{
  int mode_durs[3] = {25, 5, 15};
  char* mode_labels[3] =  {"\e[1mWork\e[0m", "Break", "Break"};
  int times_chimes[3][3] = {{1000, 1500, 3000}, 
                            {3000, 1500, 1000},
                            {1500, 700,  500}};
  int mode_seqs[8] = {0, 1, 0, 1, 0, 1, 0, 2};
  int tone_dur = 60;
  int cur_mode = 0;
  int cur_seq = 0;
  int cur_min = 0;
  int fd = open("/dev/console", O_WRONLY);

  while(1) {
    draw(mode_labels[cur_mode], GREY, mode_durs[cur_mode]);

    do {
      sleep(60);
      cur_min++;
      draw(mode_labels[cur_mode], COLOR, cur_min);
    } while(cur_min < mode_durs[cur_mode]);

    cur_seq = (cur_seq + 1) % 8;
    cur_mode = mode_seqs[cur_seq];
    cur_min = 0;
    chime(fd, times_chimes[cur_mode], tone_dur);
    printf("\n");
  }
  close(fd);
}
