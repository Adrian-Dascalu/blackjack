#include <stdio.h>
#include <stdlib.h> 
#include <conio.h>
#include <time.h>
#include <string.h>

const char* valoare_carte[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

const char* culoare_carte[4] = {"ROMB", "INIMA ROSIE", "TREFLA", "INIMA NEAGRA"};

typedef struct Carte_De_Joc
{
    int valoare;
    int culoare;
} Carte_De_Joc;

typedef struct Pachet_Carti
{
    int nr_asi[16];
    int nr_asi1[16];
    int nr_carte[16];
    int player_bet[16];
    int sume_player_hand[16];
    int lose[16];
    int bust[16];
    int blackjack[16];
    int dealer_win;
    int carti_impartite;
    int nr_jucatori;
    int jucator_split[8];

    Carte_De_Joc carti[52];

    Carte_De_Joc carti_mana[16][10];
} Pachet_Carti;

void Alege(Pachet_Carti * pachet, int i);
void Hit(Pachet_Carti * pachet, int i, int d);
void End_Game(Pachet_Carti * pachet);
void Play_Split(Pachet_Carti * pachet, int i, int d);
void Results(Pachet_Carti * pachet);

void Seteaza(Carte_De_Joc * carte, int x)
{
    carte -> valoare = x % 13;
    carte -> culoare = x / 13;
}

int Valoare(Carte_De_Joc * carte)
{
    return carte -> valoare;
}

int Culoare(Carte_De_Joc * carte)
{
    return carte -> culoare;
}

void Display(Carte_De_Joc * carte)
{
    int width = 7;

    culoare_carte[Culoare(carte)] == "INIMA ROSIE" ? width = 5 :
    culoare_carte[Culoare(carte)] == "INIMA NEAGRA" ? width = 4 :
    culoare_carte[Culoare(carte)] == "ROMB" ? width = 12 :
    culoare_carte[Culoare(carte)] == "TREFLA" ? width = 10 : 0;

    valoare_carte[Valoare(carte)] == "10" ? width-- : 0;

    printf("\n%s DE %s%*s", valoare_carte[carte -> valoare], culoare_carte[carte ->culoare], width, "");
}

int Transf(Carte_De_Joc * carte)
{
    if((Valoare(carte) > 8) && (Valoare(carte) < 12)) return 10;
    else if(Valoare(carte) == 12) return 11;
    else return Valoare(carte) + 2;
}

void Creeaza_Pachet(Pachet_Carti * pachet)
{
    for(int i = 0; i < 52; i++) Seteaza(&(pachet -> carti[i]), i);
}

void Amesteca(Pachet_Carti * pachet)
{
    int i, k;

    Carte_De_Joc temp;

    srand(time(NULL));

    for(i = 0; i < 52; i++)
    {
        k = rand() % 52;

        temp = pachet -> carti[i];
        pachet -> carti[i] = pachet -> carti[k];
        pachet -> carti[k] = temp;
    }
}

void Bet(Pachet_Carti * pachet, int n)
{
    int isChar= 0;
    int IntNum = 0;

    char str[100];

    for(int i = 0; i < n; i++)
    {
        printf("Bet jucator %d : ", i + 1);

        scanf("%s", str);

        getchar();

        for(int j = 0; j < strlen(str); j++)
        {
            if(!((str[j] >= '0') && (str[j] <= '9')))
                isChar++;
        }

        if(isChar > 0)
        {
            i--;

            isChar = 0;
            
            printf("Bet-ul trebuie sa fie numar intreg pozitiv, reintrodu un numar : \n");
        }
        else
        {
            IntNum = atoi(str);

            pachet -> player_bet[i] = IntNum;
        }
    }

    pachet -> nr_jucatori = n;
}

void Impartire(Pachet_Carti * pachet)
{
    for(int i = 0; i <= pachet -> nr_jucatori; i++)
    {
        pachet -> nr_asi[i] = 0;
        pachet -> nr_asi1[i] = 0;
        pachet -> bust[i] = 0;
        pachet -> blackjack[i] = 0;
        pachet -> nr_carte[i] = 2;

        for(int k = 0; k < 2; k++)
        {
            pachet -> carti_mana[i][k] = pachet -> carti[pachet -> carti_impartite];

            if(Transf((&(pachet -> carti_mana[i][k]))) == 11)
            {
                pachet -> nr_asi[i]++;
            }

            pachet -> carti_impartite++;
        }

        pachet -> sume_player_hand[i] = Transf(&(pachet -> carti_mana[i][0])) + Transf(&(pachet -> carti_mana[i][1]));

        if(pachet -> sume_player_hand[i] > 21)
        {
            pachet -> sume_player_hand[i] -= 10;
            pachet -> nr_asi1[i]++;
        }
        else if(pachet -> sume_player_hand[i] == 21)
        {
            pachet -> blackjack[i] = 1;
        }
    }
}

void Afisare_Masa(Pachet_Carti * pachet)
{
    system("clear");

    for(int i = 0; i < pachet -> nr_jucatori; i++)
    {
        printf("Jucator %d carti : \n", i + 1);

        Display(&(pachet -> carti_mana[i][0]));

        printf("Bet : %d", pachet -> player_bet[i]);

        Display(&(pachet -> carti_mana[i][1]));

        printf("Suma mana : %d", pachet -> sume_player_hand[i]);

        printf("\n\n--------------------\n\n");
    }

    printf("Carti Dealer : ");

    Display(&(pachet -> carti_mana[pachet -> nr_jucatori][0]));

    printf("\n\nApasa enter pentru a continua");

    getchar();

    system("clear");
}

void Mana_Jucator(Pachet_Carti * pachet, int i)
{
    Display(&(pachet -> carti_mana[i][0]));

    printf("Bet : %d", pachet -> player_bet[i]);

    Display(&(pachet -> carti_mana[i][1]));

    printf("Suma mana : %d", pachet -> sume_player_hand[i]);
}

void Mana(Pachet_Carti * pachet)
{
    for(int i = 0; i < pachet -> nr_jucatori; i++)
    {
        printf("Randul Jucatorului %d :\n", i + 1);

        Display(&(pachet -> carti_mana[i][0]));

        printf("Bet : %d", pachet -> player_bet[i]);

        Display(&(pachet -> carti_mana[i][1]));

        printf("Suma : %d\n", pachet -> sume_player_hand[i]);

        if(pachet -> sume_player_hand[i] == 21)
        {
            pachet -> blackjack[i] = 1;

            printf("\nBLACKJACK DIN PRIMA!\n\n");

            if(i < pachet -> nr_jucatori - 1)
            {
                printf("--------------------\n\n");
            }
        }
        else
        {
            Alege(pachet, i);
        }
    }
    
    printf("Enter pentru next");
    getchar();
    getchar();

    system("clear");

    End_Game(pachet);
}

void Alege(Pachet_Carti * pachet, int i)
{
    if(pachet -> nr_carte[i] == 2)
    {
        printf("\nSelecteaza : [hit / stay / double / split] ");
    }
    else
    {
        printf("\nSelecteaza : [hit / stay] ");
    }

    char tura[10];

    scanf("%s", tura);

    if(i < pachet -> nr_jucatori - 1)
    {
        printf("\n------------------------------\n");
    }

    if(strcmp(tura, "hit") == 0)
    {
        Hit(pachet, i, 0);
    }
    else if(strcmp(tura, "stay") == 0)
    {
        printf("\n");
    }
    else if((strcmp(tura, "double") == 0) && (pachet -> nr_carte[i] == 2))
    {
        pachet -> player_bet[i] *= 2;
        
        Hit(pachet, i, 1);
    }
    else if((strcmp(tura, "double") == 0))
    {
        printf("\nNu poti da double!\n");
        
        Alege(pachet, i);
    }
    else if((strcmp(tura, "split") == 0) && (pachet -> nr_carte[i] == 2) &&
     (Transf(&(pachet -> carti_mana[i][0])) == Transf(&(pachet -> carti_mana[i][1]))))
    {
        pachet -> jucator_split[i] = 1;

        pachet -> carti_mana[i + pachet -> nr_jucatori + 1][0] = pachet -> carti_mana[i][1];
        
        pachet -> carti_mana[i][1] = pachet -> carti[pachet -> carti_impartite++];
        pachet -> carti_mana[i + pachet -> nr_jucatori + 1][1] = pachet -> carti[pachet -> carti_impartite++];

        pachet -> sume_player_hand[i] = Transf(&(pachet -> carti_mana[i][0])) + Transf(&(pachet -> carti_mana[i][1]));
        pachet -> sume_player_hand[i + pachet -> nr_jucatori + 1] =
         Transf(&(pachet -> carti_mana[i + pachet -> nr_jucatori + 1][0])) + Transf(&(pachet -> carti_mana[i + pachet -> nr_jucatori + 1][1]));

        pachet -> player_bet[i] /= 2;
        pachet -> player_bet[i + pachet -> nr_jucatori + 1] = pachet -> player_bet[i];

        printf("\n");

        Play_Split(pachet, i, 0);
        Play_Split(pachet, i + pachet -> nr_jucatori + 1, 1);
    }
    else if((strcmp(tura, "split") == 0))
    {
        printf("\nNu poti da split!\n");

        Alege(pachet, i);
    }
    else
    {
        printf("\nCe vrei sa faci?\n");

        Alege(pachet, i);
    }
}

void Alege_Split(Pachet_Carti * pachet, int i)
{
    if(pachet -> nr_carte[i] == 2)
    {
        printf("\n\nSelecteaza : [hit / stay / double] ");
    }
    else
    {
        printf("\n\nSelecteaza : [hit / stay] ");
    }

    char tura[10];

    scanf("%s", tura);

    if(i < pachet -> nr_jucatori - 1)
    {
        printf("\n------------------------------\n");
    }

    if(strcmp(tura, "hit") == 0)
    {
        Hit(pachet, i, 0);
    }
    else if(strcmp(tura, "stay") == 0)
    {
        printf("\n");
    }
    else if((strcmp(tura, "double") == 0) && (pachet -> nr_carte[i] == 2))
    {
        pachet -> player_bet[i] *= 2;
        
        Hit(pachet, i, 1);
    }
    else if((strcmp(tura, "double") == 0))
    {
        printf("\nNu poti da double!\n");
        
        Alege_Split(pachet, i);
    }
    else
    {
        printf("\nCe vrei sa faci?\n");

        Alege_Split(pachet, i);
    }
}

void Play_Split(Pachet_Carti * pachet, int i, int d)
{
    if(Transf(&(pachet -> carti_mana[i][0])) == 11)
    {
        pachet -> nr_asi[i] = 1;
    }

    if(Transf(&(pachet -> carti_mana[i][1])) == 11)
    {
        pachet -> nr_asi[i] = 1;
    }

    if(pachet -> sume_player_hand[i] == 21)
    {
        pachet -> blackjack[i] = 1;
    }
    else if(pachet -> sume_player_hand[i] > 21)
    {
        pachet -> sume_player_hand[i] -= 10;
        pachet -> nr_asi1[i]++;
    }

    if(i > 6)
    {
        printf("Mana 2:\n");
    }
    else
    {
        printf("Mana 1:\n");
    }

    Display(&(pachet -> carti_mana[i][0]));

    printf("Bet : %d\n", pachet -> player_bet[i]);
        
    printf("\nRaise bet? [y/n] ");

    char r_bet[10];

    while(1)
    {
        scanf("%s", r_bet);

        if(strlen(r_bet) == 1)
        {
            if(r_bet[0] == 'n')
            {
                break;
            }
            else if(r_bet[0] == 'y')
            {
                break;
            }
            else
            {
                printf("Raspunde cu 'y' sau 'n'!\n");
            }
        }
        else
        {
            printf("Raspunde cu 'y' sau 'n'!\n");
        }
    }

    char temp[50];

    if(r_bet[0] == 'y')
    {
        printf("Cu cat sa fie marit bet-ul? ");

        while(1)
        {
            scanf("%s", temp);

            int isChar = 0;

            for(int j = 0; j < strlen(temp); j++)
            {
                if(!((temp[j] >= '0') && (temp[j] <= '9')))
                {
                    isChar++;
                }
            }

            if(isChar > 0)
            {
                printf("Bet-ul trebuie sa fie numar intreg pozitiv, reintrodu un numar : \n");
            }
            else
            {
                int IntNum = atoi(temp);

                pachet -> player_bet[i] += IntNum;
            
                break;
            }
        }
    }   
    
    printf("\nCarti :\n");

    Mana_Jucator(pachet, i);
    
    if(pachet -> sume_player_hand[i] == 21)
    {
        printf("BLACKJACK DIN PRIMA!\n\n");

        pachet -> blackjack[i] = 1;

        return;
    }

    pachet -> nr_carte[i] = 2;

    Alege_Split(pachet, i);
}

void Hit(Pachet_Carti * pachet, int i, int d)
{
    pachet -> carti_mana[i][pachet -> nr_carte[i]] = pachet -> carti[pachet -> carti_impartite];
    pachet -> sume_player_hand[i] += Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]]));

    if(Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]])) == 1)
    {
        pachet -> nr_asi[i] ++;
    }

    pachet -> carti_impartite++;
    pachet -> nr_carte[i]++;

    if(pachet -> sume_player_hand[i] == 21)
    {
        pachet -> blackjack[i] = 1;

        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            Display(&(pachet -> carti_mana[i][j]));
        }

        printf("Suma : %d\n\nBLACKJACK!\n\n", pachet -> sume_player_hand[i]);

        getchar();

        printf("--------------------\n\n");

        return;
    }
    else if(pachet -> sume_player_hand[i] > 21)
    {
        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
            {
                pachet -> sume_player_hand[i] -= 10;
                pachet -> nr_asi1[i]++;

                if(pachet -> sume_player_hand[i] == 21)
                {
                    pachet -> blackjack[i] = 1;

                    for(int j = 0; j < pachet -> nr_carte[i]; j++)
                    {
                        Display(&(pachet -> carti_mana[i][j]));
                    }

                    printf("Suma : %d\n\nBLACKJACK!\n\nEnter pentru next\n", pachet -> sume_player_hand[i]);

                    getchar();

                    printf("--------------------\n\n");

                    return;
                }

                for(int j = 0; j < pachet -> nr_carte[i]; j++)
                {
                    Display(&(pachet -> carti_mana[i][j]));
                }

                printf("Suma : %d\n", pachet -> sume_player_hand[i]);

                if(!d) Alege(pachet, i);

                if(d == 1) printf("\n--------------------\n\n");

                return;
            }
        }

        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            Display(&(pachet -> carti_mana[i][j]));
        }

        printf("Suma : %d", pachet -> sume_player_hand[i]);

        pachet -> bust[i] = 1;

        printf("\n\nAi pierdut, ghinion\n");

        printf("\n--------------------\n\n");

        return;
    }
    else
    {
        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            if((d == 2) && (j == 0))
            {
                continue;
            }
            else if((d == 3) && (j == 1))
            {
                continue;
            }
            else
            {
                Display(&(pachet -> carti_mana[i][j]));
            }
        }

        printf("Suma : %d\n", pachet -> sume_player_hand[i]);

        if(d == 1) printf("\n--------------------\n\n");

        if(!d) Alege(pachet, i);
    }
}

