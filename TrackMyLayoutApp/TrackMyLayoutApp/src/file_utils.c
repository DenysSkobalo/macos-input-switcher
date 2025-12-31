#include <stdio.h>
#include <stdlib.h>

char *read_file_to_string(const char *path) {
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = (char *)malloc(size + 1);
  if (!buffer) {
    fclose(fp);
    return NULL;
  }
  fread(buffer, 1, size, fp);
  fclose(fp);
  buffer[size] = '\0';
  return buffer;
}
