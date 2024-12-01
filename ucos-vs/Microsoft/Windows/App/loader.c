#include <os.h>
#include "luyouliang.h"
#include "usercode.h"

#define TASK_STK_SIZE   512
OS_STK	TaskStk[OS_MAX_TASKS][TASK_STK_SIZE];

int MenuLoader(void)
{
	int Experiment;

	printf("0.û���û�����\n");
	printf("1.��һ�����ӣ�һ���û�����\n");
	printf("2.�ڶ������ӣ�����������CPU��������\n");
	printf("3.���������ӣ�����Ĺ���ͻָ�\n");
	printf("4.���ĸ����ӣ��ź�������\n");
	printf("5.��������ӣ������ź�������\n");
	printf("6.���������ӣ��¼���־��\n");
	printf("7.���߸����ӣ���Ϣ����\n");
	printf("8.�ڰ˸����ӣ���Ϣ����\n");
	printf("9.�ھŸ����ӣ��ڴ����\n");
	printf("10. User hello world\n");

	printf("���������ѡ������:\n");
	scanf_s("%d", &Experiment);
	if ((Experiment<0) || (Experiment>10))
	{
		printf("��Ч������!");
		return 1;
	}

	OSInit();
	switch (Experiment)
	{
	case 1://һ����������
		OSTaskCreate(FirstTask, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		break;
	case 2://����������CPU
		OSTaskCreate(E2_task1, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E2_task2, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		break;
	case 3://����Ĺ���ͻָ�
		OSTaskCreate(E3_task0, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(E3_task1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(E3_task2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 4://�ź�����������
		OSTaskCreate(UserTaskSemA, 0, &TaskStk[5][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(UserTaskSemB, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(UserTaskSemC, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		break;
	case 5://�����ź�����������
		OSTaskCreate(TaskMutex1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMutex2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 50);
		OSTaskCreate(TaskPrint, 0, &TaskStk[8][TASK_STK_SIZE - 1], 30);
		break;
	case 6://�¼���־���������
		OSTaskCreate(TaskDataProcess, 0, &TaskStk[5][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskIO1, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskIO2, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		OSTaskCreate(TaskIO3, 0, &TaskStk[8][TASK_STK_SIZE - 1], 8);
		OSTaskCreate(TaskIO4, 0, &TaskStk[9][TASK_STK_SIZE - 1], 9);
		break;
	case 7://��Ϣ����
		OSTaskCreate(TaskMessageSen, 0, &TaskStk[6][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskMessageRec, 0, &TaskStk[7][TASK_STK_SIZE - 1], 7);
		break;
	case 8://��Ϣ����
		OSTaskCreate(TaskQSen, 0, &TaskStk[7][TASK_STK_SIZE - 1], 5);
		OSTaskCreate(TaskQRec, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		OSTaskCreate(TaskQRec, 0, &TaskStk[9][TASK_STK_SIZE - 1], 7);
		break;
	case 9://�ڴ����
		OSTaskCreate(TaskM, 0, &TaskStk[8][TASK_STK_SIZE - 1], 6);
		break;
	case 10:
		OSTaskCreate(helloworld, 0, &TaskStk[6][TASK_STK_SIZE - 1], 10);
		break;
	default:
		;
	}
	return 0;
}
