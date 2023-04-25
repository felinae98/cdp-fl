#pragma once

#include <map>
#include <tfhe.h>
#include <tfhe_io.h>
#include <lwe-functions.h>
#include <numeric_functions.h>
#include <tlwe_functions.h>

std::map<std::pair<int, int>, int> ncr_cacheT;	/* Stores <<n, r>: C(n, r)> */
std::map<std::pair<int, int>, TLweKey*> shared_key_repo;	/* Stores <<party_id, group_id>: key_share> */
