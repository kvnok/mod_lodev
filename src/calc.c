#include "../include/stuff.h"

void fill_buffer_with_wall(t_data *d, int x) {
	d->c->step = 1.0 * (d->texture[d->c->texNum]->height) / d->c->lineHeight;
	d->c->texPos = (d->c->drawStart - h / 2 + d->c->lineHeight / 2) * d->c->step;
	for(int y = d->c->drawStart; y < d->c->drawEnd; y++) {
		int texY = ((int)d->c->texPos) & ((d->texture[d->c->texNum]->height) - 1);
		d->c->texPos += d->c->step;
		uint8_t* pixelData = d->texture[d->c->texNum]->pixels;
		uint8_t r = pixelData[(texY * d->texture[d->c->texNum]->width + d->c->texX) * 4];
		uint8_t g = pixelData[(texY * d->texture[d->c->texNum]->width + d->c->texX) * 4 + 1];
		uint8_t b = pixelData[(texY * d->texture[d->c->texNum]->width + d->c->texX) * 4 + 2];
		uint8_t a = pixelData[(texY * d->texture[d->c->texNum]->width + d->c->texX) * 4 + 3];
		uint32_t color = pixel_select(r, g, b, a);
		// if (d->c->side == 1) {
		// 	color = (color >> 1) & 8355711;
		// }
		d->buffer[y][x] = color;
	}
}

/*
kind of a setup to find where the texture is hit
*/
void texture_coords(t_data *d) {
	//what texture is hit
	// d->c->texNum = d->map[d->c->mapX][d->c->mapY] - 1;
	if (d->c->side == 0 && d->c->rayDirX < 0) {
		d->c->texNum = 0;
	}
	if (d->c->side == 0 && d->c->rayDirX > 0) {
		d->c->texNum = 1;
	}
	if (d->c->side == 1 && d->c->rayDirY < 0) {
		d->c->texNum = 2;
	}
	if (d->c->side == 1 && d->c->rayDirY > 0) {
		d->c->texNum = 3;
	}
	//what side is it hit on
	//get the y-coordinate of the wall hit, the exact value
	if (d->c->side == 0) {
		d->c->wallX = d->posY + d->c->perpWallDist * d->c->rayDirY;
	}
	else {
		d->c->wallX = d->posX + d->c->perpWallDist * d->c->rayDirX;
	}
	//make it an integer
	d->c->wallX -= (floor(d->c->wallX));
	//x-coordinate on the texture
	d->c->texX = (int)(d->c->wallX * d->texture[d->c->texNum]->width);
	if (d->c->side == 0 && d->c->rayDirX > 0) {
		d->c->texX = d->texture[d->c->texNum]->width - d->c->texX - 1;
	}
	if (d->c->side == 1 && d->c->rayDirY < 0) {
		d->c->texX = d->texture[d->c->texNum]->width - d->c->texX - 1;
	}
}

/*
lineHeight = height of the line to draw on screen
drawStart = where to start drawing the line
drawEnd = where to stop drawing the line
perpWallDist = perpendicular wall distance
side = 0 for x-side, 1 for y-side
*/
void line_pos(t_data *d) {
	if (d->c->side == 0) {
		d->c->perpWallDist = (d->c->sideDistX - d->c->deltaDistX);
	}
	else {
		d->c->perpWallDist = (d->c->sideDistY - d->c->deltaDistY);
	}
	d->c->lineHeight = (int)(h / d->c->perpWallDist);
	d->c->drawStart = -d->c->lineHeight / 2 + h / 2;
	if (d->c->drawStart < 0) {
		d->c->drawStart = 0;
	}
	d->c->drawEnd = d->c->lineHeight / 2 + h / 2;
	if (d->c->drawEnd >= h) {
		d->c->drawEnd = h - 1;
	}
}

/*
dda is the digital differential analyzer algorithm
this checks for the first wall hit
*/
void dda(t_data *d) {
	d->c->hit = 0;
	while (d->c->hit == 0) {
		if (d->c->sideDistX < d->c->sideDistY) {
			d->c->sideDistX += d->c->deltaDistX;
			d->c->mapX += d->c->stepX;
			d->c->side = 0;
		}
		else {
			d->c->sideDistY += d->c->deltaDistY;
			d->c->mapY += d->c->stepY;
			d->c->side = 1;
		}
		if (d->map[d->c->mapX][d->c->mapY] > 0) {
			d->c->hit = 1;
		}
	}
}

