#include "../include/stuff.h"

void fill_part_of_buffer(t_data *d, int from, int until, uint32_t color) {
	for(int y = from; y < until; y++) {
		for(int x = 0; x < w; x++) {
			if (d->buffer[y][x] == 0) {
				d->buffer[y][x] = color;
			}
		}
	}
}

void buffer_management(t_data *d) {
	fill_part_of_buffer(d, 0, h/2, light_cyan);
	fill_part_of_buffer(d, h/2, h, light_brown);
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
}
