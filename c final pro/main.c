#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
// ** *1 *2 *3 **
// *4 11 12 13 *5
// *6 21 22 23 *7
// *8 31 32 33 *9
// ** 10 11 12 **
//enum tile_name{InspLestrade = 1 , JeremyBert , JohnPizer , JohnSmith , JosephLane , Madame , MissStealthy , SgtGoodley , WiliamGull}; // from 1 to 9
enum tile_street{up = 1 , right , down , left};
FILE *write;
FILE *read;
struct tile{
    struct tile *next;
    int row;
    int column;
    char tile_name [3];
    enum tile_street end;
    int status; // this parameter shows the tile is on the front side or not  ... 1 : front// 0 : back
    int is_jack; //this parameter shows the is jack or not ... jack1 innocent0
    int number; //just a number between 1 & 9 , but unique for each tile
    int hour; // count hourglasses
    int alibi_status; //shows that the card can be in alibi token1 or not0
    int det_vision; //1 shows that det.s maybe see him
};struct tile *head = NULL;
struct action_tokens{
    int rotation_joker;// 0 : rotation , 1 : joker
    int rotation_exchange; // 0 : rotation , 1 : exchange
    int toby_watson; // 0 : toby , 1 : watson
    int sherlock_alibi; // 0 : sherlock , 1 : alibi
};struct action_tokens tokens;
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
int hourglass = 0;
int start_key;
int game_round = 0;
int Toby; //8*2 rounds + 1  for first situation + 1
int Sherlock;
int Watson;
char MrJacks_real_name_st[3];
int MrJacks_real_name;
int tiles_array_check_repitition[10];
int round_part;
char game_name[30];
int first_player_role , second_player_role; //1 for mr jack and 0 fo detective
void start_game_menu();
void new_game_menu();
void choose_new_game();
void random_tiles_at_first_time();
void printmap(struct tile *head);
void choose_mr_jack();
void add_tile(struct tile** head_ref, char tile_info[3] ,int ro ,int co , int added_tile_count , int h);
void detectives_places();
void token1();
void token2();
void token_print_notes();
void choose_token(struct tile *head);
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
int detectives_vision(struct tile *head , int location);
int toby_vision(struct tile *head);
int watson_vision(struct tile *head);
int sherlock_vision(struct tile *head);
int can_see(struct tile *head);
int rotate_tile_cant_see(struct tile *head , int location);
int vision_can_see(struct tile *head , int location);
int detective_win_check(struct tile *head);
int jack_win_check(struct tile *head);
///////////
//////save and load
/////////
void save_game(struct tile *head);
void file_print_map(struct tile *head);
void file_print_tokens();

