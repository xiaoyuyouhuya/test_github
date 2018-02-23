#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<limits.h>
#include<errno.h>
#include<signal.h>
#include<errno.h>

#include"thread_pool.h"
#include<pthread.h>

int reboot = 1;

void creat_master(){

	pid_t PID;
	if((PID = fork()) > 0){


		FILE* fstream;
		fstream = fopen("/home/centos/ys/ys_test_1/master.pid", "w");
		if(fstream == NULL)//如果失败了
		{
			printf("错误！");
			exit(1);//中止程序
		}

		fprintf(fstream, "%d\n", PID);

		if (fclose(fstream)){
			perror("close file"); //报告相应错误
		}
		printf("Master created!:%d\n", PID);

		exit(0);
	}
	else if(PID < 0){
		fprintf(stderr, "Master create error!\n");
		exit(1);	
	}
	else if(PID == 0){
		printf("this is master\n");
		do_master();
		while(1);
	}
}


void do_worker(){

	printf("This is worker\n");
	//producer_consumer_model();
	while(1);
	//kill(getppid(),SIGINT);
	exit(0);
}

void do_sig(int a){			//信号处理函数
	printf("This is a signal!\n");

	reboot = 1;

	signal(SIGCHLD, do_sig); // 注册信号监控

}





void do_master(){

	int status;
	pid_t pid = getpid();

	signal(SIGCHLD, do_sig); // 注册信号监控
	//signal(SIGCHLD, ClearChild);

	while(1){
		if(reboot){
			pid_t PIDW;
			if((PIDW = fork()) < 0){
				fprintf(stderr, "Worker create error!\n");
				exit(1);
			}

			if(PIDW == 0){
				do_worker();
			}

			else{
				reboot = 0;
				printf("MASTER!\n");
				FILE* fstream = fopen("/home/centos/ys/ys_test_1/worker.pid", "w");
				fprintf(fstream, "%d\n", PIDW);
				printf("%x\n", fstream);
				fflush(0);
				if (fclose(fstream)){
					printf("2");
					fflush(0);
					perror("close file"); //报告相应错误
				}
				printf("3\n");
				fflush(0);
			}

			waitpid(PIDW, &status, 0);
		}
	}
}


int main(int argc, char *argv[]){

	if(argc == 1){
		creat_master();
	}
	else if(argc == 2){
		printf("%d, %s, %s\n", argc, argv[0], argv[1]);
		if(strcmp(argv[1], "restart") == 0){
			pid_t PID;
			FILE* fpreadm = fopen("/home/centos/ys/ys_test_1/master.pid", "r");
			fscanf(fpreadm, "%d", &PID);
			if (fclose(fpreadm)){
				perror("close file"); //报告相应错误
			}
			pid_t PIDW;
			FILE* fpreadw = fopen("/home/centos/ys/ys_test_1/worker.pid", "r");
			fscanf(fpreadw, "%d", &PIDW);
			if (fclose(fpreadw)){
				perror("close file"); //报告相应错误
			}

			kill(PIDW, SIGKILL);
			kill(PID, SIGKILL);
			creat_master();
		}
		else if(strcmp(argv[1], "shutdown") == 0){
			pid_t PID;
			FILE* fpreadm = fopen("/home/centos/ys/ys_test_1/master.pid", "r");
			fscanf(fpreadm, "%d", &PID);
			if (fclose(fpreadm)){
				perror("close file"); //报告相应错误
			}
			pid_t PIDW;
			FILE* fpreadw = fopen("/home/centos/ys/ys_test_1/worker.pid", "r");
			fscanf(fpreadw, "%d", &PIDW);
			if (fclose(fpreadw)){
				perror("close file"); //报告相应错误
			}

			kill(PIDW, SIGKILL);
			kill(PID, SIGKILL);
			printf("Shutdown succeed!\n");
		}
		/*else if(strcmp(argv[1], "shutdown") == 0){
			pid_t PID;
			FILE* fpreadm = fopen("/home/centos/ys/master.pid", "r");
			fscanf(fpreadm, "%d", &PID);
			if (fclose(fpreadm)){
				perror("close file"); //报告相应错误
			}
			pid_t PIDW;
			FILE* fpreadw = fopen("/home/centos/ys/worker.pid", "r");
			fscanf(fpreadw, "%d", &PIDW);
			if (fclose(fpreadw)){
				perror("close file"); //报告相应错误
			}

			kill(PIDW, SIGKILL);
			kill(PID, SIGKILL);
			printf("Shutdown succeed!\n");
		}
		else if(strcmp(argv[1], "shutdown") == 0){
			pid_t PID;
			FILE* fpreadm = fopen("/home/centos/ys/master.pid", "r");
			fscanf(fpreadm, "%d", &PID);
			if (fclose(fpreadm)){
				perror("close file"); //报告相应错误
			}
			pid_t PIDW;
			FILE* fpreadw = fopen("/home/centos/ys/worker.pid", "r");
			fscanf(fpreadw, "%d", &PIDW);
			if (fclose(fpreadw)){
				perror("close file"); //报告相应错误
			}

			kill(PIDW, SIGKILL);
			kill(PID, SIGKILL);
			printf("Shutdown succeed!\n");
		}*/
	}

}
