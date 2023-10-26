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
    int dealer_win;
    int nr_asi[8];
    int nr_asi1[8];
    int carti_impartite;
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

void Alege(Pachet_Carti * pachet, int i);
void Hit(Pachet_Carti * pachet, int i, int d);
void Sfarsit(Pachet_Carti * pachet);

void Seteaza(Carte_De_Joc * carte, int x) // Seteaza valoarea si culoarea unei carti
{
    carte -> valoare = x % 13;
    carte -> culoare = x / 13;
}

int Valoare(Carte_De_Joc * carte) // Returneaza valoarea unei carti
{
    return carte -> valoare;
}

int Culoare(Carte_De_Joc * carte) // Returneaza culoarea unei carti
{
    return carte -> culoare;
}

void Display(Carte_De_Joc * carte) // Afiseaza cartea
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

            pachet -> suma_bet[i] = IntNum;
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

        pachet -> suma_mana[i] = Transf(&(pachet -> carti_mana[i][0])) + Transf(&(pachet -> carti_mana[i][1]));

        if(pachet -> suma_mana[i] > 21)
        {
            pachet -> suma_mana[i] -= 10;
            pachet -> nr_asi1[i]++;
        }
        else if(pachet -> suma_mana[i] == 21)
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

        printf("Bet : %d", pachet -> suma_bet[i]);

        Display(&(pachet -> carti_mana[i][1]));

        printf("Suma mana : %d", pachet -> suma_mana[i]);

        printf("\n\n--------------------\n\n");
    }

    printf("Carti Dealer : ");

    Display(&(pachet -> carti_mana[pachet -> nr_jucatori][0]));

    printf("\n\nApasa enter pentru a continua");

    getchar();

    system("clear");
}

void Mana(Pachet_Carti * pachet)
{
    for(int i = 0; i < pachet -> nr_jucatori; i++)
    {
        printf("Randul Jucatorului %d :\n", i + 1);

        Display(&(pachet -> carti_mana[i][0]));

        printf("Bet : %d", pachet -> suma_bet[i]);

        Display(&(pachet -> carti_mana[i][1]));

        printf("Suma : %d\n", pachet -> suma_mana[i]);

        if(pachet -> suma_mana[i] == 21)
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

    Sfarsit(pachet);
}

void Alege(Pachet_Carti * pachet, int i)
{
    if(pachet -> nr_carte[i] == 2)
    {
        printf("\nSelecteaza : hit / stay / double / split\n");
    }
    else
    {
        printf("\nSelecteaza : hit / stay\n");
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
        pachet -> suma_bet[i] *= 2;
        
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
        printf("\nIn lucru\n");

        Alege(pachet, i);
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

void Hit(Pachet_Carti * pachet, int i, int d)
{
    pachet -> carti_mana[i][pachet -> nr_carte[i]] = pachet -> carti[pachet -> carti_impartite];
    pachet -> suma_mana[i] += Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]]));

    if(Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]])) == 1)
    {
        pachet -> nr_asi[i] ++;
    }

    pachet -> carti_impartite++;
    pachet -> nr_carte[i]++;

    if(pachet -> suma_mana[i] == 21)
    {
        pachet -> blackjack[i] = 1;

        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            Display(&(pachet -> carti_mana[i][j]));
        }

        printf("Suma : %d\n\nBLACKJACK!\n\n", pachet -> suma_mana[i]);

        getchar();

        printf("--------------------\n\n");

        return;
    }
    else if(pachet -> suma_mana[i] > 21)
    {
        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
            {
                pachet -> suma_mana[i] -= 10;
                pachet -> nr_asi1[i]++;

                if(pachet -> suma_mana[i] == 21)
                {
                    pachet -> blackjack[i] = 1;

                    for(int j = 0; j < pachet -> nr_carte[i]; j++)
                    {
                        Display(&(pachet -> carti_mana[i][j]));
                    }

                    printf("Suma : %d\n\nBLACKJACK!\n\nEnter pentru next\n", pachet -> suma_mana[i]);

                    getchar();

                    printf("--------------------\n\n");

                    return;
                }

                for(int j = 0; j < pachet -> nr_carte[i]; j++)
                {
                    Display(&(pachet -> carti_mana[i][j]));
                }

                printf("Suma : %d\n", pachet -> suma_mana[i]);

                if(!d) Alege(pachet, i);

                if(d == 1) printf("\n--------------------\n\n");

                return;
            }
        }

        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            Display(&(pachet -> carti_mana[i][j]));
        }

        printf("Suma : %d", pachet -> suma_mana[i]);

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

            }
            else if((d == 3) && (j == 1))
            {

            }
            else
            {
                Display(&(pachet -> carti_mana[i][j]));
            }
        }

        printf("Suma : %d\n", pachet -> suma_mana[i]);

        if(d == 1) printf("\n--------------------\n\n");

        if(!d) Alege(pachet, i);
    }
}

