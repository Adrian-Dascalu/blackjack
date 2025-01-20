#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <stdint.h>

#define DECKS_CARDS_PLAYING 6
#define DECK_CARDS 52
#define DECK_CARDS_VALUE 13
#define DECK_CARDS_COLOR 4

#define MAX_PLAYERS 8
#define MAX_PLAYER_CARDS 10

const char* card_value[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "JACK", "QUEEN", "KING", "ACE"};

const char* card_color[4] = {"DIAMONDS", "HEARTS", "CLUBS", "SPADES"};

typedef struct
{
    uint64_t player_money;
    uint64_t player_bet;
} Player;

typedef struct
{
    uint8_t value;
    uint8_t color;
} Playing_Card;

typedef struct
{
    uint8_t aces_counter[MAX_PLAYER_CARDS * 2];
    uint8_t aces_1_counter[MAX_PLAYER_CARDS * 2];
    uint8_t player_cards_counter[MAX_PLAYER_CARDS * 2];
    uint64_t player_bet[MAX_PLAYERS * 2];
    int sum_player_hand[MAX_PLAYERS * 2];
    int lose[MAX_PLAYERS * 2];
    int bust[MAX_PLAYERS * 2];
    int blackjack[MAX_PLAYERS * 2];
    int dealer_win;
    int dealt_cards_counter;
    uint8_t players_counter;
    int player_split[MAX_PLAYERS];

    Playing_Card deck_cards[DECK_CARDS * DECKS_CARDS_PLAYING];

    Playing_Card hand_cards[MAX_PLAYERS * 2][MAX_PLAYER_CARDS];
} Playing_Deck;

void select(Playing_Deck * deck, int i);
void hit(Playing_Deck * deck, int i, int d);
void endGame(Playing_Deck * deck);
void playSplit(Playing_Deck * deck, int i, int d);
void results(Playing_Deck * deck);

void initCards(Playing_Card * card, int x)
{
    card -> value = x % DECK_CARDS_VALUE;
    card -> color = x / DECK_CARDS_VALUE % DECK_CARDS_COLOR;
}

int getCardValue(Playing_Card * card)
{
    return card -> value;
}

int getCardColor(Playing_Card * card)
{
    return card -> color;
}

void displayCard(Playing_Card * card)
{
    int width = 7;

    card_color[getCardColor(card)] == "HEARTS" ? width = 9 :
    card_color[getCardColor(card)] == "SPADES" ? width = 9 :
    card_color[getCardColor(card)] == "DIAMONDS" ? width = 7 :
    card_color[getCardColor(card)] == "CLUBS" ? width = 10 : 0;

    card_value[getCardValue(card)] == "10" ? width-- :
    card_value[getCardValue(card)] == "JACK" ? width-= 3 :
    card_value[getCardValue(card)] == "QUEEN" ? width-= 4 :
    card_value[getCardValue(card)] == "KING" ? width-= 3 :
    card_value[getCardValue(card)] == "ACE" ? width-= 2 : 0;

    printf("\n%s OF %s%*s", card_value[card -> value], card_color[card ->color], width, "");
}

int transformCardValue(Playing_Card * card)
{
    if(getCardValue(card) == 12)
    {
        return 11;
    }
    else if(getCardValue(card) > 8)
    {
        return 10;
    }
    else
    {
        return getCardValue(card) + 2;
    }
}

void generatePlayingDecks(Playing_Deck * deck)
{
    for(int i = 0; i < 52 * 6; i++)
    {
        initCards(&(deck -> deck_cards[i]), i);
    }
}

void shuffleCards(Playing_Deck * deck)
{
    int i, k;

    Playing_Card temp;

    srand(time(NULL));

    for(i = 0; i < 52 * 6; i++)
    {
        k = rand() % 52 * 6;

        temp = deck -> deck_cards[i];
        deck -> deck_cards[i] = deck -> deck_cards[k];
        deck -> deck_cards[k] = temp;
    }
}

