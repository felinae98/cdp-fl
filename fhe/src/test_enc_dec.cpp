#include "ci_int.hxx"
#include "tfhe_bit_exec.hxx"
#include<cdp_utils.hpp>
#include <iostream>
#include <memory>

using namespace cingulata;

int main() {
      CiContext::set_config(
          std::make_shared<decorator::Attach<TfheBitExec, decorator::Stat<IBitExecFHE>>>(
              "tfhe.sk", TfheBitExec::Secret),
          std::make_shared<IntOpGenSize>());

    int mul = 127;
    auto one = CiInt{CiInt::u8};
    auto zero = CiInt{CiInt::u8};

    one.read("one");
    zero.read("zero");

    auto enced = CiInt(CiInt::u8);
    scalar_mul(enced, one, zero, mul);

    int deced_val = enced.decrypt().get_val();
    std::cout << int(deced_val) << std::endl;
}
