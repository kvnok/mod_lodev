#include "../include/stuff.h"

void loop_hook(void *param) {
	t_data *d;
	d = (t_data *)param;

	remake_image(d);
	for(int x = 0; x < w; x++) {
		calc(d, x);
	}
	buffer_management(d);
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
