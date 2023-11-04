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

// #define mapWidth 24
// #define mapHeight 24
// int worldMap[mapHeight][mapWidth]=
// {
// 	{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
// 	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
// 	{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
// 	{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
// 	{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
// 	{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
// 	{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
// 	{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
// 	{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
// 	{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
// 	{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
// 	{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
// 	{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
// 	{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
// 	{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
// 	{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
// 	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
// 	{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
// 	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
// 	{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
// 	{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
// 	{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
// 	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
// 	{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
// };

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
1:jungle | 2:redbrick | 3:purplestone | 4:greystone | 5:bluestone | 6:mossy | 7:wood | 8:colorstone 
*/

uint32_t buffer[screenHeight][screenWidth];

/*
data->color = pixel_select(
	rand() % 255,
	rand() % 255,
	rand() % 255,
	255
);
*/
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

void bres_draw_line(int x1, int y1, int x2, int y2, t_data *data) {
	int dx = abs(x2 - x1); //distance x coords
	int dy = abs(y2 - y1); //distance y coords
	int sx = x1 < x2 ? 1 : -1; //slope x
	int sy = y1 < y2 ? 1 : -1; //slope y
	int err = dx - dy; //move vertical or horizontal
	int e2; //error correction * 2
	while (x1 != x2 && y1 != y2) {
		mlx_put_pixel(data->img, x1, y1, blue);
		e2 = err * 2; //err * 2 at the start of next calculation
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
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
	// fill_coords(data, 0, 0, screenWidth, screenHeight/2, dark_green);
	// fill_coords(data, 0, screenHeight/2, screenWidth, screenHeight, dark_yellow);
	
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
		double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
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

		int pitch = 0; // 100
		int drawStart = -lineHeight / 2 + h / 2 + pitch;
		if (drawStart < 0) {
			drawStart = 0;
		}
		int drawEnd = lineHeight / 2 + h / 2 + pitch;
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
		// int texX = (int)(wallX * (double)texWidth);
		// if (side == 0 && rayDirX > 0) {
		// 	texX = texWidth - texX - 1;
		// }
		// if (side == 1 && rayDirY < 0) {
		// 	texX = texWidth - texX - 1;
		// }
		int texX = (int)(wallX * data->texture[texNum]->width);
		if (side == 0 && rayDirX > 0) {
			texX = data->texture[texNum]->width - texX - 1;
		}
		if (side == 1 && rayDirY < 0) {
			texX = data->texture[texNum]->width - texX - 1;
		}
		// Calculate texY based on the wall's height and lineHeight
		// double step = 1.0 * texHeight / lineHeight;
		double step = 1.0 * (data->texture[texNum]->height) / lineHeight;
		double texPos = (drawStart - pitch - h / 2 + lineHeight / 2) * step;
		// printf("[%.2f][%.2f], width %d, map n %d, line height %d\n", data->posY, data->posX, x, texNum, lineHeight);
		for(int y = drawStart; y < drawEnd; y++) {
			// int texY = ((int)texPos) & (texHeight - 1);
			int texY = ((int)texPos) & ((data->texture[texNum]->height) - 1);
			// int texY = (int)(texPos * (data->texture[texNum]->height) / lineHeight);
			texPos += step;
			// uint32_t color = data->texture[texNum]->pixels[data->texture[texNum]->width * texY + texX];
			// uint32_t color = data->texture[texNum]->pixels[texY];
			uint8_t* pixelData = data->texture[texNum]->pixels; // Get the pixel data for a specific texture
			// int texX = x;// Calculate the x-coordinate within the texture
			// int texY = 0;// Calculate the y-coordinate within the texture

			// Access the RGBA values of the pixel at (texX, texY)
			uint8_t r = pixelData[(texY * data->texture[texNum]->width + texX) * 4];     // Red channel
			uint8_t g = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 1]; // Green channel
			uint8_t b = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 2];  // Blue channel
			uint8_t a = pixelData[(texY * data->texture[texNum]->width + texX) * 4 + 3]; // Alpha channel
			uint32_t color = pixel_select(r, g, b, a);
			// printf("%d, %d, %d, %d\n", r, g, b, a);
			if (side == 1) {
				color = (color >> 1) & 8355711;
			}
			buffer[y][x] = color;
		}
	}
	// printf("\nprinting top buffer\n");
	// for(int x = 0; x < h; x++) {
	// 	printf(" |%d| ", buffer[0][x]);
	// }
	// printf("\nprinting DONE\n\n\n");
	// fill_coords(data, 0, 0, screenWidth, screenHeight/2, dark_green);
	// fill_coords(data, 0, screenHeight/2, screenWidth, screenHeight, dark_yellow);

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
	//speed modifiers
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
		//both camera direction and camera plane must be rotated
		rotation(data, rotSpeed, 1);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT)) {
		//both camera direction and camera plane must be rotated
		rotation(data, rotSpeed, -1);
	}
}

