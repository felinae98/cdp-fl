#include "ci_fncs.hxx"
#include "ci_int.hxx"
#include <cdp_utils.hpp>
#include <cstdint>
#include <iostream>

using namespace cingulata;
int scalar_mul(CiInt &res, CiInt one, CiInt zero, int32_t multiplier, int size=8) {
    res = CiInt(zero);
    res = res.resize(size);
    CiInt tmp = CiInt(one.cast(size));
    tmp = tmp.resize(size);
    while(multiplier > 0) {
        // std::cout << "mul res " << res.decrypt().get_val() << " tmp " << tmp.decrypt().get_val() << std::endl;
        if (multiplier & 1) {
            res += tmp;
        }
        tmp += tmp;
        multiplier >>= 1;
    }
    return 0;
}

void gen_laplace_coes(uint32_t coes[]) {
    for (int i = 1; i <= laplace_order; i++) {
        coes[i-1] = laplace_b / (i * uint32_t(pow(laplace_f, i)));
    }
}

void map_to_laplace(CiInt &res, CiInt input, CiInt one, CiInt zero) {
    auto signed_input = input.to_signed();
    auto is_positive = signed_input.sign();
    auto abs_input = input << 1;
    abs_input >>= 1;
    abs_input = CiInt(abs_input.cast(32));
    // std::cout << abs_input.decrypt().get_val() << std::endl;
    auto tmp = CiInt{one.cast(32)};
    uint32_t coefs[laplace_order];
    gen_laplace_coes(coefs);
    for (int i = 1; i <= laplace_order; i++) {
        tmp *= abs_input;
        CiInt coef{CiInt::u32};
        std::cout << coefs[i-1] << std::endl;
        scalar_mul(coef, one, zero, coefs[i-1], 32);
        auto add_temp = CiInt{CiInt::u32};
        add_temp = coef * tmp;
        res += add_temp;
        std::cout << "i: " << i <<" tmp: " << tmp.decrypt().get_val() << " coef: " << coef.decrypt().get_val() << " to_add: " << add_temp.decrypt().get_val() << " res: " << res.decrypt().get_val() << std::endl;
    }
    res = select(is_positive, res, -res);
}
