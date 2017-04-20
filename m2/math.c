int mod(int a, int b) {
  while(a > b)
    a = a - b;
  return a;
}

int div(int a, int b) {
  int c = 0;
  while(a > b) {
    a = a - b;
    c = c + 1;
  }
  return c;
}
