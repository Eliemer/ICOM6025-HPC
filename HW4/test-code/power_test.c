#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(){
  int a, b=2, c;
  char bin_buffer [33];
  for (a = 0; a < 10; a ++){
    c = a^b;
    printf("a: %d\tb: %d\tc: %d\n", a, b, c);
  }
}