void load_game(struct tile *head);
char file_games[10][15] ;
char file_name_load[10];
int name_count =0;
void loadable_games();
void load_selected_game(struct tile *head);
void read_map(struct tile** head_ref);
void continue_game();
///////
int main() {
    start_game_menu();
    return 0;
}
void start_game_menu()
{
    int flag = 0;
    printf("1- New Game\n");
    printf("2- Load Game\n");
    printf("3- Exit\n");
    fflush(stdin);
    do {
        scanf("%d", &start_key);
        if (start_key == 1)
        {
            new_game_menu();
            flag = 1;
        }
        if (start_key == 2)
        {
            load_game(head);
            flag = 1;
        }

        if(flag == 0)
        {
            printf("pls enter a valid number between 1 & 3:\n");
        }
    } while (flag == 0);
}
void new_game_menu() {
        write = fopen("save.txt" , "a+");
        printf("- Please enter the name of this Game:\n");
        fflush(stdin);
        scanf("%s" , game_name);

    int flag_name = 0;
    do {
        printf("- Choose which role you want to play? (Mr.Jack/Detective)\n");
        char role[30];
        fflush(stdin);
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
      fflush(stdin);
        scanf("%d", &close_eye);
        if (close_eye != 1)
            printf("DON'T CHEAT!!! CLOSE YOUR EYES!\nmr jack! if he/she closed his/her eyes enter '1'\n");
    } while (close_eye != 1);

    choose_mr_jack();
    random_tiles_at_first_time();
    int added_tiles_count = 0;
    char tile_info[3];
    int h;
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            int info = tiles_array_check_repitition[added_tiles_count];
            if (info == 1) {
                strcpy(tile_info, "IL");
                h = 0;
            }
            if (info == 2) {
                strcpy(tile_info, "JB");
                h = 1;
            }
            if (info == 3) {
                strcpy(tile_info, "JP");
                h = 1;
            }
            if (info == 4) {
                strcpy(tile_info, "JS");
                h = 1;
            }
            if (info == 5) {
                strcpy(tile_info, "JL");
                h = 1;
            }
            if (info == 6) {
                strcpy(tile_info, "MA");
                h = 2;
            }
            if (info == 7) {
                strcpy(tile_info, "MS");
                h = 1;
            }
            if (info == 8) {
                strcpy(tile_info, "SG");
                h = 0;
            }
            if (info == 9) {
                strcpy(tile_info, "WG");
                h = 1;
            }
            add_tile(&head, tile_info, i, j , added_tiles_count , h);
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
    struct tile *current;
    printmap(head);

    printf("if you are ready , pls enter a character to start the game!\n");
    char start_char;
    getchar();
    fflush(stdin);
    scanf("%c" , &start_char);
    char choice[10];
//    int round_part; //an integer between 0 & 1


    for (game_round = 1; game_round < 9; game_round++) {
        for (current = head ; current!= NULL ; current= current->next)
        {
            current->det_vision = 0;
        }
        rotation_joker_status = 0;
        rotation_exchange_status = 0;
        sherlock_alibi_status = 0;
        toby_watson_status = 0;
        printf("- - - - - - - - - - - - - - -\n-%dth ROUND-\n- - - - - - - - - - - - - - -\n", game_round);
        if (game_round % 2 == 0) {
            token2();
        }
        if (game_round % 2 == 1) {
            token1();
        }
        if (game_round % 2 == 0) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        if (game_round % 2 == 1) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        choose_token( head );
        printmap(head);
        if(detective_win_check(head) == 1)
            main();
        ////////////
        token_print_notes();
        if (game_round % 2 == 0) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        if (game_round % 2 == 1) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        choose_token( head );
        printmap(head);
        if(detective_win_check(head) == 1)
            main();
        /////////////
        token_print_notes();
        if (game_round % 2 == 0) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        if (game_round % 2 == 1)
        {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        choose_token( head );
        printmap(head);
        if(detective_win_check(head) == 1)
            main();
        //////////
        token_print_notes();
        if (game_round % 2 == 0) {
            printf("Mr.jack! pls choose one token and enter its name as a string : \n");
            turn = 1;
        }
        if (game_round % 2 == 1) {
            printf("Detective! pls choose one token and enter its name as a string : \n");
            turn = 0;
        }
        choose_token( head );
        printf("- - - - - - - - - - - - - - -\n");
        printmap(head);
        can_see(head);
        printmap(head);
        if(detective_win_check(head) == 1)
            main();
        if( jack_win_check(head) == 1)
            main();
        if(game_round == 8) {
            printf("* * * * * * * * * * * *\n\n");
            printf("congratulations!!!\nMR.JACK IS WINNER!\n\n");
            printf(" * * * * * * * * * * * *\n\n");
            main();
        }
        printf("enter 'SAVE' to save game:\n");
        printf("if you don't want to save game , enter another character:\n");
        char SaveChar[10];
        fflush(stdin);
        scanf("%s" , SaveChar);
        if (strcmpi(SaveChar , "save") == 0) {
            save_game(head);
            printf("return?\n");
            int ret;
            scanf("%d" , &ret);
            if (ret == 1)
                return;
        }
//    fclose(write);
    }
}
void choose_mr_jack()
{
    srand(time(0));
    MrJacks_real_name = rand() % (9 - 1 + 1) + 1;
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
void add_tile(struct tile** head_ref, char tile_info[3] ,int ro ,int co ,int added_tile_count ,int h)
{
    struct tile* nn = (struct tile*) malloc(sizeof(struct tile));
    struct tile *last = *head_ref;
    int c = rand()%(left - up + 1) + up; //be soorate random samte bonbast moshakhas mishe
    strcpy(nn->tile_name , tile_info);
    if(ro == 1 && co == 3)
        nn->end = right;
    else if(ro == 1 && co == 1)
        nn->end = left;
    else if(ro == 3 && co == 2)
        nn->end = down;
    else
        nn->end = c;
    nn->row = ro;
    nn->column = co;
    nn->status = 1;
    nn->number = added_tile_count;
    nn->hour = h;
    nn->alibi_status = 1;
    nn->det_vision = 0;
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
                                printf("%s,>\t", current->tile_name);
                            if (current->end == left)
                                printf("<,%s\t", current->tile_name);
                        }
                        if (current->status == 0) {
                            if (current->end == up)
                                printf("^,--\t");
                            if (current->end == down)
                                printf("v,--\t");
                            if (current->end == right)
                                printf("--,>\t");
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
void token1()
{
    tokens.rotation_joker = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens.rotation_exchange = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens.toby_watson = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    tokens.sherlock_alibi = rand()% (1 - 0 + 1) + 0; //random num between 0 & 1
    token_print_notes();

}
void token2()
{
    if(tokens.rotation_joker == 0)
        tokens.rotation_joker = 1;
    else if(tokens.rotation_joker == 1)
        tokens.rotation_joker = 0;

    if(tokens.rotation_exchange == 0)
        tokens.rotation_exchange = 1;
    else if(tokens.rotation_exchange == 1)
        tokens.rotation_exchange = 0;

    if(tokens.toby_watson == 0)
        tokens.toby_watson = 1;
    else if(tokens.toby_watson == 1)
        tokens.toby_watson = 0;

    if(tokens.sherlock_alibi == 0)
        tokens.sherlock_alibi = 1;
    else if(tokens.sherlock_alibi == 1)
        tokens.sherlock_alibi = 0;
    token_print_notes();
}
void token_print_notes()
{
    printf("- - - - - - - - - - - - - - -\n!!TOKENS!!\n- - - - - - - - - - - - - - -\n");
    if(tokens.rotation_joker == 0 && rotation_joker_status == 0)
        printf("1 : ROTATION1\nnote : you can rotate one tile to each side that you want\n");
    if(tokens.rotation_joker == 1 && rotation_joker_status == 0)
        printf("1 : JOKER\nnote : you can move one of the detectives for one block\n");

    if(tokens.rotation_exchange == 0 && rotation_exchange_status == 0)
        printf("2 : ROTATION2\nnote : you can rotate one tile to each side that you want\n");
    if(tokens.rotation_exchange == 1 && rotation_exchange_status == 0)
        printf("2 : EXCHANGE\nnote : you can change two tiles\n");

    if(tokens.toby_watson == 0 && toby_watson_status == 0)
        printf("3 : TOBY\nnote : you can move Toby for one or two blocks\n");
    if(tokens.toby_watson == 1 && toby_watson_status == 0)
        printf("3 : WATSON\nnote : you can move Watson for one or two blocks\n");

    if(tokens.sherlock_alibi == 0 && sherlock_alibi_status == 0)
        printf("4 : SHERLOCK\nnote : you can move Sherlock for one or two blocks\n");
    if(tokens.sherlock_alibi == 1 && sherlock_alibi_status == 0)
        printf("4 : ALIBI\nnote : you can choose one of suspects cards\n");
}
void choose_token(struct tile *head)
{
    char choice[10] ;
    int flag = 0;
    do {
        fflush(stdin);
        scanf("%s" , choice);
        if (strcmpi(choice, "rotation1") == 0) {
            rotation(head);
            rotation_joker_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "rotation2") == 0) {
            rotation(head);
            rotation_exchange_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "joker") == 0) {
            joker();
            rotation_joker_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "toby") == 0) {
            toby();
            toby_watson_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "watson") == 0) {
            watson();
            toby_watson_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "sherlock") == 0) {
            sherlock();
            sherlock_alibi_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "exchange") == 0) {
            head = exchange(head);
            rotation_exchange_status = 1;
            flag = 1;
        }
        if (strcmpi(choice, "alibi") == 0) {
            alibi(head, turn);
            sherlock_alibi_status = 1;
            flag = 1;
        }
        if (flag == 0)
            printf("pls enter a valid string!\n");
    } while (flag == 0);
}
void rotation(struct tile* head)
{
    printf("pls enter name of the character that you want to rotate: \n");
    char rotate_character[3];
    fflush(stdin);
    scanf("%s" , rotate_character);
    printf("which direction do yo want to be closed?(up , down , left , right)\n");
    char dir[10];
    fflush(stdin);
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
}
void toby()
{
    printf("how many blocks do you want to move TOBY? (1 or 2) ");
    int toby_blocks;
    fflush(stdin);
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
    fflush(stdin);
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
    fflush(stdin);
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
    fflush(stdin);
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
    fflush(stdin);
    scanf("%s" , sus1);
    fflush(stdin);
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
//            printf("name = %s , row = %d , col = %d , end = %d\n" , new->tile_name , new ->row , new->column , new->end);
        }
    }
    for (new = head; new->next != NULL; new = new->next)
    {
        if(strcmpi(new->tile_name, sus2) == 0 && flag == 1) {
            new->row = r1;
            new->column = c1;
            new ->end = e2;
//            strcpy(new->tile_name, sus1);
//            printf("*name = %s , row = %d , col = %d , end = %d\n" , new->tile_name , new ->row , new->column , new->end);
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
        } while (current->is_jack == 1 || current->status == 0 || current->alibi_status ==0);
        printf("%s was reversed\n" , current->tile_name);
        current->status = 0;
        current->alibi_status = 0;
    }
    if(turn == 1)
    {
        do {
            r = rand()%(9 - 1 + 1) + 1;
            for (current = head ; current!= NULL && current->number != r ; current = current->next );
        } while (current->is_jack == 1 || current->status == 0 || current->alibi_status == 0 );
        hourglass = hourglass + current->hour;
        printf("--hourglasses are updated!--\n");
        current->alibi_status = 0;
    }
}
int detectives_vision(struct tile *head , int location) {
    struct tile *current1;
    struct tile *current2;
    struct tile *current3;
    if (location == 1)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("1 :row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("1 :row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("1 :row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 2)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("2 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == down) {
            return 0;
        }
//        printf("case : 2 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("2 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == down) {
            return 0;
        }
//        printf("case : 2 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("2 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 3)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("3 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == down) {
            return 0;
        }
