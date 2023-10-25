#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <random>
#include <time.h>
#include <cstdlib>
#include <string.h>
#include <sstream>

using namespace std;

const char* valoare_carte[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

const char* culoare_carte[4] = {"ROMB", "INIMA ROSIE", "TREFLA", "INIMA NEAGRA"};

class Pachet_Carti
{
    public:
        class Carte_De_Joc
        {
            private:
                int valoare;
                int culoare;

            public:
                void Seteaza(int x)
                {
                    valoare = x % 13;
                    culoare = x / 13;
                }

                int Valoare(void)
                {
                    return valoare;
                }

                int Culoare(void)
                {
                    return culoare;
                }

                void Display(void);

                int transf(void);
        };

    private:
        Carte_De_Joc carti[52];
        
    public:
        void Creeaza_Pachet(void)
        {
            for(int i = 0; i < 52; i++) carti[i].Seteaza(i);
        }

        void Amesteca(void)
        {
            int i, k;
        
            Carte_De_Joc temp;
    
            srand(time(NULL));
            
            for(i = 0; i < 52; i++)
            {
                k = rand() % 52;
        
                temp = carti[i];
                carti[i] = carti[k];
                carti[k] = temp;
            }
        }

        class JucatorSplit
        {
            private:
                Carte_De_Joc carte_mana[2][10];

            public:
                JucatorSplit(Carte_De_Joc carte_manaa, int carti_impartite, Carte_De_Joc carti[])
                {
                    carte_mana[0][0] = carte_manaa;
                    carte_mana[0][0].Carte_De_Joc::Display();
                    carte_mana[0][1] = carti[carti_impartite];
                    carte_mana[0][1].Carte_De_Joc::Display();
                }
                JucatorSplit(Carte_De_Joc carte_manaa, int carti_impartite, Carte_De_Joc carti[], int j)
                {
                    carte_mana[1][0] = carte_manaa;
                    carte_mana[1][0].Carte_De_Joc::Display();
                    carte_mana[1][1] = carti[carti_impartite];
                    carte_mana[1][1].Carte_De_Joc::Display();
                }
                ~JucatorSplit(){}

                void hitsplit(Carte_De_Joc carti_manaaa, int i);
                void show();
        };

        void Bet(int n);

        void impartire()
        {
            for(int i = 0; i <= nr_jucatori; i++)
            {
                nr_asi[i] = 0;
                nr_asi1[i] = 0;
                bust[i] = 0;
                blackjack[i] = 0;
                nr_carte[i] = 2;   

                for(int k = 0; k < 2; k++)
                {
                    carti_mana[i][k] = carti[carti_impartite];

                    if(carti_mana[i][k].transf() == 11)
                    {
                        nr_asi[i]++;
                    }
                    
                    carti_impartite++;
                }

                suma_mana[i] = carti_mana[i][0].transf() + carti_mana[i][1].transf();
                
                if(suma_mana[i] > 21)
                {
                    suma_mana[i] -= 10;
                    nr_asi1[i]++;
                }
                else if(suma_mana[i] == 21)
                {
                    blackjack[i] = 1;
                }
            }
        }

        void afisare_masa()
        {
            for(int i = 0; i < nr_jucatori; i++)
            {
                cout << "Jucator " << i + 1 << " carti:\n";

                carti_mana[i][0].Display();

                cout << "Bet : " << suma_bet[i];

                carti_mana[i][1].Display();

                cout << "Suma : " << suma_mana[i];

                cout << endl << endl;

                cout << "--------------------\n\n";
            }

            cout << "Carti Dealer :\n";
            carti_mana[nr_jucatori][0].Display();
            cout << endl << endl << "Apasa enter pentru next";
        }

        void alege(int i)
        {
            if(nr_carte[i] == 2)
            {
                cout << "\nhit / stay / double / split\n";
            }
            else
            {
                cout << "\nhit / stay\n";
            }

            char tura[10];

            cin >> tura;

            getchar();

            cout << "\n------------------------------\n";

            if(strcmp(tura, "hit") == 0)
            {
                hit(i, 0);
            }
            else if(strcmp(tura, "stay") == 0)
            {
                cout << endl;
            }
            else if((strcmp(tura, "double") == 0) && (nr_carte[i] == 2))
            {
                suma_bet[i] *= 2;
                hit(i, 1);

                return;
            }
            else if((strcmp(tura, "split") == 0) && (nr_carte[i] == 2) && (carti_mana[i][0].transf() == carti_mana[i][1].transf()))
            {
                if(carti_mana[i][0].transf() == carti_mana[i][1].transf())
                {
                    nr_asi1[i] = 0;
                    cout << "\nprima mana : \n";
                    suma_mana[i] = carti_mana[i][0].transf();
                    carti_split[i][0][0] = carti_mana[i][0];
                    JucatorSplit jucator1(carti_mana[i][0], carti_impartite, carti, 1);
                    carti_impartite++;

                    nr_asi1[1] = 0;
                    cout << "\n\na doua mana : \n";
                    suma_mana[i] = carti_mana[i][1].transf();
                    carti_split[i][1][0] = carti_mana[i][1];
                    JucatorSplit jucator2(carti_mana[i][1], carti_impartite, carti);
                    carti_impartite++;
                    cout << endl << endl;
                }
                else
                {
                    cout << "nu poti da split";
                    
                    alege(i);
                }
            }
            else
            {
                cout << "\nce vrei sa faci ma?\n";

                alege(i);
            }
        }

        void hit(int i, int d)  
        {
            carti_mana[i][nr_carte[i]] = carti[carti_impartite];
            suma_mana[i] += carti_mana[i][nr_carte[i]].transf();

            if(carti_mana[i][nr_carte[i]].transf() == 11)
            {
                nr_asi[i]++;
            }

            carti_impartite++;
            nr_carte[i]++;

            if(suma_mana[i] == 21)
            {
                blackjack[i] = 1;

                for(int j = 0; j < nr_carte[i]; j++)
                {
                    carti_mana[i][j].Display();
                }

                cout << "Suma : 21\n\nBLACKJACK!\n" << endl << "Enter pentru next\n";

                getchar();

                cout << "--------------------\n\n";

                return;
            }
            else if(suma_mana[i] > 21)
            {   
                for(int j = 0; j < nr_carte[i]; j++)
                {
                    if(nr_asi[i] > nr_asi1[i])
                    {
                        suma_mana[i] -= 10;
                        nr_asi1[i]++;

                        if(suma_mana[i] == 21)
                        {
                            blackjack[i] = 1;

                            for(int j = 0; j < nr_carte[i]; j++)
                            {
                                carti_mana[i][j].Display();
                            }

                            cout << "Suma : 21\n\nBLACKJACK!\n" << endl << "Enter pentru next\n";

                            getchar();

                            cout << "--------------------\n\n";

                            return;
                        }

                        for(int j = 0; j < nr_carte[i]; j++)
                        {
                            carti_mana[i][j].Display();
                        }

                        cout << "Suma : " << suma_mana[i] << endl;
                        
                        if(!d) alege(i);

                        if(d == 1) cout << "\n--------------------\n\n";

                        return;
                    }
                }

                for(int j = 0; j < nr_carte[i]; j++)
                {
                    carti_mana[i][j].Display();
                }

                cout << "Suma : " << suma_mana[i];
                bust[i] = 1;

                cout << "\n\nAi pierdut, ghinion\n\nEnter pentru next";

                getchar();

                cout << "\n--------------------\n\n";

                return;
            }
            else
            {
                for(int j = 0; j < nr_carte[i]; j++)
                {
                    if((d == 2) && (j == 0))
                    {}
                    else if((d == 3) && (j == 1))
                    {}
                    else
                    carti_mana[i][j].Display();
                }

                cout << "Suma : " << suma_mana[i] << endl;
            }

            if(d == 1) cout << "\n--------------------\n\n";

            if(!d) alege(i);
        }

        void mana()
        {
            for(int i = 0; i < nr_jucatori; i++)
            {
                cout << "Randul jucatorului " << i + 1 << endl;

                carti_mana[i][0].Display();

                cout << "Bet : " << suma_bet[i];

                carti_mana[i][1].Display();
                
                cout << "Suma : " << suma_mana[i] << endl;

                if(suma_mana[i] == 21)
                {
                    blackjack[i] = 1;

                    cout << "\nBLACKJACK DIN PRIMA!\n\nEnter pentru next\n";
                    
                    getchar();

                    cout << "--------------------\n\n";
                }
                else
                {
                    alege(i);
                }
            }

            sfarsit();
        }

        void sfarsit()
        {
            int i = nr_jucatori;

            if(suma_mana[i] == 21)
            {
                blackjack[i] = 1;
            }

            while((bust[i] == 0) && (dealer_win == 0) && (blackjack[i] == 0))
            {
                if(suma_mana[i] == 21)
                {
                    blackjack[i] = 1;
                }
//
                for(int j = 0; j < nr_jucatori; j++)
                {
                    if(bust[j])
                    {
                        dealer_win++;
                    }
                }

                if(dealer_win >= nr_jucatori / 2)
                {
                    dealer_win = 1;
                }
                else
                {
                    dealer_win = 0;
                }
//
                for(int j = 0; j < nr_jucatori; j++)
                {
                    if(suma_mana[i] > suma_mana[j])
                    {
                        dealer_win = 1;

                        break;
                    }
                }
//
                if((dealer_win == 0) && (bust[i] == 0) && (blackjack[i] == 0) && (suma_mana[i] < 17) || (suma_mana[i] < 11))
                {
                    carti_mana[i][nr_carte[i]] = carti[carti_impartite];
                    suma_mana[i] += carti_mana[i][nr_carte[i]].transf();

                    if(carti_mana[i][nr_carte[i]].transf() == 11)
                    {
                        nr_asi[i] += 1;
                    }

                    if(suma_mana[i] == 21)
                    {
                        blackjack[i] = 1;
                    }

                    if(suma_mana[i] > 21)
                    {
                        if(nr_asi[i] > nr_asi1[i])
                        {
                            suma_mana[i] -= 10;
                            nr_asi1[i]++;
                        }
                        else
                        {
                            bust[i] = 1;
                        }
                    }

                    nr_carte[i]++;
                    carti_impartite++;
                }
            }

            cout << "Masa finala:\n\n";

            for(i = 0; i < nr_jucatori; i++)
            {
                cout << "Jucator : " << i + 1 << endl;

                for(int j = 0; j < nr_carte[i]; j++)
                {
                    carti_mana[i][j].Display();
                }                  
                
                cout << "Suma : " << suma_mana[i] << endl << endl;

                if(bust[i])
                {
                    cout << "Bust, ai pierdut : " << suma_bet[i] << endl << endl;
                }
                else if((blackjack[i]) && ((suma_mana[i] > suma_mana[nr_jucatori]) || (suma_mana[i] < suma_mana[nr_jucatori])))
                {
                    cout << "Blackjack, ai castigat : " << suma_bet[i] * 2 << endl << endl;
                }
                else if((blackjack[i]) && (suma_mana[i] == suma_mana[nr_jucatori]))
                {
                    cout << "Libertate, egalitate, fraternitate, ai primit inapoi : " << suma_bet[i] << endl << endl;
                }
                else if(suma_mana[i] > suma_mana[nr_jucatori])
                {
                    cout << "Ai castigat : " << suma_bet[i] * 2 << endl << endl;
                }
                else if(bust[nr_jucatori])
                {
                    cout << "Ai castigat : " << suma_bet[i] * 2 << endl << endl;
                }
                else
                {
                    cout << "Ai pierdut : " << suma_bet[i] << endl << endl;
                }

                cout << "--------------------\n\n";
            }

            cout << "Mana dealer :\n";
            
            for(int j = 0; j < nr_carte[nr_jucatori]; j++)
            {
                carti_mana[nr_jucatori][j].Display();
            }

            cout << "Suma : " << suma_mana[i] << "\n\n--------------------\n\n";
        }

        int dealer_win = 0;
        int nr_asi[8];
        int nr_asi1[8];
        int carti_impartite = 0;
        int nr_carte[8];
        int blackjack[8];
        int bust[8];
        int lose[8];
        int valori_c[13];
        double suma_bet[8];
        int nr_jucatori;
        int suma_mana[8];

        Carte_De_Joc carti_split[8][10][2];
        Carte_De_Joc carti_mana[8][10];

};

int Pachet_Carti::Carte_De_Joc::transf(void)
{
    if((Valoare() > 8) && (Valoare() < 12)) return 10;
    else if(Valoare() == 12) return 11;
    else return Valoare() + 2;
}

void Pachet_Carti::Carte_De_Joc::Display(void)
{
    int width = 7;

    culoare_carte[Culoare()] == "INIMA ROSIE" ? width = 5
    : culoare_carte[Culoare()] == "INIMA NEAGRA" ? width = 4
    : culoare_carte[Culoare()] == "ROMB" ? width = 12
    : culoare_carte[Culoare()] == "TREFLA" ? width = 10 : 0;
    valoare_carte[Valoare()] == "10" ? width-- : 0;
    
    cout << endl; cout << valoare_carte[Valoare()] << " DE " << culoare_carte[Culoare()] << setw(width) << " ";
    //delete ' << " "' for align with ":";
}

void Pachet_Carti::Bet(int n)
{
    string user_input;

    for(int i = 0; i < n; i++)
    {
        cout << "Bet jucator " << i + 1 << " : ";
        while(1)
        {
            getline(cin, user_input);
            stringstream convert(user_input);
            if(convert >> suma_bet[i] &&!(convert >> user_input))
            {
                break;
            }
            else
            {
            cin.clear();
            cout << "Bet-ul trebuie sa fie numar real, reintrodu un numar : ";
            }
        }
    };

    nr_jucatori = n;
}

void Pachet_Carti::JucatorSplit::hitsplit(Carte_De_Joc carti_manaaa, int i)
{
    //Pachet_Carti jucator1;

    //jucator1.hit(i, 0);

    //carte_mana[0][0] = 
}

void Pachet_Carti::JucatorSplit::show()
{
    carte_mana[0][0].Pachet_Carti::Carte_De_Joc::Display();
}

int main (void)
{
    while(1)
    {
        int n;
        string user_input;
    
        cout << "Numar jucatori : ";
        while(1)
        {
            getline(cin, user_input);
            stringstream convert(user_input);
            if(convert >> n &&!(convert >> user_input) && (n > 0) && (n < 8))
            {
                break;
            }
            else
            {
            cin.clear();
            cout << "Numarul maxim de jucatori este intre 1 si 7, reintrodu un numar:\n";
            }
        }

        cout << endl;

        Pachet_Carti::Carte_De_Joc carte;

        Pachet_Carti blackjack;

        blackjack.Creeaza_Pachet();

    // system("clear");

        blackjack.Amesteca();

    // system("clear");

        blackjack.Bet(n);

    // system("clear");

        blackjack.impartire();

        system("clear");

        blackjack.afisare_masa();
        
        getchar();

        system("clear");

        blackjack.mana();

        char replay;

        cout << "vrei sa continui joaca? y/n\n";
        while(1)
        {
            cin >> replay;
            getchar();
            if(replay == 'n')
            {   
                break;
            }
            else if(replay == 'y')
                break;
            else
                cout << "raspunde!\n";
        }

        if(replay == 'n') break;
    }
}

//#include <unistd.h>
//write(1, "hello", 5);
