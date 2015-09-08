#ifndef _Contouring_h_
#define _Contouring_h_


namespace HeightMap {


enum VertexZState
{
    VZS_Below = 0,
    VZS_Equal,
    VZS_Above,

    VZS_COUNT
};


const int vertex_layout_map[VZS_COUNT][VZS_COUNT][VZS_COUNT] = {
    { { 0, 0, 8 }, { 0, 2, 5 }, { 7, 6, 9 } },
    { { 0, 3, 4 }, { 1, 10, 1 }, { 4, 3, 0 } },
    { { 9, 6, 7 }, { 5, 2, 0 }, { 8, 0, 0 } }
};

/*
    vertex_layout_map[1][1][1] may be changed to the case
    determining the line along the rectangular side, i.e.
    if v2 is always the center point, it would be case 3.
*/


const unsigned int VERTEX_NUMBER = 4;

// bottom-right-relative sequence of vertex values obtaining
const int xShiftSequence[VERTEX_NUMBER] = { -1, 0, 0, -1 };
const int yShiftSequence[VERTEX_NUMBER] = { -1, -1, 0, 0 };

const int v1Sequence[VERTEX_NUMBER] = { 0, 1, 2, 3 };
// v2 is always the rectangle center
const int v3Sequence[VERTEX_NUMBER] = { 3, 0, 1, 2 };


inline VertexZState vertexZState(double v, int level)
{
    if (v < level)
        return VZS_Below;
    else if (v > level)
        return VZS_Above;
    else
        return VZS_Equal;
}


inline double intersect_pos(
    double p1,
    double val1,
    double p2,
    double val2,
    double intersectVal)
{
    return p1 + (p2 - p1) * (intersectVal - val1) / (val2 - val1);
}


inline int min_level(
    const int *levels,
    const int *const levels_end)
{
    if (levels == levels_end)
        return 0;

    int result = *levels;
    while (++levels != levels_end) {
        if (result > *levels)
            result = *levels;
    }
    return result;
}


inline int max_level(
    const int *levels,
    const int *const levels_end)
{
    if (levels == levels_end)
        return 0;

    int result = *levels;
    while (++levels != levels_end) {
        if (result < *levels)
            result = *levels;
    }
    return result;
}


} // namespace HeightMap


#endif // _Contouring_h_