void placeBet(Playing_Deck * deck, int n)
{
    int isChar= 0;
    int IntNum = 0;

    char str[100];

    for(int i = 0; i < n; i++)
    {
        printf("Bet Player %d : ", i + 1);

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
            
            printf("\nBet must be a positive number!!!\n\n");
        }
        else
        {
            IntNum = atoi(str);

            deck -> player_bet[i] = IntNum;
        }
    }

    deck -> players_counter = n;
}

void dealCards(Playing_Deck * deck)
{
    for(int i = 0; i <= deck -> players_counter; i++)
    {
        deck -> aces_counter[i] = 0;
        deck -> aces_1_counter[i] = 0;
        deck -> bust[i] = 0;
        deck -> blackjack[i] = 0;
        deck -> player_cards_counter[i] = 2;

        for(int k = 0; k < 2; k++)
        {
            deck -> hand_cards[i][k] = deck -> deck_cards[deck -> dealt_cards_counter];

            if(transformCardValue((&(deck -> hand_cards[i][k]))) == 11)
            {
                deck -> aces_counter[i]++;
            }

            deck -> dealt_cards_counter++;
        }

        deck -> sum_player_hand[i] = transformCardValue(&(deck -> hand_cards[i][0])) + transformCardValue(&(deck -> hand_cards[i][1]));

        if(deck -> sum_player_hand[i] > 21)
        {
            deck -> sum_player_hand[i] -= 10;
            deck -> aces_1_counter[i]++;
        }
        else if(deck -> sum_player_hand[i] == 21)
        {
            deck -> blackjack[i] = 1;
        }
    }
}

void showTable(Playing_Deck * deck)
{
    for(int i = 0; i < 5; i++)
    {
        printf("\n");
    }

    for(int i = 0; i < deck -> players_counter; i++)
    {
        printf("Player %d Cards : \n", i + 1);

        displayCard(&(deck -> hand_cards[i][0]));

        printf("Bet : %d", deck -> player_bet[i]);

        displayCard(&(deck -> hand_cards[i][1]));

        printf("Sum Hand : %d", deck -> sum_player_hand[i]);

        printf("\n\n--------------------\n\n");
    }

    printf("Dealer Cards : ");

    displayCard(&(deck -> hand_cards[deck -> players_counter][0]));

    printf("\n\nPress 'Enter' to continue");

    while(getchar() != '\n');

    for(int i = 0; i < 5; i++)
    {
        printf("\n");
    }
}

void playerHand(Playing_Deck * deck, int i)
{
    displayCard(&(deck -> hand_cards[i][0]));

    printf("Bet : %d", deck -> player_bet[i]);

    displayCard(&(deck -> hand_cards[i][1]));

    printf("Sum Hand : %d", deck -> sum_player_hand[i]);
}

void playerTurn(Playing_Deck * deck)
{
    for(int i = 0; i < deck -> players_counter; i++)
    {
        printf("Player %d Turn :\n", i + 1);

        displayCard(&(deck -> hand_cards[i][0]));

        printf("Bet : %d", deck -> player_bet[i]);

        displayCard(&(deck -> hand_cards[i][1]));

        printf("Sum : %d\n", deck -> sum_player_hand[i]);

        if(deck -> sum_player_hand[i] == 21)
        {
            deck -> blackjack[i] = 1;

            printf("\nBLACKJACK!\n\n");
        }
        else
        {
            select(deck, i);
            
            getchar();
        }

        printf("End of Player %d Turn\n", i + 1);
        printf("\nPress 'Enter' to continue");
        
        while(getchar() != '\n');

        if(i < deck -> players_counter - 1)
        {
            printf("\n------------------------------\n\n");
        }
    }
    

    for(int i = 0; i < 5; i++)
    {
        printf("\n");
    }

    endGame(deck);
}

