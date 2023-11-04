#include "../include/stuff.h"

void calc(t_data *d, int x) {
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

	if (d->c->rayDirX == 0) {
		d->c->deltaDistX = 1e30;
	}
	else {
		d->c->deltaDistX = fabs(1 / d->c->rayDirX);
	}
	if (d->c->rayDirY == 0) {
		d->c->deltaDistY = 1e30;
	}
	else {
		d->c->deltaDistY = fabs(1 / d->c->rayDirY);
	}
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

	d->c->texNum = d->map[d->c->mapX][d->c->mapY] - 1;
	if (d->c->side == 0) {
		d->c->wallX = d->posY + d->c->perpWallDist * d->c->rayDirY;
	}
	else {
		d->c->wallX = d->posX + d->c->perpWallDist * d->c->rayDirX;
	}
	d->c->wallX -= (floor(d->c->wallX));
	int texX = (int)(d->c->wallX * d->texture[d->c->texNum]->width);
	if (d->c->side == 0 && d->c->rayDirX > 0) {
		texX = d->texture[d->c->texNum]->width - texX - 1;
	}
	if (d->c->side == 1 && d->c->rayDirY < 0) {
		texX = d->texture[d->c->texNum]->width - texX - 1;
	}

	d->c->step = 1.0 * (d->texture[d->c->texNum]->height) / d->c->lineHeight;
	d->c->texPos = (d->c->drawStart - h / 2 + d->c->lineHeight / 2) * d->c->step;
	for(int y = d->c->drawStart; y < d->c->drawEnd; y++) {
		int texY = ((int)d->c->texPos) & ((d->texture[d->c->texNum]->height) - 1);
		d->c->texPos += d->c->step;
		uint8_t* pixelData = d->texture[d->c->texNum]->pixels;
		uint8_t r = pixelData[(texY * d->texture[d->c->texNum]->width + texX) * 4];
		uint8_t g = pixelData[(texY * d->texture[d->c->texNum]->width + texX) * 4 + 1];
		uint8_t b = pixelData[(texY * d->texture[d->c->texNum]->width + texX) * 4 + 2];
		uint8_t a = pixelData[(texY * d->texture[d->c->texNum]->width + texX) * 4 + 3];
		uint32_t color = pixel_select(r, g, b, a);
		if (d->c->side == 1) {
			color = (color >> 1) & 8355711;
		}
		d->buffer[y][x] = color;
	}
	free(d->c);
}
