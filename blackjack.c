#include <stdio.h>
//#include <ctime.h>
//#include <iomanip>
#include <stdlib.h>
#include <conio.h>
//#include <random.h>
#include <time.h>
//#include <cstdlib>
#include <string.h>
//#include <sstream>

const char* valoare_carte[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

const char* culoare_carte[4] = {"ROMB", "INIMA ROSIE", "TREFLA", "INIMA NEAGRA"};

typedef struct Carte_De_Joc
{
    int valoare;
    int culoare;
} Carte_De_Joc;

typedef struct Pachet_Carti
{
    int dealer_win; // = 0;
    int nr_asi[8];
    int nr_asi1[8];
    int carti_impartite; // = 0;
    int nr_carte[8];
    int blackjack[8];
    int bust[8];
    int lose[8];
    int valori_c[13];
    int suma_bet[8];
    int nr_jucatori;
    int suma_mana[8];

    Carte_De_Joc carti[52];

    Carte_De_Joc carti_split[8][10][2];
    Carte_De_Joc carti_mana[8][10];
} Pachet_Carti;

void Alege(Pachet_Carti * pachet_carti, int i);
void Hit(Pachet_Carti * pachet_carti, int i, int d);
void Sfarsit(Pachet_Carti * pachet_carti);

void Seteaza(Carte_De_Joc * carte_de_joc, int x)
{
    carte_de_joc -> valoare = x % 13;
    carte_de_joc -> culoare = x / 13;
}

int Valoare(Carte_De_Joc * carte_de_joc)
{
    return carte_de_joc -> valoare;
}

int Culoare(Carte_De_Joc * carte_de_joc)
{
    return carte_de_joc -> culoare;
}

void Display(Carte_De_Joc * carte_de_joc)
{
    int width = 7;

    culoare_carte[Culoare(carte_de_joc)] == "INIMA ROSIE" ? width = 5 :
    culoare_carte[Culoare(carte_de_joc)] == "INIMA NEAGRA" ? width = 4 :
    culoare_carte[Culoare(carte_de_joc)] == "ROMB" ? width = 12 :
    culoare_carte[Culoare(carte_de_joc)] == "TREFLA" ? width = 10 : 0;

    valoare_carte[Valoare(carte_de_joc)] == "10" ? width-- : 0;

    printf("\n%s DE %s%*s", valoare_carte[carte_de_joc -> valoare], culoare_carte[carte_de_joc ->culoare], width, "");
}

int Transf(Carte_De_Joc * carte_de_joc)
{
    if((Valoare(carte_de_joc) > 8) && (Valoare(carte_de_joc) < 12)) return 10;
    else if(Valoare(carte_de_joc) == 12) return 11;
    else return Valoare(carte_de_joc) + 2;
}

void Creeaza_Pachet(Pachet_Carti * pachet_carti)
{
    for(int i = 0; i < 52; i++) Seteaza(&(pachet_carti -> carti[i]), i);
}

void Amesteca(Pachet_Carti * pachet_carti)
{
    int i, k;

    Carte_De_Joc temp;

    srand(time(NULL));

    for(i = 0; i < 52; i++)
    {
        k = rand() % 52;

        temp = pachet_carti -> carti[i];
        pachet_carti -> carti[i] = pachet_carti -> carti[k];
        pachet_carti -> carti[k] = temp;
    }
}

void Bet(Pachet_Carti * pachet_carti, int n)
{
    int isChar, IntNum = 0;

    char str[100];

    for(int i = 0; i < n; i++)
    {
        printf("Bet jucator %d : ", i + 1);

        scanf("%s", str);

        getchar();

        for(int j = 0; j < strlen(str); j++)
        {
            if(!((str[j] >= '0') && (str[j] <= '9')))
            {
                isChar++;
            }
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

            pachet_carti -> suma_bet[i] = IntNum;
        }
    }

    pachet_carti -> nr_jucatori = n;
}

void Impartire(Pachet_Carti * pachet_carti)
{
    for(int i = 0; i <= pachet_carti -> nr_jucatori; i++)
    {
        pachet_carti -> nr_asi[i] = 0;
        pachet_carti -> nr_asi1[i] = 0;
        pachet_carti -> bust[i] = 0;
        pachet_carti -> blackjack[i] = 0;
        pachet_carti -> nr_carte[i] = 2;

        for(int k = 0; k < 2; k++)
        {
            pachet_carti -> carti_mana[i][k] = pachet_carti -> carti[pachet_carti -> carti_impartite];

            if(Transf((&(pachet_carti -> carti_mana[i][k]))) == 11)
            {
                pachet_carti -> nr_asi[i]++;
            }

            pachet_carti -> carti_impartite++;
        }

        pachet_carti -> suma_mana[i] = Transf(&(pachet_carti -> carti_mana[i][0])) + Transf(&(pachet_carti -> carti_mana[i][1]));

        if(pachet_carti -> suma_mana[i] > 21)
        {
            pachet_carti -> suma_mana[i] -= 10;
            pachet_carti -> nr_asi1[i]++;
        }
        else if(pachet_carti -> suma_mana[i] == 21)
        {
            pachet_carti -> blackjack[i] = 1;
        }
    }
}

void Afisare_Masa(Pachet_Carti * pachet_carti)
{
    system("clear");

    for(int i = 0; i < pachet_carti -> nr_jucatori; i++)
    {
        printf("Jucator %d carti : \n", i + 1);

        Display(&(pachet_carti -> carti_mana[i][0]));

        printf("Bet : %d", pachet_carti -> suma_bet[i]);

        Display(&(pachet_carti -> carti_mana[i][1]));

        printf("Suma mana : %d", pachet_carti -> suma_mana[i]);

        printf("\n\n--------------------\n\n");
    }

    printf("Carti Dealer : ");

    Display(&(pachet_carti -> carti_mana[pachet_carti -> nr_jucatori][0]));

    printf("\n\nApasa enter pentru a continua");

    getchar();

    system("clear");
}

void Mana(Pachet_Carti * pachet_carti)
{
    for(int i = 0; i < pachet_carti -> nr_jucatori; i++)
    {
        printf("Randul Jucatorului %d :\n", i + 1);

        Display(&(pachet_carti -> carti_mana[i][0]));

        printf("Bet : %d", pachet_carti -> suma_bet[i]);

        Display(&(pachet_carti -> carti_mana[i][1]));

        printf("Suma : %d\n", pachet_carti -> suma_mana[i]);

        if(pachet_carti -> suma_mana[i] == 21)
        {
            pachet_carti -> blackjack[i] = 1;

            printf("\nBLACKJACK DIN PRIMA!\n\n");

            printf("--------------------\n\n");
        }
        else
        {
            Alege(pachet_carti, i);
        }
    }
    
    printf("Enter pentru next");
    getchar();
    getchar();

    system("clear");

    Sfarsit(pachet_carti);
}

void Alege(Pachet_Carti * pachet_carti, int i)
{
    if(pachet_carti -> nr_carte[i] == 2)
    {
        printf("\nSelecteaza : hit / stay / double / split\n");
    }
    else
    {
        printf("\nSelecteaza : hit / stay\n");
    }

    char tura[10];

    scanf("%s", tura);

    printf("\n------------------------------\n");

    if(strcmp(tura, "hit") == 0)
    {
        Hit(pachet_carti, i, 0);
    }
    else if(strcmp(tura, "stay") == 0)
    {
        printf("\n");
    }
    else if((strcmp(tura, "double") == 0) && (pachet_carti -> nr_carte[i] == 2))
    {
        pachet_carti -> suma_bet[i] *= 2;
        
        Hit(pachet_carti, i, 1);
    }
    else if((strcmp(tura, "double") == 0))
    {
        printf("\nNu poti da double!\n");
        
        Alege(pachet_carti, i);
    }
    else if((strcmp(tura, "split") == 0) && (pachet_carti -> nr_carte[i] == 2) && (Transf(&(pachet_carti -> carti_mana[i][0])) == Transf(&(pachet_carti -> carti_mana[i][1]))))
    {
        printf("\nIn lucru\n");

        Alege(pachet_carti, i);
    }
    else if((strcmp(tura, "split") == 0))
    {
        printf("\nNu poti da split!\n");

        Alege(pachet_carti, i);
    }
    else
    {
        printf("\nCe vrei sa faci?\n");

        Alege(pachet_carti, i);
    }
}

void Hit(Pachet_Carti * pachet_carti, int i, int d)
{
    pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]] = pachet_carti -> carti[pachet_carti -> carti_impartite];
    pachet_carti -> suma_mana[i] += Transf(&(pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]]));

    if(Transf(&(pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]])) == 1)
    {
        pachet_carti -> nr_asi[i] ++;
    }

    pachet_carti -> carti_impartite++;
    pachet_carti -> nr_carte[i]++;

    if(pachet_carti -> suma_mana[i] == 21)
    {
        pachet_carti -> blackjack[i] = 1;

        for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
        {
            Display(&(pachet_carti -> carti_mana[i][j]));
        }

        printf("Suma : %d\n\nBLACKJACK!\n\n", pachet_carti -> suma_mana[i]);

        getchar();

        printf("--------------------\n\n");

        return;
    }
    else if(pachet_carti -> suma_mana[i] > 21)
    {
        for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
        {
            if(pachet_carti -> nr_asi[i] > pachet_carti -> nr_asi1[i])
            {
                pachet_carti -> suma_mana[i] -= 10;
                pachet_carti -> nr_asi1[i]++;

                if(pachet_carti -> suma_mana[i] == 21)
                {
                    pachet_carti -> blackjack[i] = 1;

                    for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
                    {
                        Display(&(pachet_carti -> carti_mana[i][j]));
                    }

                    printf("Suma : %d\n\nBLACKJACK!\n\nEnter pentru next\n", pachet_carti -> suma_mana[i]);

                    getchar();

                    printf("--------------------\n\n");

                    return;
                }

                for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
                {
                    Display(&(pachet_carti -> carti_mana[i][j]));
                }

                printf("Suma : %d\n", pachet_carti -> suma_mana[i]);

                if(!d) Alege(pachet_carti, i);

                if(d == 1) printf("\n--------------------\n\n");

                return;
            }
        }

        for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
        {
            Display(&(pachet_carti -> carti_mana[i][j]));
        }

        printf("Suma : %d", pachet_carti -> suma_mana[i]);

        pachet_carti -> bust[i] = 1;

        printf("\n\nAi pierdut, ghinion\n");

        printf("\n--------------------\n\n");

        return;
    }
    else
    {
        for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
        {
            if((d == 2) && (j == 0))
            {

            }
            else if((d == 3) && (j == 1))
            {

            }
            else
            {
                Display(&(pachet_carti -> carti_mana[i][j]));
            }
        }

        printf("Suma : %d\n", pachet_carti -> suma_mana[i]);

        if(d == 1) printf("\n--------------------\n\n");

        if(!d) Alege(pachet_carti, i);
    }
}

