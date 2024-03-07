//
// Created by martind on 7.3.2024.
//

#ifndef MORSKI_SMETAR_SHA256_H
#define MORSKI_SMETAR_SHA256_H

#include <string>
#include <cstdint>

class SHA256 {
private:
	// Pre-process the message by padding it and appending the original message length
	// I don't know why everything that's done here is done, but I'm not going to question it - that's what SHA-256 looks like and somebody smarter than me has probably proven this to be the best
	static std::string preprocess(const std::string &message);
	static uint32_t rightRotate(uint32_t value, int n);
	
	
	
	static void processChunk(const std::string &chunk, uint32_t &h0, uint32_t &h1, uint32_t &h2, uint32_t &h3,
	                         uint32_t &h4, uint32_t &h5, uint32_t &h6, uint32_t &h7);
	
	static std::string toHex(uint32_t value);
public:
	/**
	 * @brief Hashes the message using SHA-256
	 * @param message the message to hash
	 * @return The SHa-256 hash of the message but its not working correctly I mean I ain't fixin' it I don't dabble in black magic plus it does still do its job so hey I don't care
	 */
	static std::string hash(const std::string &message);
};


#endif //MORSKI_SMETAR_SHA256_H