void select(Playing_Deck * deck, int i)
{
    if(deck -> player_cards_counter[i] == 2)
    {
        printf("\nSelect : [hit / stay / double / split] ");
    }
    else
    {
        printf("\nSelect : [hit / stay] ");
    }

    char tura[10];

    scanf("%s", tura);

    if(strcmp(tura, "hit") == 0)
    {
        hit(deck, i, 0);
    }
    else if(strcmp(tura, "stay") == 0)
    {
        printf("\n");
    }
    else if((strcmp(tura, "double") == 0) && (deck -> player_cards_counter[i] == 2))
    {
        deck -> player_bet[i] *= 2;
        
        hit(deck, i, 1);
    }
    else if((strcmp(tura, "double") == 0))
    {
        printf("\nYou can't double!\n");
        
        select(deck, i);
    }
    else if((strcmp(tura, "split") == 0) && (deck -> player_cards_counter[i] == 2) &&
     (transformCardValue(&(deck -> hand_cards[i][0])) == transformCardValue(&(deck -> hand_cards[i][1]))))
    {
        deck -> player_split[i] = 1;

        deck -> hand_cards[i + deck -> players_counter + 1][0] = deck -> hand_cards[i][1];
        
        deck -> hand_cards[i][1] = deck -> deck_cards[deck -> dealt_cards_counter++];
        deck -> hand_cards[i + deck -> players_counter + 1][1] = deck -> deck_cards[deck -> dealt_cards_counter++];

        deck -> sum_player_hand[i] = transformCardValue(&(deck -> hand_cards[i][0])) + transformCardValue(&(deck -> hand_cards[i][1]));
        deck -> sum_player_hand[i + deck -> players_counter + 1] =
         transformCardValue(&(deck -> hand_cards[i + deck -> players_counter + 1][0])) + transformCardValue(&(deck -> hand_cards[i + deck -> players_counter + 1][1]));

        deck -> player_bet[i] /= 2;
        deck -> player_bet[i + deck -> players_counter + 1] = deck -> player_bet[i];

        printf("\n");

        playSplit(deck, i, 0);
        playSplit(deck, i + deck -> players_counter + 1, 1);
    }
    else if((strcmp(tura, "split") == 0))
    {
        printf("\nYou can't split!\n");

        select(deck, i);
    }
    else
    {
        printf("\nWhat do You want to do?\n");

        select(deck, i);
    }
}

void splitTurn(Playing_Deck * deck, int i)
{
    if(deck -> player_cards_counter[i] == 2)
    {
        printf("\n\nSelect : [hit / stay / double] ");
    }
    else
    {
        printf("\n\nSelect : [hit / stay] ");
    }

    char tura[10];

    scanf("%s", tura);

    if(i < deck -> players_counter - 1)
    {
        printf("\n------------------------------\n");
    }

    if(strcmp(tura, "hit") == 0)
    {
        hit(deck, i, 0);
    }
    else if(strcmp(tura, "stay") == 0)
    {
        printf("\n");
    }
    else if((strcmp(tura, "double") == 0) && (deck -> player_cards_counter[i] == 2))
    {
        deck -> player_bet[i] *= 2;
        
        hit(deck, i, 1);
    }
    else if((strcmp(tura, "double") == 0))
    {
        printf("\nYou can't double!\n");
        
        splitTurn(deck, i);
    }
    else
    {
        printf("\nWhat do You want to do?\n");

        splitTurn(deck, i);
    }
}

void playSplit(Playing_Deck * deck, int i, int d)
{
    if(transformCardValue(&(deck -> hand_cards[i][0])) == 11)
    {
        deck -> aces_counter[i] = 1;
    }

    if(transformCardValue(&(deck -> hand_cards[i][1])) == 11)
    {
        deck -> aces_counter[i] = 1;
    }

    if(deck -> sum_player_hand[i] == 21)
    {
        deck -> blackjack[i] = 1;
    }
    else if(deck -> sum_player_hand[i] > 21)
    {
        deck -> sum_player_hand[i] -= 10;
        deck -> aces_1_counter[i]++;
    }

    if(i > 6)
    {
        printf("Hand 2:\n");
    }
    else
    {
        printf("Hand 1:\n");
    }

    displayCard(&(deck -> hand_cards[i][0]));

    printf("Bet : %d\n", deck -> player_bet[i]);
        
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
                printf("Respond cu 'y' or 'n'!\n");
            }
        }
        else
        {
            printf("Respond with 'y' or 'n'!\n");
        }
    }

    char temp[50];

    if(r_bet[0] == 'y')
    {
        printf("By how much the Bet should be increased? ");

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
                printf("Bet must be a positive number : \n");
            }
            else
            {
                int IntNum = atoi(temp);

                deck -> player_bet[i] += IntNum;
            
                break;
            }
        }
    }   
    
    printf("\nCards :\n");

    playerHand(deck, i);
    
    if(deck -> sum_player_hand[i] == 21)
    {
        printf("\n\nBLACKJACK!\n\n");

        deck -> blackjack[i] = 1;

        return;
    }

    deck -> player_cards_counter[i] = 2;

    splitTurn(deck, i);
}

