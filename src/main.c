#include "../include/stuff.h"

typedef struct s_data {
	mlx_t			*mlx;
	mlx_image_t		*img;
	uint32_t		color;
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	mlx_texture_t **texture;
}	t_data;

#define screenWidth 1280
#define screenHeight 960
#define texWidth 64
#define texHeight 64
#define w 1280
#define h 960

#define mapWidth 10
#define mapHeight 6
int worldMap[mapHeight][mapWidth]=
{
	{4,4,1,4,2,4,3,4,4,4},
	{4,0,0,0,0,0,0,0,0,4},
	{4,0,0,0,0,0,0,0,0,8},
	{4,0,0,0,0,0,0,1,0,4},
	{4,0,0,0,0,0,0,0,0,4},
	{4,4,5,4,6,4,7,4,4,4}
};
/*
1:eagle | 2:wave | 3:pillar | 4:greystone | 5:bluestone | 6:mossy | 7:wood | 8:colorstone 
*/

// #define mapWidth 200
// #define mapHeight 6
// int worldMap[mapHeight][mapWidth]=
// {
// 	{4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,4,4,4},
// 	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 	{4,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 	{4,4,5,4,6,4,7,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,1,4,2,4,3,4,4,4,4,4,4,4}
// };

uint32_t buffer[screenHeight][screenWidth];

int32_t pixel_select(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void fill_coords(t_data *data, int x1, int y1, int x2, int y2, int color) {
	int x;
	int y;

	x = x1;
	y = y1;
	while (x < x2) { //left to right
		y = y1;
		while (y < y2) { // top to bottom
			mlx_put_pixel(data->img, x, y, color);
			y++;
		}
		x++;
	}
}

void back_and_forth(t_data *data, double moveSpeed, int modifier) {
	float x = data->posX;
	float y = data->posY;
	float dx = data->dirX * (moveSpeed * modifier);
	float dy = data->dirY * (moveSpeed * modifier);
	if(worldMap[(int)(x + dx)][(int)(y)] == 0) {
		data->posX += dx;
	}
	if(worldMap[(int)(x)][(int)(y + dy)] == 0) {
		data->posY += dy;
	}
}

void strafe(t_data *data, double moveSpeed, int modifier) {
	float x = data->posX;
	float y = data->posY;
	float dx = data->dirY * (moveSpeed * modifier);
	float dy = data->dirX * (moveSpeed * modifier);
	if(worldMap[(int)(x + dx)][(int)(y)] == 0) {
		data->posX += dx;
	}
	if(worldMap[(int)(x)][(int)(y - dy)] == 0) {
		data->posY -= dy;
	}
}

void rotation(t_data *data, double rotSpeed, int modifier) {
	double oldDirX = data->dirX;
	rotSpeed = rotSpeed * modifier;
	data->dirX = data->dirX * cos(rotSpeed) - data->dirY * sin(rotSpeed);
	data->dirY = oldDirX * sin(rotSpeed) + data->dirY * cos(rotSpeed);
	double oldPlaneX = data->planeX;
	data->planeX = data->planeX * cos(rotSpeed) - data->planeY * sin(rotSpeed);
	data->planeY = oldPlaneX * sin(rotSpeed) + data->planeY * cos(rotSpeed);
}

void loop_hook(void *param) {
	t_data *data;
	data = (t_data *)param;
	u_int32_t ceiling_c = light_cyan;
	u_int32_t floor_c = light_brown;
	mlx_delete_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, screenWidth, screenHeight);
	mlx_image_to_window(data->mlx, data->img, 0, 0);

	for(int x = 0; x < w; x++) {
		double cameraX = 2 * x / (double)w - 1;
		double rayDirX = data->dirX + data->planeX*cameraX;
		double rayDirY = data->dirY + data->planeY*cameraX;
		int mapX = (int)data->posX;
		int mapY = (int)data->posY;
		double sideDistX;
		double sideDistY;
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
		double perpWallDist;
		int stepX;
		int stepY;
		int hit = 0;
		int side;
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (data->posX - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - data->posX) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (data->posY - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - data->posY) * deltaDistY;
		}
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
			if (worldMap[mapX][mapY] > 0) {
				hit = 1;
			}
		}
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
		int texNum = worldMap[mapX][mapY] - 1;
		double wallX;
		if (side == 0) {
			wallX = data->posY + perpWallDist * rayDirY;
		}
		else {
			wallX = data->posX + perpWallDist * rayDirX;
		}
		wallX -= (floor(wallX));
		int texX = (int)(wallX * data->texture[texNum]->width);
		if (side == 0 && rayDirX > 0) {
			texX = data->texture[texNum]->width - texX - 1;
		}
		if (side == 1 && rayDirY < 0) {
			texX = data->texture[texNum]->width - texX - 1;
		}
		double step = 1.0 * (data->texture[texNum]->height) / lineHeight;
		double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
		for(int y = drawStart; y < drawEnd; y++) {
			int texY = ((int)texPos) & ((data->texture[texNum]->height) - 1);
			texPos += step;
			uint8_t* pixelData = data->texture[texNum]->pixels;
			uint8_t r = pixelData[(texY * data->texture[texNum]->width + texX) * 4];
			uint8_t g = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 1];
			uint8_t b = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 2];
			uint8_t a = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 3];
			uint32_t color = pixel_select(r, g, b, a);
			if (side == 1) {
				color = (color >> 1) & 8355711;
			}
			buffer[y][x] = color;
		}
	}
	for(int y = 0; y < h/2; y++) {
		for(int x = 0; x < w; x++) {
			if (buffer[y][x] == 0) {
				buffer[y][x] = ceiling_c;
			}
		}
	}
	for(int y = h/2; y < h; y++) {
		for(int x = 0; x < w; x++) {
			if (buffer[y][x] == 0) {
				buffer[y][x] = floor_c;
			}
		}
	}
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			mlx_put_pixel(data->img, x, y, buffer[y][x]);
		}
	}
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			buffer[y][x] = 0;
		}
	}
	double moveSpeed = 0.15;
	double rotSpeed = 0.08;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE)) {
		mlx_close_window(data->mlx);
		mlx_terminate(data->mlx);
		exit(0);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_W)) {
		back_and_forth(data, moveSpeed, 1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S)) {
		back_and_forth(data, moveSpeed, -1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A)) {
		strafe(data, moveSpeed, -1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D)) {
		strafe(data, moveSpeed, 1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT)) {
		rotation(data, rotSpeed, 1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT)) {
		rotation(data, rotSpeed, -1);
	}
}

