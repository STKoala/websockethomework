#include <stdio.h>

int main() {
  FILE* fp;
  char path[1035];

  /* 执行命令 */
  fp = _popen("dir", "r");
  if (fp == NULL) {
    printf("无法执行命令\n");
    return 1;
  }

  /* 读取命令输出到缓冲区 */
  while (fgets(path, sizeof(path) - 1, fp) != NULL) {
    printf("%s", path);
  }

  /* 关闭文件指针 */
  _pclose(fp);
  return 0;
}
