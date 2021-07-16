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
    int status; // this parameter shows the tile is on the front side or not  ... 1 front 0 back
    int is_jack; //this parameter shows the is jack or not ... jack1 innocent0
    int number; //just a number between 1 & 9 , but unique for each tile
};
struct action_tokens{
    int rotation_joker;// 0 : rotation , 1 : joker
    int rotation_exchange; // 0 : rotation , 1 : exchange
    int toby_watson; // 0 : toby , 1 : watson
    int sherlock_alibi; // 0 : sherlock , 1 : alibi
};
struct t
{
    struct t *next;
};
struct t *h = NULL;
int turn; // 0detective 1mr.jack
int rotation_joker_status; //0 for didnt printed
int rotation_exchange_status;
int toby_watson_status;
int sherlock_alibi_status;
struct action_tokens tokens[17]; //two side for each round(2*8) + 1
int glasshour = 0;
int start_key;
int game_round = 0;
int Toby; //8*2 rounds + 1  for first situation + 1
int Sherlock;
int Watson;
char MrJacks_real_name_st[3];
int tiles_array_check_repitition[10];
int round_part;
char game_name[30];
int first_player_role , second_player_role; //1 for mr jack and 0 fo detective
void start_game_menu();
void new_game_menu();
void random_tiles_at_first_time();
void printmap(struct tile *head);
void add_tile(struct tile** head_ref, char tile_info[3] ,int ro ,int co , int added_tile_count);
void detectives_places();
void token1();
void token2();
void token_print_notes();
void choose_token(char choice[10] , struct tile *head , int round_part);
void rotation(struct tile* head);
void toby();
int toby1(int Toby);
int toby2(int Toby);
void watson();
int watson1(int Watson);
int watson2(int Watson);
void sherlock();
int sherlock1(int sherlock);
int sherlock2(int sherlock);
void joker();
struct tile* exchange(struct tile *head);
void alibi(struct tile* head , int turn);
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
            add_tile(&head, tile_info, i, j , added_tiles_count);
//            printf("%s , i = %d, j = %d\n" , tile_info , i , j);
            added_tiles_count++;
        }
    }
    struct tile* curr;
    for (curr = head ; curr != NULL && strcmpi(curr->tile_name , MrJacks_real_name_st) !=0 ; curr = curr->next);
    curr->is_jack = 1;
//    for (curr = head ; curr != NULL && curr->is_jack == 0 ; curr = curr->next);
//    printf("mr jaaaaack = %s" , MrJacks_real_name_st);
    printf("- - - - - - - - - - - - - - -\n");
    if (game_round == 0) {
        Toby = 11;
        Watson = 5;
        Sherlock = 4;
    }
    printmap(head);
//    detectives_places();
    printf("if you are ready , pls enter a character to start the game!\n");
    char start_char;
    getchar();
    scanf("%c" , &start_char);
    char choice[10];