void Sfarsit(Pachet_Carti * pachet_carti)
{
    int i = pachet_carti -> nr_jucatori;

    if(pachet_carti -> suma_mana[i] == 21)
    {
        pachet_carti -> blackjack[i] = 1;
        pachet_carti -> dealer_win = 1;
        pachet_carti -> bust[i] = 0;
    }
    else
    {
        pachet_carti -> dealer_win = 0;
        pachet_carti -> bust[i] = 0;
        pachet_carti -> dealer_win = 0;
    }

    while((pachet_carti -> bust[i] == 0) && (pachet_carti -> dealer_win == 0) && (pachet_carti -> blackjack[i] == 0) && (pachet_carti -> suma_mana[i] < 17))
    {
        if(pachet_carti -> suma_mana[i] == 21)
        {
            pachet_carti -> blackjack[i] = 1;
            pachet_carti -> dealer_win = 1;

            break;
        }
        else if(pachet_carti -> suma_mana[i] > 21)
        {
            if(pachet_carti -> nr_asi[i] > pachet_carti -> nr_asi1[i])
            {
                pachet_carti -> suma_mana[i] -= 10;
                pachet_carti -> nr_asi1[i]++;
            }
            else
            {
                pachet_carti -> bust[i] = 1;
                pachet_carti -> dealer_win = 0;
                pachet_carti -> blackjack[i] = 0;
            }
        }
        else
        {
            pachet_carti -> dealer_win = 0;
            pachet_carti -> blackjack[i] = 0;
            pachet_carti -> bust[i] = 0;
        }


        for(int j = 0; j < pachet_carti -> nr_jucatori; j++)
        {
            if(pachet_carti -> bust[j] || pachet_carti -> suma_mana[j] <= pachet_carti -> suma_mana[pachet_carti -> nr_jucatori])
            {
                pachet_carti -> dealer_win++;
            }
        }

        if(pachet_carti -> dealer_win > pachet_carti -> nr_jucatori / 2)
        {
            pachet_carti -> dealer_win = 1;
        }
        else
        {
            pachet_carti -> dealer_win = 0;
        }

        if((pachet_carti -> dealer_win == 0) && (pachet_carti -> bust[i] == 0) && (pachet_carti -> blackjack[i] == 0) && (pachet_carti -> suma_mana[i] < 17))
        {
            pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]] = pachet_carti -> carti[pachet_carti -> carti_impartite];
            pachet_carti -> suma_mana[i] += Transf(&(pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]]));

            if(Transf(&(pachet_carti -> carti_mana[i][pachet_carti -> nr_carte[i]])) == 11)
            {
                pachet_carti -> nr_asi[i]++;
            }

            if(pachet_carti -> suma_mana[i] == 21)
            {
                pachet_carti -> blackjack[i] = 1;

                pachet_carti -> dealer_win = 1;

                pachet_carti -> nr_carte[i]++;
                pachet_carti -> carti_impartite++;

                break;
            }

            if(pachet_carti -> suma_mana[i] > 21)
            {
                if(pachet_carti -> nr_asi[i] > pachet_carti -> nr_asi1[i])
                {
                    pachet_carti -> suma_mana[i] -= 10;
                    pachet_carti -> nr_asi1[i]++;
                }
                else
                {
                    pachet_carti -> bust[i] = 1;
                }
            }

            pachet_carti -> nr_carte[i]++;
            pachet_carti -> carti_impartite++;
        }
    }

    printf("Masa finala : \n\n");

    for(i = 0; i < pachet_carti -> nr_jucatori; i++)
    {
        printf("Jucator %d carti : \n", i + 1);

        for(int j = 0; j < pachet_carti -> nr_carte[i]; j++)
        {
            Display(&(pachet_carti -> carti_mana[i][j]));
        }

        printf("Suma : %d\n\n", pachet_carti -> suma_mana[i]);

        if(pachet_carti -> bust[i])
        {
            printf("Bust, ai pierdut %d\n\n", pachet_carti -> suma_bet[i]);
        }
        else if((pachet_carti -> blackjack[i]) && (!pachet_carti -> blackjack[pachet_carti -> nr_jucatori]))
        {
            printf("Blackjack, ai castigat %d\n\n", pachet_carti -> suma_bet[i] * 2);
        }
        else if((pachet_carti -> suma_mana[i] == pachet_carti -> suma_mana[pachet_carti -> nr_jucatori]))
        {
            printf("Libertate, egalitate, fraternitate, ai primit inapoi : %d\n\n", pachet_carti -> suma_bet[i]);
        }
        else if(pachet_carti -> suma_mana[i] > pachet_carti -> suma_mana[pachet_carti -> nr_jucatori])
        {
            printf("Ai castigat %d\n\n", pachet_carti -> suma_bet[i] * 2);
        }
        else if(pachet_carti -> bust[pachet_carti -> nr_jucatori])
        {
            printf("Ai castigat %d\n\n", pachet_carti -> suma_bet[i] * 2);
        }
        else
        {
            printf("Ai pierdut %d\n\n", pachet_carti -> suma_bet[i]);
        }

        printf("--------------------\n\n");
    }

    printf("Mana dealer:\n");

    for(int j = 0; j < pachet_carti -> nr_carte[pachet_carti -> nr_jucatori]; j++)
    {
        Display(&(pachet_carti -> carti_mana[pachet_carti -> nr_jucatori][j]));
    }

    printf("Suma : %d\n\n--------------------\n\n", pachet_carti -> suma_mana[pachet_carti -> nr_jucatori]);
}

int main(void)
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

        printf("Vrei sa continui sa te joci? [y/n]\n");
        
        while(1)
        {
            scanf("%s", replay);

            if(strlen(replay) == 1)
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
            else
            {
                printf("Raspunde cu 'y' sau 'n'!\n");
            }
        }

        if(replay[0] == 'n') break;
    }

    return 0;
}