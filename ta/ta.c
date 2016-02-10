#define STR_TRACE_USER_TA "QUIZ"

#include <tee_ta_api.h>
#include <tee_api.h>
#include <tee_internal_api_extensions.h>
#include "ta_quiz.h"
#include <string.h>
#include <neolib.h>


/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
}

/* Declare Global variables */
int score;
int q_no;
int max_qs;
char questions[2][DEF_QUES_SIZE];
char answers[2][DEF_QUES_SIZE];
/*
 * Called By the OpenSessionEntryPoint to initialize the variables
 * sets score to 0, q_no to 0 and fetches number of questions
 */
 static void set_params(void){
 	score = 0;
 	q_no = 0;
 	max_qs = 2;
 	string_copy(questions[0],"Who is the first Indian to go to space ?");
 	string_copy(questions[1],"Which is the only mammal to have wings?");
 	string_copy(answers[0],"Rakesh Sharma");
 	string_copy(answers[1],"Bat");
 }

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param  params[4], void **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;
	set_params(); 
	/*
	 * The IMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	
	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	// IMSG("Secure World Exit!\n");
}

static TEE_Result check_answer(uint32_t param_types, TEE_Param params[4]){
	char *answer_text;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	
	answer_text = params[1].memref.buffer;
	IMSG("Entered Answer is %s ",answer_text);

	return TEE_SUCCESS;
}

/*
 * Function to initiate the quiz and fetch questions, takes care of iteration
 */
static TEE_Result start_quiz(uint32_t param_types, TEE_Param params[4])
{	
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_MEMREF_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	
	if( q_no < max_qs){
		char *plaintext = params[1].memref.buffer;
		params[0].value.a = 1;
		memcpy(plaintext,questions[q_no],sizeof(char) * DEF_QUES_SIZE);
		
	}else{
		params[0].value.a = 0;
	}
	q_no++;
	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_QUIZ_CMD_START_QUIZ:
		return start_quiz(param_types, params);
	case TA_QUIZ_CMD_CHECK_ANSWER:
		return check_answer(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
