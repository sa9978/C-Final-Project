#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
//enum tile_name{InspLestrade = 1 , JeremyBert , JohnPizer , JohnSmith , JosephLane , Madame , MissStealthy , SgtGoodley , WiliamGull}; // from 1 to 9
enum tile_street{up = 1 , right , down , left};
FILE *write;
struct tile{
//    struct tile *right;
//    struct tile *left;
//    struct tile *up;
//    struct tile *down;
    struct tile *next;
    int row;
    int column;
    char tile_name [3];
    enum tile_street end;
};
struct action_tokens{
    int rotation_joker; // 0 : rotation , 1 : joker
    int rotation_exchange; // 0 : rotation , 1 : exchange
    int toby_watson; // 0 : toby , 1 : watson
    int sherlock_alibi; // 0 : sherlock , 1 : alibi
};
struct action_tokens tokens[17]; //two side for each round(2*8) + 1
int glasshour = 0;
int start_key;
int game_round = 0;
int Toby; //8*2 rounds + 1  for first situation + 1
int Sherlock;
int Watson;
char MrJacks_real_name_st[3];
int tiles_array_check_repitition[10];
char game_name[30];
int first_player_role , second_player_role; //1 for mr jack and 0 fo detective
void start_game_menu();
void new_game_menu();
void random_tiles_at_first_time();
void printmap(struct tile *head);
void add_tile(struct tile** head_ref, char tile_info[3] ,int row ,int column);
void detectives_places();
void token1();
void token2();
void token_print_notes();
void choose_token(char choice[10] , struct tile *head , int round_part);
void rotation(struct tile* head);
void toby();
void toby1(int Toby);
void toby2(int Toby);
void watson();
void watson1(int Watson);
void watson2(int Watson);
void sherlock();
void sherlock1(int sherlock);
void sherlock2(int sherlock);
int main() {
    start_game_menu();
    return 0;
}
void start_game_menu()
{
    printf("1- New Game\n");
    printf("2- Load Game\n");
    printf("3- Exit\n");
    scanf("%d" , &start_key);
    if (start_key == 1)
        new_game_menu();
}
void new_game_menu() {
//    do {
//        printf("- Please enter the name of this Game:\n");
//        getchar();
//        gets(game_name);
////        write = fopen(game_name, "w");
//        if (write == NULL) {
//            printf("Error / pls try again\n");
//        }
//    } while (write == NULL);
    int flag_name = 0;
    do {
        printf("- Choose which role you want to play? (Mr.Jack/Detective)\n");
        char role[30];
        scanf("%s", role);
        if (strcmpi(role, "Mr.Jack") == 0) {
            first_player_role = 1;
            second_player_role = 0;
            printf("the first player is : Mr.Jack\n& the second payer is :Detective\n");
            flag_name = 1;
        }
        if (strcmpi(role, "Detective") == 0) {
            first_player_role = 0;
            second_player_role = 1;
            printf("the first player is : Detective\n& the second payer is :Mr.Jack\n");
            flag_name = 1;
        }
        if (flag_name == 0)
            printf("pls enter a valid name!\n");
    } while (flag_name == 0);
    int close_eye;
    printf("- Mr.Jack's identity will be defined. if the detective close his/her eyes , mr jack enter '1'\n");
    do {
        scanf("%d", &close_eye);
        if (close_eye != 1)
            printf("DON'T CHEAT!!! CLOSE YOUR EYES!\nmr jack! if he/she closed his/her eyes enter '1'\n");
    } while (close_eye != 1);
    srand(time(0));
    int MrJacks_real_name = rand() % (9 - 1 + 1) + 1;
    if (MrJacks_real_name == 1) {
//        MrJacks_real_name = InspLestrade;
        strcpy(MrJacks_real_name_st, "IL");
        printf("MrJack's real name is InspLestrade, we call him in the game 'IL' \n");
    }
    if (MrJacks_real_name == 2) {
//        MrJacks_real_name = JeremyBert;
        strcpy(MrJacks_real_name_st, "JB");
        printf("MrJack's real name is JeremyBert, we call him in the game 'JB' \n");
    }
    if (MrJacks_real_name == 3) {
//        MrJacks_real_name = JohnPizer;
        strcpy(MrJacks_real_name_st, "JP");
        printf("MrJack's real name is JohnPizer, we call him in the game 'JP' \n");
    }
    if (MrJacks_real_name == 4) {
//        MrJacks_real_name = JohnSmith;
        strcpy(MrJacks_real_name_st, "JS");
        printf("MrJack's real name is JohnSmith, we call him in the game 'JS' \n");
    }
    if (MrJacks_real_name == 5) {
//        MrJacks_real_name = JosephLane;
        strcpy(MrJacks_real_name_st, "JL");
        printf("MrJack's real name is JosephLane, we call him in the game 'JL' \n");
    }
    if (MrJacks_real_name == 6) {
        strcpy(MrJacks_real_name_st, "MA");
        printf("MrJack's real name is Madame, we call her in the game 'MA' \n");
    }
    if (MrJacks_real_name == 7) {
//        MrJacks_real_name = MissStealthy;
        strcpy(MrJacks_real_name_st, "MS");
        printf("MrJack's real name is MissStealthy, we call her in the game 'MS' \n");
    }
    if (MrJacks_real_name == 8) {
//        MrJacks_real_name = SgtGoodley;
        strcpy(MrJacks_real_name_st, "SG");
        printf("MrJack's real name is SgtGoodley, we call him in the game 'SG' \n");
    }
    if (MrJacks_real_name == 9) {
//        MrJacks_real_name = WiliamGull;
        strcpy(MrJacks_real_name_st, "WG");
        printf("MrJack's real name is WiliamGull, we call him in the game 'WG' \n");
    }
    random_tiles_at_first_time();
    int added_tiles_count = 0;
    struct tile *head = NULL;
    char tile_info[3];
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            int info = tiles_array_check_repitition[added_tiles_count];
            if (info == 1)
                strcpy(tile_info, "IL");
            if (info == 2)
                strcpy(tile_info, "JB");
            if (info == 3)
                strcpy(tile_info, "JP");
            if (info == 4)
                strcpy(tile_info, "JS");
            if (info == 5)
                strcpy(tile_info, "JL");
            if (info == 6)
                strcpy(tile_info, "MA");
            if (info == 7)
                strcpy(tile_info, "MS");
            if (info == 8)
                strcpy(tile_info, "SG");
            if (info == 9)
                strcpy(tile_info, "WG");
            add_tile(&head, tile_info, i, j);
//         printf("tile %dth is added , name: %s , row : %d , col : %d\n" , added_tiles_count , tile_info , i , j);
            added_tiles_count++;
        }
    }
    printf("- - - - - - - - - - - - - - -\n");
    printmap(head);
    if (game_round == 0) {
        Toby = 11;
        Watson = 5;
        Sherlock = 4;
    }
    printf("Toby : %d\nWatson : %d\nSherlock : %d\n", Toby, Watson, Sherlock);
    char choice[10];
    int round_part; //an integer between 0 & 1
    for (game_round = 1; game_round < 9; game_round++) {
        round_part = 0;
        printf("-%dth ROUND-\n", game_round);
        printf("- - - - - - - - - - - - - - -\n");
        token1();
        if (game_round % 2 == 0)
            printf("so , Mr.jack! pls choose one token and enter its name as a string : \n");
        if (game_round % 2 == 1)
            printf("so , Detective! pls choose one token and enter its name as a string : \n");
        scanf("%s", choice);
        choose_token(choice , head , round_part);
        printf("- - - - - - - - - - - - - - -\n");
        //////////////
        if (game_round % 2 == 0)
            printf("so , Detective! pls choose one token and enter its name as a string : \n");
        if (game_round % 2 == 1)
            printf("so , Mr.jack! pls choose one token and enter its name as a string : \n");
        scanf("%s", choice);
        choose_token(choice , head , round_part);
        /////////////
        round_part = 1;
        if (game_round % 2 == 0)
            printf("so , Detective! pls choose one token and enter its name as a string : \n");
        if (game_round % 2 == 1)
            printf("so , Mr.jack! pls choose one token and enter its name as a string : \n");
        scanf("%s", choice);
        choose_token(choice , head , round_part);
        //////////
        if (game_round % 2 == 0)
            printf("so , Mr.jack! pls choose one token and enter its name as a string : \n");
        if (game_round % 2 == 1)
            printf("so , Detective! pls choose one token and enter its name as a string : \n");
        scanf("%s", choice);
        choose_token(choice , head , round_part);
        printf("- - - - - - - - - - - - - - -\n");

//    fclose(write);
    }
}
void random_tiles_at_first_time()
{
    srand(time(0));
    for (int i = 0 ; i < 9 ; i++) {
        tiles_array_check_repitition[i] = i + 1;
    }
    for (int i = 0 ; i < 9 ; i++)
    {
        int t = rand()% (9 + 1);
        int temp = tiles_array_check_repitition[i];
        tiles_array_check_repitition[i] = tiles_array_check_repitition[t];
        tiles_array_check_repitition[t] = temp;
    }
}
void add_tile(struct tile** head_ref, char tile_info[3] ,int row ,int column)
{
    struct tile* nn = (struct tile*) malloc(sizeof(struct tile));
    struct tile *last = *head_ref;
    int c = rand()%(left - up + 1) + up; //be soorate random samte bonbast moshakhas mishe
    strcpy(nn->tile_name , tile_info);
    nn->end = c;
    nn->row = row;
    nn->column = column;
    nn->next = NULL;
    if (*head_ref == NULL)
    {
        *head_ref = nn;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = nn;
//    printf("name : %s , end : %d , col : %d , row : %d\n" , nn->tile_name , nn->end , nn->row, nn->column );
    }
void printmap(struct tile *head)
{
    struct tile *current;
    current = head;
    int margin_count = 1;
    for (int i = 0 ; i < 5 ; i++) {
        for (int j = 0; j < 5; j++) {
            if (j == 0 || j == 4 || i == 0 || i == 4) {
                printf("*");
                if ((j == 0 && i == 0) || (i == 0 & j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4))
                    printf("*\t");
                else {
                    printf("%d\t", margin_count);
                    margin_count++;
                }

            } else
            {
                if(current->end == up)
                    printf("U,%s\t" , current->tile_name);
                if(current->end == down)
                    printf("D,%s\t" , current->tile_name);
                if(current->end == right)
                    printf("R,%s\t" , current->tile_name);
                if(current->end == left)
                    printf("L,%s\t" , current->tile_name);
                current = current->next;
            }

        }
        printf("\n");
    }
}
void detectives_places()
{

}
void token1()
{
    tokens[2 * game_round - 1].rotation_joker = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens[2 * game_round - 1].rotation_exchange = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens[2 * game_round - 1].toby_watson = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens[2 * game_round - 1].sherlock_alibi = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    token_print_notes(2 * game_round - 1);

}
void token2()
{
    if(tokens[2 * game_round - 1].rotation_joker == 0)
        tokens[2 * game_round].rotation_joker = 1;
    if(tokens[2 * game_round - 1].rotation_joker == 1)
        tokens[2 * game_round].rotation_joker = 0;

    if(tokens[2 * game_round - 1].rotation_exchange == 0)
        tokens[2 * game_round].rotation_exchange = 1;
    if(tokens[2 * game_round - 1].rotation_exchange == 1)
        tokens[2 * game_round].rotation_exchange = 0;

    if(tokens[2 * game_round - 1].toby_watson == 0)
        tokens[2 * game_round].toby_watson = 1;
    if(tokens[2 * game_round - 1].toby_watson == 1)
        tokens[2 * game_round].toby_watson = 0;

    if(tokens[2 * game_round - 1].sherlock_alibi == 0)
        tokens[2 * game_round].sherlock_alibi = 1;
    if(tokens[2 * game_round - 1].sherlock_alibi == 1)
        tokens[2 * game_round].sherlock_alibi = 0;
    token_print_notes(2 * game_round);
}
void token_print_notes(int r)
{
    printf("!!TOKENS!!\n- - - - - - - - - - - - - - -\n");
    if(tokens[r].rotation_joker == 0)
        printf("1 : ROTATION\nnote : you can rotate one tile to each side that you want\n");
    if(tokens[r].rotation_joker == 1)
        printf("1 : JOKER\nnote : you can move one of the detectives for one block\n");

    if(tokens[r].rotation_exchange == 0)
        printf("2 : ROTATION\nnote : you can rotate one tile to each side that you want\n");
    if(tokens[r].rotation_exchange == 1)
        printf("2 : EXCHANGE\nnote : yo can change two tiles\n");

    if(tokens[r].toby_watson == 0)
        printf("3 : TOBY\nnote : you can move Toby for one or two blocks\n");
    if(tokens[r].toby_watson == 1)
        printf("3 : WATSON\nnote : you can move Watson for one or two blocks\n");

    if(tokens[r].sherlock_alibi == 0)
        printf("4 : SHERLOCK\nnote : you can move Sherlock for one or two blocks\n");
    if(tokens[r].sherlock_alibi == 1)
        printf("4 : ALIBI\nnote : you can choose one of suspects cards\n");
}
void choose_token(char choice[10] , struct tile *head ,int round_part)
{
    if(strcmpi(choice , "rotation") == 0)
        rotation(head);
//    if(strcmpi(choice , "joker") == 0)
//        joker();
    if(strcmpi(choice , "toby") == 0)
        toby();
    if(strcmpi(choice , "watson") == 0)
        watson();
    if(strcmpi(choice , "sherlock") == 0)
        sherlock();
//    if(strcmpi(choice , "exchange") == 0)
//        exchange();
//    if(strcmpi(choice , "alibi") == 0)
//        alibi();
}
void rotation(struct tile* head)
{
    printf("pls enter name of the character that you want to rotate: \n");
    char rotate_character[3];
    scanf("%s" , rotate_character);
    printf("which direction do yo want to be closed?(up , down , left , right)\n");
    char dir[10];
    scanf("%s" , dir);
    struct tile* current;
    for (current = head; current->next != NULL && strcmpi(current->tile_name, rotate_character) != 0; current = current->next);
    if(strcmpi(dir , "up") == 0)
        current->end = up;
    if(strcmpi(dir , "down") == 0)
        current->end = down;
    if(strcmpi(dir , "right") == 0)
        current->end = right;
    if(strcmpi(dir , "left") == 0)
        current->end = left;
    printmap(head);
}
void toby()
{
    printf("how many blocks do you want to move TOBY? (1 or 2) ");
    int toby_blocks;
        scanf("%d" , &toby_blocks);
    if (toby_blocks == 1)
    {
        toby1(Toby);
    }
    if (toby_blocks == 2)
    {
        toby2(Toby);
    }
}
void toby1(int Toby)
{
    switch (Toby)
    {
        case 1:   Toby = 2;
        case 2:   Toby = 3;
        case 3:   Toby = 5;
        case 4:   Toby = 1;
        case 5:   Toby = 7;
        case 6:   Toby = 4;
        case 7:   Toby = 9;
        case 8:   Toby = 6;
        case 9:   Toby = 12;
        case 10:   Toby = 8;
        case 11:   Toby = 10;
        case 12:   Toby = 11;
    }
}
void toby2(int Toby)
{
    switch (Toby)
    {
        case 1:   Toby = 3;
        case 2:   Toby = 5;
        case 3:   Toby = 7;
        case 4:   Toby = 2;
        case 5:   Toby = 9;
        case 6:   Toby = 1;
        case 7:   Toby = 12;
        case 8:   Toby = 4;
        case 9:   Toby = 11;
        case 10:   Toby = 6;
        case 11:   Toby = 8;
        case 12:   Toby = 10;
    }
}
void watson()
{
    printf("how many blocks do you want to move WATSON? (1 or 2) ");
    int watson_blocks;
    scanf("%d" , &watson_blocks);
    if (watson_blocks == 1)
    {
        watson1(Watson);
    }
    if (watson_blocks == 2)
    {
        watson2(Watson);
    }
}
void watson1(int Watson)
{
    switch (Watson)
    {
        case 1:   Watson = 2;
        case 2:   Watson = 3;
        case 3:   Watson = 5;
        case 4:   Watson = 1;
        case 5:   Watson = 7;
        case 6:   Watson = 4;
        case 7:   Watson = 9;
        case 8:   Watson = 6;
        case 9:   Watson = 12;
        case 10:   Watson = 8;
        case 11:   Watson = 10;
        case 12:   Watson = 11;
    }
}
void watson2(int Watson)
{
    switch (Watson)
    {
        case 1:   Watson = 3;
        case 2:   Watson = 5;
        case 3:   Watson = 7;
        case 4:   Watson = 2;
        case 5:   Watson = 9;
        case 6:   Watson = 1;
        case 7:   Watson = 12;
        case 8:   Watson = 4;
        case 9:   Watson = 11;
        case 10:   Watson = 6;
        case 11:   Watson = 8;
        case 12:   Watson = 10;
    }
}
void sherlock()
{
    printf("how many blocks do you want to move SHERLOCK? (1 or 2) ");
    int sherlock_blocks;
    scanf("%d" , &sherlock_blocks);
    if (sherlock_blocks == 1)
    {
        sherlock1(Sherlock);
    }
    if (sherlock_blocks == 2)
    {
        sherlock2(Sherlock);
    }
}
void sherlock1(int Sherlock)
{
    switch (Sherlock)
    {
        case 1:   Sherlock = 2;
        case 2:   Sherlock = 3;
        case 3:   Sherlock = 5;
        case 4:   Sherlock = 1;
        case 5:   Sherlock = 7;
        case 6:   Sherlock = 4;
        case 7:   Sherlock = 9;
        case 8:   Sherlock = 6;
        case 9:   Sherlock = 12;
        case 10:   Sherlock = 8;
        case 11:   Sherlock = 10;
        case 12:   Sherlock = 11;
    }
}
void sherlock2(int Sherlock)
{
    switch (Sherlock)
    {
        case 1:   Sherlock = 3;
        case 2:   Sherlock = 5;
        case 3:   Sherlock = 7;
        case 4:   Sherlock = 2;
        case 5:   Sherlock = 9;
        case 6:   Sherlock = 1;
        case 7:   Sherlock = 12;
        case 8:   Sherlock = 4;
        case 9:   Sherlock = 11;
        case 10:   Sherlock = 6;
        case 11:   Sherlock = 8;
        case 12:   Sherlock = 10;
    }
}