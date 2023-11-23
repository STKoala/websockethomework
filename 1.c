#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // 链接静态库

int main(int argc, char** argv) {
  //-----------------------------------------
  // Declare and initialize variables
  WSADATA wsaData;
  int iResult;

  DWORD dwError;
  int i = 0;

  struct hostent* remoteHost;  // 定义host entry结构体指针变量，定义见书p48。
  char* host_name;
  struct in_addr addr;  // 以各种形式存放4个字节的IPv4地址

  char** pAlias;  // 定义别名列表指针变量

  // Validate the parameters
  if (argc != 2) {
    printf(
      "usage: GetHostIP hostname\n");  // 提示程序运行时需要输入主机名作为参数
    return 1;
  }
  // Initialize Winsock
  iResult =
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // 与操作系统确认支持的WinSock版本
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }

  host_name = argv[1];  // 提取用户输入的主机名

  printf("Calling gethostbyname with %s\n", host_name);
  remoteHost = gethostbyname(host_name);
  // 获取主机的信息存放在hostent结构体中。该函数调用DNS系统来获取对应域名或主机名的相关信息，可以通过Wireshark抓包来进行验证。

  // 对返回结果进行判断
  if (remoteHost == NULL) {  // 报错，返回。
    dwError = WSAGetLastError();
    if (dwError != 0) {
      if (dwError == WSAHOST_NOT_FOUND) {
        printf("Host not found\n");
        return 1;
      }
      else if (dwError == WSANO_DATA) {
        printf("No data record found\n");
        return 1;
      }
      else {
        printf("Function failed with error: %ld\n", dwError);
        return 1;
      }
    }
  }
  else {  // 依次显示获取的主机名称、多个别名、地址类型、地址长度、地址列表
    printf("Function returned:\n");
    printf("\tOfficial name: %s\n", remoteHost->h_name);  // 显示主机名称
    for (pAlias = remoteHost->h_aliases; *pAlias != 0;
      pAlias++) {  // 循环显示别名
      printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
    }
    printf("\tAddress type: ");
    switch (remoteHost->h_addrtype) {
    case AF_INET:
    printf("AF_INET\n");  // 显示地址类型
    break;
    case AF_NETBIOS:
    printf("AF_NETBIOS\n");
    break;
    default:
    printf(" %d\n", remoteHost->h_addrtype);
    break;
    }
    printf("\tAddress length: %d\n", remoteHost->h_length);  // 显示地址长度

    i = 0;
    if (remoteHost->h_addrtype == AF_INET) {
      while (remoteHost->h_addr_list[i] != 0) {  // 循环显示主机地址
        addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
        printf("\tIP Address #%d: %s\n", i,
          inet_ntoa(addr));  // 将地址转换成点分十进制显示。
      }
    }
    else if (remoteHost->h_addrtype == AF_NETBIOS) {
      printf("NETBIOS address was returned\n");
    }
  }
  iResult = WSACleanup();  // 与操作系统确认支持的WinSock版本
  if (iResult != 0) {
    printf("WSACleanup failed: %d\n", iResult);
    return 1;
  }

  return 0;
}
