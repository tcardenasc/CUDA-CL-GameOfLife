__kernel void game_of_life(__global uchar* data, __global uchar* result, uint width, uint height) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    int x0 = (x + width - 1) % width;
    int x2 = (x + 1) % width;
    int y0 = (y + height - 1) % height;
    int y2 = (y + 1) % height;

    int aliveCells = data[x0 + y0 * width] + data[x + y0 * width] + data[x2 + y0 * width]
                   + data[x0 + y * width] + data[x2 + y * width]
                   + data[x0 + y2 * width] + data[x + y2 * width] + data[x2 + y2 * width];

    uchar currentState = data[x + y * width];
    result[x + y * width] = (aliveCells == 3 || (aliveCells == 2 && currentState)) ? 1 : 0;
}