#include "../include/stuff.h"

void remake_image(t_data *d) {
	mlx_delete_image(d->mlx, d->img);
	d->img = mlx_new_image(d->mlx, screenWidth, screenHeight);
	mlx_image_to_window(d->mlx, d->img, 0, 0);
}

void loop_hook(void *param) {
	t_data *d;
	d = (t_data *)param;
	u_int32_t ceiling_c = light_cyan;
	u_int32_t floor_c = light_brown;
	remake_image(d);

	for(int x = 0; x < w; x++) {
		double cameraX = 2 * x / (double)w - 1;
		double rayDirX = d->dirX + d->planeX*cameraX;
		double rayDirY = d->dirY + d->planeY*cameraX;

		int mapX = (int)d->posX;
		int mapY = (int)d->posY;
		
		double deltaDistX;
		if (rayDirX == 0) {
			deltaDistX = 1e30;
		}
		else {
			deltaDistX = fabs(1 / rayDirX);
		}
		double deltaDistY;
		if (rayDirY == 0) {
			deltaDistY = 1e30;
		}
		else {
			deltaDistY = fabs(1 / rayDirY);
		}
		
		double sideDistX;
		double sideDistY;
		int stepX;
		int stepY;
		int hit = 0;
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (d->posX - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - d->posX) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (d->posY - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - d->posY) * deltaDistY;
		}

		int side;
		while (hit == 0) {
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (d->map[mapX][mapY] > 0) {
				hit = 1;
			}
		}

		double perpWallDist;
		if (side == 0) {
			perpWallDist = (sideDistX - deltaDistX);
		}
		else {
			perpWallDist = (sideDistY - deltaDistY);
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

		int texNum = d->map[mapX][mapY] - 1;
		double wallX;
		if (side == 0) {
			wallX = d->posY + perpWallDist * rayDirY;
		}
		else {
			wallX = d->posX + perpWallDist * rayDirX;
		}
		wallX -= (floor(wallX));
		int texX = (int)(wallX * d->texture[texNum]->width);
		if (side == 0 && rayDirX > 0) {
			texX = d->texture[texNum]->width - texX - 1;
		}
		if (side == 1 && rayDirY < 0) {
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
	}
	for(int y = 0; y < h/2; y++) {
		for(int x = 0; x < w; x++) {
			if (d->buffer[y][x] == 0) {
				d->buffer[y][x] = ceiling_c;
			}
		}
	}
	for(int y = h/2; y < h; y++) {
		for(int x = 0; x < w; x++) {
			if (d->buffer[y][x] == 0) {
				d->buffer[y][x] = floor_c;
			}
		}
	}
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			mlx_put_pixel(d->img, x, y, d->buffer[y][x]);
		}
	}
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			d->buffer[y][x] = 0;
		}
	}
	movement(d);
}

int main(int argc, char **argv) {
	t_data d;

	init_data(&d);
	make_map(&d);
	mlx_image_to_window(d.mlx, d.img, 0, 0);
	mlx_loop_hook(d.mlx, &loop_hook, &d);
	mlx_loop(d.mlx);
	cleanup(&d);
	return 0;
}
