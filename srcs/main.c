/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarutel <mbarutel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:19:11 by mbarutel          #+#    #+#             */
/*   Updated: 2022/10/09 17:20:05 by mbarutel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#include <curses.h>
#include <term.h>

#define ESCAPE      0x001b
#define ENTER       0x000a
#define UP          0x0105
#define DOWN        0x0106
#define LEFT        0x0107
#define RIGHT       0x0108
#define BACKSPACE   0x007F

static struct termios term, oterm;

static int g_etch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

static int g_etch(void)
{
    int c;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getc(stdin);
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getc(stdin);
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1)
        ungetc(c, stdin);
    if (c != -1)
        return (1);
    return (0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return ESCAPE;
    c = g_etch();
    if (c == '[') 
    {
        c = g_etch();
        if (c == 'A')
            c = UP;
        if (c == 'B')
            c = DOWN;
        if (c == 'D')
            c = LEFT;
        if (c == 'C')
            c = RIGHT;
    }
    else
    {
        c = 0;
    }
    if (c == 0) 
    {
        while (kbhit())
        {
            g_etch();
        }
    }
    return c;
}

static int kbget(void)
{
    int c;

    c = g_etch();
    if (c == ESCAPE)
        return (kbesc());
    return (c);
}

static void shift_bits(char *input, int i, int cur)
{
    while (&input[i] >= &input[cur])
    {
        input[i] = input[i] ^ input[i + 1];
        input[i + 1] = input[i] ^ input[i + 1];
        input[i] = input[i] ^ input[i + 1];
        i--;
    }
}

static void backspace(char *input, int *i, int *cur)
{
    int cur_cpy;

    if (*cur == *i)
    {
        i[0]--;
        cur[0]--;
        input[*cur] = '\0';
    }
    else
    {
        cur[0]--;
        cur_cpy = *cur;
        input[cur_cpy] = '\0';
        while (&input[cur_cpy] < &input[*i])
        {
            input[cur_cpy] = input[cur_cpy] ^ input[cur_cpy + 1];
            input[cur_cpy + 1] = input[cur_cpy] ^ input[cur_cpy + 1];
            input[cur_cpy] = input[cur_cpy] ^ input[cur_cpy + 1];
            cur_cpy++;
        }
        i[0]--;
    }
    
}

int main(void)
{
    int     c;
    char    input[1096];
    int     i;
    int     cur;

    i = 0;
    cur = 0;
    memset(input, '\0', 1096 + 1);
    while (1) 
    {
        c = kbget();
        if (c == ENTER || c == ESCAPE || c == UP || c == DOWN)
            break ;
        if (c == LEFT && cur)
        {
            cur--;
            printf("\033[%dD", 1);
        }
        if (c == RIGHT && (cur < i))
        {
            cur++;
            printf("\033[%dC", 1);
        } 
        if (c == BACKSPACE && cur)
        {
            printf("\033[%dD", 1);
            printf("\033[K");
            backspace(input, &i, &cur);
            if (input[cur])
            {
                printf("\033[s");
                printf("%s", &input[cur]);
                printf("\033[0;%dH", cur);
                printf("\033[u");
            }
        }
        if (isprint(c))
        {
            printf("%c", c);
            if (input[cur])
            {   
                printf("\033[K");
                shift_bits(input, i, cur);
            }
            input[cur++] = c;
            if (input[cur])
            {
                printf("\033[s");
                printf("%s", &input[cur]);
                printf("\033[0;%dH", cur);
                printf("\033[u");
            }
            i++;
        }
    }
    printf("\n%s\n", input);
    return 0;
}
