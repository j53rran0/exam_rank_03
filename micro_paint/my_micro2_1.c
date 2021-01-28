/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_micro2_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpunet <rpunet@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 21:25:18 by rpunet            #+#    #+#             */
/*   Updated: 2021/01/28 21:25:52 by rpunet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_draw
{
	int	width, height;
	char	*draw;
}				t_draw;

typedef struct s_rec
{
	char	type, color;
	float	x, y, width, height;
}				t_rec;

int		in_rectangle(t_rec *rec, float x, float y)
{
	if (x < rec->x || rec->x + rec->width < x || y < rec->y || rec->y + rec->height < y)
		return 0;
	if (x - rec->x < 1 || rec->x + rec->width - x < 1 || y - rec->y < 1 || rec->y + rec->height - y < 1)
		return 2;
	return 1;
}

void	create_one(t_draw *draw, t_rec *rec, int x, int y)
{
	int	is;

	is = in_rectangle(rec, (float)x, (float)y);
	if (is == 2 || (is == 1 && rec->type == 'R'))
		draw->draw[x + y * draw->width] = rec->color;
}

int	create(t_draw *draw, t_rec *rec)
{
	int	i,j;

	if (rec->width <= 0 || rec->height <= 0 || (rec->type != 'r' && rec->type != 'R'))
		return 1;
	i = 0;
	while (i < draw->width)
	{
		j = 0;
		while (j < draw->height)
			create_one(draw, rec, i, j++);
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
	t_rec	rec;
	int		scan;

	if (!get_info(file, &draw))
	{
		while ((scan = fscanf(file, "%c %f %f %f %f %c \n", &rec.type, &rec.x, &rec.y, &rec.width, &rec.height, &rec.color)) == 6)
		{
			if (create(&draw, &rec))
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