void hit(Playing_Deck * deck, int i, int d)
{
    deck -> hand_cards[i][deck -> player_cards_counter[i]] = deck -> deck_cards[deck -> dealt_cards_counter];
    deck -> sum_player_hand[i] += transformCardValue(&(deck -> hand_cards[i][deck -> player_cards_counter[i]]));

    if(transformCardValue(&(deck -> hand_cards[i][deck -> player_cards_counter[i]])) == 1)
    {
        deck -> aces_counter[i] ++;
    }

    deck -> dealt_cards_counter++;
    deck -> player_cards_counter[i]++;

    if(deck -> sum_player_hand[i] == 21)
    {
        deck -> blackjack[i] = 1;

        for(int j = 0; j < deck -> player_cards_counter[i]; j++)
        {
            displayCard(&(deck -> hand_cards[i][j]));
        }

        printf("Sum : %d\n\nBLACKJACK!\n\n", deck -> sum_player_hand[i]);

        return;
    }
    else if(deck -> sum_player_hand[i] > 21)
    {
        for(int j = 0; j < deck -> player_cards_counter[i]; j++)
        {
            if(deck -> aces_counter[i] > deck -> aces_1_counter[i])
            {
                deck -> sum_player_hand[i] -= 10;
                deck -> aces_1_counter[i]++;

                if(deck -> sum_player_hand[i] == 21)
                {
                    deck -> blackjack[i] = 1;

                    for(int j = 0; j < deck -> player_cards_counter[i]; j++)
                    {
                        displayCard(&(deck -> hand_cards[i][j]));
                    }

                    printf("Sum : %d\n\nBLACKJACK!\n\nPress 'Enter' to continue\n", deck -> sum_player_hand[i]);

                    getchar();

                    return;
                }

                for(int j = 0; j < deck -> player_cards_counter[i]; j++)
                {
                    displayCard(&(deck -> hand_cards[i][j]));
                }

                printf("Sum : %d\n", deck -> sum_player_hand[i]);

                if(!d) select(deck, i);

                return;
            }
        }

        for(int j = 0; j < deck -> player_cards_counter[i]; j++)
        {
            displayCard(&(deck -> hand_cards[i][j]));
        }

        printf("Sum : %d", deck -> sum_player_hand[i]);

        deck -> bust[i] = 1;

        printf("\n\nBust! You Lost!\n\n");

        return;
    }
    else
    {
        for(int j = 0; j < deck -> player_cards_counter[i]; j++)
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
                displayCard(&(deck -> hand_cards[i][j]));
            }
        }

        printf("Sum : %d\n", deck -> sum_player_hand[i]);

        if(!d) select(deck, i);
    }
}

