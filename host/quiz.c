/*
 * @Author - Rahul Mahadev
 * Quiz application to demonstrate Secure World execution
 * email - rahul.mahadev7@gmail.com
 */
#include <stdio.h>
#include <string.h>
#include <tee_client_api.h>
#include <ta_quiz.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function to send answer to the TEE
 */
void send_answer(TEEC_Session sess,char *answer,uint32_t err_origin){
	TEEC_Operation op;
	TEEC_Result res;
	memset(&op, 0 , sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,TEEC_MEMREF_TEMP_INPUT,TEEC_VALUE_OUTPUT,TEEC_NONE);
	op.params[1].tmpref.buffer = answer;
	op.params[1].tmpref.size = sizeof(char) * strlen(answer);
	res = TEEC_InvokeCommand(&sess, TA_QUIZ_CMD_CHECK_ANSWER, &op,&err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Send_Answer failed with code 0x%x origin 0x%x",res, err_origin);	

}
int main(void){
 	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_QUIZ_UUID;
	char question[DEF_QUES_SIZE] = {0};
	uint32_t err_origin;
	res = TEEC_InitializeContext(NULL, &ctx); // This basically checks if a TEE is present in the system
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);   
	/*
	 * Open a session to the  TA
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",res, err_origin);


	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	op.params[1].tmpref.buffer = question;
	op.params[1].tmpref.size  = sizeof(question);
	
	printf("********************** QUIZ **************************\n");
	while(1){
		res = TEEC_InvokeCommand(&sess, TA_QUIZ_CMD_START_QUIZ, &op,&err_origin);
		if (res != TEEC_SUCCESS)
			errx(1, "TEEC_Get_Question failed with code 0x%x origin 0x%x",res, err_origin);	
		if(op.params[0].value.a == 0){
			break;
		}else{
			printf("Question:\n %s \n",question );
			printf("Enter your anwer:\n");
			char answer[DEF_QUES_SIZE];
			fgets(answer,DEF_QUES_SIZE,stdin);
			if ((strlen(answer)>0) && (answer[strlen (answer) - 1] == '\n'))
        		answer[strlen (answer) - 1] = '\0';
        	send_answer(sess,answer,err_origin);

		}
		
	}	
	printf("Quiz ended\n");
	/* 
	 * Close the session 
	 */
	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);


 	return 0;
}
