// <copyright file="Sha3_256.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	export class Sha3_256
	{
	public:
		/// <summary>
		/// Hash the input value
		/// </summary>
		static std::string HashBase64(std::string value)
		{
			const EVP_MD* algorithm = EVP_sha3_256();

			auto digestContext = EVP_MD_CTX_create();
			if (digestContext == nullptr)
				throw new std::runtime_error("EVP_MD_CTX_create() failed");

			// Initialize the digest engine
			if (EVP_DigestInit_ex(digestContext, algorithm, NULL) != 1)
			{
				EVP_MD_CTX_destroy(digestContext);
				throw new std::runtime_error("EVP_DigestInit_ex() failed");
			}

			// Pass the entire contents of the stream into the engine
			if (EVP_DigestUpdate(digestContext, value.data(), value.size()) != 1)
			{
				EVP_MD_CTX_destroy(digestContext);
				throw new std::runtime_error("EVP_DigestUpdate() failed");
			}

			// Allocate enough space for the digest
			unsigned int digestLength = EVP_MD_size(algorithm);
			auto digest = reinterpret_cast<unsigned char*>(OPENSSL_malloc(digestLength));
			if (digest == nullptr)
			{
				EVP_MD_CTX_destroy(digestContext);
				throw new std::runtime_error("OPENSSL_malloc() failed");
			}

			// Produce the digest
			if (EVP_DigestFinal_ex(digestContext, digest, &digestLength) != 1)
			{
				OPENSSL_free(digest);
				EVP_MD_CTX_destroy(digestContext);
				throw new std::runtime_error("EVP_DigestFinal_ex() failed");
			}

			// Base64 encode the result
			// unsigned int encodedLength = 4* ((digestLength + 2) / 3);
			// // +1 for the terminating null that EVP_EncodeBlock adds on
			// auto encodedDigest = reinterpret_cast<char*>(OPENSSL_malloc(encodedLength + 1));
			// if (encodedDigest == nullptr)
			// {
			// 	OPENSSL_free(digest);
			// 	EVP_MD_CTX_destroy(digestContext);
			// 	throw new std::runtime_error("OPENSSL_malloc() failed");
			// }

			// unsigned int outLength = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encodedDigest), reinterpret_cast<unsigned char*>(digest), digestLength);
			// if (encodedLength != outLength)
			// {
			// 	OPENSSL_free(digest);
			// 	EVP_MD_CTX_destroy(digestContext);
			// 	throw new std::runtime_error("EVP_EncodeBlock() Did not match length");
			// }

			// auto result = std::string(encodedDigest);

			std::stringstream result;
			result << std::hex;
			for (auto i = 0u; i < digestLength; i++)
				result << (int)digest[i];

			OPENSSL_free(digest);
			// OPENSSL_free(encodedDigest);
			EVP_MD_CTX_destroy(digestContext);

			return result.str();
		}
	};
}
