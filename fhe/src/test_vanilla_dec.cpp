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
    
    LweSample *ciphertext = new_gate_bootstrapping_ciphertext_array(8, param);
    char filename[20];
    for(int i = 0; i < 8; i++) {
        sprintf(filename, "one_%d", i);
        file = fopen(filename, "rb");
        import_gate_bootstrapping_ciphertext_fromFile(file, &ciphertext[i], param);
        fclose(file);
    }

    TorusPolynomial *partdec[3];
    auto thtfhe = new ThFHE();
    thtfhe->SplitKey(m_sk, 3, 3);
    
    TLweParams *tlwe_params = new_TLweParams(1024, 1, 3e-8, 0.2);
    auto tlwe_ciphertext = new_TLweSample(tlwe_params);
    TLweFromLwe(tlwe_ciphertext, ciphertext, tlwe_params);
    partdec[0] = new_TorusPolynomial(tlwe_params->N);
    partdec[1] = new_TorusPolynomial(tlwe_params->N);
    partdec[2] = new_TorusPolynomial(tlwe_params->N);
    std::vector<int> parties{1,2,3};

    auto keyshare1 = new ThFHEKeyShare();
    thtfhe->GetShareSet(1, keyshare1);
    keyshare1->PartialDecrypt(tlwe_ciphertext, tlwe_params, partdec[0], parties, 3, 3, 0.0125);

    auto keyshare2 = new ThFHEKeyShare();
    thtfhe->GetShareSet(2, keyshare2);
    keyshare2->PartialDecrypt(tlwe_ciphertext, tlwe_params, partdec[1], parties, 3, 3, 0.0125);
    
    auto keyshare3 = new ThFHEKeyShare();
    thtfhe->GetShareSet(3, keyshare3);
    keyshare3->PartialDecrypt(tlwe_ciphertext, tlwe_params, partdec[2], parties, 3, 3, 0.0125);

    auto res = finalDecrypt(tlwe_ciphertext, partdec, tlwe_params, parties, 3, 3);
    std::cout << res << std::endl;
}
