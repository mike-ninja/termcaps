/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarutel <mbarutel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:19:11 by mbarutel          #+#    #+#             */
/*   Updated: 2022/10/07 14:57:48 by mbarutel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios og_raw;

void    disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &og_raw);
}

void    enable_raw_mode(void)
{
   
    struct termios raw;

    tcgetattr(STDIN_FILENO, &og_raw);
    raw = og_raw;
    raw.c_iflag &= ~(ICRNL | IXON);
    // raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void)
{
    char c;

    enable_raw_mode();
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        atexit(disable_raw_mode);
        if (c == 'q')
            break ;
        else if (iscntrl(c))
            printf("%d\n", c);
        else
            printf("%d %c\n", c, c);
    }
    return (0);
}
