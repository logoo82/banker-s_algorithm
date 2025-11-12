#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

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

//자원 상태 출력 함수
void state();
//자원 요청 함수
void request(int thread_num,int rsc_request[]);
bool safe_unsafe();



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

    //사용자 명령 입력받기
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
            request(thread_num, rsc_request);
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

void request(int thread_num, int res_request[])
{
    //thread의 번호 검사
    if(thread_num < 0 || thread_num > TH_NUM)
    {
        printf("올바른 thread 번호가 아닙니다\n");
        return;

    }

    //1단계: request <= need인지 검사
    for(int i = 0; i < RSC; i++)
    {
        if(res_request[i] > need[thread_num][i])
        {
            printf("최대 요청치를 초과하였습니다\n");
            return;
        }
    }

    //2단계: request <= available인지 검사
    for(int i = 0; i < RSC; i++)
    {
        if(res_request[i] > available[i])
        {
            printf("현재 사용 가능 자원 초과\n");
            return;
        }
    }

    //3단계: 요청된 자원을 쓰레드에 할당
    for(int i = 0; i < RSC; i++)
    {
        available[i] = available[i] - res_request[i];
        alloc[thread_num][i] = alloc[thread_num][i] + res_request[i];
        need[thread_num][i] = need[thread_num][i] - res_request[i]; 
    }

}

bool safe_unsafe()
{
    int work[RSC];
    //각 쓰레드의 작업 완료 여부
    bool Finish[TH_NUM] = {false};
    //현재까지 작업 완료된 쓰레드 수
    int cnt = 0;

    //안전한 작업 순서
    int safe_job[TH_NUM];

    //work를 available로 초기화
    for(int i = 0; i < RSC; i++)
    {
        work[i] = available[i];
    }

    //작업을 완료하지 않은 쓰레드가 남아있을 경우 반복
    while(cnt < TH_NUM)
    {
        bool found = false;

        for(int i = 0; i < TH_NUM; i++)
        {
            if(Finish[i] == false)
            {
                int j;
                for(j = 0; i < RSC; j++)
                {
                    if(need[i][j] > work[j])
                        break;
                }

                if(j == RSC)
                {
                    for(int k = 0; k < RSC; k++)
                    {
                        work[k] += alloc[i][k];
                    }

                    safe_job[cnt] = i;
                    cnt++;

                    Finish[i] = true;

                    found = true;
                }
            }
        }

        if(found == false)
            return false;
    }

    return true;

    
}

//현재 상태 출력
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