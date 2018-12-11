#include "udpprocess.h"
#include "packet.h"

SOCKET s1;

void recvpack(struct sockaddr_in *addr,TzhPacket *pack)
{
	int i;
	char buf[32];
	char ip[16];
	unsigned short port;
	
	zhPackReadInt(pack,&i);
	zhPackReadString(pack,buf);
	zhUdpAddrToPram(addr,ip,&port);
	printf("%s:%d->%d %s len=%d\n",ip,port,i,buf,pack->wSize);
	
	zhPackWriteInit(pack);
	zhPackWriteInt(pack,i);
	zhPackWriteString(pack,"收到了");
	zhUdpSendToPack(s1,pack,addr);
}

void udpThread()
{
	TzhPacket pack;
	int ret;
	unsigned char recvbuf[1024];
	struct sockaddr_in addr;
	int addrlen=sizeof(struct sockaddr_in);
	while(true)
	{
		Sleep(1);
		ret=zhUdpRecvFrom(s1,recvbuf,sizeof(recvbuf),&addr,&addrlen);
		if(ret>0)
		{
			zhPackReadInit(&pack,recvbuf,ret);
			recvpack(&addr,&pack);
		}
	}
}

int main()
{
	//socket1
	zhSockInit(&s1,ZH_SOCK_UDP);
	if(zhSockSetReuseAddr(s1,true))
	{
		printf("打开udp:2329端口\n");
		zhSockBindAddr(s1,NULL,2329); 
	}
	//缓冲区默认8K 
	zhSockSetSendBufferSize(s1,64*1024);
    zhSockSetRecvBufferSize(s1,64*1024);

	CREATE_THREAD(udpThread,NULL);
	getchar();
	return 0;
}