int main(int argc, char **argv) {
	t_data data;

	data.mlx = mlx_init(screenWidth, screenHeight, "testingyep", true);
	data.img = mlx_new_image(data.mlx, screenWidth, screenHeight);
	data.posX = 2.0;
	data.posY = 5.2;	
	data.dirX = -1.0;
	data.dirY = 0.0;
	data.planeX = 0.0;
	data.planeY = 0.66;
	data.texture = malloc(sizeof(mlx_texture_t) * 8);
	data.texture[0] = mlx_load_png("pics/cyanmountains.png");
	data.texture[1] = mlx_load_png("pics/wave.png");
	data.texture[2] = mlx_load_png("pics/pillar.png");
	data.texture[3] = mlx_load_png("pics/oceanoutlook.png");
	data.texture[4] = mlx_load_png("pics/seaweed.png");
	data.texture[5] = mlx_load_png("pics/trainwater.png");
	data.texture[6] = mlx_load_png("pics/wood.png");
	data.texture[7] = mlx_load_png("pics/yellowocean.png");

	mlx_image_to_window(data.mlx, data.img, 0, 0);
	fill_coords(&data, 0, 0, screenWidth, screenHeight, white);
	mlx_loop_hook(data.mlx, &loop_hook, &data);
	mlx_loop(data.mlx);
	mlx_close_window(data.mlx);
	mlx_terminate(data.mlx);
	return 0;
}
