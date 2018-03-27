/**
 * Name:Serial Port Send example
 * Date:3/27/2018
 * Auth:Blackbamboo
 * Description:
 *          Open 'COM10' and send from 1,
 *      each time increasing by 1, until sending one hundred thousand bytes.
 *   Press the button 'g' at the time of sending, and press other keys to stop sending
 *
 *   Just a simple case.Maybe helpful to someone.
 * Environment(what i used):
 *      windows10, CodeBlocks 17.12��
 *
*/


#include "stdio.h"
#include "Windows.h"
#include "string.h"
#include "stdlib.h"


/*
 * ws2_32.lib�ļ����ṩ�˶������������API��֧�֣�
 * ��ʹ�����е�API����Ӧ�ý�ws2_32.lib���빤��
 * ��������Ҫ��̬����ws2_32.dll��
*/
#pragma comment(lib,"ws2_32.lib")

int main(void)
{
	HANDLE hCom;//������
	//�򿪴���
	//���ںŴ��ڵ���10ʱ����ǰ��Ҫ��'\\\\.\\',���򲻼�
	hCom = CreateFile(TEXT("\\\\.\\COM10"),//com��
		GENERIC_READ | GENERIC_WRITE,      //�����д
		0,              //��ռģʽ
		NULL,           //��ȫ����
		OPEN_EXISTING,  //��ʽΪ�򿪶����Ǵ���
		0,              //�ļ�����
		NULL);
	if (hCom == INVALID_HANDLE_VALUE) {
		printf("wrong");
		exit(EXIT_FAILURE);
	}
	else {
		printf("OK\n");
	}
	SetupComm(hCom, 256, 256);//�����������������

	COMMTIMEOUTS Timeouts;

	//��ʼ������ʱ�ṹ
	Timeouts.ReadIntervalTimeout = 1000;        //�������ַ��������ʱ
	Timeouts.ReadTotalTimeoutMultiplier = 500;  //��ÿ���ַ��ĳ�ʱ
	Timeouts.ReadTotalTimeoutConstant = 5000;   //һ�ζ�ȡ�����ĳ�ʱ
	/*һ�ζ�ȡ�������ݵĹ̶���ʱ��������һ�ζ�ȡ���ڵĲ����У�
	�䳬ʱΪReadTotalTimeoutMultiplier���Զ�ȡ���ֽ����ټ���
	ReadTotalTimeoutConstant����ReadIntervalTimeout����ΪMAXDWORD��
	����ReadTotalTimeoutMultiplier ��ReadTotalTimeoutConstant����Ϊ0��
	��ʾ��ȡ�������������ش�������뻺�������ַ���
	*/



	//��ʼ��д��ʱ�ṹ
	Timeouts.WriteTotalTimeoutMultiplier = 500; //д��ÿ���ַ��ļ�ĳ�ʱ
	Timeouts.WriteTotalTimeoutConstant = 2000;  //һ��д�����ĳ�ʱ
	SetCommState(hCom, &Timeouts);              //�ýṹ�����ó�ʱ
	DCB dcb;                    //DCBΪ���ô��ڵ�һ���ṹ��
	dcb.BaudRate = 9600;        //������9600
	dcb.ByteSize = 8;           //8λ����λ
	dcb.Parity = NOPARITY;      //��У��λ
	dcb.StopBits = ONE5STOPBITS;//1λֹͣλ
	SetCommState(hCom, &dcb);   //���ô��ڲ���

	DWORD wCount=0;
	int wSend = 0,sendCt=0;
	BOOL bStat;
	char strNum[20] = { 0 };


	while (1) {
        printf("input g to run ,others will stop\n");
        while(1){
            if(getch() == 'g')break;
        }
		for (wSend = 100000; wCount <= 200000; wSend++) {
            //����������������
			PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR);
			sprintf_s(strNum,20, "%d", wSend);//������תΪ�ַ�
			printf("%s,%d\t", strNum,strlen(strNum));
            //�����Ƿ��а������£�Ϊ'g'��������У�����ֱֹͣ����'g'
			while((kbhit())){
			    while(1){
                    if(getch() == 'g') break;
                }
			}
			//�����ַ�
			bStat = WriteFile(hCom,//���
				strNum,         //���͵��ַ�����ַ
				strlen(strNum), //Ҫ���͵��ַ�����
				&sendCt,        //ʵ�ʷ��͵��ֽ������ظ���ֵ
				NULL            //��ʹ��ģ��
				);
            //��֤�Ƿ��ͳɹ�
			if (!bStat){
				printf("д����ʧ��!");
				return FALSE;
			}

			/*
			���ڽ��պ���
             bStat = ReadFile(hCom,     //���
                              strNum,   //�����ַ����Ļ�������ַ
                              20,       //Ҫ���յ��ַ�����
                              &sendCt,  //ʵ�ʽ��յ��ֽ������ظ���ֵ
                              NULL      //��ʹ��ģ��
                             );
            */

			//���������͵��ֽ�
            wCount = wCount + sendCt;
			Sleep(20);//ms
			printf("count:%ld\n", wCount);

		}
		//��ͣ��Ȼ����ռ���
		system("pause");
		wCount = 0;
	}
	//�ͷž��
    CloseHandle( hCom );
}
