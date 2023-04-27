#include "ci_context.hxx"
#include "lwesamples.h"
#include "polynomials.h"
#include "tfhe_core.h"
#include "tfhe_gate_bootstrapping_functions.h"
#include "tfhe_io.h"
#include "tfhe_gate_bootstrapping_structures.h"
#include "thfhe.hpp"
#include "tlwe.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <vector>

const int num_menber = 3;

int main() {
    FILE *file = fopen("tfhe.sk", "rb");
    // auto key = new_tfheGateBootstrappingSecretKeySet_fromFile(skFile);
    // auto m_pk = const_cast<TFheGateBootstrappingCloudKeySet *>(
    //       &(key->cloud)); // no other easy way to avoid const cast
    // fclose(skFile);
    TFheGateBootstrappingSecretKeySet *m_sk = new_tfheGateBootstrappingSecretKeySet_fromFile(file);
    TFheGateBootstrappingCloudKeySet *m_pk = const_cast<TFheGateBootstrappingCloudKeySet *>(
            &(m_sk->cloud)); // no other easy way to avoid const cast
    auto param = m_pk->params;
    fclose(file);
    
    LweSample *ciphertext = new_gate_bootstrapping_ciphertext_array(32, param);
    char filename[20];
    for(int i = 0; i < 32; i++) {
        sprintf(filename, "final_%d", i);
        file = fopen(filename, "rb");
        import_gate_bootstrapping_ciphertext_fromFile(file, &ciphertext[i], param);
        fclose(file);
    }

    TorusPolynomial *partdec[32][num_menber];
    auto thtfhe = new ThFHE();
    thtfhe->SplitKey(m_sk, num_menber, num_menber);
    
    int32_t res = 0;
    for (int i = 0; i < 32; i++) {
        TLweParams *tlwe_params = new_TLweParams(1024, 1, 3e-8, 0.2);
        auto tlwe_ciphertext = new_TLweSample(tlwe_params);
        TLweFromLwe(tlwe_ciphertext, &ciphertext[i], tlwe_params);
        partdec[i][0] = new_TorusPolynomial(tlwe_params->N);
        partdec[i][1] = new_TorusPolynomial(tlwe_params->N);
        partdec[i][2] = new_TorusPolynomial(tlwe_params->N);
        std::vector<int> parties;
        for (int j = 1; j <= num_menber; j++) {
            parties.push_back(j);
        }

        for (int j = 0; j < num_menber; j++) {
            auto keyshare = new ThFHEKeyShare();
            thtfhe->GetShareSet(j+1, keyshare);
            keyshare->PartialDecrypt(tlwe_ciphertext, tlwe_params, partdec[i][j], parties, num_menber, num_menber, 0.0125);
        }

        int32_t bit_res = finalDecrypt(tlwe_ciphertext, partdec[i], tlwe_params, parties, num_menber, num_menber);
        bit_res <<= i;
        res |= bit_res;
    }
    std::cout << res << std::endl;
}
