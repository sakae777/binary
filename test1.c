#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LEN 255
#define MAX_FRIENDS 255
#define FILENAME "player_data.bin"

typedef struct {
    char id[MAX_ID_LEN];
    unsigned char age;
    unsigned short hp;
    double x, y;
    char friends[MAX_FRIENDS][MAX_ID_LEN];
    int num_friends;
} Player;

void write_player() {
    FILE* fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("오류: 파일을 열 수 없습니다.\n");
        return;
    }

    Player p;
    printf("ID를 입력하세요: ");
    fgets(p.id, MAX_ID_LEN, stdin);
    p.id[strcspn(p.id, "\n")] = '\0'; // 개행 문자 제거

    printf("나이를 입력하세요: ");
    scanf("%hhu", &p.age);

    printf("HP를 입력하세요: ");
    scanf("%hu", &p.hp);

    printf("X 좌표를 입력하세요: ");
    scanf("%lf", &p.x);

    printf("Y 좌표를 입력하세요: ");
    scanf("%lf", &p.y);

    getchar(); // 버퍼 비우기

    p.num_friends = 0;
    char add_friends = 'Y';
    while (add_friends == 'Y' || add_friends == 'y'){// 동맹 추가 코드
        printf("동맹 ID를 입력하세요: ");
        fgets(p.friends[p.num_friends], MAX_ID_LEN, stdin);
        p.friends[p.num_friends][strcspn(p.friends[p.num_friends], "\n")] = '\0'; // 개행 문자 제거

        p.num_friends++;
        printf("동맹을 계속 추가하시겠습니까? (Y/N): ");
        scanf("%c", &add_friends);
        getchar(); // 버퍼 비우기
    }

    fwrite(&p, sizeof(Player), 1, fp);
    fclose(fp);

    printf("완료되었습니다.\n\n");
}

void read_all_players() {
    FILE* fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("오류: 파일을 열 수 없습니다.\n");
        return;
    }

    Player p;
    int count = 0;// 플래이어 수

    while (fread(&p, sizeof(Player), 1, fp) == 1) {// 파일에서 플래이어 읽어오기 
        printf("[플레이어%d]\n", ++count);
        printf("ID: %s / 나이: %d / HP: %d / 좌표: %.3lf, %.3lf\n", p.id, p.age, p.hp, p.x, p.y);
        printf("등록된 동맹 수: %d명\n", p.num_friends);

        for (int i = 0; i < p.num_friends; i++) {
            printf("%s", p.friends[i]);
            if (i != p.num_friends - 1) {// 마지막 동맹원이 아닐경우에는 / 붙이기 
                printf(" / ");
            }
        }
        printf("\n\n");
    }
    fclose(fp);
}

void delete_player() {
    char id[MAX_ID_LEN];
    printf("삭제할 ID를 입력하세요: ");
    fgets(id, MAX_ID_LEN, stdin);// 삭제할 아이디 입력받기
    id[strcspn(id, "\n")] = '\0'; // 개행 문자 제거

    FILE* fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("오류: 파일을 열 수 없습니다.\n");
        return;
    }

    FILE* temp = fopen("temp.bin", "wb");// 임시 파일 생성 
    if (temp == NULL) {
        printf("오류: 임시 파일을 생성할 수 없습니다.\n");
        fclose(fp);
        return;
    }

    Player p;
    int found = 0;
    while (fread(&p, sizeof(Player), 1, fp) == 1) {
        if (strcmp(p.id, id) == 0) {
            found = 1;
            continue;
        }
        fwrite(&p, sizeof(Player), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("해당 데이터는 존재하지 않습니다.\n");
        remove("temp.bin");
        return;
    }

    remove(FILENAME);
    rename("temp.bin", FILENAME);
    printf("%s 삭제 완료.\n", id);
}

int main() {
    while (1) {
        printf("1. 신규 플레이어 정보 기록\n");
        printf("2. 기록된 모든 플레이어 정보 확인\n");
        printf("3. 플레이어 삭제\n");
        printf("4. 종료\n");
        printf("입력하세요> ");

        int choice;
        scanf("%d", &choice);
        getchar(); // 버퍼 비우기

        switch (choice) {
            case 1:
                write_player();
                printf("완료되었습니다.\n\n");
                break;

            case 2:
                read_all_players();
                printf("완료되었습니다.\n\n");
                break;

            case 3:
                delete_player();
                printf("\n");
                break;

            case 4:
                printf("종료되었습니다.\n");
                return 0;

            default:
                printf("잘못된 입력입니다. 다시 입력해주세요.\n\n");
                break;
        }
    }
}
