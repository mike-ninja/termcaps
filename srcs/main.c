/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lain.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarutel <mbarutel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 11:52:45 by mbarutel          #+#    #+#             */
/*   Updated: 2022/10/10 13:23:40 by mbarutel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcaps.h"

static struct termios term, oterm;

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

static int get_input(void)
{
    int c;

    c = 0;
	tcgetattr(STDIN_FILENO, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oterm);
    return (c);
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

static int esc_parse(int c)
{
    if (c == '[')
    {
        c = get_input();
        if (c == 'D')
            c = LEFT;
        if (c == 'C')
            c = RIGHT;
    }
    return (c);
}

int main(void)
{
    int     c;
    int     bytes;
    int     cursor;
    char    input[1096];

    c = 0;
    bytes = 0;
    cursor = 0;
    memset(input, '\0', 1096);
    while (1) 
    {
        c = get_input();
        if (c == ESCAPE)
        {
            c = get_input();
            if (c == '[')
                c = esc_parse(c);
            else
                c = ESCAPE;
        }
        if (c == ENTER || c == ESCAPE)
            break;
        if (c == LEFT && cursor)
        {
            cursor--;
            write(1, "\033[1D", 4);
        }
        else if (c == RIGHT && (cursor < bytes))
        {
            cursor++;
            write(1, "\033[1C", 4);
        }
        else if (c == BACKSPACE && cursor >= 0)
        {
            write(1, "\033[1D", 4);
            ft_putstr("\033[K");
            backspace(input, &bytes, &cursor);
            if (input[cursor])
            {
                ft_putstr("\033[s");
                ft_putstr(&input[cursor]);
                ft_putstr("\033[H");
                ft_putstr("\033[u");
            }
        }
        else if (isprint(c))
        {
            write(1, &c, 1);
            if (input[cursor])
            {   
                ft_putstr("\033[K");
                shift_bits(input, bytes, cursor);
            }
            input[cursor++] = c;
            if (input[cursor])
            {
                ft_putstr("\033[s");
                ft_putstr(&input[cursor]);
                ft_putstr("\033[H");
                ft_putstr("\033[u");
            }
            bytes++;
        }
    }
    printf("\n%s\n", input);
    return 0;
}
