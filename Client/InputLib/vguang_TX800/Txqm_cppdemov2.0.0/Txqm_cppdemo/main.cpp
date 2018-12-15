#include <iostream>
#include "develop_interface.h"
#include <stdio.h>
#include <string.h>


using namespace std;

int main()
{
	cout << "-----------���豸�ӿڲ���-----------" << endl;
	cout << "**********���豸״̬��";
	
   
    struct vbar_channel *dev = vbar_connectDevice((unsigned long)"COM18");
    if(!dev) {
        cout << "���豸ʧ��**********\n";
		cout << "**********���򴮿ں�Ĭ��ΪCOM18,���ֶ��޸�**********" << endl;
        return -1;
    } else {
        cout << "���豸�ɹ�**********\n";	
    }
	
	
#if 1	
	cout << "----------���ô��ڲ���----------" << endl;
	cout << "���ڲ�����115200-8-N-1.������-����λ-У��λ-ֹͣλ���������ù����е���������д" << endl;
	cout << "���������õ�ѡ��Ϊ115200-8-N-1" << endl;
	
	if(vbar_channel_config(dev,(unsigned long)"115200-8-N-1") == 0)
	{
		cout << "���óɹ�" <<endl;
	}
	else
	{
		cout << "����ʧ��" <<endl;
	}

	cout << "----------��ȡ�豸�Žӿڲ���----------" << endl;
	static unsigned char devnum[256]; 
	if(vbar_get_devnum(dev, devnum) == 0)
	{
		devnum[256] = '\0';
		cout << "�豸��Ϊ:" << devnum <<endl;
	}
	else
	{
		cout << "δ��ȡ���豸��" <<endl;
	}
	cout << "----------��ȡ�汾�Žӿڲ���----------" << endl;
	static unsigned char devversion[256]; 
	int devlen = 0;
	if(vbar_getVersion(dev, devversion, &devlen) == 0)
	{
		devversion[256] = '\0';
		cout << "�汾��Ϊ:" << devversion <<endl;
	}
	else
	{
		cout << "δ��ȡ���豸��" <<endl;
	}
	
	
	cout << "------------�������ӿڲ���-------------" << endl;
	int beepstimes;
	cout << "���������������������";
	cin >> beepstimes;
	if(vbar_beepControl(dev, beepstimes) == 0)
	{
		cout << "���������Ƴɹ�\n";
	} else {
		cout << "����������ʧ��\n";
	}
	cout << "-------------����ƽӿڲ���------------" << endl;
	bool backlight1;
	cout << "������ ����1 �ص�0 : ";
	cin >> backlight1;
    if(vbar_backlight(dev, backlight1) == 0) //���ü��ʱ��
	{
		cout << "�ƹ���Ƴɹ�!\n";
	} else {
		cout << "�ƹ����ʧ��!\n";
	}
	bool backlight2;
	cout << "������ ����1 �ص�0 : ";
	cin >> backlight2;
    if(vbar_backlight(dev, backlight2) == 0) //���ü��ʱ��
	{
		cout << "�ƹ���Ƴɹ�!\n";
	} else {
		cout << "�ƹ����ʧ��!\n";
	}


	cout << "-----------���ʱ��ӿڲ���------------" << endl;
	int interval;
	cout << "��������ʱ��(����)��";
	cin >> interval;
    if(vbar_interval(dev, interval) == 0) //���ü��ʱ��
	{
		cout << "���ʱ�����óɹ���\n";
	} else {
		cout << "���ʱ������ʧ�ܣ�\n";
	}

	cout << "------------������ӽӿڲ���------------" << endl;
    cout << "��������ID�������£�\n";
	cout << "NONE           0      �����������\n";
	cout << "QRCODE         1      ���֧��QR��\n";
	cout << "EAN8           2	   ���֧��EAN8��\n";
	cout <<	"EAN13          3      ���֧��EAN13��\n";
	cout <<	"ISBN13         4      ���֧��ISBN13��\n";
	cout <<	"CODE39         5      ���֧��CODE39��\n";
	cout <<	"CODE93         6      ���֧��CODE93��\n";
	cout <<	"CODE128        7      ���֧��CODE128��\n";
	cout <<	"DATABAR        8      ���֧��DATABAR��\n";
	cout <<	"DATABAR_EXP    9      ���֧��DATABAR_EXP��\n";
	cout <<	"PDF417         10     ���֧��PDF417��\n";
	cout <<	"DATAMATRIX     11     ���֧��DATAMATRIX��\n";
	cout <<	"ITF            12     ���֧��ITF��\n";
	cout <<	"ISBN10         13     ���֧��ISBN10��\n";
	cout <<	"UPCE           14     ���֧��UPCE��\n";
	cout <<	"UPCA           15     ���֧��UPCA��\n";
	int codetype1,codetype2,codetype3;
	cout << "���������ƶ�Ӧ����(���س��л���һ������ID):";
	cin >> codetype1;
    if(vbar_addCodeFormat(dev, codetype1) == 0) 
	{
		cout << "����" << codetype1 << "��ӳɹ���\n";
	} else {
		cout << "�������ʧ�ܣ�\n";
	}
	cin >> codetype2;
    if(vbar_addCodeFormat(dev, codetype2) == 0) 
	{
		cout << "����" << codetype2 << "��ӳɹ���\n";
	} else {
		cout << "����" << codetype2 << "���ʧ�ܣ�\n";
	}
	cin >> codetype3;
    if(vbar_addCodeFormat(dev, codetype3) == 0) 
	{
		cout << "����" << codetype3 << "��ӳɹ���\n";
	} else {
		cout << "����" << codetype3 << "���ʧ�ܣ�\n";
	}
	cout << "-------------��ʼ�������---------------" << endl;
	cout << "������ӳɹ������ƽ���ɨ�����(δ��ӹ����벻��ɨ�裬��ӹ�Ӧ�ò��ر�һֱ��Ч)" << endl; 
	static unsigned char result_buffer[1024];
    int datalen = 0;
    int datatype = 0;
    for(;;) {
        int ret = vbar_getResultStr(dev, result_buffer, &datalen, &datatype);
        if(ret < 0){
                ;
        }
        else{
            if(datalen > 0) {
                ret = vbar_beepControl(dev, 1);
				result_buffer[datalen] = '\0';
                cout << "decode:" << result_buffer <<endl;
            }
        }
    }
	#endif
    return 0;
}



