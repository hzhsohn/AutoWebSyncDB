#include <iostream>
#include "develop_interface.h"
#include <stdio.h>
#include <string.h>


using namespace std;

int main()
{
	cout << "-----------打开设备接口测试-----------" << endl;
	cout << "**********打开设备状态：";
	
   
    struct vbar_channel *dev = vbar_connectDevice((unsigned long)"COM18");
    if(!dev) {
        cout << "打开设备失败**********\n";
		cout << "**********程序串口号默认为COM18,请手动修改**********" << endl;
        return -1;
    } else {
        cout << "打开设备成功**********\n";	
    }
	
	
#if 1	
	cout << "----------设置串口参数----------" << endl;
	cout << "串口参数：115200-8-N-1.波特率-数据位-校验位-停止位，按照配置工具中的配置项填写" << endl;
	cout << "本程序按配置的选项为115200-8-N-1" << endl;
	
	if(vbar_channel_config(dev,(unsigned long)"115200-8-N-1") == 0)
	{
		cout << "设置成功" <<endl;
	}
	else
	{
		cout << "设置失败" <<endl;
	}

	cout << "----------获取设备号接口测试----------" << endl;
	static unsigned char devnum[256]; 
	if(vbar_get_devnum(dev, devnum) == 0)
	{
		devnum[256] = '\0';
		cout << "设备号为:" << devnum <<endl;
	}
	else
	{
		cout << "未获取到设备号" <<endl;
	}
	cout << "----------获取版本号接口测试----------" << endl;
	static unsigned char devversion[256]; 
	int devlen = 0;
	if(vbar_getVersion(dev, devversion, &devlen) == 0)
	{
		devversion[256] = '\0';
		cout << "版本号为:" << devversion <<endl;
	}
	else
	{
		cout << "未获取到设备号" <<endl;
	}
	
	
	cout << "------------蜂鸣器接口测试-------------" << endl;
	int beepstimes;
	cout << "请输入蜂鸣器响声次数：";
	cin >> beepstimes;
	if(vbar_beepControl(dev, beepstimes) == 0)
	{
		cout << "蜂鸣器控制成功\n";
	} else {
		cout << "蜂鸣器控制失败\n";
	}
	cout << "-------------背光灯接口测试------------" << endl;
	bool backlight1;
	cout << "请输入 开灯1 关灯0 : ";
	cin >> backlight1;
    if(vbar_backlight(dev, backlight1) == 0) //设置间隔时间
	{
		cout << "灯光控制成功!\n";
	} else {
		cout << "灯光控制失败!\n";
	}
	bool backlight2;
	cout << "请输入 开灯1 关灯0 : ";
	cin >> backlight2;
    if(vbar_backlight(dev, backlight2) == 0) //设置间隔时间
	{
		cout << "灯光控制成功!\n";
	} else {
		cout << "灯光控制失败!\n";
	}


	cout << "-----------间隔时间接口测试------------" << endl;
	int interval;
	cout << "请输入间隔时间(毫秒)：";
	cin >> interval;
    if(vbar_interval(dev, interval) == 0) //设置间隔时间
	{
		cout << "间隔时间设置成功！\n";
	} else {
		cout << "间隔时间设置失败！\n";
	}

	cout << "------------码制添加接口测试------------" << endl;
    cout << "码制类型ID定义如下：\n";
	cout << "NONE           0      代表清空码制\n";
	cout << "QRCODE         1      添加支持QR码\n";
	cout << "EAN8           2	   添加支持EAN8码\n";
	cout <<	"EAN13          3      添加支持EAN13码\n";
	cout <<	"ISBN13         4      添加支持ISBN13码\n";
	cout <<	"CODE39         5      添加支持CODE39码\n";
	cout <<	"CODE93         6      添加支持CODE93码\n";
	cout <<	"CODE128        7      添加支持CODE128码\n";
	cout <<	"DATABAR        8      添加支持DATABAR码\n";
	cout <<	"DATABAR_EXP    9      添加支持DATABAR_EXP码\n";
	cout <<	"PDF417         10     添加支持PDF417码\n";
	cout <<	"DATAMATRIX     11     添加支持DATAMATRIX码\n";
	cout <<	"ITF            12     添加支持ITF码\n";
	cout <<	"ISBN10         13     添加支持ISBN10码\n";
	cout <<	"UPCE           14     添加支持UPCE码\n";
	cout <<	"UPCA           15     添加支持UPCA码\n";
	int codetype1,codetype2,codetype3;
	cout << "请输入码制对应数字(按回车切换下一个码制ID):";
	cin >> codetype1;
    if(vbar_addCodeFormat(dev, codetype1) == 0) 
	{
		cout << "码制" << codetype1 << "添加成功！\n";
	} else {
		cout << "码制添加失败！\n";
	}
	cin >> codetype2;
    if(vbar_addCodeFormat(dev, codetype2) == 0) 
	{
		cout << "码制" << codetype2 << "添加成功！\n";
	} else {
		cout << "码制" << codetype2 << "添加失败！\n";
	}
	cin >> codetype3;
    if(vbar_addCodeFormat(dev, codetype3) == 0) 
	{
		cout << "码制" << codetype3 << "添加成功！\n";
	} else {
		cout << "码制" << codetype3 << "添加失败！\n";
	}
	cout << "-------------开始解码测试---------------" << endl;
	cout << "按照添加成功的码制进行扫码测试(未添加过的码不能扫描，添加过应用不关闭一直有效)" << endl; 
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



