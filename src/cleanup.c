#include "../include/stuff.h"

void free_array(void **array, int size) {
	for (int i = 0; i < size; i++) {
		free(array[i]);
	}
	free(array);
}

void cleanup(t_data *d) {
	free_array((void **)d->buffer, screenHeight);
	free_array((void **)d->map, mapHeight);
	free(d->texture);
	mlx_close_window(d->mlx);
	mlx_terminate(d->mlx);
	exit(0);
}
