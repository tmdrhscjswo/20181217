#include <stdio.h>
#include <stdlib.h> 

#define ARR_SIZE_R 15			// 배열 행 크기
#define ARR_SIZE_C 15			// 배열 열 크기
#define WIN_NUMBER 5			// 오목 승리 숫자

int display(void);							// 초기 화면 출력 함수
int omok_play(void);					// 게임 시작 함수	

int set_pan(char [][ARR_SIZE_C]);								// 오목판 초기값 입력 함수
int print_pan(char [][ARR_SIZE_C]);							// 오목판 출력 함수
int input_player(int*, int*, int*, char [][ARR_SIZE_C]);		// 사용자 위치 입력 함수
int input_check(int, int, char [][ARR_SIZE_C]);				// 입력 위치 가능 여부 검사 함수

int win_check(int, int, int, char [][ARR_SIZE_C]);					// 승리 여부 검사 함수
int horizontal_check(char, int, int, char [][ARR_SIZE_C]);		// 가로줄 검사
int	vertical_check(char, int, int, char [][ARR_SIZE_C]);			// 세로줄 검사
int	diagonal1_check(char, int, int, char [][ARR_SIZE_C]);		// 대각선1 검사	
int	diagonal2_check(char, int, int, char [][ARR_SIZE_C]);		// 대각선2 검사

int retry_game(void);		// 게임 재 시작 여부 확인 함수

// 메인 함수
int main(void)
{
	display();			// 초기 화면 출력 함수

	omok_play();		// 오목 게임 시작 함수

	return 0;
}

// 초기 화면 출력
int display(void)
{
	printf("  =========================================\n");
	printf("               오 목 게 임 (%d X %d)\n", ARR_SIZE_R, ARR_SIZE_C);
	printf("  =========================================\n");

	return 0;
}

// 게임 시작 함수
int omok_play(void)
{
	char omok_pan[ARR_SIZE_R][ARR_SIZE_C];				// 오목 판 배열 선언
	int panIdx_r=0, panIdx_c=0, play=1, initialize=1, win, sPlayer, ckCnt;		// 기타 변수 선언(배열 행 인덱스, 열 인덱스, 게임시작종료, 초기화, 승리, 플레이어, 돌의 수)

	while(play)		// 게임 시작(play가 1일 경우)
	{
		if(initialize)		// 초기화 변수가 참이면. (최초 게임 시작할때)
		{
			set_pan(omok_pan);		// 오목판 초기화 함수 호출
			sPlayer = 1;					// 플레이어 선택
			win = 0;						// 승리 여부 변수
			ckCnt = 1;					// 반복문의 반복수(돌의 개수)
			initialize = 0;					// 초기화 변수

		}		

		print_pan(omok_pan);			// 오목판 출력

		if(win == 0)			// 이긴 사용자가 나오지 않을 경우
		{
			input_player(&sPlayer, &panIdx_r, &panIdx_c, omok_pan);		// 사용자 입력 함수 호출

			if(ckCnt >= (WIN_NUMBER*2)-1)				// 카운터가 일정 수 이상일때 승리 조건 검사
				win = win_check(sPlayer, panIdx_r-1, panIdx_c-1, omok_pan);		// 승리 조건 검사 함수 호출
		}
		else				// 승리한 사용자가 나온 경우
		{
			printf("%s이 승리 하였습니다.\n\n", (win == 1)?"검은돌":"하얀돌");		// 승리 메시지 출력

			play = retry_game();			// 게임의 재 시작 여부 함수 호출

			if(play == 1)				// 재시작 할 경우
			{
				initialize = 1;			// 초기화 변수의 값을 1로 변경하여 사용되는 변수 초기화
			}
		}

		if(win == 0 && sPlayer == 1)				// 사용자 입력 후 다음 사용자로 변경
			sPlayer = 2;
		else if(win == 0 && sPlayer == 2)		// 사용자 입력 후 다음 사용자로 변경
			sPlayer = 1;

		ckCnt++;				// 반복 카운터

		if(play == 1)				// 게임이 실행 중에 경우,
			system("cls");			// 화면 지우기
	}

	return 0;
}

// 오목판의 초기값 입력 함수
int set_pan(char omok_pan[][ARR_SIZE_C])
{
	int nIdx_r, nIdx_c;

	for(nIdx_r=0; nIdx_r<ARR_SIZE_R; nIdx_r++)
	{
		for(nIdx_c=0; nIdx_c<ARR_SIZE_C; nIdx_c++)
		{
			omok_pan[nIdx_r][nIdx_c] = '_';			// 오목판 배열의 값을 '_' 문자로 저장
		}
	}

	return 0;
}

