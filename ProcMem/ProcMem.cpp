// ProcMem.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
#pragma comment( lib, "psapi.lib" )
#include <Psapi.h>
#include <algorithm>

typedef struct _Task
{
	char* process_name;
	char* file_name;
}TaskInfo;

typedef TaskInfo* PTaskInfo;

bool LoadConfig(PTaskInfo &task_list, int &task_num, int &monitor_tick);

//获取指定名称的内存使用数，单位为KB
int getProcMemoryUsage2( char* pProcessName, char* filename)
{
	DWORD ret = 0;
	FILE* fp = fopen(filename, "a");
	if(!fp || !pProcessName)
		return 0;

	 char chBuf[1024];
	 ZeroMemory( chBuf , 1024 );
	 DWORD dwProcs[1024*2];
	 DWORD dwNeeded;

	 if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
	 {
		  //输出出错信息
		  sprintf( chBuf , "EnumProcesses Failed (%d).\n" , GetLastError() ) ;
		  return false;
	 }

	 //计算有多少个进程ID
	 DWORD dwProcCount =  dwNeeded / sizeof( DWORD );

	 HMODULE hMod;
	 DWORD arraySize;
	 char processName[512];

	 for( int i=0; i<dwProcCount ; i++ )
	 {
		  DWORD m_processid = dwProcs[i];
		  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE,3444); //此函数不能读取system用户名的进程，局限性很大
		  if(hProcess)
		  {
			   if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
			   {
					GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
					strupr(processName);
					strupr(pProcessName);

					if( strcmp(processName , pProcessName )==0 )
					{
						 PROCESS_MEMORY_COUNTERS pmc;
						 GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));
						 SYSTEMTIME sys;
						 GetLocalTime( &sys );
						 fprintf(fp, "%d-%02d-%02d %02d:%02d%:%02d\t%8d,%03d\t%8d,%03d\n" ,
							 sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond,
							 pmc.WorkingSetSize / 1024 / 1000, pmc.WorkingSetSize / 1024 % 1000,
							 pmc.PeakWorkingSetSize / 1024 / 1000, pmc.PeakWorkingSetSize / 1024 % 1000);
						 fclose(fp);
						 //break;
					}
					else
					{
						//printf("%s\n", processName);
					}
			   }
			   else
			   {
				   ret = GetLastError();
			   }
		  }
	 }

	return -1;
}

int getProcMemoryUsage(TaskInfo& task_info)
{
	char cmd[128];
	ZeroMemory(cmd, 128);
	char result[1024];
	ZeroMemory(result, 1024);
	FILE* fp_result = fopen(task_info.file_name, "a");
	if (!fp_result)
		return -1;

	FILE* fp_tmp = fopen("tmp", "w");
	fclose(fp_tmp);
	sprintf(cmd, "tasklist /FI \"IMAGENAME eq %s\" >> tmp", task_info.process_name);
	system(cmd);

	fp_tmp = fopen("tmp", "r");
	fseek(fp_tmp,-15,SEEK_END);
	fgets(result, 14, fp_tmp);
	fclose(fp_tmp);

	SYSTEMTIME sys;
	GetLocalTime( &sys );
	
	fprintf(fp_result, "%d-%02d-%02d %02d:%02d%:%02d\t%sB\n" ,
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond,
		result);
	fclose(fp_result);
	return 0;
}

int main(int argc, char* argv[])
{
	PTaskInfo task_list;
	int task_num = 0;
	int monitor_tick = 0;

	if(!LoadConfig(task_list, task_num, monitor_tick))
	{
		return -1;
	}
	// output
	printf("monitoring every %d second...", monitor_tick);
	while(1)
	{
		for(int i = 0; i<task_num; i++)
		{
			getProcMemoryUsage(task_list[i]);
		}

		Sleep(monitor_tick * 500);
	}
	return 0;
}

bool LoadConfig(PTaskInfo &task_list, int &task_num, int &monitor_tick)
{
	char tmp[100];
	int idx = 0;

	// open config
	FILE* fp = fopen("config.ini", "r");
	if(!fp)
	{
		printf("Find config.ini failed.");
		getchar();
		return false;
	}

	// get tick
	ZeroMemory(tmp, 100);
	fgets(tmp, 100, fp);
	monitor_tick = atoi(tmp);

	// get task count
	while(!feof(fp))
	{
		ZeroMemory(tmp, 100);
		fgets(tmp, 100, fp);
		
		if(strlen(tmp) > 1)
			task_num++;
	}
	task_list = (PTaskInfo) new TaskInfo[sizeof(TaskInfo) * task_num];

	// get taskinfo list
	fseek(fp, 0, SEEK_SET);
	fgets(tmp, 100, fp);
	while(!feof(fp))
	{
		ZeroMemory(tmp, 100);
		fgets(tmp, 100, fp);
		if(strlen(tmp) < 1)
			continue;
		tmp[strlen(tmp) - 1] = 0; // delete '\n'

		task_list[idx].process_name = new char[strlen(tmp) + 1];
		ZeroMemory(task_list[idx].process_name, strlen(tmp) + 1);
		strcpy(task_list[idx].process_name, tmp);

		task_list[idx].file_name = new char[strlen(tmp) + strlen(".txt") + 1];
		ZeroMemory(task_list[idx].file_name, strlen(tmp) + strlen(".txt") + 1);
		strcat(tmp, ".txt");
		strcpy(task_list[idx].file_name, tmp);
		++idx;
	}

	return true;
}