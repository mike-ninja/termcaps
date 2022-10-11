/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarutel <mbarutel@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:55:58 by mbarutel          #+#    #+#             */
/*   Updated: 2022/10/10 12:57:02 by mbarutel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMCAPS_H
# define TERMCAPS_H

# include "../libft/incs/libft.h"
# include "../libft/incs/ft_printf.h"

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#define ESCAPE      0x001b
#define ENTER       0x000a
#define UP          0x0105
#define DOWN        0x0106
#define LEFT        0x0107
#define RIGHT       0x0108
#define BACKSPACE   0x007F

#endif