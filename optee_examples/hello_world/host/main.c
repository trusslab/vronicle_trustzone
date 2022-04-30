/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <hello_world_ta.h>

static void usage(int argc, char *argv[], int error_code)
{
	const char *pname = "hello_world";

	if (argc)
		pname = argv[0];

	switch (error_code)
	{
	case 1:
		fprintf(stderr, "usage: %s <function_number_to_be_invoked> <resolution_w> <resolution_h> <num_of_frames> <with_sign_verify>\n", pname);
		break;
	
	default:
		fprintf(stderr, "usage: %s <function_number_to_be_invoked>\n", pname);
		break;
	}
	exit(1);
}

int main(int argc, char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_HELLO_WORLD_UUID;
	uint32_t err_origin;

	// Get arguments
	if (argc < 2) {
		warnx("Unexpected number of arguments %d (expected 1)",
		      argc - 1);
		usage(argc, argv, 0);
	}
	int function_to_invoke = atoi(argv[1]);

	// Check each arguemnts cases
	if (function_to_invoke != TA_HELLO_WORLD_CMD_CUSTOM_TEST && argc < 6) {
		warnx("Unexpected number of arguments %d (expected 4)",
		      argc - 1);
		usage(argc, argv, 1);
	}

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open a session to the "hello world" TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));

	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = 1280;
	op.params[0].value.b = 720;

	if (function_to_invoke != TA_HELLO_WORLD_CMD_CUSTOM_TEST) {
		op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_INOUT,
					 TEEC_NONE, TEEC_NONE);
		op.params[0].value.a = atoi(argv[2]);
		op.params[0].value.b = atoi(argv[3]);
		op.params[1].value.a = atoi(argv[4]);
		op.params[1].value.b = atoi(argv[5]);
		printf("op.params[0].value.a is set to: %d, \
			op.params[0].value.b is set to: %d, op.params[1].value.a is set to: %d, \
			op.params[1].value.b is set to: %d\n",
			op.params[0].value.a, op.params[0].value.b, op.params[1].value.a, op.params[1].value.b);
	}

	/*
	 * xxx is the actual function in the TA to be
	 * called.
	 */
	// printf("Invoking TA to customize %d\n", op.params[0].value.a);
	printf("(TEST9)Invoking TA with customized command %d\n", function_to_invoke);

	clock_t start, end;
    double cpu_time_used;

	start = clock();
	res = TEEC_InvokeCommand(&sess, function_to_invoke, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	// printf("TA customized value to %d\n", op.params[0].value.a);
	printf("TA execution completed, (possibly)result: %d, execution time: %f\n", op.params[0].value.a, cpu_time_used);

	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 *
	 * The TA will print "Goodbye!" in the log when the
	 * session is closed.
	 */

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}
