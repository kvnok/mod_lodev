#include "../include/stuff.h"

void calc(t_data *d, int x) {
	//init vars in struct
	d->c = NULL;
	d->c = malloc(sizeof(t_calc));
	if (d->c == NULL) {
		cleanup(d);
	}
	d->c->cameraX = 2 * x / (double)w - 1;
	// double cameraX = 2 * x / (double)w - 1;
	d->c->rayDirX = d->dirX + d->planeX * d->c->cameraX;
	d->c->rayDirY = d->dirY + d->planeY * d->c->cameraX;

	d->c->mapX = (int)d->posX;
	d->c->mapY = (int)d->posY;
	
	d->c->deltaDistX;
	if (d->c->rayDirX == 0) {
		d->c->deltaDistX = 1e30;
	}
	else {
		d->c->deltaDistX = fabs(1 / d->c->rayDirX);
	}
	d->c->deltaDistY;
	if (d->c->rayDirY == 0) {
		d->c->deltaDistY = 1e30;
	}
	else {
		d->c->deltaDistY = fabs(1 / d->c->rayDirY);
	}
	
	double sideDistX;
	double sideDistY;
	int stepX;
	int stepY;
	int hit = 0;
	if (d->c->rayDirX < 0) {
		stepX = -1;
		sideDistX = (d->posX - d->c->mapX) * d->c->deltaDistX;
	}
	else {
		stepX = 1;
		sideDistX = (d->c->mapX + 1.0 - d->posX) * d->c->deltaDistX;
	}
	if (d->c->rayDirY < 0) {
		stepY = -1;
		sideDistY = (d->posY - d->c->mapY) * d->c->deltaDistY;
	}
	else {
		stepY = 1;
		sideDistY = (d->c->mapY + 1.0 - d->posY) * d->c->deltaDistY;
	}

	int side;
	while (hit == 0) {
		if (sideDistX < sideDistY) {
			sideDistX += d->c->deltaDistX;
			d->c->mapX += stepX;
			side = 0;
		}
		else {
			sideDistY += d->c->deltaDistY;
			d->c->mapY += stepY;
			side = 1;
		}
		if (d->map[d->c->mapX][d->c->mapY] > 0) {
			hit = 1;
		}
	}

	double perpWallDist;
	if (side == 0) {
		perpWallDist = (sideDistX - d->c->deltaDistX);
	}
	else {
		perpWallDist = (sideDistY - d->c->deltaDistY);
	}
	int lineHeight = (int)(h / perpWallDist);

	int drawStart = -lineHeight / 2 + h / 2;
	if (drawStart < 0) {
		drawStart = 0;
	}
	int drawEnd = lineHeight / 2 + h / 2;
	if (drawEnd >= h) {
		drawEnd = h - 1;
	}

	int texNum = d->map[d->c->mapX][d->c->mapY] - 1;
	double wallX;
	if (side == 0) {
		wallX = d->posY + perpWallDist * d->c->rayDirY;
	}
	else {
		wallX = d->posX + perpWallDist * d->c->rayDirX;
	}
	wallX -= (floor(wallX));
	int texX = (int)(wallX * d->texture[texNum]->width);
	if (side == 0 && d->c->rayDirX > 0) {
		texX = d->texture[texNum]->width - texX - 1;
	}
	if (side == 1 && d->c->rayDirY < 0) {
		texX = d->texture[texNum]->width - texX - 1;
	}

	double step = 1.0 * (d->texture[texNum]->height) / lineHeight;
	double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
	for(int y = drawStart; y < drawEnd; y++) {
		int texY = ((int)texPos) & ((d->texture[texNum]->height) - 1);
		texPos += step;
		uint8_t* pixelData = d->texture[texNum]->pixels;
		uint8_t r = pixelData[(texY * d->texture[texNum]->width + texX) * 4];
		uint8_t g = pixelData[(texY * d->texture[texNum]->width + texX) * 4 + 1];
		uint8_t b = pixelData[(texY * d->texture[texNum]->width + texX) * 4 + 2];
		uint8_t a = pixelData[(texY * d->texture[texNum]->width + texX) * 4 + 3];
		uint32_t color = pixel_select(r, g, b, a);
		if (side == 1) {
			color = (color >> 1) & 8355711;
		}
		d->buffer[y][x] = color;
	}
	//free vars struct
	free(d->c);
}
