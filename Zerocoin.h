/**
* @file       Zerocoin.h
*
* @brief      Exceptions and constants for Zerocoin
*
* @author     Ian Miers, Christina Garman and Matthew Green
* @date       June 2013
*
* @copyright  Copyright 2013 Ian Miers, Christina Garman and Matthew Green
* @license    This project is released under the MIT license.
**/

#ifndef ZEROCOIN_H_
#define ZEROCOIN_H_

#include <stdexcept>

//#define ZEROCOIN_DEBUG                  1

#define ZEROCOIN_DEFAULT_SECURITYLEVEL      80
#define ZEROCOIN_MIN_SECURITY_LEVEL         80
#define ZEROCOIN_MAX_SECURITY_LEVEL         80
#define ACCPROOF_KPRIME                     160
#define ACCPROOF_KDPRIME                    128
#define MAX_COINMINT_ATTEMPTS               10000
#define ZEROCOIN_VERSION_STRING             "0.1"
#define ZEROCOIN_PROTOCOL_VERSION           "1"
#define HASH_OUTPUT_BITS                    256
#define ZEROCOIN_COMMITMENT_EQUALITY_PROOF  "COMMITMENT_EQUALITY_PROOF"
#define ZEROCOIN_ACCUMULATOR_PROOF          "ACCUMULATOR_PROOF"
#define ZEROCOIN_SERIALNUMBER_PROOF         "SERIALNUMBER_PROOF"
#define ZEROCOIN_THREADING 1

// Errors thrown by the Zerocoin library

class ZerocoinException : public std::runtime_error
{
public:
    explicit ZerocoinException(const std::string& str) : std::runtime_error(str) {}
};

#endif /* ZEROCOIN_H_ */