int main(int argc, char **argv) {

	t_data data;

	data.mlx = mlx_init(screenWidth, screenHeight, "flat", true);
	data.img = mlx_new_image(data.mlx, screenWidth, screenHeight);
	// data.posX = 22.0;
	// data.posY = 11.5;

	// x and y seem to be flipped in the implementation
	// this is all very wack
	data.posX = 2.0;
	data.posY = 5.2;
	
	data.dirX = -1.0;
	data.dirY = 0.0;
	data.planeX = 0.0;
	data.planeY = 0.66;
	data.texture = malloc(sizeof(mlx_texture_t) * 8);
	// data.texture[0] = mlx_load_png("images/img128/jungle.png");
	// data.texture[1] = mlx_load_png("images/mray.png");
	// data.texture[2] = mlx_load_png("images/mray1.png");
	data.texture[0] = mlx_load_png("pics/eagle.png");
	data.texture[1] = mlx_load_png("pics/wave.png");
	data.texture[2] = mlx_load_png("pics/pillar.png");
	data.texture[3] = mlx_load_png("pics/greystone.png");
	data.texture[4] = mlx_load_png("pics/bluestone.png");
	data.texture[5] = mlx_load_png("pics/mossy.png");
	data.texture[6] = mlx_load_png("pics/wood.png");
	data.texture[7] = mlx_load_png("pics/colorstone.png");

	mlx_image_to_window(data.mlx, data.img, 0, 0);
	fill_coords(&data, 0, 0, screenWidth, screenHeight, white);

	/*
	print pixels
	*/
	// int coolindex = 0;
	// // for(int y = 0; y < data.texture[coolindex]->height; y++)
	// 	for(int x = 0; x < data.texture[coolindex]->width; x++) {
	// 		uint8_t* pixelData = data.texture[coolindex]->pixels; // Get the pixel data for a specific texture
	// 		int texX = x;// Calculate the x-coordinate within the texture
	// 		int texY = 0;// Calculate the y-coordinate within the texture

	// 		// Access the RGBA values of the pixel at (texX, texY)
	// 		uint8_t r = pixelData[(texY * data.texture[coolindex]->width + texX) * 4];     // Red channel
	// 		uint8_t g = pixelData[(texY * data.texture[coolindex]->width + texX) * 4 + 1]; // Green channel
	// 		uint8_t b = pixelData[(texY * data.texture[coolindex]->width + texX) * 4 + 2];  // Blue channel
	// 		uint8_t a = pixelData[(texY * data.texture[coolindex]->width + texX) * 4 + 3]; // Alpha channel
	// 		// printf("%d, %d, %d, %d\n", r, g, b, a);
	// 		printf("|x : %d| (%d, %d, %d)\n", x, r, g, b);
	// 	}
	// mlx_close_window(data.mlx);
	// mlx_terminate(data.mlx);
	// exit(0);
	/*
	uint8_t* pixelData = textures[textureIndex].pixels; // Get the pixel data for a specific texture
	int texX = // Calculate the x-coordinate within the texture
	int texY = // Calculate the y-coordinate within the texture

	// Access the RGBA values of the pixel at (texX, texY)
	uint8_t red = pixelData[(texY * textures[textureIndex].width + texX) * 4];     // Red channel
	uint8_t green = pixelData[(texY * textures[textureIndex].width + texX) * 4 + 1]; // Green channel
	uint8_t blue = pixelData[(texY * textures[textureIndex].width + texX) * 4 + 2];  // Blue channel
	uint8_t alpha = pixelData[(texY * textures[textureIndex].width + texX) * 4 + 3]; // Alpha channel
	*/

	// printf("\n");
	mlx_loop_hook(data.mlx, &loop_hook, &data);
	mlx_loop(data.mlx);
	mlx_close_window(data.mlx);
	mlx_terminate(data.mlx);
	return 0;
}

/**
 * Base object for disk loaded textures.
 * It contains rudementary information about the texture.
 * 
 * @param width The width of the texture.
 * @param height The height of the texture.
 * @param pixels The literal pixel data.
 * @param bytes_per_pixel The amount of bytes in a pixel, always 4.
 */
// typedef struct mlx_texture
// {
// 	uint32_t	width;
// 	uint32_t	height;
// 	uint8_t		bytes_per_pixel;
// 	uint8_t*	pixels;
// }	mlx_texture_t;

/*
TODO:
- fix the texture loading
- fix the texture rendering

gotta make sure i get the correct values from the texture
meaning the correct cordinates and the correct color (rgba)

plan:
- print values and compare to image to see if its the correct color and cordinate
- take the rgba and fill the buffer using the pixel select function
*/
