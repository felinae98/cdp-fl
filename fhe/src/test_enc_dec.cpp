#include "ci_int.hxx"
#include "tfhe_bit_exec.hxx"
#include<cdp_utils.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace cingulata;

int main(int argc, char* argv[]) {
    if (argc<2) {
        exit(255);
    }
      CiContext::set_config(
          std::make_shared<decorator::Attach<TfheBitExec, decorator::Stat<IBitExecFHE>>>(
              "tfhe.sk", TfheBitExec::Secret),
          std::make_shared<IntOpGenSize>());

    int mul = 127;
    auto one = CiInt{CiInt::u8};
    auto zero = CiInt{CiInt::u8};

    one.read("one");
    zero.read("zero");

    int i = atoi(argv[1]);

        auto enced = CiInt(CiInt::s32);
        auto enced_input = CiInt{i, 8, false}.encrypt();
        // enced_input.set_name("i");
        std::cout << int(enced_input.decrypt().get_val()) << std::endl;
        map_to_laplace(enced, enced_input, one, zero);
        int deced_val = enced.decrypt().get_val();
        std::cout << int(deced_val) << std::endl;
}
