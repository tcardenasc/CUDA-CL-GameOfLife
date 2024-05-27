__kernel void game_of_life(__global uchar* data, __global uchar* result, uint width, uint height) {
    uint worldSize = width * height;
    for (uint cellId = get_global_id(0); cellId < worldSize; cellId += get_global_size(0)) {
            uint x = cellId % width;
            uint yAbs = cellId - x;

            uint xLeft = (x + width - 1) % width;
            uint xRight = (x + 1) % width;

            uint yAbsUp = (yAbs + worldSize - width) % worldSize;
            uint yAbsDown = (yAbs + width) % worldSize;

            // Count alive cells.
            uint aliveCells = data[xLeft + yAbsUp] + data[x + yAbsUp] + data[xRight + yAbsUp]
                              + data[xLeft + yAbs] + data[xRight + yAbs]
                              + data[xLeft + yAbsDown] + data[x + yAbsDown] + data[xRight + yAbsDown];

            result[x + yAbs] = aliveCells == 3 || (aliveCells == 2 && data[x + yAbs]) ? 1 : 0;
    }
}