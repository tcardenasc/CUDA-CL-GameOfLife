__kernel void game_of_life_cond(__global uchar* data, __global uchar* result, uint width, uint height) {
    int x = get_global_id(0);
    int y = get_global_id(1);
    int cnt = 0;
    int x0 = (x + width - 1) % width;
    int x2 = (x + 1) % width;
    int y0 = (y + height - 1) % height;
    int y2 = (y + 1) % height;

    int aliveCells = 0;
    int xOffsets[3] = {x0, x, x2};
    int yOffsets[3] = {y0, y, y2};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 1 && j == 1) {
                continue;
            }
            if (data[xOffsets[i] + yOffsets[j] * width]) {
                aliveCells++;
            }
        }
    }
    uchar currentState = data[x + y * width];
    result[x + y * width] = (aliveCells == 3 || (aliveCells == 2 && currentState)) ? 1 : 0;
}