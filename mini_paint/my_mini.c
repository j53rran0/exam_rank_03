/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_mini.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpunet <rpunet@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 21:25:21 by rpunet            #+#    #+#             */
/*   Updated: 2021/01/28 21:25:49 by rpunet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_draw
{
	int	width, height;
	char	*draw;
}				t_draw;

typedef struct s_cir
{
	char	type, color;
	float	x, y, radius;
}				t_cir;

int		in_cirtangle(t_cir *cir, float x, float y)
{
	float	dist, sqrtdist;

	sqrtdist = sqrtf(powf(x - cir->x, 2) + powf(y - cir->y, 2));
	dist = sqrtdist - cir->radius;
	if (dist <= 0)
	{
		if (dist <= -1)
			return 1;
		return 2;
	}
	return 0;
}

void	create_one(t_draw *draw, t_cir *cir, int x, int y)
{
	int	is;

	is = in_cirtangle(cir, (float)x, (float)y);
	if (is == 2 || (is == 1 && cir->type == 'C'))
		draw->draw[x + y * draw->width] = cir->color;
}

int	create(t_draw *draw, t_cir *cir)
{
	int	i,j;

	if (cir->radius <= 0 || (cir->type != 'c' && cir->type != 'C'))
		return 1;
	i = 0;
	while (i < draw->width)
	{
		j = 0;
		while (j < draw->height)
			create_one(draw, cir, i, j++);
		i++;
	}
	return 0;
}

int	get_info(FILE *file, t_draw *draw)
{
	int	i;
	char	fill;

	if (fscanf(file, "%d %d %c \n", &draw->width, &draw->height, &fill) != 3)
		return 1;
	if (draw->width <= 0 || draw->width > 300 || draw->height <= 0 || draw->height > 300)
		return 1;
	if (!(draw->draw = malloc(draw->width * draw->height)))
		return 1;
	i = 0;
	while (i < draw->height * draw->width)
		draw->draw[i++] = fill;
	return 0;
}

void	print_draw(t_draw *draw)
{
	int	i;

	i = 0;
	while (i < draw->height)
	{
		write(1, draw->draw + i++ * draw->width, draw->width);
		write(1, "\n", 1);
	}
}

int	run(FILE *file)
{
	t_draw	draw;
	t_cir	cir;
	int		scan;

	if (!get_info(file, &draw))
	{
		while ((scan = fscanf(file, "%c %f %f %f %c \n", &cir.type, &cir.x, &cir.y, &cir.radius, &cir.color)) == 5)
		{
			if (create(&draw, &cir))
			{
				free(draw.draw);
				return 1;
			}
		}
		if (scan == -1)
		{
			print_draw(&draw);
			free(draw.draw);
			return 0;
		}
		free(draw.draw);
	}
	return 1;
}

int	main(int argc, char **argv)
{
	FILE *file;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !run(file))
		{
			fclose(file);
			return 0;
		}
		write(1, "Error: Operation file corrupted\n", 32);
		if (file)
			fclose(file);
	}
	else
		write(1, "Error: argument\n", 16);
	return 1;
}
