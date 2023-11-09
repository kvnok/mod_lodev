#ifndef STUFF_H
# define STUFF_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <fcntl.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../MLX42/include/MLX42/MLX42_Int.h"
#include <time.h>

#define red 0xFF0000FF
#define green 0x00FF00FF
#define blue 0x0000FFFF
#define black 0x000000FF
#define white 0xFFFFFFFF
#define grey 0x302F2CFF
#define yellow 0xFFFF00FF
#define purple 0x800080FF
#define orange 0xFFA500FF
#define brown 0x8B4513FF
#define pink 0xFFC0CBFF
#define cyan 0x00FFFFFF
#define dark_green 0x006400FF
#define dark_blue 0x00008BFF
#define dark_red 0x8B0000FF
#define dark_grey 0x696969FF
#define dark_yellow 0xBDB76BFF
#define dark_purple 0x4B0082FF
#define dark_orange 0xFF8C00FF
#define dark_brown 0x5D4037FF
#define dark_pink 0xFF1493FF
#define dark_cyan 0x008B8BFF
#define light_green 0x90EE90FF
#define light_blue 0xADD8E6FF
#define light_red 0xFFB6C1FF
#define light_grey 0xD3D3D3FF
#define light_yellow 0xFFFFE0FF
#define light_purple 0xE6E6FAFF
#define light_orange 0xFFDAB9FF
#define light_brown 0xD2B48CFF
#define light_pink 0xFFC0CBFF
#define light_cyan 0xE0FFFFFF

#define screenWidth 1280
#define screenHeight 960
#define texWidth 64
#define texHeight 64
#define w 1280
#define h 960

#define mapWidth 15
#define mapHeight 15

typedef struct s_calc {
	double	cameraX;
	double	rayDirX;
	double	rayDirY;
	int		mapX;
	int		mapY;
	double	sideDistX;
	double	sideDistY;
	double	deltaDistX;
	double	deltaDistY;
	double	perpWallDist;
	int		stepX;
	int		stepY;
	int		hit;
	int		side;
	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	double	wallX;
	int		texX;
	int		texY;
	double	step;
	double	texPos;
	int		texNum;
	int		color;
} t_calc;

typedef struct s_data {
	mlx_t			*mlx;
	mlx_image_t		*img;
	uint32_t		color;
	double			posX;
	double			posY;
	double			dirX;
	double			dirY;
	double			planeX;
	double			planeY;
	mlx_texture_t	**texture;
	uint32_t		**buffer;
	int				**map;
	t_calc			*c;
}	t_data;

//main.c
void loop_hook(void *param);

//init.c
void	init_data(t_data *data, char *s);

//utils.c
void	remake_image(t_data *d);
void	print_map(int **map);
void	make_map(t_data *d);
int32_t	pixel_select(int32_t r, int32_t g, int32_t b, int32_t a);
void	fill_coords(t_data *d, int x1, int y1, int x2, int y2, int color);

//cleanup.c
void	free_array(void **array, int size);
void	cleanup(t_data *data);

//calc.c
void	calc(t_data *d);

//display.c
void fill_part_of_buffer(t_data *d, int from, int until, uint32_t color);
void buffer_management(t_data *d);

//move.c
void back_and_forth(t_data *d, double moveSpeed, int modifier);
void strafe(t_data *d, double moveSpeed, int modifier);
void rotation(t_data *d, double rotSpeed, int modifier);
void movement(t_data *d);

#endif
