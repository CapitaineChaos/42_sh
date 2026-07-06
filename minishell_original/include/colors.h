/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 23:31:16 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:04:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# define RVB1 "\033[38;2;200;100;255m\002" // Purple
# define RVB2 "\033[38;2;255;100;200m\002" // Pink
# define RVB3 "\033[38;2;100;200;255m\002" // Blue
# define RVB4 "\033[38;2;255;200;100m\002" // Yellow
# define RVB5 "\033[38;2;100;255;200m\002" // Green
# define RVB6 "\033[38;2;50;200;100m\002" // Green
# define RVB7 "\033[38;2;0;150;50m\002" // Green
# define RVB8 "\033[38;2;100;0;150m\002" // Purple
# define RVB9 "\033[38;2;50;0;100m\002" // Purple
# define RVB10 "\033[38;2;0;0;50m\002" // Purple

/* Reset */
# define RT "\001\033[0m\002"

/* Attributes */
/* BOLD, DIM, ITALIC, UNDERLINE, BLINK, REVERSE, HIDDEN */
# define BD "\001\033[1m\002"
# define DM "\001\033[2m\002"
# define IT "\001\033[3m\002"
# define UN "\001\033[4m\002"
# define BN "\001\033[5m\002"
# define RV "\001\033[7m\002"
# define HD "\001\033[8m\002"

/* Foreground Colors */
# define BK "\001\033[30m\002"
# define RD "\001\033[31m\002"
# define GN "\001\033[32m\002"
# define YL "\001\033[33m\002"
# define BL "\001\033[34m\002"
# define MG "\001\033[35m\002"
# define CY "\001\033[36m\002"
# define WT "\001\033[37m\002"

/* Bright Foreground Colors */
# define BBK "\001\033[90m\002"
# define BRD "\001\033[91m\002"
# define BGN "\001\033[92m\002"
# define BYL "\001\033[93m\002"
# define BBL "\001\033[94m\002"
# define BMG "\001\033[95m\002"
# define BCY "\001\033[96m\002"
# define BWT "\001\033[97m\002"

/* Background Colors */
# define BK_ "\001\033[40m\002"
# define RD_ "\001\033[41m\002"
# define GN_ "\001\033[42m\002"
# define YL_ "\001\033[43m\002"
# define BL_ "\001\033[44m\002"
# define MG_ "\001\033[45m\002"
# define CY_ "\001\033[46m\002"
# define WT_ "\001\033[47m\002"

/* Bright Background Colors */
# define BBK_ "\001\033[100m\002"
# define BRD_ "\001\033[101m\002"
# define BGN_ "\001\033[102m\002"
# define BYL_ "\001\033[103m\002"
# define BBL_ "\001\033[104m\002"
# define BMG_ "\001\033[105m\002"
# define BCY_ "\001\033[106m\002"
# define BWT_ "\001\033[107m\002"

# define LGR "\001\033[38;2;245;245;245m\002"
# define LGN "\001\033[38;2;200;255;200m\002"
# define LBL "\001\033[38;2;200;200;255m\002"
# define LMG "\001\033[38;2;255;200;255m\002"
# define LRD "\001\033[38;2;255;200;200m\002"
# define LYL "\001\033[38;2;255;255;200m\002"

#endif
