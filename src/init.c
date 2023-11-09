#include "../include/stuff.h"

void base_init(t_data *d, char *s) {
	d->mlx = mlx_init(screenWidth, screenHeight, "testingyep", true);
	d->img = mlx_new_image(d->mlx, screenWidth, screenHeight);
	
	d->posX = mapHeight - 2; //row
	d->posY = 2; //column

	//north
	d->dirY = 0.0;
	d->planeX = 0.0;
	d->dirX = -1.0;
	d->planeY = 0.66;
	if (s == NULL) {
		return ;
	} else if (s[0] == 's' || s[0] == 'S') { //south
		d->dirX = -d->dirX;
		d->planeY = -d->planeY;
	} else if (s[0] == 'w' || s[0] == 'W') { //west
		d->dirY = -1.0;
		d->planeX = -0.66;
		d->dirX = 0.0;
		d->planeY = 0.0;
	} else if (s[0] == 'e' || s[0] == 'E') { //east
		d->dirY = 1.0;
		d->planeX = 0.66;
		d->dirX = 0.0;
		d->planeY = 0.0;
	}
}

void texture_init(t_data *d) {
	d->texture = NULL;
	d->texture = malloc(sizeof(mlx_texture_t) * 8);
	if (d->texture == NULL) {
		cleanup(d);
	}
	d->texture[0] = mlx_load_png("pics/north.png");
	d->texture[1] = mlx_load_png("pics/south.png");
	d->texture[2] = mlx_load_png("pics/east.png");
	d->texture[3] = mlx_load_png("pics/west.png");
	d->texture[4] = mlx_load_png("pics/cyanmountains.png");
	d->texture[5] = mlx_load_png("pics/trainwater.png");
	d->texture[6] = mlx_load_png("pics/wood.png");
	d->texture[7] = mlx_load_png("pics/yellowocean.png");
}

void map_init(t_data *d) {
	d->map = NULL;
	d->map = (int**) malloc(mapHeight * sizeof(int*));
	if (d->map == NULL) {
		cleanup(d);
	}
	for (int i = 0; i < mapHeight; i++) {
		d->map[i] = NULL;
		d->map[i] = (int*)malloc(mapWidth * sizeof(int));
		if (d->map[i] == NULL) {
			cleanup(d);
		}
	}
}

void buffer_init(t_data *d) {
	d->buffer = NULL;
	d->buffer = (uint32_t**) malloc(screenHeight * sizeof(uint32_t*));
	if (d->buffer == NULL) {
		cleanup(d);
	}
	for (int i = 0; i < screenHeight; i++) {
		d->buffer[i] = NULL;
		d->buffer[i] = (uint32_t*)malloc(screenWidth * sizeof(uint32_t));
		if (d->buffer[i] == NULL) {
			cleanup(d);
		}
	}
}

void init_data(t_data *d, char *s) {
	base_init(d, s);
	texture_init(d);
	map_init(d);
	buffer_init(d);
}
