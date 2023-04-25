#include <cdp_utils.hpp>

using namespace cingulata;
int scalar_mul(CiInt &res, CiInt one, CiInt zero, int multiplier) {
    res = CiInt(zero);
    CiInt tmp = CiInt{one};
    while(multiplier > 0) {
        if (multiplier & 1) {
            res += tmp;
        }
        tmp += tmp;
        multiplier >>= 1;
    }
    return 0;
}