//        printf("case : 3 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("3 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == down) {
            return 0;
        }
//        printf("case : 3 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("3 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 4)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("4 : row = %d , col = %d , is_jack = %d\n", current1->row, current1->column, current1->is_jack);
            return 1;
        } else if (current1->end == right) {
            return 0;
        }
//        printf("case : 4 , az %d %d rad sod\n", current1->row, current1->column);
        //
        for (current2 = head;((current2->row != 1) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("4 : row = %d , col = %d , is_jack = %d\n", current2->row, current2->column, current2->is_jack);
            return 1;
        } else if (current2->end == right) {
            return 0;
        }
//        printf("case : 4 , az %d %d rad sod\n", current2->row, current2->column);
        //
        for (current3 = head;((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("4 : row = %d , col = %d , is_jack = %d\n", current3->row, current3->column, current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 5)
    {
        for (current1 = head;(current1->row != 1 || current1->column != 3); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("5 : row = %d , col = %d , is_jack = %d\n", current1->row, current1->column, current1->is_jack);
            return 1;
        } else if (current1->end == left) {
            return 0;
        }
//        printf("case : 5 , az %d %d rad sod\n", current1->row, current1->column);
        //
        for (current2 = head;(current2->row != 1 || current2->column != 2); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("5 : row = %d , col = %d , is_jack = %d\n", current2->row, current2->column, current2->is_jack);
            return 1;
        } else if (current2->end == left) {
            return 0;
        }
//        printf("case : 5 , az %d %d rad sod\n", current2->row, current2->column);
        //
        for (current3 = head;(current3->row != 1 || current3->column != 1); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("5 : row = %d , col = %d , is_jack = %d\n", current3->row, current3->column, current3->is_jack);
            return 1;
        }
        return 0;

    }
    /////////////////////
    if (location == 6)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("6 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == right) {
            return 0;
        }
//        printf("case : 6 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("6 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == right) {
            return 0;
        }
//        printf("case : 6 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("6 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 7)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("7 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == left) {
            return 0;
        }
//        printf("case : 7 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("7 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == left) {
            return 0;
        }
//        printf("case : 7 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("7 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 8)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("8 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == right) {
            return 0;
        }
