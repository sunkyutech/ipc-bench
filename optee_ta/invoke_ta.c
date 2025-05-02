#include <assert.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <invoke_ta.h>

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
				    TEE_Param __maybe_unused params[4],
				    void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __unused *sess_ctx)
{
}

static TEE_Result setup_pid(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result tee_res = TEE_SUCCESS;

	uint32_t exp_param_types = 
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT, TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) 
    	return TEE_ERROR_BAD_PARAMETERS;

	// receive CA PID
	pid_t ca_pid = params[0].value.a;

	// send back TA PID
	pid_t ta_pid = getpid();
	params[0].value.b = ta_pid;

	return tee_res;
}

static TEE_Result setup_fd(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result tee_res = TEE_SUCCESS;
	TEE_UUID uuid = PLUGIN_UUID;

	uint32_t exp_param_types = 
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
        		TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) 
    	return TEE_ERROR_BAD_PARAMETERS;

	tee_res = tee_invoke_supp_plugin(&uuid, FDPASSING_CMD, NULL,
					NULL, NULL, NULL);
	if (tee_res) {
		EMSG("invoke plugin failed fdpassing with code 0x%x", tee_res);
	}

	return tee_res;
}

static TEE_Result setup_shm(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result tee_res = TEE_SUCCESS;

	return tee_res;
}

static TEE_Result invoke_main(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result tee_res = TEE_SUCCESS;

	uint32_t exp_param_types = 	
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_NONE,
        		TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

    if (param_types != exp_param_types) 
    	return TEE_ERROR_BAD_PARAMETERS;
	
    char *argv_buf = params[0].memref.buffer;
    size_t argv_buf_size = params[0].memref.size;

    // char *envp_buf = params[1].memref.buffer;
    // size_t envp_buf_size = params[1].memref.size;

    int argc = 0;
    char *p = argv_buf;
    while (p < argv_buf + argv_buf_size) {
        if (*p == '\0') break;
        argc++;
        p += strlen(p) + 1;
    }

    char **argv = TEE_Malloc((argc + 1) * sizeof(char *), 0);
    if (!argv)
        return TEE_ERROR_OUT_OF_MEMORY;

    p = argv_buf;
    for (int i = 0; i < argc; i++) {
        argv[i] = p;
        p += strlen(p) + 1;
    }
    argv[argc] = NULL;

    // int envc = 0;
    // char *q = envp_buf;
    // while (q < envp_buf + envp_buf_size) {
    //     if (*q == '\0') break;
    //     envc++;
    //     q += strlen(q) + 1;
    // }

    // char **envp = TEE_Malloc((envc + 1) * sizeof(char *), 0);
    // if (!envp)
    //     return TEE_ERROR_OUT_OF_MEMORY;

    // q = envp_buf;
    // for (int i = 0; i < envc; i++) {
    //     envp[i] = q;
    //     q += strlen(q) + 1;
    // }
    // envp[envc] = NULL;

	main(argc, argv);
	
	TEE_Free(argv);
	// TEE_Free(envp);
	
	return tee_res;
}


TEE_Result TA_InvokeCommandEntryPoint(void __unused *sess_ctx,
				      uint32_t cmd_id, uint32_t param_types,
				      TEE_Param __unused params[4])
{
	switch (cmd_id) {
		case SETUP_PID:
			return setup_pid(param_types, params);
		case SETUP_FD:
			return setup_fd(param_types, params);
		case SETUP_SHM:
			return setup_shm(param_types, params);
		case TA_MAIN:
			return invoke_main(param_types, params);
		default:
			return TEE_ERROR_NOT_SUPPORTED;
	}
    return 0;
}