/*
stepX = step direction of the ray in x-direction
stepY = step direction of the ray in y-direction
sideDistX = distance the ray has to travel from its starting position to the first x-side
sideDistY = distance the ray has to travel from its starting position to the first y-side
*/
void pre_dda(t_data *d) {
	if (d->c->rayDirX < 0) {
		d->c->stepX = -1;
		d->c->sideDistX = (d->posX - d->c->mapX) * d->c->deltaDistX;
	}
	else {
		d->c->stepX = 1;
		d->c->sideDistX = (d->c->mapX + 1.0 - d->posX) * d->c->deltaDistX;
	}
	if (d->c->rayDirY < 0) {
		d->c->stepY = -1;
		d->c->sideDistY = (d->posY - d->c->mapY) * d->c->deltaDistY;
	}
	else {
		d->c->stepY = 1;
		d->c->sideDistY = (d->c->mapY + 1.0 - d->posY) * d->c->deltaDistY;
	}
}

/*
cannot divide by zero, so we set the deltaDistX and deltaDistY to INT_MAX
deltaDistX = distance ray has to travel to go from 1 x-side to the next x-side
deltaDistY = distance ray has to travel to go from 1 y-side to the next y-side
fabs is used because we need the absolute value of the rayDirX and rayDirY
*/
void set_delta_dist(t_data *d) {
	if (d->c->rayDirX == 0) {
		d->c->deltaDistX = INT_MAX;
	}
	else {
		d->c->deltaDistX = fabs(1 / d->c->rayDirX);
	}
	if (d->c->rayDirY == 0) {
		d->c->deltaDistY = INT_MAX;
	}
	else {
		d->c->deltaDistY = fabs(1 / d->c->rayDirY);
	}
}

/*
cameraX = x-coordinate in camera space
rayDirX = x-component of ray direction
rayDirY = y-component of ray direction
mapX = x-coordinate in map space
mapY = y-coordinate in map space
*/
void calc_init(t_data *d, int x) {
	d->c = NULL;
	d->c = malloc(sizeof(t_calc));
	if (d->c == NULL) {
		cleanup(d);
	}
	d->c->cameraX = 2 * x / (double)w - 1;
	d->c->rayDirX = d->dirX + d->planeX * d->c->cameraX;
	d->c->rayDirY = d->dirY + d->planeY * d->c->cameraX;

	d->c->mapX = (int)d->posX;
	d->c->mapY = (int)d->posY;
}

void print_calc_variables(t_data *d) {
	printf("cameraX %f\n", d->c->cameraX);
	printf("rayDirX %f\n", d->c->rayDirX);
	printf("rayDirY %f\n", d->c->rayDirY);
	printf("mapX %d\n", d->c->mapX);
	printf("mapY %d\n", d->c->mapY);
	printf("sideDistX %f\n", d->c->sideDistX);
	printf("sideDistY %f\n", d->c->sideDistY);
	printf("deltaDistX %f\n", d->c->deltaDistX);
	printf("deltaDistY %f\n", d->c->deltaDistY);
	printf("perpWallDist %f\n", d->c->perpWallDist);
	printf("stepX %d\n", d->c->stepX);
	printf("stepY %d\n", d->c->stepY);
	printf("hit %d\n", d->c->hit);
	printf("side %d\n", d->c->side);
	printf("lineHeight %d\n", d->c->lineHeight);
	printf("drawStart %d\n", d->c->drawStart);
	printf("drawEnd %d\n", d->c->drawEnd);
	printf("wallX %f\n", d->c->wallX);
	printf("texX %d\n", d->c->texX);
	printf("texY %d\n", d->c->texY);
	printf("step %f\n", d->c->step);
	printf("texPos %f\n", d->c->texPos);
	printf("texNum %d\n", d->c->texNum);
	printf("color %d\n\n\n\n", d->c->color);
}

void calc(t_data *d) {
	for(int x = 0; x < w; x++) {
		calc_init(d, x);
		set_delta_dist(d);
		pre_dda(d);
		dda(d);
		line_pos(d);
		texture_coords(d);
		fill_buffer_with_wall(d, x);
		// print_calc_variables(d);
		free(d->c);
	}
	// printf("(%.0f,%.0f)\n", d->posY, d->posX);
	// printf("side: %d\n", d->c->side);
}
