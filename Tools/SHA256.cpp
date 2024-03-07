//
// Created by martind on 7.3.2024.
//

#include "SHA256.h"

std::string SHA256::preprocess(const std::string &message) {
	uint64_t orig_len = message.size() * 8; // It's in bits, hence the * 8
	std::string padded_message = message;
	
	padded_message += (char)0x80; // Append a '1' bit to the message
	
	while ((padded_message.size() * 8) % 512 != 448) {
		padded_message += (char)0x00; // Append zeros until the message length is congruent to 448 (mod 512)
	}
	
	for (size_t i = 0; i < 8; i++) {
		padded_message += (char)(orig_len >> (8 * (7 - i))); // Append the 64-bit original message length in big-endian
	}
	
	return padded_message;
}

uint32_t SHA256::rightRotate(uint32_t value, int n) {
	return (value >> n) | (value << (32 - n));
}

void SHA256::processChunk(const std::string &chunk, uint32_t &h0, uint32_t &h1, uint32_t &h2, uint32_t &h3, uint32_t &h4,
                     uint32_t &h5, uint32_t &h6, uint32_t &h7) {
	// Them constants are magic numbers, don't ask me why they are what they are
	const uint32_t K[] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
			0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
			0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
			0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
			0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
			0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	
	// I don't even know what's going on anymore
	uint32_t W[64];
	for (size_t i = 0; i < 16; i++) {
		W[i] = (uint32_t(chunk[i * 4]) << 24) | (uint32_t(chunk[i * 4 + 1]) << 16) |
		       (uint32_t(chunk[i * 4 + 2]) << 8) | (uint32_t(chunk[i * 4 + 3]));
	}
	
	// I'm not even going to try to understand this
	for (size_t i = 16; i < 64; ++i){
		uint32_t s0 = rightRotate(W[i - 15], 7) ^ rightRotate(W[i - 15], 18) ^ (W[i - 15] >> 3);
		uint32_t s1 = rightRotate(W[i - 2], 17) ^ rightRotate(W[i - 2], 19) ^ (W[i - 2] >> 10);
		W[i] = W[i - 16] + s0 + W[i - 7] + s1;
	}
	
	// More black magic
	uint32_t a = h0;
	uint32_t b = h1;
	uint32_t c = h2;
	uint32_t d = h3;
	uint32_t e = h4;
	uint32_t f = h5;
	uint32_t g = h6;
	uint32_t h = h7;
	
	// Even Voldemort himself doesn't understand this
	for (size_t i = 0; i < 64; ++i){
		uint32_t S1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + S1 + ch + K[i] + W[i];
		uint32_t S0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = S0 + maj;
		
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	
	// Like what even is this
	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
	h5 += f;
	h6 += g;
	h7 += h;
}

std::string SHA256::toHex(uint32_t value) {
	// I could try to understand this but my brain is already fried from the rest of the code
	std::string hex;
	for (size_t i = 0; i < 4; i++) {
		uint8_t byte = (value >> (24 - i * 8)) & 0xff;
		char hexDigits[] = "0123456789abcdef";
		hex += hexDigits[byte >> 4];
		hex += hexDigits[byte & 0x0f];
	}
	return hex;
}

std::string SHA256::hash(const std::string &message) {
	uint32_t h0, h1, h2, h3, h4, h5, h6, h7;
	h0 = 0x6a09e667;
	h1 = 0xbb67ae85;
	h2 = 0x3c6ef372;
	h3 = 0xa54ff53a;
	h4 = 0x510e527f;
	h5 = 0x9b05688c;
	h6 = 0x1f83d9ab;
	h7 = 0x5be0cd19;
	
	// Pre-processing
	std::string padded_message = preprocess(message);
	
	// Process the message in successive 512-bit chunks
	for (size_t i = 0; i < padded_message.size(); i += 64) {
		processChunk(padded_message.substr(i, 64), h0, h1, h2, h3, h4, h5, h6, h7);
	}
	
	// Concatenate the hash values
	std::string hash_result;
	hash_result.append(toHex(h0));
	hash_result.append(toHex(h1));
	hash_result.append(toHex(h2));
	hash_result.append(toHex(h3));
	hash_result.append(toHex(h4));
	hash_result.append(toHex(h5));
	hash_result.append(toHex(h6));
	hash_result.append(toHex(h7));
	
	return hash_result;
}