void endGame(Playing_Deck * deck)
{
    int i = deck -> players_counter;

    if(deck -> sum_player_hand[i] == 21)
    {
        deck -> blackjack[i] = 1;
        deck -> dealer_win = 1;
        deck -> bust[i] = 0;
    }
    else
    {
        deck -> dealer_win = 0;
        deck -> bust[i] = 0;
        deck -> dealer_win = 0;
    }

    while((deck -> bust[i] == 0) && (deck -> dealer_win == 0) &&
     (deck -> blackjack[i] == 0) && (deck -> sum_player_hand[i] < 17))
    {
        if(deck -> sum_player_hand[i] == 21)
        {
            deck -> blackjack[i] = 1;
            deck -> dealer_win = 1;

            break;
        }
        else if(deck -> sum_player_hand[i] > 21) //sus
        {
            if(deck -> aces_counter[i] > deck -> aces_1_counter[i])
            {
                deck -> sum_player_hand[i] -= 10;
                deck -> aces_1_counter[i]++;
            }
            else
            {
                deck -> bust[i] = 1;
                deck -> dealer_win = 0;
                deck -> blackjack[i] = 0;
            }
        }
        else
        {
            deck -> dealer_win = 0;
            deck -> blackjack[i] = 0;
            deck -> bust[i] = 0;
        }


        for(int j = 0; j < deck -> players_counter; j++)
        {
            if(deck -> bust[j] || deck -> sum_player_hand[j] <= deck -> sum_player_hand[deck -> players_counter])
            {
                deck -> dealer_win++;
            }
        }

        if(deck -> dealer_win > deck -> players_counter / 2)
        {
            deck -> dealer_win = 1;
        }
        else
        {
            deck -> dealer_win = 0;
        }

        if((deck -> dealer_win == 0) && (deck -> bust[i] == 0) &&
         (deck -> blackjack[i] == 0) && (deck -> sum_player_hand[i] < 17))
        {
            deck -> hand_cards[i][deck -> player_cards_counter[i]] = deck -> deck_cards[deck -> dealt_cards_counter];
            deck -> sum_player_hand[i] += transformCardValue(&(deck -> hand_cards[i][deck -> player_cards_counter[i]]));

            if(transformCardValue(&(deck -> hand_cards[i][deck -> player_cards_counter[i]])) == 11)
            {
                deck -> aces_counter[i]++;
            }

            if(deck -> sum_player_hand[i] == 21)
            {
                deck -> blackjack[i] = 1;

                deck -> dealer_win = 1;

                deck -> player_cards_counter[i]++;
                deck -> dealt_cards_counter++;

                break;
            }

            if(deck -> sum_player_hand[i] > 21)
            {
                if(deck -> aces_counter[i] > deck -> aces_1_counter[i])
                {
                    deck -> sum_player_hand[i] -= 10;
                    deck -> aces_1_counter[i]++;
                }
                else
                {
                    deck -> bust[i] = 1;
                }
            }

            deck -> player_cards_counter[i]++;
            deck -> dealt_cards_counter++;
        }
    }

    results(deck);
}

