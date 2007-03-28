/*
 * MySQL User Defined Functions for FNV (Fowler/Noll/Vo) Hash
 * 
 * See also MySQL section 24.2 Adding New Functions to MySQL
 *
 * Copyright (c) 2007, Fraudwall Technologies. All rights reserved.
 *
 * udf_fnv.c	2007/03/07 19:48:09	r61
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 * - Neither the name of Fraudwall Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * To compile:
 * gcc -g -O3 -fPIC -I/usr/include/mysql -shared -o udf_fnv.so udf_fnv.c hash_32a.c hash_64a.c
 *
 * To load into MySQL:
 * 
 * DROP FUNCTION IF EXISTS fnv0_32;
 * DROP FUNCTION IF EXISTS fnv0_64;
 * DROP FUNCTION IF EXISTS fnv1_32;
 * DROP FUNCTION IF EXISTS fnv1_64;
 * DROP FUNCTION IF EXISTS fnv1a_32;
 * DROP FUNCTION IF EXISTS fnv1a_64;
 * CREATE FUNCTION fnv0_32 RETURNS STRING SONAME "udf_fnv.so";
 * CREATE FUNCTION fnv0_64 RETURNS STRING SONAME "udf_fnv.so";
 * CREATE FUNCTION fnv1_32 RETURNS STRING SONAME "udf_fnv.so";
 * CREATE FUNCTION fnv1_64 RETURNS STRING SONAME "udf_fnv.so";
 * CREATE FUNCTION fnv1a_32 RETURNS STRING SONAME "udf_fnv.so";
 * CREATE FUNCTION fnv1a_64 RETURNS STRING SONAME "udf_fnv.so";
 */

#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "fnv.h"

/* Prototypes */

my_bool fnv0_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv0_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);
my_bool fnv0_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv0_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);
my_bool fnv1_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv1_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);
my_bool fnv1_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv1_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);
my_bool fnv1a_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv1a_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);
my_bool fnv1a_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg);
char *fnv1a_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error);

/* Init */

my_bool fnv0_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV0_32() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 8;
    return 0;
}

my_bool fnv0_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV0_64() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 16;
    return 0;
}

my_bool fnv1_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV1_32() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 8;
    return 0;
}

my_bool fnv1_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV1_64() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 16;
    return 0;
}

my_bool fnv1a_32_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV1A_32() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 8;
    return 0;
}

my_bool fnv1a_64_init(UDF_INIT *initid, UDF_ARGS *args, char *msg)
{
    if (args->arg_count != 1)
    {
	strcpy(msg, "FNV1A_64() requires one argument");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;
    initid->maybe_null = 1;
    initid->max_length = 16;
    return 0;
}

/* Functions */

char *fnv0_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error)
{
    Fnv32_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_32_buf(args->args[0], args->lengths[0], FNV0_32_INIT);
    sprintf(result, "%08" PRIx32, (uint32_t) hval);
    *length = 8;
    return result;
}

char *fnv0_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error)
{
    Fnv64_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_64_buf(args->args[0], args->lengths[0], FNV0_64_INIT);
#if defined(HAVE_64BIT_LONG_LONG)
    sprintf(result, "%016" PRIx64, (uint64_t) hval);
#else
    sprintf(result, "%08" PRIx32 "%08" PRIx32,
	    (uint32_t) hval.w32[1], (uint32_t) hval.w32[0]);
#endif
    *length = 16;
    return result;
}


char *fnv1_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error)
{
    Fnv32_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_32_buf(args->args[0], args->lengths[0], FNV1_32_INIT);
    sprintf(result, "%08" PRIx32, (uint32_t) hval);
    *length = 8;
    return result;
}

char *fnv1_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	      unsigned long *length, char *is_null, char *error)
{
    Fnv64_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_64_buf(args->args[0], args->lengths[0], FNV1_64_INIT);
#if defined(HAVE_64BIT_LONG_LONG)
    sprintf(result, "%016" PRIx64, (uint64_t) hval);
#else
    sprintf(result, "%08" PRIx32 "%08" PRIx32,
	    (uint32_t) hval.w32[1], (uint32_t) hval.w32[0]);
#endif
    *length = 16;
    return result;
}


char *fnv1a_32(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error)
{
    Fnv32_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_32a_buf(args->args[0], args->lengths[0], FNV1_32A_INIT);
    sprintf(result, "%08" PRIx32, (uint32_t) hval);
    *length = 8;
    return result;
}

char *fnv1a_64(UDF_INIT *initid, UDF_ARGS *args, char *result,
	       unsigned long *length, char *is_null, char *error)
{
    Fnv64_t hval;

    if (args->args[0] == NULL)
    {
	*is_null = 1;
	return NULL;
    }

    hval = fnv_64a_buf(args->args[0], args->lengths[0], FNV1_64A_INIT);
#if defined(HAVE_64BIT_LONG_LONG)
    sprintf(result, "%016" PRIx64, (uint64_t) hval);
#else
    sprintf(result, "%08" PRIx32 "%08" PRIx32,
	    (uint32_t) hval.w32[1], (uint32_t) hval.w32[0]);
#endif
    *length = 16;
    return result;
}