void End_Game(Pachet_Carti * pachet)
{
    int i = pachet -> nr_jucatori;

    if(pachet -> sume_player_hand[i] == 21)
    {
        pachet -> blackjack[i] = 1;
        pachet -> dealer_win = 1;
        pachet -> bust[i] = 0;
    }
    else
    {
        pachet -> dealer_win = 0;
        pachet -> bust[i] = 0;
        pachet -> dealer_win = 0;
    }

    while((pachet -> bust[i] == 0) && (pachet -> dealer_win == 0) &&
     (pachet -> blackjack[i] == 0) && (pachet -> sume_player_hand[i] < 17))
    {
        if(pachet -> sume_player_hand[i] == 21)
        {
            pachet -> blackjack[i] = 1;
            pachet -> dealer_win = 1;

            break;
        }
        else if(pachet -> sume_player_hand[i] > 21) //sus
        {
            if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
            {
                pachet -> sume_player_hand[i] -= 10;
                pachet -> nr_asi1[i]++;
            }
            else
            {
                pachet -> bust[i] = 1;
                pachet -> dealer_win = 0;
                pachet -> blackjack[i] = 0;
            }
        }
        else
        {
            pachet -> dealer_win = 0;
            pachet -> blackjack[i] = 0;
            pachet -> bust[i] = 0;
        }


        for(int j = 0; j < pachet -> nr_jucatori; j++)
        {
            if(pachet -> bust[j] || pachet -> sume_player_hand[j] <= pachet -> sume_player_hand[pachet -> nr_jucatori])
            {
                pachet -> dealer_win++;
            }
        }

        if(pachet -> dealer_win > pachet -> nr_jucatori / 2)
        {
            pachet -> dealer_win = 1;
        }
        else
        {
            pachet -> dealer_win = 0;
        }

        if((pachet -> dealer_win == 0) && (pachet -> bust[i] == 0) &&
         (pachet -> blackjack[i] == 0) && (pachet -> sume_player_hand[i] < 17))
        {
            pachet -> carti_mana[i][pachet -> nr_carte[i]] = pachet -> carti[pachet -> carti_impartite];
            pachet -> sume_player_hand[i] += Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]]));

            if(Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]])) == 11)
            {
                pachet -> nr_asi[i]++;
            }

            if(pachet -> sume_player_hand[i] == 21)
            {
                pachet -> blackjack[i] = 1;

                pachet -> dealer_win = 1;

                pachet -> nr_carte[i]++;
                pachet -> carti_impartite++;

                break;
            }

            if(pachet -> sume_player_hand[i] > 21)
            {
                if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
                {
                    pachet -> sume_player_hand[i] -= 10;
                    pachet -> nr_asi1[i]++;
                }
                else
                {
                    pachet -> bust[i] = 1;
                }
            }

            pachet -> nr_carte[i]++;
            pachet -> carti_impartite++;
        }
    }

    Results(pachet);
}

