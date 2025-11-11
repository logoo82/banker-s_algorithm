#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RSC 4
#define TH_NUM 5

//최대로 필요로 하는 자원 개수
int max[TH_NUM][RSC];
//작업을 완료하기 위해 필요한 자원의 개수
int need[TH_NUM][RSC];
//현재 사용하고 있는 자원들
int alloc[TH_NUM][RSC];
//사용 가능한 자원
int available[RSC];

void state();
void request(*request, );


int main(int argc, char* argv[])
{
    /*max.txt를 인자로 max 배열 초기화*/
    
    //파일을 읽기 모드로 열기 
    FILE* fs;
    fs = fopen("max.txt", "r");

    if(fs == NULL)
    {
        printf("파일 열기 실패\n");
        return 1;
    }
    
    //max 초기화
    for(int i = 0; i < TH_NUM; i++)
    {
        for(int j = 0; j < RSC; j++)
        {
            fscanf(fs, "%d", &max[i][j]);
        }
    }
    fclose(fs);

    /*파일을 실행할 때 인자를 받아 available 배열 채우기*/
    //available 초기화
    for(int i = 1; i < argc; i++)
    {
        available[i-1] = atoi(argv[i]);
    }
    
    //need, alloc 초기화
    for(int i = 0; i < TH_NUM; i++)
    {
        for(int j = 0; j < RSC; j++)
        {
            alloc[i][j] = 0;
            need[i][j] = max[i][j];
        }
    }

    //쓰레드의 번호(index)
    int thread_num;
    //사용자가 요청한 자원의 수
    int rsc_request[RSC];
    //사용자가 입력한 명령어
    char user_cmd[10];

    while(1)
    {
        printf(">> ");
        scanf("%s", user_cmd);


        if(strcmp(user_cmd, "exit") == 0 || strcmp(user_cmd, "quit") == 0)
        {
            printf("프로그램을 종료합니다\n");
            break;
        }
        else if(strcmp(user_cmd, "state") == 0)
        {
            state();
        }
        else if(strcmp(user_cmd, "RR") == 0)
        {
            scanf("%d", &thread_num);
            for(int i = 0; i < RSC; i++)
            {
                scanf("%d", &rsc_request[i]);
            }


        }
        else if(strcmp(user_cmd, "RL") == 0)
        {
            for(int i = 0; i < RSC; i++)
            {
                scanf("%d", &rsc_request[i]);
            }
        }
        

    }
}

//
void state()
{
    printf("available: ");
    for(int i = 0; i < RSC; i++)
    {
        printf("%d ", available[i]);
    }
    printf("\n\n");

    printf("max\t| alloc\t  | need\n\n");

    for(int i = 0; i < TH_NUM; i++)
    {
        for(int j = 0; j < RSC; j++)
        {
            printf("%d ", max[i][j]);
        }

        printf("| ");

        for(int j = 0; j < RSC; j++)
        {
            printf("%d ", alloc[i][j]);
        }

        printf("| ");

        for(int j = 0; j < RSC; j++)
        {
            printf("%d ", need[i][j]);
        }
        printf("\n\n");
    }
}