void results(Playing_Deck * deck)
{
    printf("Results : \n\n");

    for(int i = 0; i < deck -> players_counter; i++)
    {
        if(deck -> player_split[i] == 1)
        {
            printf("Player %d Hand 1 : \n", i + 1);

            for(int j = 0; j < deck -> player_cards_counter[i]; j++)
            {
                displayCard(&(deck -> hand_cards[i][j]));
            }

            printf("Sum : %d\n\n", deck -> sum_player_hand[i]);

            if(deck -> bust[i])
            {
                printf("Bust! You Lost %d\n\n", deck -> player_bet[i]);
            }
            else if((deck -> blackjack[i]) && (!deck -> blackjack[deck -> players_counter]))
            {
                printf("BLACKJACK! You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else if((deck -> sum_player_hand[i] == deck -> sum_player_hand[deck -> players_counter]))
            {
                printf("Draw! You received back : %d\n\n", deck -> player_bet[i]);
            }
            else if(deck -> sum_player_hand[i] > deck -> sum_player_hand[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else if(deck -> bust[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else
            {
                printf("You Lost %d\n\n", deck -> player_bet[i]);
            }

            printf("--------------------\n\n");

            printf("Player %d Hand 2 : \n", i + 1);

            for(int j = 0; j < deck -> player_cards_counter[i + deck -> players_counter + 1]; j++)
            {
                displayCard(&(deck -> hand_cards[i + deck -> players_counter + 1][j]));
            }

            printf("Sum : %d\n\n", deck -> sum_player_hand[i + deck -> players_counter + 1]);

            if(deck -> bust[i + deck -> players_counter + 1])
            {
                printf("Bust! You Lost %d\n\n", deck -> player_bet[i + deck -> players_counter + 1]);
            }
            else if((deck -> blackjack[i + deck -> players_counter + 1]) && (!deck -> blackjack[deck -> players_counter]))
            {
                printf("BLACKJACK! You Won %d\n\n", deck -> player_bet[i + deck -> players_counter + 1] * 2);
            }
            else if((deck -> sum_player_hand[i + deck -> players_counter + 1] == deck -> sum_player_hand[deck -> players_counter]))
            {
                printf("Draw! You received back : %d\n\n", deck -> player_bet[i + deck -> players_counter + 1]);
            }
            else if(deck -> sum_player_hand[i + deck -> players_counter + 1] > deck -> sum_player_hand[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i + deck -> players_counter + 1] * 2);
            }
            else if(deck -> bust[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i + deck -> players_counter + 1] * 2);
            }
            else
            {
                printf("You Lost %d\n\n", deck -> player_bet[i + deck -> players_counter + 1]);
            }

            printf("--------------------\n\n");
        }
        else
        {
            printf("Player %d : \n", i + 1);

            for(int j = 0; j < deck -> player_cards_counter[i]; j++)
            {
                displayCard(&(deck -> hand_cards[i][j]));
            }

            printf("Sum : %d\n\n", deck -> sum_player_hand[i]);

            if(deck -> bust[i])
            {
                printf("Bust! You Lost %d\n\n", deck -> player_bet[i]);
            }
            else if((deck -> blackjack[i]) && (!deck -> blackjack[deck -> players_counter]))
            {
                printf("BLACKJACK! You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else if((deck -> sum_player_hand[i] == deck -> sum_player_hand[deck -> players_counter]))
            {
                printf("Draw! You received back : %d\n\n", deck -> player_bet[i]);
            }
            else if(deck -> sum_player_hand[i] > deck -> sum_player_hand[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else if(deck -> bust[deck -> players_counter])
            {
                printf("You Won %d\n\n", deck -> player_bet[i] * 2);
            }
            else
            {
                printf("You Lost %d\n\n", deck -> player_bet[i]);
            }

            printf("--------------------\n\n");
        }
    }

    printf("Dealer Hand :\n");

    for(int j = 0; j < deck -> player_cards_counter[deck -> players_counter]; j++)
    {
        displayCard(&(deck -> hand_cards[deck -> players_counter][j]));
    }

    printf("Sum : %d\n\n", deck -> sum_player_hand[deck -> players_counter]);
}

void playersNumber(Playing_Deck * deck)
{
    int n;

    char str[100];

    while(1)
    {
        printf("Number of Players : ");
        
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
            printf("\nNumber of Players must be between 1 and 7!!!\n\n");
        }
    }

    deck -> players_counter = n;
}

int getPlayersNumber(Playing_Deck * deck)
{
    return deck -> players_counter;
}

void playAgain(Playing_Deck * deck)
{
    printf("Do You want to play again? [y/n] ");
        
    char replay[100];

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
                printf("Respond with 'y' or 'n'! [y/n] ");
            }
        }
        else
        {
            printf("Respond with 'y' or 'n'! [y/n] ");
        }
    }

    if(replay[0] == 'n')
    {
        exit(0);
    }
    else
    {
        for(int i = 0; i < 5; i++)
        {
            printf("\n");
        }
    }
}

int main()
{
    while(1)
    {
        Player player;

        memset(&player, 0, sizeof(player));

        Playing_Deck bicycle;

        //Playing_Player player;

        memset(&bicycle, 0, sizeof(bicycle));
        
        playersNumber(&bicycle);

        generatePlayingDecks(&bicycle);

        shuffleCards(&bicycle);

        placeBet(&bicycle,  getPlayersNumber(&bicycle));

        dealCards(&bicycle);

        showTable(&bicycle);

        playerTurn(&bicycle);

        playAgain(&bicycle);
    }

    return 0;
}