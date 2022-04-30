#ifndef OPTEE_CRYPTO_H
#define OPTEE_CRYPTO_H

#define SIZE_OF_VEC(vec) (sizeof(vec) - 1)

/* RSA (NIST) */
uint8_t modulus[] = "\xa8\xd6\x8a\xcd\x41\x3c\x5e\x19\x5d\x5e\xf0\x4e\x1b\x4f\xaa\xf2"
		    "\x42\x36\x5c\xb4\x50\x19\x67\x55\xe9\x2e\x12\x15\xba\x59\x80\x2a"
		    "\xaf\xba\xdb\xf2\x56\x4d\xd5\x50\x95\x6a\xbb\x54\xf8\xb1\xc9\x17"
		    "\x84\x4e\x5f\x36\x19\x5d\x10\x88\xc6\x00\xe0\x7c\xad\xa5\xc0\x80"
		    "\xed\xe6\x79\xf5\x0b\x3d\xe3\x2c\xf4\x02\x6e\x51\x45\x42\x49\x5c"
		    "\x54\xb1\x90\x37\x68\x79\x1a\xae\x9e\x36\xf0\x82\xcd\x38\xe9\x41"
		    "\xad\xa8\x9b\xae\xca\xda\x61\xab\x0d\xd3\x7a\xd5\x36\xbc\xb0\xa0"
		    "\x94\x62\x71\x59\x48\x36\xe9\x2a\xb5\x51\x73\x01\xd4\x51\x76\xb5";

uint8_t public_exp[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		       "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03";

uint8_t private_exp[] = "\x1c\x23\xc1\xcc\xe0\x34\xba\x59\x8f\x8f\xd2\xb7\xaf\x37\xf1\xd3"
			"\x0b\x09\x0f\x73\x62\xae\xe6\x8e\x51\x87\xad\xae\x49\xb9\x95\x5c"
			"\x72\x9f\x24\xa8\x63\xb7\xa3\x8d\x6e\x3c\x74\x8e\x29\x72\xf6\xd9"
			"\x40\xb7\xba\x89\x04\x3a\x2d\x6c\x21\x00\x25\x6a\x1c\xf0\xf5\x6a"
			"\x8c\xd3\x5f\xc6\xee\x20\x52\x44\x87\x66\x42\xf6\xf9\xc3\x82\x0a"
			"\x3d\x9d\x2c\x89\x21\xdf\x7d\x82\xaa\xad\xca\xf2\xd7\x33\x4d\x39"
			"\x89\x31\xdd\xbb\xa5\x53\x19\x0b\x3a\x41\x60\x99\xf3\xaa\x07\xfd"
			"\x5b\x26\x21\x46\x45\xa8\x28\x41\x9e\x12\x2c\xfb\x85\x7a\xd7\x3b";

static int warp_asym_op(TEE_ObjectHandle key,
			TEE_OperationMode mode,
			uint32_t alg,
			TEE_Attribute *params,
			uint32_t paramCount,
			void *in_chunk,
			uint32_t in_chunk_len,
			void *out_chunk,
			uint32_t *out_chunk_len)
{
	TEE_Result ret = TEE_SUCCESS;
	TEE_OperationHandle handle = (TEE_OperationHandle)NULL;
	TEE_ObjectInfo info;

	TEE_GetObjectInfo(key, &info);

	ret = TEE_AllocateOperation(&handle, alg, mode, info.maxObjectSize);
	if (ret != TEE_SUCCESS) {
		DMSG("Failed to alloc operation handle : 0x%x", ret);
		goto err;
	}

	ret = TEE_SetOperationKey(handle, key);
	if (ret != TEE_SUCCESS) {
		DMSG("Failed to set key : 0x%x", ret);
		goto err;
	}

	if (mode == TEE_MODE_SIGN) {

		ret = TEE_AsymmetricSignDigest(handle, params, paramCount,
					       in_chunk, in_chunk_len, out_chunk, out_chunk_len);
		if (ret != TEE_SUCCESS) {
			DMSG("Sign failed : 0x%x", ret);
			goto err;
		}

	} else if (mode == TEE_MODE_VERIFY) {

		ret = TEE_AsymmetricVerifyDigest(handle, params, paramCount,
						 in_chunk, in_chunk_len, out_chunk, *out_chunk_len);
		if (ret == TEE_SUCCESS) {
			/* Do nothing */
		} else if (ret == TEE_ERROR_SIGNATURE_INVALID) {
			DMSG("Signature invalid");
			goto err;
		} else {
			DMSG("Verify failed : 0x%x", ret);
			goto err;
		}

	} else if (mode == TEE_MODE_ENCRYPT) {

		ret = TEE_AsymmetricEncrypt(handle, params, paramCount,
					    in_chunk, in_chunk_len, out_chunk, out_chunk_len);
		if (ret != TEE_SUCCESS) {
			DMSG("Encrypt failed : 0x%x", ret);
			goto err;
		}

	} else if (mode == TEE_MODE_DECRYPT) {

		ret = TEE_AsymmetricDecrypt(handle, params, paramCount,
					    in_chunk, in_chunk_len, out_chunk, out_chunk_len);
		if (ret != TEE_SUCCESS) {
			DMSG("Decrypt failed : 0x%x", ret);
			goto err;
		}

	} else {
		goto err;
	}

	TEE_FreeOperation(handle);
	return 0;

err:
	TEE_FreeOperation(handle);
	return 1;
}

static int calc_digest(uint32_t hash_alg,
		       void *msg,
		       uint32_t msg_len,
		       void *hash,
		       uint32_t *hash_len)
{
	TEE_OperationHandle operation = (TEE_OperationHandle)NULL;
	TEE_Result ret;

	ret = TEE_AllocateOperation(&operation, hash_alg, TEE_MODE_DIGEST, 0);
	if (ret != TEE_SUCCESS) {
		DMSG("Failed allocate digest operation");
		return 1;
	}

	ret = TEE_DigestDoFinal(operation, msg, msg_len, hash, hash_len);
	TEE_FreeOperation(operation);

	if (ret != TEE_SUCCESS) {
		DMSG("Final failed");
		return 1;
	}

	return 0;
}

#endif //OPTEE_CRYPTO_H