void Sfarsit(Pachet_Carti * pachet)
{
    int i = pachet -> nr_jucatori;

    if(pachet -> suma_mana[i] == 21)
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
     (pachet -> blackjack[i] == 0) && (pachet -> suma_mana[i] < 17))
    {
        if(pachet -> suma_mana[i] == 21)
        {
            pachet -> blackjack[i] = 1;
            pachet -> dealer_win = 1;

            break;
        }
        else if(pachet -> suma_mana[i] > 21)
        {
            if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
            {
                pachet -> suma_mana[i] -= 10;
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
            if(pachet -> bust[j] || pachet -> suma_mana[j] <= pachet -> suma_mana[pachet -> nr_jucatori])
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
         (pachet -> blackjack[i] == 0) && (pachet -> suma_mana[i] < 17))
        {
            pachet -> carti_mana[i][pachet -> nr_carte[i]] = pachet -> carti[pachet -> carti_impartite];
            pachet -> suma_mana[i] += Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]]));

            if(Transf(&(pachet -> carti_mana[i][pachet -> nr_carte[i]])) == 11)
            {
                pachet -> nr_asi[i]++;
            }

            if(pachet -> suma_mana[i] == 21)
            {
                pachet -> blackjack[i] = 1;

                pachet -> dealer_win = 1;

                pachet -> nr_carte[i]++;
                pachet -> carti_impartite++;

                break;
            }

            if(pachet -> suma_mana[i] > 21)
            {
                if(pachet -> nr_asi[i] > pachet -> nr_asi1[i])
                {
                    pachet -> suma_mana[i] -= 10;
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

    printf("Masa finala : \n\n");

    for(i = 0; i < pachet -> nr_jucatori; i++)
    {
        printf("Jucator %d carti : \n", i + 1);

        for(int j = 0; j < pachet -> nr_carte[i]; j++)
        {
            Display(&(pachet -> carti_mana[i][j]));
        }

        printf("Suma : %d\n\n", pachet -> suma_mana[i]);

        if(pachet -> bust[i])
        {
            printf("Bust, ai pierdut %d\n\n", pachet -> suma_bet[i]);
        }
        else if((pachet -> blackjack[i]) && (!pachet -> blackjack[pachet -> nr_jucatori]))
        {
            printf("Blackjack, ai castigat %d\n\n", pachet -> suma_bet[i] * 2);
        }
        else if((pachet -> suma_mana[i] == pachet -> suma_mana[pachet -> nr_jucatori]))
        {
            printf("Libertate, egalitate, fraternitate, ai primit inapoi : %d\n\n", pachet -> suma_bet[i]);
        }
        else if(pachet -> suma_mana[i] > pachet -> suma_mana[pachet -> nr_jucatori])
        {
            printf("Ai castigat %d\n\n", pachet -> suma_bet[i] * 2);
        }
        else if(pachet -> bust[pachet -> nr_jucatori])
        {
            printf("Ai castigat %d\n\n", pachet -> suma_bet[i] * 2);
        }
        else
        {
            printf("Ai pierdut %d\n\n", pachet -> suma_bet[i]);
        }

        printf("--------------------\n\n");
    }

    printf("Mana dealer:\n");

    for(int j = 0; j < pachet -> nr_carte[pachet -> nr_jucatori]; j++)
    {
        Display(&(pachet -> carti_mana[pachet -> nr_jucatori][j]));
    }

    printf("Suma : %d\n\n", pachet -> suma_mana[pachet -> nr_jucatori]);
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