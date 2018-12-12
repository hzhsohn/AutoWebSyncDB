#include "udpprocess.h"

SOCKET s1,s2;

void recvpack(struct sockaddr_in *addr,TzhPacket *pack)
{
	int i;
	char buf[32];
	char ip[16];
	unsigned short port;
	
	zhPackReadInt(pack,&i);
	zhPackReadString(pack,buf);
	zhUdpAddrToPram(addr,ip,&port);
	printf("%s:%d->%d %s\n",ip,port,i,buf);
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
	zhSockInit(&s1,ZH_SOCK_UDP);
	if(zhSockSetReuseAddr(s1,true))
	{ zhSockBindAddr(s1,NULL,3329); }
	//»º³åÇøÄ¬ÈÏ8K 
	zhSockSetSendBufferSize(s1,64*1024);
    zhSockSetRecvBufferSize(s1,64*1024);

	zhSockInit(&s2,ZH_SOCK_UDP);
	if(zhSockSetReuseAddr(s2,true))
	{ zhSockBindAddr(s2,NULL,3330); }
	//»º³åÇøÄ¬ÈÏ8K 
	zhSockSetSendBufferSize(s2,64*1024);
    zhSockSetRecvBufferSize(s2,64*1024);
    
	CREATE_THREAD(udpThread,NULL);
	while(1)
	{
		TzhPacket pack;
		struct sockaddr_in addr;
		static int i=0;
		i++;
		//·¢ËÍ
		zhPackWriteInit(&pack);
		zhPackWriteInt(&pack,i);
		zhPackWriteString(&pack,"ºÙºÙ¹þ¹þ¹þ¹þ");
		zhUdpPramToAddr("127.0.0.1",2329,&addr);
		zhUdpSendToPack(s1,&pack,&addr);
		Sleep(500);
	}
	return 0;
}
