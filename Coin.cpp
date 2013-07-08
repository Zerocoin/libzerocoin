/**
* @file       Coin.cpp
*
* @brief      PublicCoin and PrivateCoin classes for the Zerocoin library.
*
* @author     Ian Miers, Christina Garman and Matthew Green
* @date       June 2013
*
* @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
* @license    This project is released under the MIT license.
**/

#include <stdexcept>
#include "Zerocoin.h"

namespace libzerocoin {

//PublicCoin class
PublicCoin::PublicCoin(const Params* p):
        params(p), denomination(ZQ_LOVELACE) {
    if(this->params->initialized == false){
        throw std::invalid_argument("Params are not initialized");
    }
};

PublicCoin::PublicCoin(const Params* p, const Bignum& coin, const CoinDenomination d):
        params(p), value(coin), denomination(d) {
    if(this->params->initialized == false){
        throw std::invalid_argument("Params are not initialized");
    }
};

bool PublicCoin::operator==(const PublicCoin& rhs) const {
    return this->value == rhs.value;// FIXME check param equality
}

bool PublicCoin::operator!=(const PublicCoin& rhs) const {
    return !(*this == rhs);
}

const Bignum& PublicCoin::getValue() const {
    return this->value;
}

const CoinDenomination PublicCoin::getDenomination() const {
    return static_cast<CoinDenomination>(this->denomination);
}

bool PublicCoin::validate() const {
    return (this->params->accumulatorParams.minCoinValue < value) && (value < this->params->accumulatorParams.maxCoinValue) && value.isPrime(params->zkp_iterations);
}

//PrivateCoin class
PrivateCoin::PrivateCoin(const Params* p, const CoinDenomination denomination): params(p), publicCoin(p) {
    // Verify that the parameters are valid
    if(this->params->initialized == false){
        throw std::invalid_argument("Params are not initialized");
    }
    
    // Mint a new coin with a random serial number.
    this->mintCoin(denomination);
}

/**
 *
 * @return the coins serial number
 */
const Bignum& PrivateCoin::getSerialNumber() const {
    return this->serialNumber;
}

const Bignum& PrivateCoin::getRandomness() const {
    return this->randomness;
}

void PrivateCoin::mintCoin(const CoinDenomination denomination) {
    // Repeat this process up to MAX_COINMINT_ATTEMPTS times until
    // we obtain a prime number
    for(uint32_t attempt = 0; attempt < MAX_COINMINT_ATTEMPTS; attempt++){
        
        // Generate a random serial number in the range 0...{q-1} where
        // "q" is the order of the commitment group.
        Bignum s = Bignum::randBignum(this->params->coinCommitmentGroup.groupOrder);
                  
        // Generate a Pedersen commitment to the serial number "s"
        Commitment coin(&params->coinCommitmentGroup, s);

        // Now verify that the commitment is a prime number
        // in the appropriate range. If not, we'll throw this coin
        // away and generate a new one.
        if (coin.getCommitmentValue().isPrime() &&
            coin.getCommitmentValue() >= params->accumulatorParams.minCoinValue &&
            coin.getCommitmentValue() <= params->accumulatorParams.maxCoinValue) {
            // Found a valid coin. Store it.
            this->serialNumber = s;
            this->randomness = coin.getRandomness();
            this->publicCoin = PublicCoin(params,coin.getCommitmentValue(), denomination);

            // Success! We're done.
            return;
        }
    }
    
    // We only get here if we did not find a coin within
    // MAX_COINMINT_ATTEMPTS. Throw an exception.
    throw ZerocoinException("Unable to mint a new Zerocoin (too many attempts)");
}

const PublicCoin& PrivateCoin::getPublicCoin() const {
    return this->publicCoin;
}

} /* namespace libzerocoin */