//        printf("case : 8 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("8 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == right) {
            return 0;
        }
//        printf("case : 8 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("8 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 9)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("9 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == left) {
            return 0;
        }
//        printf("case : 9 , az %d %d rad sod\n" ,current1->row , current1->column );
        //
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("9 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == left) {
            return 0;
        }
//        printf("case : 9 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("9 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 10)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("10 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == up) {
            return 0;
        }
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("10 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == up) {
            return 0;
        }
//        printf("case : 10 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("10 :row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 11)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("11 : row = %d , col = %d , is_jack = %d\n", current1->row, current1->column, current1->is_jack);
            return 1;
        } else if (current1->end == up) {
            return 0;
        }
//        printf("case : 11 , az %d %d rad sod\n", current1->row, current1->column);
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("11 : row = %d , col = %d , is_jack = %d\n", current2->row, current2->column, current2->is_jack);
            return 1;
        } else if (current2->end == up) {
            return 0;
        }
//        printf("case : 11 , az %d %d rad sod\n", current2->row, current2->column);
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("11 : row = %d , col = %d , is_jack = %d\n", current3->row, current3->column, current3->is_jack);
            return 1;
        }
        return 0;
    }
    /////////////////////
    if (location == 12)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        if (current1->is_jack == 1) {
//            printf("12 : row = %d , col = %d , is_jack = %d\n" , current1->row , current1->column , current1->is_jack);
            return 1;
        } else if (current1->end == up) {
            return 0;
        }
        //
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        if (current2->is_jack == 1) {
//            printf("12 : row = %d , col = %d , is_jack = %d\n" , current2->row , current2->column , current2->is_jack);
            return 1;
        } else if (current2->end == up) {
            return 0;
        }
