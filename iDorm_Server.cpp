// iDorm_Server.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <cstring>
#include <WinSock2.h>
#include "op.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	//初始化 DLL  
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字  
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字  
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充  
	sockAddr.sin_family = PF_INET;  //使用IPv4地址  
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //具体的IP地址  
	sockAddr.sin_port = htons(1234);  //端口  
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态  
	listen(servSock, 20);

	//接收客户端请求  
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

		//接收客户端的数据
		char Buf[500];
		recv(clntSock, Buf, strlen(Buf) + sizeof(char), 0);
		string reply = operate(handle(Buf));
		send(clntSock, reply.c_str(), reply.length(), 0);
		cout << reply << endl;

		closesocket(clntSock);
	}

	system("pause");
	//关闭套接字  
	closesocket(servSock);

	//终止 DLL 的使用  
	WSACleanup();

    return 0;
}