// 오목판 출력 함수
int print_pan(char omok_pan[][ARR_SIZE_C])
{
	int nIdx_r, nIdx_c, nCnt;

	puts("  -----------------------------------------");
	puts("\t\t오목판 상황");
	puts("  -----------------------------------------\n");

	for(nCnt=1; nCnt<=ARR_SIZE_C; nCnt++)			// 열 번호 출력
	{	
		if(nCnt == 1)
		{
			printf("    ");
		}

		printf("%3d ", nCnt);
	}

	puts("\n");

	for(nIdx_r=0; nIdx_r<ARR_SIZE_R; nIdx_r++)			// 행 번호와 배열 내용 출력
	{
		printf("%3d ", nIdx_r+1);

		for(nIdx_c=0; nIdx_c<ARR_SIZE_C; nIdx_c++)
		{
			printf(" %2c ", omok_pan[nIdx_r][nIdx_c]);
		}
		puts("\n");
	}

	return 0;
}

// 사용자 오목 알 입력 함수
int input_player(int* sPlayer, int* pIdx_r, int* pIdx_c, char omok_pan[][ARR_SIZE_C])
{
	int inFlag=0;			// 입력 받은 값이 가능한지 확인 변수

	while(inFlag == 0)			// 잘못된 범위나 위치의 값을 입력하면 계속 입력
	{
		printf("\n%s돌을 입력할 위치를 입력하시오[행 열] : ", (*sPlayer == 1)?"검은[X]":"하얀[O]");			// 사용사에 따른 입력 문자 출력
		scanf("%d %d", pIdx_r, pIdx_c);			// 행과 열을 입력

		inFlag = input_check(*pIdx_r, *pIdx_c, omok_pan);			// 입력한 위치가 배열에서 입력 가능한지 검사

		if(inFlag)			// 입력 가능할 경우
			omok_pan[*pIdx_r-1][*pIdx_c-1] = (*sPlayer == 1)?'X':'O';		// 해당 사용자의 문자를 배열의 해당 위치에 입력
		else				// 잘못된 위치를 입력한 경우
			puts("입력할 곳에 이미 돌이 있거나 범위를 벗어났습니다.");
	}

	return 0;
}

// 입력 가능 여부 검사 함수
int input_check(int pIdx_r, int pIdx_c, char omok_pan[][ARR_SIZE_C])
{
	int inFlag=0;			// 입력 가능 여부 검사 변수

	if((pIdx_r-1>=0 && pIdx_r-1<ARR_SIZE_R) && (pIdx_c-1>=0 && pIdx_c-1<ARR_SIZE_C))			// 배열이 범위 안이고
	{
		if(omok_pan[pIdx_r-1][pIdx_c-1] == '_')			// 오목판 배열의 빈 공간 문자일 경우
			inFlag=1;			// 플레그 값 1
		else					// 오목판 배열의 빈 공간 문자가 아닌 경우
			inFlag = 0;		// 플레그 값 0
	}
	else		// 범위를 벗어난 경우
	{
		inFlag = 0;			// 플레그 값 0
	}

	return inFlag;		// 플레그 값이 넘겨줌
}

// 승리 여부 검사 함수
int win_check(int sPlayer, int panIdx_r, int panIdx_c, char omok_pan[][ARR_SIZE_C])
{
	char cStone = (sPlayer == 1)?'X':'O';			// 각 사용자의 문자를 특정 변수에 입력
	int win=0;			// 승리 여부 변수

	if(sPlayer == 1)			// 1번 플레이어일 경우
	{
		// 돌이 승리 숫자와 같을 경우
		if((horizontal_check(cStone, panIdx_r, panIdx_c, omok_pan) == WIN_NUMBER) ||		// 가로줄 검사
				(vertical_check(cStone, panIdx_r, panIdx_c, omok_pan) == WIN_NUMBER) ||			// 세로줄 검사
				(diagonal1_check(cStone, panIdx_r, panIdx_c, omok_pan) == WIN_NUMBER) ||		// 대각선1 검사				
				(diagonal2_check(cStone, panIdx_r, panIdx_c, omok_pan) == WIN_NUMBER))		// 대각선2 검사
		{
			win = 1;		
		}
	}
	else if(sPlayer == 2)			// 2번 플레이어일 경우
	{
		// 돌이 승리 숫자와 같거나 클 경우
		if((horizontal_check(cStone, panIdx_r, panIdx_c, omok_pan) >= WIN_NUMBER) ||		// 가로줄 검사
				(vertical_check(cStone, panIdx_r, panIdx_c, omok_pan) >= WIN_NUMBER) ||			// 세로줄 검사
				(diagonal1_check(cStone, panIdx_r, panIdx_c, omok_pan) >= WIN_NUMBER) ||		// 대각선1 검사				
				(diagonal2_check(cStone, panIdx_r, panIdx_c, omok_pan) >= WIN_NUMBER))		// 대각선2 검사
		{
			win = 2;
		}
	}

	return win;
}

