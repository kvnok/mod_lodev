#include "../include/stuff.h"

void remake_image(t_data *d) {
	mlx_delete_image(d->mlx, d->img);
	d->img = mlx_new_image(d->mlx, screenWidth, screenHeight);
	mlx_image_to_window(d->mlx, d->img, 0, 0);
}

void print_map(int **map) {
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			printf("%d", map[i][j]);
		}
		printf("\n");
	}
}

void make_map(t_data *d) {
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (i == 0 || i == mapHeight - 1 || j == 0 || j == mapWidth - 1) {
				d->map[i][j] = 1;
			}
			else
				d->map[i][j] = 0;
		}
	}
	d->map[5][3] = 2;
	d->map[5][4] = 3;
	d->map[5][5] = 4;
	d->map[5][6] = 5;
	d->map[5][7] = 6;
	d->map[5][8] = 7;
}

int32_t pixel_select(int32_t r, int32_t g, int32_t b, int32_t a) {
	return (r << 24 | g << 16 | b << 8 | a);
}

void fill_coords(t_data *d, int x1, int y1, int x2, int y2, int color) {
	int x;
	int y;

	x = x1;
	y = y1;
	while (x < x2) { //left to right
		y = y1;
		while (y < y2) { // top to bottom
			mlx_put_pixel(d->img, x, y, color);
			y++;
		}
		x++;
	}
}
