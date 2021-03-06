#define STR_TRACE_USER_TA "QUIZ"

#include <tee_ta_api.h>
#include <tee_api.h>
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "ta_quiz.h"
#include <string.h>
#include <neolib.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXTIME 100
struct question{
	char q[120],answer[40];
};

static int dump_hash(uint8_t *hash, int len,char *arr)
{
	char strhash[40];
	int i;
	int dup[len];
	for(i = 0;i < len;++i){
		dup[i] = hash[i];
		DMSG("%d %x \n",i,dup[i] );
	}
	for( i =0; i < len; ++i){
		char temp[2];
		unsigned int loc;
		loc = dup[i];
		snprintf(temp,4,"%x",loc);
		if(strlen(temp) == 1){
			temp[1] = temp[0];
			temp[0] = '0';
		}
		DMSG("%d %x %s \n",i,loc,temp);
		strhash[i*2] = temp[0];
		strhash[(i*2)+1] = temp[1];
	}
	arr[40] = '\0';
	strhash[40] = '\0'; // MIGHT CAUSE SEG FAULT IN THE FUTURE (BAD CODE)
	DMSG("length %d %d \n",strlen(arr),strlen(strhash));
	
	DMSG("answer: %s\n",arr);
	DMSG("digest: %s\n",strhash);
	if(strcmp(strhash,arr) == 0){
		DMSG("%s","Correct");
		return 1;
	}else{
		DMSG("%s","Incorrect");
		return 0;
	}


}

/*
 *	Function to get one way hash value of the answer
 *	Using sha-1 algorithm
 */
static int call_gp_sha1_interface(char *str, int len,char *key)
{
	TEE_OperationHandle operation = NULL;
	TEE_Result res = TEE_ERROR_GENERIC;

	/* Temporary variable for the input (message to be hashed) */
	void *message = NULL;
	//uint8_t arr[20];
	size_t message_len = 0;
	uint8_t d[20];
	/* Temporary variables for the digest */
	void *digest = NULL;
	size_t digest_len = 0;

	DMSG("Calling GP Internal API for doing SHA1 hashing");
	message = str;
	message_len = len;


	digest = d;
	digest_len = 20;


	memset(digest, 0, digest_len);

	res = TEE_AllocateOperation(&operation, TEE_ALG_SHA1, TEE_MODE_DIGEST, 0);

	if (res != TEE_SUCCESS) {
		DMSG("TEE_AllocateOperation failed! res: 0x%x", res);
		goto out;
	}

	DMSG("message to hash: %s", (char *)message);
	/* Could (should?) be called, raised the question with GP */
	/* TEE_DigestUpdate(operation, (void *)text, message_len); */

	res = TEE_DigestDoFinal(operation, message, message_len, digest,
				&digest_len);

	if (res != TEE_SUCCESS) {
		DMSG("TEE_DigestDoFinal failed! res: 0x%x", res);
		goto out;
	}
	/*
	for(int i=0;i < 40;i +=2){
		char temp[2];
		temp[0] = key[i];
		temp[1] = key[i+1];
		arr[i/2] = strtol(temp,NULL,16);
		snprintf();
	}
	*/
	return dump_hash(digest, digest_len, key);

out:
	if (operation)
		TEE_FreeOperation(operation);

	return 0;
}

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
uint32_t start_time;
int max_qs;
char (*questions)[DEF_QUES_SIZE];


char (*answers)[DEF_QUES_SIZE];
/*
 * Called By the OpenSessionEntryPoint to initialize the variables
 * sets score to 0, q_no to 0 and fetches number of questions
 */
static void set_params(struct question* ques, int max){
 	int i;
 	score = 0;
 	q_no = 0;
 	max_qs = max;
 	DMSG("breakpoint alpha");
 	questions = malloc(max_qs * DEF_QUES_SIZE * sizeof(char ));
 	answers += (max_qs * DEF_QUES_SIZE *sizeof(char ));
 	answers = malloc(max_qs * DEF_QUES_SIZE * sizeof(char ));

 	for(i=0;i < max_qs;++i){
 		string_copy(questions[i],(*(ques+i)).q);
 		string_copy(answers[i],(*(ques+i)).answer);
 	}
 	for ( i = 0; i < max_qs; ++i)
 	{
 		DMSG("%s\n",answers[i]);
 	}
 	// string_copy(questions[0],(*(ques)).q);
 	for ( i = 0; i < max_qs; ++i)
 	{
 		DMSG("\n%s\n",answers[i]);
 	}
 	// string_copy(questions[1],"Which is the only mammal to have wings?");
 	// string_copy(answers[0],"Rakesh Sharma");
 	// string_copy(answers[1],"Bat");
 }


/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param  params[4], void **sess_ctx)
{	struct question *ques;

	TEE_Time time; 
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
						   TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	
	/* Unused parameters */
	(void)&sess_ctx;
	TEE_GetSystemTime(&time);
	start_time = time.seconds;
	IMSG("Start Time is %d \n",start_time);
	ques = params[1].memref.buffer;
	IMSG("%d %s \n ",params[0].value.a,(*(ques+1)).q);
	set_params(ques,params[0].value.a); 
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
	TEE_Time time;
	char *answer_text;
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	TEE_GetSystemTime(&time);
	if((time.seconds - start_time) >= MAXTIME){
		IMSG("Time up");
		q_no = max_qs;
		goto skip_check;	
	}
	answer_text = params[1].memref.buffer;
	answer_text[params[1].memref.size] = '\0';
	IMSG("Entered Answer is %s.",answer_text);
	IMSG("DB Answer is %s.",answers[q_no-1]);
	
	if(call_gp_sha1_interface(answer_text,strlen(answer_text),answers[q_no-1]) == 1){

		score++;
	}
	skip_check:
	if(q_no == max_qs){
		TEE_ObjectHandle o;
		TEE_Result file_op;
		char send_val[1] = {"0"};
		char str[5] = "hello";
		params[2].value.a = ((float)score/(float)max_qs ) * 100;
		params[0].value.a = 1;
		send_val[0] += params[2].value.a;
		file_op = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE,str,strlen(str),TEE_DATA_FLAG_ACCESS_WRITE |
			  TEE_DATA_FLAG_ACCESS_READ |
			  TEE_DATA_FLAG_ACCESS_WRITE_META,0,send_val,strlen(send_val),&o);
		if(file_op == TEE_SUCCESS){
			DMSG("Object created successfully\n");
		}
	}else{
		params[0].value.a = 0;
	}


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
		DMSG("plain: %s \nafter:%s \n\n",plaintext,questions[q_no]);
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
