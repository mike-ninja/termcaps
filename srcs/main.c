/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarutel <mbarutel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 10:19:11 by mbarutel          #+#    #+#             */
/*   Updated: 2022/10/07 10:28:32 by mbarutel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>

void    enable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void)
{
    char c;

    enable_raw_mode();
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c == 'q')
            break ;
    }
    return (0);
}