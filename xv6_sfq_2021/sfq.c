#include "types.h"
#include "stat.h"
#include "user.h"

#define N 3
#define PRINT_TIME 100000000
#define MAX_COUNTER 500000000

void sfq_test(void)
{
  int n, pid;

  printf(1, "simple fair queuing scheduling test\n");

  for (n = 0; n < N; n++)
  {
    pid = fork();
    if (pid < 0)
      break;
    if (pid == 0)
    {
      long long counter = 0;
      int print_counter = PRINT_TIME;

      int weight = n + 1;

      sfq(weight);
      uint start_ticks = uptime();
      uint end_ticks;
      int first = 1;

      while (counter <= MAX_COUNTER)
      {
        counter++;
        print_counter--;

        if (print_counter == 0)
        {
          if (first)
          {
            end_ticks = uptime();
            printf(1, "PID: %d (weight: %d), ", getpid(), weight);
            printf(1, "it takes %d milliseconds to complete %d additions.\n", (end_ticks - start_ticks) * 10, counter);
            first = 0;
          }
          print_counter = PRINT_TIME;
        }
      }

      printf(1, "PID: %d terminated\n", getpid());
      exit();
    }
  }

  for (; n > 0; n--)
  {
    if (wait() < 0)
    {
      printf(1, "wait stopped early\n");
      exit();
    }
  }

  if (wait() != -1)
  {
    printf(1, "wait got too many\n");
    exit();
  }

  printf(1, "simple fair queuing scheduling test OK\n");
}

int main(void)
{
  sfq_test();
  exit();
}
