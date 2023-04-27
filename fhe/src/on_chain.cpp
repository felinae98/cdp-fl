#include "ci_int.hxx"
#include "tfhe_bit_exec.hxx"
#include <cdp_utils.hpp>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>

using namespace cingulata;
int main(int argc, char* argv[]) {
    // argv[1] n
    // argv[2-n+2] name
    CiContext::set_config(
        std::make_shared<decorator::Attach<TfheBitExec, decorator::Stat<IBitExecFHE>>>(
            "tfhe.sk", TfheBitExec::Secret),
        std::make_shared<IntOpGenSize>());
    int n = atoi(argv[1]);
    // char names[20][20];
    char grad_name[100];
    char rand_name[100];
    sprintf(grad_name, "%s-grad", argv[2]);
    sprintf(rand_name, "%s-rand", argv[2]);

    CiInt input_rand = CiInt{CiInt::u8};
    CiInt grad_sum = CiInt{CiInt::s32};
    input_rand.read(rand_name);
    grad_sum.read(grad_name);

    std::cout << grad_sum.decrypt().get_val() << "   " << int(input_rand.decrypt().get_val()) << std::endl;

    for (int i = 1; i < n; i++) {
        CiInt cur_rand = CiInt{CiInt::u8};
        CiInt cur_grad = CiInt{CiInt::s32};
        sprintf(grad_name, "%s-grad", argv[2+i]);
        sprintf(rand_name, "%s-rand", argv[2+i]);
        cur_rand.read(rand_name);
        cur_grad.read(grad_name);
        
        std::cout << "rand: " << int(cur_rand.decrypt().get_val()) << " grad: " << int(cur_grad.decrypt().get_val()) << std::endl;
        grad_sum += cur_grad;
        input_rand ^= cur_rand; 
        std::cout << grad_sum.decrypt().get_val() << "   " << int(input_rand.decrypt().get_val()) << std::endl;
    }

    std::cout << grad_sum.decrypt().get_val() << "   " << int(input_rand.decrypt().get_val()) << std::endl;

    auto mapped_rand = CiInt{CiInt::s32}; 
    CiInt one = CiInt(CiInt::u8);
    CiInt zero = CiInt(CiInt::u8);
    one.read("one");
    zero.read("zero"); 
    map_to_laplace(mapped_rand, input_rand, one, zero);
    std::cout << "frand: " << int32_t(mapped_rand.decrypt().get_val()) << std::endl;
    grad_sum += mapped_rand;
    std::cout << "frand: " << int32_t(mapped_rand.decrypt().get_val()) << " final: " << int32_t(grad_sum.decrypt().get_val()) << std::endl;
    grad_sum.write("final");
}