//        printf("case : 12 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        if (current3->is_jack == 1) {
//            printf("12 : row = %d , col = %d , is_jack = %d\n" , current3->row , current3->column , current3->is_jack);
            return 1;
        }
        return 0;
    }

}
int can_see(struct tile *head)
{
    if((detectives_vision(head , Toby) == 1) || (detectives_vision(head , Watson) == 1) || (detectives_vision(head , Sherlock) == 1))
    {
        printf("**MR.JACK CAN be seen by detectives!**\n");
        vision_can_see(head , Toby);
        vision_can_see(head , Watson);
        vision_can_see(head , Sherlock);
        struct tile *current;
        struct tile *current1;

        for (current = head ; current != NULL ; current= current->next)
        {
            if(current->det_vision == 0)
            {
                current->status = 0;
            }
        }
//        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
//        current->status = 0;
    }
    if((detectives_vision(head , Toby) == 0) && (detectives_vision(head , Watson) == 0) && (detectives_vision(head , Sherlock) == 0))
    {
        printf("**MR.JACK CAN'T be seen by detectives!**\n");
        rotate_tile_cant_see(head , Toby);
        rotate_tile_cant_see(head , Watson);
        rotate_tile_cant_see(head , Sherlock);
        hourglass++;
    }
}
int rotate_tile_cant_see(struct tile *head , int location)
{
    struct tile *current1;
    struct tile *current2;
    struct tile *current3;
    if (location == 1)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 2)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 3)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 4)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 1) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 5)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 1) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->status = 0;
        return 0;

    }
    /////////////////////
    if (location == 6)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 7)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 8)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 9)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 10)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 11)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
    /////////////////////
    if (location == 12)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->status = 0;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->status = 0;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->status = 0;
        return 0;
    }
}
int vision_can_see(struct tile *head , int location)
{
    struct tile *current1;
    struct tile *current2;
    struct tile *current3;
    if (location == 1)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 2)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 3)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == up) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == up) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == down) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == up) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 4)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 1) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 5)
    {
        for (current1 = head;((current1->row != 1) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 1) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;

    }
    /////////////////////
    if (location == 6)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 7)
    {
        for (current1 = head;((current1->row != 2) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 2) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 8)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == left) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == left) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == right) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == left) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 9)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == right) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 3) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == right) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == left) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 3) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == right) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 10)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 1)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 1)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 1)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 11)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 2)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 2)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 2)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
    /////////////////////
    if (location == 12)
    {
        for (current1 = head;((current1->row != 3) || (current1->column != 3)); current1 = current1->next);
        if (current1->end == down) {
            return 0;
        }
        current1->det_vision = 1;
        if (current1->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current1->row , current1->column );
        ////
        for (current2 = head; ((current2->row != 2) || (current2->column != 3)); current2 = current2->next);
        if (current2->end == down) {
            return 0;
        }
        current2->det_vision = 1;
        if (current2->end == up) {
            return 0;
        }
//        printf("case : 1 , az %d %d rad sod\n" ,current2->row , current2->column );
        //
        for (current3 = head; ((current3->row != 1) || (current3->column != 3)); current3 = current3->next);
        if (current3->end == down) {
            return 0;
        }
        current3->det_vision = 1;
        return 0;
    }
}
int detective_win_check(struct tile *head)
{
    struct tile *current ;
    int front_count = 0;
    for (current = head ; current != NULL ; current= current->next)
    {
        if(current->status == 1)
        {
            front_count++;
        }
    }
    if ((front_count == 1) && ((game_round != 8)))
    {
        printf("* * * * * * * * * * * *\n\n");
        printf("congratulations!!!\nDETECTIVE IS WINNER!\n\n");
        printf(" * * * * * * * * * * * *\n");
        return 1;
    }
    return 0;
}
int jack_win_check(struct tile *head)
{
    if(hourglass == 6)
    {
        printf("* * * * * * * * * * * *\n\n");
        printf("congratulations!!!\nMR.JACK IS WINNER!\n\n");
        printf(" * * * * * * * * * * * *\n");
        return 1;
    }
    return 0;
}
///////////////////////////

