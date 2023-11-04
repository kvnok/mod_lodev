#include "../include/stuff.h"

void back_and_forth(t_data *d, double moveSpeed, int modifier) {
	float x = d->posX;
	float y = d->posY;
	float dx = d->dirX * (moveSpeed * modifier);
	float dy = d->dirY * (moveSpeed * modifier);
	if(d->map[(int)(x + dx)][(int)(y)] == 0) {
		d->posX += dx;
	}
	if(d->map[(int)(x)][(int)(y + dy)] == 0) {
		d->posY += dy;
	}
}

void strafe(t_data *d, double moveSpeed, int modifier) {
	float x = d->posX;
	float y = d->posY;
	float dx = d->dirY * (moveSpeed * modifier);
	float dy = d->dirX * (moveSpeed * modifier);
	if(d->map[(int)(x + dx)][(int)(y)] == 0) {
		d->posX += dx;
	}
	if(d->map[(int)(x)][(int)(y - dy)] == 0) {
		d->posY -= dy;
	}
}

void rotation(t_data *d, double rotSpeed, int modifier) {
	double oldDirX = d->dirX;
	rotSpeed = rotSpeed * modifier;
	d->dirX = d->dirX * cos(rotSpeed) - d->dirY * sin(rotSpeed);
	d->dirY = oldDirX * sin(rotSpeed) + d->dirY * cos(rotSpeed);
	double oldPlaneX = d->planeX;
	d->planeX = d->planeX * cos(rotSpeed) - d->planeY * sin(rotSpeed);
	d->planeY = oldPlaneX * sin(rotSpeed) + d->planeY * cos(rotSpeed);
}

void movement(t_data *d) {
	double moveSpeed = 0.15;
	double rotSpeed = 0.08;
	if (mlx_is_key_down(d->mlx, MLX_KEY_ESCAPE)) {
		cleanup(d);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_W)) {
		back_and_forth(d, moveSpeed, 1);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_S)) {
		back_and_forth(d, moveSpeed, -1);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_A)) {
		strafe(d, moveSpeed, -1);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_D)) {
		strafe(d, moveSpeed, 1);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_LEFT)) {
		rotation(d, rotSpeed, 1);
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_RIGHT)) {
		rotation(d, rotSpeed, -1);
	}
}