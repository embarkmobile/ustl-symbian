#include <cmath>

float abs(float v) {
    return v > 0 ? v : -v;
}

int abs(int v) {
    return v > 0 ? v : -v;
}

long double fmaxl(long double x, long double y) {
    return x > y ? x : y;
}