// 가로줄 검사 함수
int horizontal_check(char cStone, int panIdx_r, int panIdx_c, char omok_pan[][ARR_SIZE_C])
{	
	int winCnt=0, pIdx_c, mIdx_c;

	pIdx_c = mIdx_c = panIdx_c;			// 배열의 위치값을 저장

	while(omok_pan[panIdx_r][pIdx_c--] == cStone)		// 가로줄의 왼쪽 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}
	while(omok_pan[panIdx_r][++mIdx_c] == cStone)		// 가로줄의 오른쪽 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}

	//printf("가로줄 검사 : %d\n", winCnt);

	return winCnt;
}

// 세로줄 검사 함수
int vertical_check(char cStone, int panIdx_r, int panIdx_c, char omok_pan[][ARR_SIZE_C])
{

	int winCnt=0, pIdx_r, mIdx_r;

	pIdx_r = mIdx_r = panIdx_r;			// 배열의 위치값을 저장

	while(omok_pan[mIdx_r--][panIdx_c] == cStone)			// 세로줄의 윗쪽 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}
	while(omok_pan[++pIdx_r][panIdx_c] == cStone)			// 세로줄의 아래쪽 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}

	//printf("세로줄 검사 : %d\n", winCnt);

	return winCnt;
}

// 대각선1 검사 함수
int diagonal1_check(char cStone, int panIdx_r, int panIdx_c, char omok_pan[][ARR_SIZE_C])
{

	int winCnt=0, pIdx_r, mIdx_r, pIdx_c, mIdx_c;

	pIdx_r = mIdx_r = panIdx_r;			// 배열의 위치값을 저장
	pIdx_c = mIdx_c = panIdx_c;			// 배열의 위치값을 저장

	while(omok_pan[mIdx_r--][mIdx_c--] == cStone)			// 대각선의 왼쪽 위 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}
	while(omok_pan[++pIdx_r][++pIdx_c] == cStone)			// 대각선의 오른쪽 아래 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}

	//printf("대각선1 검사 : %d\n", winCnt);

	return winCnt;
}

// 대각선2 검사 함수
int diagonal2_check(char cStone, int panIdx_r, int panIdx_c, char omok_pan[][ARR_SIZE_C])
{

	int winCnt=0, pIdx_r, mIdx_r, pIdx_c, mIdx_c;

	pIdx_r = mIdx_r = panIdx_r;			// 배열의 위치값을 저장
	pIdx_c = mIdx_c = panIdx_c;			// 배열의 위치값을 저장

	while(omok_pan[mIdx_r--][pIdx_c++] == cStone)			// 대각선의 오른쪽 위 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}
	while(omok_pan[++pIdx_r][--mIdx_c] == cStone)			// 대각선의 왼쪽 아래 검사
	{
		winCnt++;		// 연속된 돌의 카운터 증가
	}

	//printf("대각선2 검사 : %d\n", winCnt);

	return winCnt;
}

// 게임의 계속 여부 선택 함수
int retry_game(void)
{
	char cRetry;			// 계속 여부 입력 변수
	int reNum=0;		// 계속 여부 값 변수

	fflush(stdin);			// 버퍼의 내용 비움

	puts("오목 게임을 계속 하시겠습니까?");

	do
	{
		printf("계속 하시려면[Y(y)], 종료하려면[N(n)]을 눌러주십시오 : ");
		scanf("%c", &cRetry);
	}
	while((cRetry != 'Y') && (cRetry != 'y') && (cRetry != 'N') && (cRetry != 'n'));		// 정확한 값을 받을 때까지 반복

	puts("");

	if((cRetry == 'Y') || (cRetry == 'y'))		// 계속할 경우
	{
		reNum = 1;
	}
	else if((cRetry == 'N') || (cRetry == 'n'))		// 끝낼 경우
	{
		puts("오목 게임을 종료합니다.");

		reNum = 0;
	}

	return reNum;
}
