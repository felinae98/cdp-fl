/* local includes */
#include <bit_exec/decorator/attach.hxx>
#include <bit_exec/decorator/stat.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <cstdint>
#include <int_op_gen/size.hxx>

// const std::uint64_t laplace_b = 515396075520u;
const std::uint64_t laplace_b = 6291456;
const int laplace_order = 3;
const int laplace_f = 128;

int scalar_mul(cingulata::CiInt &res, cingulata::CiInt one, cingulata::CiInt zero, std::int32_t multiplier, int size=8);
void map_to_laplace(cingulata::CiInt &res, cingulata::CiInt input, cingulata::CiInt one, cingulata::CiInt zero);