//    int round_part; //an integer between 0 & 1
    for (game_round = 1; game_round < 9; game_round++) {
        rotation_joker_status = 0;
        rotation_exchange_status = 0;
        sherlock_alibi_status = 0;
        toby_watson_status = 0;
        printf("- - - - - - - - - - - - - - -\n-%dth ROUND-\n- - - - - - - - - - - - - - -\n", game_round);
        if (game_round % 2 == 0)
            token2();
        if (game_round % 2 == 1)
            token1();
        if (game_round % 2 == 0) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        if (game_round % 2 == 1) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        scanf("%s", choice);
        choose_token(choice , head , round_part);
        printmap(head);
//        detectives_places();
        //////////////
        if (game_round % 2 == 0)
            token2();
        if (game_round % 2 == 1)
            token1();
        if (game_round % 2 == 0) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        if (game_round % 2 == 1) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        scanf("%s", choice);
        choose_token(choice , head , round_part);
//        detectives_places();
        printmap(head);
        /////////////
        if (game_round % 2 == 0)
            token2();
        if (game_round % 2 == 1)
            token1();
        if (game_round % 2 == 0) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        if (game_round % 2 == 1)
        {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        scanf("%s", choice);
        choose_token(choice , head , round_part);
//        detectives_places();
        printmap(head);
        //////////
        if (game_round % 2 == 0)
            token2();
        if (game_round % 2 == 1)
            token1();
        if (game_round % 2 == 0) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        if (game_round % 2 == 1) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        scanf("%s", choice);
        choose_token(choice , head , round_part);
//        detectives_places();
        printmap(head);
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
//    for (int i = 0 ; i < 9 ; i++)
//        printf("%d\t" , tiles_array_check_repitition[i]);
//    printf("\n");
    for (int i = 0 ; i < 9 ; i++)
    {
        int t = rand()% (8 + 1);
        int temp = tiles_array_check_repitition[i];
        tiles_array_check_repitition[i] = tiles_array_check_repitition[t];
        tiles_array_check_repitition[t] = temp;
    }
//    for (int i = 0 ; i < 9 ; i++)
//        printf("%d\t" , tiles_array_check_repitition[i]);
//    printf("\n");
}
void add_tile(struct tile** head_ref, char tile_info[3] ,int ro ,int co ,int added_tile_count)
{
    struct tile* nn = (struct tile*) malloc(sizeof(struct tile));
    struct tile *last = *head_ref;
    int c = rand()%(left - up + 1) + up; //be soorate random samte bonbast moshakhas mishe
    strcpy(nn->tile_name , tile_info);
    nn->end = c;
    nn->row = ro;
    nn->column = co;
    nn->status = 1;
    nn->number = added_tile_count;
    nn->next = NULL;
    if (*head_ref == NULL)
    {
        *head_ref = nn;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = nn;

    }
void printmap(struct tile *head) {
    printf("- - - - - - - - - - - - - - -\n");
    struct tile *current;
    current = head;
    int margin_count = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (j == 0 || j == 4 || i == 0 || i == 4) {
//                printf("*");
                if ((j == 0 && i == 0) || (i == 0 & j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4))
                    printf("  \t");
                else {
                    int f = 0;
                    if (Toby == margin_count) {
                        printf("T");
                        f = 1;
                    }
                    if (Sherlock == margin_count) {
                        printf("S");
                        f = 1;
                    }
                    if (Watson == margin_count) {
                        printf("W");
                        f = 1;
                    }
                    if (f == 1)
                        printf("\t");
                    else
                        printf("  \t");
                    margin_count++;
                }

            } else {
                int count = 0;
                for (current = head; count < 9; current = current->next) {
                    if ((current->row == i) && (current->column == j)) {
                        if (current->status == 1) {
                            if (current->end == up)
                                printf("^,%s\t", current->tile_name);
                            if (current->end == down)
                                printf("v,%s\t", current->tile_name);
                            if (current->end == right)
                                printf(">,%s\t", current->tile_name);
                            if (current->end == left)
                                printf("<,%s\t", current->tile_name);
                        }
                        if (current->status == 0) {
                            if (current->end == up)
                                printf("^,--\t");
                            if (current->end == down)
                                printf("v,--\t");
                            if (current->end == right)
                                printf(">,--\t");
                            if (current->end == left)
                                printf("<,--\t");
//                    current = current->next;
                        }
                    }
                    count++;
                }

            }
        }
        printf("\n");
//    int count = 0;
//    for (current = head ; count < 9 ; current = current -> next)
//    {
//        printf("**name : %s , end : %d , col : %d , row : %d\n", current->tile_name, current->end, current->row,current->column);
//        count++;
//    }
    }
    printf("- - - - - - - - - - - - - - -\n");
}
//void detectives_places()
//{
//    printf("Toby : %d\nWatson : %d\nSherlock : %d\n", Toby, Watson, Sherlock);
//}
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
    printf("- - - - - - - - - - - - - - -\n!!TOKENS!!\n- - - - - - - - - - - - - - -\n");
    if(tokens[r].rotation_joker == 0 && rotation_joker_status == 0)
        printf("1 : ROTATION1\nnote : you can rotate one tile to each side that you want\n");
    if(tokens[r].rotation_joker == 1 && rotation_joker_status == 0)
        printf("1 : JOKER\nnote : you can move one of the detectives for one block\n");

    if(tokens[r].rotation_exchange == 0 && rotation_exchange_status == 0)
        printf("2 : ROTATION2\nnote : you can rotate one tile to each side that you want\n");
    if(tokens[r].rotation_exchange == 1 && rotation_exchange_status == 0)
        printf("2 : EXCHANGE\nnote : you can change two tiles\n");

    if(tokens[r].toby_watson == 0 && toby_watson_status == 0)
        printf("3 : TOBY\nnote : you can move Toby for one or two blocks\n");
    if(tokens[r].toby_watson == 1 && toby_watson_status == 0)
        printf("3 : WATSON\nnote : you can move Watson for one or two blocks\n");

    if(tokens[r].sherlock_alibi == 0 && sherlock_alibi_status == 0)
        printf("4 : SHERLOCK\nnote : you can move Sherlock for one or two blocks\n");
    if(tokens[r].sherlock_alibi == 1 && sherlock_alibi_status == 0)
        printf("4 : ALIBI\nnote : you can choose one of suspects cards\n");
}
void choose_token(char choice[10] , struct tile *head ,int round_part)
{
    if(strcmpi(choice , "rotation1") == 0)
    {
        rotation(head);
        rotation_exchange_status = 1;
    }
    if(strcmpi(choice , "rotation2") == 0)
    {
        rotation(head);
        rotation_joker_status = 1;
    }
    if(strcmpi(choice , "joker") == 0)
    {
        joker();
        rotation_joker_status = 1;
    }
    if(strcmpi(choice , "toby") == 0) {
        toby();
        toby_watson_status = 1;
    }
    if(strcmpi(choice , "watson") == 0) {
        watson();
        toby_watson_status = 1;
    }
    if(strcmpi(choice , "sherlock") == 0) {
        sherlock();
        sherlock_alibi_status=1;
    }
    if(strcmpi(choice , "exchange") == 0)
    {
        head = exchange(head);
        rotation_exchange_status = 1;

    }
    if(strcmpi(choice , "alibi") == 0)
    {
        alibi(head , turn);
        sherlock_alibi_status = 1;
    }
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
        Toby = toby1(Toby);
    }
    if (toby_blocks == 2)
    {
        Toby=toby2(Toby);
    }
}
int toby1(int Toby)
{
    switch (Toby)
    {
        case 1:   return 2;
        case 2:   return 3;
        case 3:   return 5;
        case 4:   return 1;
        case 5:   return 7;
        case 6:   return 4;
        case 7:   return 9;
        case 8:   return 6;
        case 9:   return 12;
        case 10:   return 8;
        case 11:   return 10;
        case 12:   return 11;
    }
}
int toby2(int Toby)
{
    switch (Toby)
    {
        case 1:   return 3;
        case 2:   return 5;
        case 3:   return 7;
        case 4:   return 2;
        case 5:   return 9;
        case 6:   return 1;
        case 7:   return 12;
        case 8:   return 4;
        case 9:   return 11;
        case 10:   return 6;
        case 11:   return 8;
        case 12:   return 10;
    }
}
void watson()
{
    printf("how many blocks do you want to move WATSON? (1 or 2) ");
    int watson_blocks;
    scanf("%d" , &watson_blocks);
    if (watson_blocks == 1)
    {
        Watson= watson1(Watson);
    }
    if (watson_blocks == 2)
    {
        Watson=watson2(Watson);
    }
}
int watson1(int Watson)
{
    switch (Watson)
    {
        case 1:   return 2;
        case 2:   return 3;
        case 3:   return 5;
        case 4:   return 1;
        case 5:   return 7;
        case 6:   return 4;
        case 7:   return 9;
        case 8:   return 6;
        case 9:   return 12;
        case 10:   return 8;
        case 11:   return 10;
        case 12:   return 11;
    }
}
int watson2(int Watson)
{
    switch (Watson)
    {
        case 1:   return 3;
        case 2:   return 5;
        case 3:   return 7;
        case 4:   return 2;
        case 5:   return 9;
        case 6:   return 1;
        case 7:   return 12;
        case 8:   return 4;
        case 9:   return 11;
        case 10:   return 6;
        case 11:   return 8;
        case 12:   return 10;
    }
}
void sherlock()
{
    printf("how many blocks do you want to move SHERLOCK? (1 or 2) ");
    int sherlock_blocks;
    scanf("%d" , &sherlock_blocks);
    if (sherlock_blocks == 1)
    {
        Sherlock=sherlock1(Sherlock);
    }
    if (sherlock_blocks == 2)
    {
        Sherlock=sherlock2(Sherlock);
    }
}
int sherlock1(int Sherlock)
{
    switch (Sherlock)
    {
        case 1:   return 2;
        case 2:   return 3;
        case 3:   return 5;
        case 4:   return 1;
        case 5:   return 7;
        case 6:   return 4;
        case 7:   return 9;
        case 8:   return 6;
        case 9:   return 12;
        case 10:   return 8;
        case 11:   return 10;
        case 12:   return 11;
    }
}
int sherlock2(int Sherlock)
{
    switch (Sherlock)
    {
        case 1:   return 3;
        case 2:   return 5;
        case 3:   return 7;
        case 4:   return 2;
        case 5:   return 9;
        case 6:   return 1;
        case 7:   return 12;
        case 8:   return 4;
        case 9:   return 11;
        case 10:   return 6;
        case 11:   return 8;
        case 12:   return 10;
    }
}
void joker()
{
    char character[10];
    printf("choose the detective that you want to move it (sherlock , toby , watson) : \n");
    scanf("%s" , character);
    if (strcmpi(character , "sherlock") == 0)
        Sherlock = sherlock1(Sherlock);
    if (strcmpi(character , "watson") == 0)
        Watson = watson1(Watson);
    if (strcmpi(character , "toby") == 0)
        Toby = toby1(Toby);
}
struct tile* exchange(struct tile* head)
{
    struct tile *new;
    char sus1[3] , sus2[3];
    printf("pls enter the name of suspects that you want to change them : \n");
    scanf("%s" , sus1);
    scanf("%s" , sus2);
    struct tile* current1;
    struct tile* current2;
    for (current1 = head; current1->next != NULL && strcmpi(current1->tile_name, sus1) != 0; current1 = current1->next);
    for (current2 = head; current2->next != NULL && strcmpi(current2->tile_name, sus2) != 0; current2 = current2->next);
    char temp[3];
    int c1 , c2 , r1 , r2 , e1 , e2;
    c1 = current1->column;
    c2 = current2 ->column;
    r1 = current1 -> row;
    r2 = current2 ->row;
    e1 = current1->end;
    e2 = current2->end;
//    printf("r1 = %d . c1 = %d , sus1 = %s , r2 = %d . c2 = %d , sus2 = %s \n" , r1 , c1 , sus1 , r2 , c2 , sus2);
    int flag = 0;
    for (new = head; new->next != NULL; new = new->next) {
        if( strcmpi(new->tile_name, sus1) == 0 && flag == 0) {
            new->row = r2;
            new->column = c2;
            new ->end = e1;
//            strcpy(new->tile_name, sus2);
            flag = 1;
            printf("name = %s , row = %d , col = %d , end = %d\n" , new->tile_name , new ->row , new->column , new->end);
        }
    }
    for (new = head; new->next != NULL; new = new->next)
    {
        if(strcmpi(new->tile_name, sus2) == 0 && flag == 1) {
            new->row = r1;
            new->column = c1;
            new ->end = e2;
//            strcpy(new->tile_name, sus1);
            printf("*name = %s , row = %d , col = %d , end = %d\n" , new->tile_name , new ->row , new->column , new->end);
        }
    }
    return new;
}
void alibi(struct tile* head , int turn)
{
    srand(time(0));
    int r;
    struct tile* current;
    if (turn == 0) //detective's turn
    {
        do {
            r = rand()%(9 - 1 + 1) + 1;
            for (current = head ; current!= NULL && current->number != r ; current = current->next );
        } while (current->is_jack == 1 || current->status == 0);
    }
    printf("%s is reversed\n" , current->tile_name);
    current->status = 0;
}