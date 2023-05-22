#include "ci_int.hxx"
#include "tfhe_bit_exec.hxx"
#include <cdp_utils.hpp>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace cingulata;
int main(int argc, char* argv[]) {
    // argv[1] name
    // argv[2] grad
    // argv[3] rand
      CiContext::set_config(
          std::make_shared<decorator::Attach<TfheBitExec, decorator::Stat<IBitExecFHE>>>(
              "tfhe.pk", TfheBitExec::Public),
          std::make_shared<IntOpGenSize>());

    if (argc < 4) { exit(255); }
    int grad = atoi(argv[2]);
    uint8_t rand = atoi(argv[3]);
    char grad_name[100];
    std::sprintf(grad_name, "%s-grad", argv[1]);
    char rand_name[100];
    sprintf(rand_name, "%s-rand", argv[1]);

    CiInt one = CiInt(CiInt::u8);
    CiInt zero = CiInt(CiInt::u8);
    one.read("one");
    zero.read("zero"); 

    auto grad_ci = CiInt(CiInt::s32);
    auto rand_ci = CiInt(CiInt::u8);

    scalar_mul(grad_ci, one, zero, grad, 32);
    scalar_mul(rand_ci, one, zero, rand);

    rand_ci.write(rand_name);
    grad_ci.write(grad_name);
    CiContext::get_bit_exec_t<decorator::Stat<IBitExecFHE>>()->print();
}