void Results(Pachet_Carti * pachet)
{
    printf("Results : \n\n");

    for(int i = 0; i < pachet -> nr_jucatori; i++)
    {
        if(pachet -> jucator_split[i] == 1)
        {
            printf("Player %d mana 1 : \n", i + 1);

            for(int j = 0; j < pachet -> nr_carte[i]; j++)
            {
                Display(&(pachet -> carti_mana[i][j]));
            }

            printf("Suma : %d\n\n", pachet -> sume_player_hand[i]);

            if(pachet -> bust[i])
            {
                printf("Bust, ai pierdut %d\n\n", pachet -> player_bet[i]);
            }
            else if((pachet -> blackjack[i]) && (!pachet -> blackjack[pachet -> nr_jucatori]))
            {
                printf("Blackjack, ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else if((pachet -> sume_player_hand[i] == pachet -> sume_player_hand[pachet -> nr_jucatori]))
            {
                printf("Libertate, egalitate, fraternitate, ai primit inapoi : %d\n\n", pachet -> player_bet[i]);
            }
            else if(pachet -> sume_player_hand[i] > pachet -> sume_player_hand[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else if(pachet -> bust[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else
            {
                printf("Ai pierdut %d\n\n", pachet -> player_bet[i]);
            }

            printf("--------------------\n\n");

            printf("Player %d mana 2 : \n", i + 1);

            for(int j = 0; j < pachet -> nr_carte[i + pachet -> nr_jucatori + 1]; j++)
            {
                Display(&(pachet -> carti_mana[i + pachet -> nr_jucatori + 1][j]));
            }

            printf("Suma : %d\n\n", pachet -> sume_player_hand[i + pachet -> nr_jucatori + 1]);

            if(pachet -> bust[i + pachet -> nr_jucatori + 1])
            {
                printf("Bust, ai pierdut %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1]);
            }
            else if((pachet -> blackjack[i + pachet -> nr_jucatori + 1]) && (!pachet -> blackjack[pachet -> nr_jucatori]))
            {
                printf("Blackjack, ai castigat %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1] * 2);
            }
            else if((pachet -> sume_player_hand[i + pachet -> nr_jucatori + 1] == pachet -> sume_player_hand[pachet -> nr_jucatori]))
            {
                printf("Libertate, egalitate, fraternitate, ai primit inapoi : %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1]);
            }
            else if(pachet -> sume_player_hand[i + pachet -> nr_jucatori + 1] > pachet -> sume_player_hand[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1] * 2);
            }
            else if(pachet -> bust[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1] * 2);
            }
            else
            {
                printf("Ai pierdut %d\n\n", pachet -> player_bet[i + pachet -> nr_jucatori + 1]);
            }

            printf("--------------------\n\n");
        }
        else
        {
            printf("Player %d : \n", i + 1);

            for(int j = 0; j < pachet -> nr_carte[i]; j++)
            {
                Display(&(pachet -> carti_mana[i][j]));
            }

            printf("Suma : %d\n\n", pachet -> sume_player_hand[i]);

            if(pachet -> bust[i])
            {
                printf("Bust, ai pierdut %d\n\n", pachet -> player_bet[i]);
            }
            else if((pachet -> blackjack[i]) && (!pachet -> blackjack[pachet -> nr_jucatori]))
            {
                printf("Blackjack, ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else if((pachet -> sume_player_hand[i] == pachet -> sume_player_hand[pachet -> nr_jucatori]))
            {
                printf("Libertate, egalitate, fraternitate, ai primit inapoi : %d\n\n", pachet -> player_bet[i]);
            }
            else if(pachet -> sume_player_hand[i] > pachet -> sume_player_hand[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else if(pachet -> bust[pachet -> nr_jucatori])
            {
                printf("Ai castigat %d\n\n", pachet -> player_bet[i] * 2);
            }
            else
            {
                printf("Ai pierdut %d\n\n", pachet -> player_bet[i]);
            }

            printf("--------------------\n\n");
        }
    }

    printf("Dealer hand :\n");

    for(int j = 0; j < pachet -> nr_carte[pachet -> nr_jucatori]; j++)
    {
        Display(&(pachet -> carti_mana[pachet -> nr_jucatori][j]));
    }

    printf("Suma : %d\n\n", pachet -> sume_player_hand[pachet -> nr_jucatori]);
}

int main()
{
    while(1)
    {
        system("clear");

        int n;

        char replay[100];

        char str[100];

        printf("Numar Jucatori : ");

        while(1)
        {
            scanf("%s", str);
            
            int isChar = 0;

            for(int j = 0; j < strlen(str); j++)
            {
                if(!((str[j] >= '0') && (str[j] <= '9')))
                {
                    isChar++;
                }
            }

            n = atoi(str);

            if(!isChar && (n > 0) && (n <= 7))
            {
                break;
            }
            else
            {
                printf("Numarul de jucatori trebuie sa fie intre 1 si 7, reintrodu un numar : ");
            }
        }

        Pachet_Carti bicycle;
        
        memset(&bicycle, 0, sizeof(bicycle));

        Creeaza_Pachet(&bicycle);

        Amesteca(&bicycle);

        Bet(&bicycle, n);

        Impartire(&bicycle);

        Afisare_Masa(&bicycle);

        Mana(&bicycle);

        printf("Vrei sa continui sa te joci? [y/n] ");
        
        while(1)
        {
            scanf("%s", replay);

            if(strlen(replay) == 1)
            {
                if(replay[0] == 'n')
                {
                    break;
                }
                else if(replay[0] == 'y')
                {
                    break;
                }
                else
                {
                    printf("Raspunde cu 'y' sau 'n'!\n");
                }
            }
            else
            {
                printf("Raspunde cu 'y' sau 'n'!\n");
            }
        }

        if(replay[0] == 'n') break;
    }

    return 0;
}