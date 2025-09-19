#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct trump {
    int order;
    char shape[4]; 
    char number[3]; 
};

void make_card(struct trump m_card[]);
void display_card(struct trump m_card[]);
void shuffle_card(struct trump m_card[]);

int main(void)
{
    struct trump card[52];

    make_card(card);
    shuffle_card(card);
    display_card(card);

    return 0;
}

void make_card(struct trump m_card[])
{
    int i, j;
    char shape[4][4] = {"♠", "◆", "♥", "♣"};

    for (i = 0; i < 4; i++) {
        for (j = i * 13; j < i * 13 + 13; j++) {
            m_card[j].order = i;
            strcpy(m_card[j].shape, shape[i]);

            int num = j % 13 + 1;
            switch (num) {
                case 1:
                    strcpy(m_card[j].number, "A");
                    break;
                case 11:
                    strcpy(m_card[j].number, "J");
                    break;
                case 12:
                    strcpy(m_card[j].number, "Q");
                    break;
                case 13:
                    strcpy(m_card[j].number, "K");
                    break;
                default:
                    sprintf(m_card[j].number, "%d", num);
                    break;
            }
        }
    }
}

void display_card(struct trump m_card[])
{
    int i;
    for (i = 0; i < 52; i++) {
        printf("%s%-2s ", m_card[i].shape, m_card[i].number);
        if ((i + 1) % 13 == 0)
            printf("\n");
    }
}

void shuffle_card(struct trump m_card[])
{
    int i, rnd;
    struct trump temp;

    srand(time(NULL));

    for (i = 0; i < 52; i++) {
        rnd = rand() % 52;
        temp = m_card[rnd];
        m_card[rnd] = m_card[i];
        m_card[i] = temp;
    }
}