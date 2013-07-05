/**
 * @file       Commitment.cpp
 *
 * @brief      Commitment and CommitmentProof classes for the Zerocoin library.
 *
 * @author     Ian Miers, Christina Garman and Matthew Green
 * @date       June 2013
 *
 * @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
 * @license    This project is released under the MIT license.
 **/

#include "Commitment.h"
#include "bitcoin_bignum/hash.h"

namespace libzerocoin {

//Commitment class
Commitment::Commitment::Commitment(const IntegerGroupParams* p,
                                   const Bignum& value): params(p), contents(value) {
    this->randomness = Bignum::randBignum(params->groupOrder);
    this->commitmentValue = (params->g.pow_mod(this->contents, params->modulus) *
                             params->h.pow_mod(this->randomness, params->modulus)) % params->modulus;
}

const Bignum& Commitment::getCommitmentValue() const {
    return this->commitmentValue;
}

const Bignum& Commitment::getRandomness() const {
    return this->randomness;
}

const Bignum& Commitment::getContents() const {
    return this->contents;
}

//CommitmentProofOfKnowledge class
CommitmentProofOfKnowledge::CommitmentProofOfKnowledge(const IntegerGroupParams* ap, const IntegerGroupParams* bp): ap(ap), bp(bp) {}

// TODO: get parameters from the commitment group
CommitmentProofOfKnowledge::CommitmentProofOfKnowledge(const IntegerGroupParams* aParams,
        const IntegerGroupParams* bParams, const Commitment& a, const Commitment& b):
        ap(aParams),bp(bParams) {
    Bignum r1;
    
    // First: make sure that the two commitments have the
    // same contents.
    if(a.getContents() != b.getContents()){
        throw std::invalid_argument("Both commitments must contain the same value");
    }
    
    // In order to ensure statistical zero knowledge, we pick "r1" out of the
    // largest possible range. In this case, the smaller of the two group orders.
    if(this->ap->groupOrder < this->bp->groupOrder){
        r1 = Bignum::randBignum(ap->groupOrder);
    }else{
        r1 = Bignum::randBignum(bp->groupOrder);
    }
    
    // Generate two random, ephemeral commitments "T1, T2" to "r1" under the two different
    // sets of commitment parameters.
    Commitment t1(aParams, r1);
    Commitment t2(bParams, r1);
    Bignum T1 = t1.getCommitmentValue();
    Bignum T2 = t2.getCommitmentValue();
    
    // Now hash commitment "A" with commitment "B" as well as the
    // parameters and the two ephemeral commitments "T1, T2" we just generated
    this->challenge = calculateChallenge(a.getCommitmentValue(), b.getCommitmentValue(), T1, T2);
    
    // Let "m" be the contents of the commitments. We'll implicitly define
    // A =  g1^m  * h1^x  mod p1
    // B =  g2^m  * h2^y  mod p2
    // T1 = g1^r1 * h1^r2 mod p1
    // T2 = g2^r1 * h2^r3 mod p2
    //
    // Now compute:
    //  S1 = r1 + (m * challenge)
    //  S2 = r2 + (x * challenge)
    //  S3 = r3 + (y * challenge)
    S1 = t1.getContents() + (a.getContents() * challenge);
    S2 = t1.getRandomness() + (a.getRandomness() * challenge);
    S3 = t2.getRandomness() + (b.getRandomness() * challenge);
    
    // We're done. The proof is S1, S2, S3 and "challenge".
}

bool CommitmentProofOfKnowledge::Verify(const Bignum& A, const Bignum& B) const
{
    // TODO: First verify that the values
    // S1, S2 and S3 and "challenge" are in the correct ranges
    if((this->challenge < Bignum(0)) || (this->challenge > (Bignum(2).pow(256) - Bignum(1)))){
        return false;
    }
    
    // Compute T1 = g1^S1 * h1^S2 * inverse(A^{challenge}) mod p1
    Bignum T1 = A.pow_mod(this->challenge, ap->modulus).inverse(ap->modulus).mul_mod(
                                                                         (ap->g.pow_mod(S1, ap->modulus).mul_mod(ap->h.pow_mod(S2, ap->modulus), ap->modulus)),
                                                                         ap->modulus);
    
    // Compute T2 = g2^S1 * h2^S3 * inverse(B^{challenge}) mod p2
    Bignum T2 = B.pow_mod(this->challenge, bp->modulus).inverse(bp->modulus).mul_mod(
                                                                         (bp->g.pow_mod(S1, bp->modulus).mul_mod(bp->h.pow_mod(S3, bp->modulus), bp->modulus)),
                                                                         bp->modulus);
    
    // Hash T1 and T2 along with all of the public parameters
    Bignum computedChallenge = calculateChallenge(A, B, T1, T2);
    
    // Return success if the computed challenge matches the incoming challenge
    if(computedChallenge == this->challenge){
        return true;
    }
    
    // Otherwise return failure
    return false;
}

const Bignum CommitmentProofOfKnowledge::calculateChallenge(const Bignum& a, const Bignum& b, const Bignum &commitOne, const Bignum &commitTwo) const {
    CHashWriter hasher(0,0);
    
    hasher << std::string(ZEROCOIN_COMMITMENT_EQUALITY_PROOF);
    hasher << commitOne;
    hasher << std::string("||");
    hasher << commitTwo;
    hasher << std::string("||");
    hasher << a;
    hasher << std::string("||");
    hasher << b;
    hasher << std::string("||");
    hasher << *(this->ap);
    hasher << std::string("||");
    hasher << *(this->bp);
    
    return Bignum(hasher.GetHash());
}    
    
} /* namespace libzerocoin */