//////////////////////////

//////save and load//////

////////////////////////

///////////////////////
void save_game(struct tile *head)
{
    fprintf(write , "%s\n" , game_name);
    fprintf(write , "%d\n" , game_round);
    fprintf(write , "%s\n" , MrJacks_real_name_st);
    fprintf(write , "%d\n" , hourglass);
    file_print_tokens();
    file_print_map(head);
    fprintf(write , "*\n");
}
void file_print_map(struct tile *head)
{
    struct tile *current;
    current = head;
    int margin_count = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (j == 0 || j == 4 || i == 0 || i == 4) {
                if ((j == 0 && i == 0) || (i == 0 & j == 4) || (i == 4 && j == 0) || (i == 4 && j == 4))
                   fprintf(write , "  \t");
                else {
                    int f = 0;
                    if (Toby == margin_count) {
                        fprintf(write , "T");
                        f = 1;
                    }
                    if (Sherlock == margin_count) {
                        fprintf(write ,"S");
                        f = 1;
                    }
                    if (Watson == margin_count) {
                        fprintf(write , "W");
                        f = 1;
                    }
                    if (f == 1)
                        fprintf(write , "\t");
                    else
                        fprintf(write , "  \t");
                    margin_count++;
                }

            } else {
                int count = 0;
                for (current = head; count < 9; current = current->next) {
                    if ((current->row == i) && (current->column == j)) {
                        if (current->status == 1) {
                            if (current->end == up)
                                fprintf(write , "^%s\t", current->tile_name);
                            if (current->end == down)
                                fprintf(write , "v%s\t", current->tile_name);
                            if (current->end == right)
                                fprintf(write , ">%s\t", current->tile_name);
                            if (current->end == left)
                                fprintf(write , "<%s\t", current->tile_name);
                        }
                        if (current->status == 0) {
                            if (current->end == up)
                                fprintf(write , "^--\t");
                            if (current->end == down)
                                fprintf(write , "v--\t");
                            if (current->end == right)
                                fprintf(write , ">--\t");
                            if (current->end == left)
                                fprintf(write , "<--\t");
                        }
                    }
                    count++;
                }

            }
        }
        fprintf(write , "\n");
    }
}
void file_print_tokens()
{
    if(tokens.rotation_joker == 0 && rotation_joker_status == 1)
        fprintf(write , "rotation1\n");
    if(tokens.rotation_joker == 1 && rotation_joker_status == 1)
        fprintf(write , "joker\n");

    if(tokens.rotation_exchange == 0 && rotation_exchange_status == 1)
        fprintf(write , "rotation2\n");
    if(tokens.rotation_exchange == 1 && rotation_exchange_status == 1)
        fprintf(write , "exchange\n");

    if(tokens.toby_watson == 0 && toby_watson_status == 1)
        fprintf(write , "toby\n");
    if(tokens.toby_watson == 1 && toby_watson_status == 1)
        fprintf(write , "watson\n");

    if(tokens.sherlock_alibi == 0 && sherlock_alibi_status == 1)
        fprintf(write , "sherlock\n");
    if(tokens.sherlock_alibi == 1 && sherlock_alibi_status == 1)
        fprintf(write , "alibi\n");
}
void load_game(struct tile *head)
{
    read = fopen("save.txt" , "r");
    printf("pls enter the name:\n");
    loadable_games();
    fflush(stdin);
    scanf("%s" , file_name_load);
    load_selected_game(head);
    printmap(head);
}
void loadable_games()
{
    rewind(read);
    char c ;
    while (feof(read) == 0)
    {
        int flag = 0;
        fscanf(read , "%s" , file_games[name_count]);
        name_count++;
        while (flag == 0)
        {
            fscanf(read , "%c" , &c);
            if(c == '*')
            {
                flag = 1;
            }
        }
    }
    name_count = name_count - 1;
    for (int i = 0 ; i < name_count ; i++)
        printf("%dth : %s\n" ,i ,  file_games[i]);
}
void load_selected_game(struct tile *head)
{
    int file_game_round;
    rewind(read);
    char c ;
    while (feof(read) == 0)
    {
        char st[15] ;
        fscanf(read , "%s" , st);
        if(strcmpi(st , file_name_load) == 0)
        {
            fscanf(read , "%d" , &file_game_round);
//            printf("game round = %d\n" , file_game_round);
        }

        int flag = 0;
        while (flag == 0)
        {
            fscanf(read , "%c" , &c);
            if(c == '*')
            {
                flag = 1;
            }
        }
    }

    rewind(read);
    int x;
    while (feof(read) == 0)
    {
        char st[15]  , tok[10];
        fscanf(read , "%s" , st);
        if(strcmpi(st , file_name_load) == 0)
        {
            fscanf(read , "%d" , &x);
            if(x == file_game_round)
            {
                printf("round %d\n" , file_game_round);
                fscanf(read , "%s" , MrJacks_real_name_st);
                printf("mr jack's real name : %s\n" , MrJacks_real_name_st);
                fscanf(read , "%d\n" , &hourglass);
                printf("hg = %d\n" , hourglass);
                for (int k = 0 ; k < 4 ; k++) {
                    fscanf(read, "%s", tok);
                    if (strcmpi(tok, "rotation1") == 0) {
                        rotation_joker_status = 1;
                    }
                    if (strcmpi(tok, "rotation2") == 0) {
                        rotation_exchange_status = 1;
                    }
                    if (strcmpi(tok, "joker") == 0) {
                        rotation_joker_status = 1;
                    }
                    if (strcmpi(tok, "toby") == 0) {
                        toby_watson_status = 1;
                    }
                    if (strcmpi(tok, "watson") == 0) {
                        toby_watson_status = 0;
                    }
                    if (strcmpi(tok, "sherlock") == 0) {
                        sherlock_alibi_status = 1;
                    }
                    if (strcmpi(tok, "exchange") == 0) {
                        rotation_exchange_status = 0;
                    }
                    if (strcmpi(tok, "alibi") == 0) {
                        sherlock_alibi_status = 0;
                    }
                    printf("tok : %s\n" , tok);
                }
                read_map(&head);
                return;
            }
        }
        int flag = 0;
        while (flag == 0)
        {
            fscanf(read , "%c" , &c);
            if(c == '*')
            {
                flag = 1;
            }
        }
    }
}
void read_map(struct tile** head_ref) {
//    void add_tile(struct tile** head_ref, char tile_info[3] ,int ro ,int co ,int added_tile_count ,int h)
    struct tile* nn = (struct tile*) malloc(sizeof(struct tile));
    struct tile *last = *head_ref;
    int added_tile_count = 0;
    int margin_count = 0;
    int tab_count = 0;
    char c;
    //first line
    while (1)
    {
        fscanf(read , "%c" , &c);
        if(c == 'S')
        {
            Sherlock = margin_count;
        }
        if(c == 'T')
        {
            Toby = margin_count;
        }
        if(c == 'W')
        {
            Sherlock = margin_count;
        }
        if(c == '\n')
        {
            break;
        }
    }
    //second line
    for (int i = 0 ; i < 3 ; i++)
    {
        int ro = 0;
    while (1) {
        fscanf(read, "%c", &c);
        if (c == 'S') {
            Sherlock = margin_count;
        }
        if (c == 'T') {
            Toby = margin_count;
        }
        if (c == 'W') {
            Sherlock = margin_count;
        }
        if (c == '\n') {
            break;
        }
        if (c == '^') {
            nn->end = up;
            char s[3];
            fscanf(read, "%s", s);
            strcpy(nn->tile_name, s);
            nn->row = ro;
            ro++;
            nn->column = i;
            nn->status = 1;
            nn->number = added_tile_count;
            if(strcmpi(nn->tile_name , "SG") == 0 || strcmpi(nn->tile_name , "IL") == 0)
                nn->hour = 0;
            else if(strcmpi(nn->tile_name , "MA") == 0 )
                nn->hour = 2;
            else
                nn->hour = 1;
            nn->alibi_status = 1;
            nn->det_vision = 0;
            nn->next = NULL;
            if (*head_ref == NULL)
            {
                *head_ref = nn;
                return;
            }
            while (last->next != NULL)
                last = last->next;
            last->next = nn;

        } else if (c == 'v') {
            nn->end = down;
            char s[3];
            fscanf(read, "%s", s);
            strcpy(nn->tile_name, s);
            nn->row = ro;
            ro++;
            nn->column = i;
            nn->status = 1;
            nn->number = added_tile_count;
            if(strcmpi(nn->tile_name , "SG") == 0 || strcmpi(nn->tile_name , "IL") == 0)
                nn->hour = 0;
            else if(strcmpi(nn->tile_name , "MA") == 0 )
                nn->hour = 2;
            else
                nn->hour = 1;
            nn->alibi_status = 1;
            nn->det_vision = 0;
            nn->next = NULL;
            if (*head_ref == NULL)
            {
                *head_ref = nn;
                return;
            }
            while (last->next != NULL)
                last = last->next;
            last->next = nn;

        } else if (c == '<') {
            nn->end = left;
            char s[3];
            fscanf(read, "%s", s);
            strcpy(nn->tile_name, s);
            nn->row = ro;
            ro++;
            nn->column = i;
            nn->status = 1;
            nn->number = added_tile_count;
            if(strcmpi(nn->tile_name , "SG") == 0 || strcmpi(nn->tile_name , "IL") == 0)
                nn->hour = 0;
            else if(strcmpi(nn->tile_name , "MA") == 0 )
                nn->hour = 2;
            else
                nn->hour = 1;
            nn->alibi_status = 1;
            nn->det_vision = 0;
            nn->next = NULL;
            if (*head_ref == NULL)
            {
                *head_ref = nn;
                return;
            }
            while (last->next != NULL)
                last = last->next;
            last->next = nn;

        } else if (c == '>') {
            nn->end = right;
            char s[3];
            fscanf(read, "%s", s);
            strcpy(nn->tile_name, s);
            nn->row = ro;
            ro++;
            nn->column = i;
            nn->status = 1;
            nn->number = added_tile_count;
            if(strcmpi(nn->tile_name , "SG") == 0 || strcmpi(nn->tile_name , "IL") == 0)
                nn->hour = 0;
            else if(strcmpi(nn->tile_name , "MA") == 0 )
                nn->hour = 2;
            else
                nn->hour = 1;
            nn->alibi_status = 1;
            nn->det_vision = 0;
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
    }
    }
    while (1)
    {
        fscanf(read , "%c" , &c);
        if(c == 'S')
        {
            Sherlock = margin_count;
        }
        if(c == 'T')
        {
            Toby = margin_count;
        }
        if(c == 'W')
        {
            Sherlock = margin_count;
        }
        if(c == '\n')
        {
            break;
        }
    }
    printmap(head);

}
//void continue_game()
//{
//
//}