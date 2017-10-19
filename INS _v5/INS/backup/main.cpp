#include "INS.h"

int main()
{
	INS* ins = new INS(COMPORT);
	INSInfo* insData = new INSInfo;

	while (1){
		system("cls");//����
		printf("--------------------write--------------------------\n");

		ins->initPos(120, 110, 0);//����װ��
		Sleep(1000);
		//ins->seeking();//��׼
		//ins->verify();//�Լ�  OK

		
		//printf("---------------------read-------------------------\n");
		//Sleep(1000);
		//ins->read();
		//printf("---------------------end-------------------------\n");
		//Sleep(1000);


		int result = ins->read(insData);
		if (result){
			printf("Error to read!\n");
		}
		else{
			printf("Connection established at port %s\n", COMPORT);
		}

		/***********TEST*****************/
		char p[1024] = {};
		result = ins->formatINSInfo(insData, p, 1024);
		if (!result){
			printf("success to formatINSInfo!\n");
		}
		printf("%s\n", p);
		Sleep(1000);
	}
	delete insData;
	return 0;
}