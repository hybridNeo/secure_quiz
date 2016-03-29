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
#include <unistd.h>

struct question{
	char q[120],answer[40];
};



void dbg(char *inp){
	FILE *debug;
	debug = fopen("/data/data/com.example.rahulmahadev.myapplication/files/debug","a");
	fprintf(debug, "%s\n",inp );
	fflush(debug);
	fclose(debug);
					
}

/*
 * Function to send answer to the TEE
 */
int send_answer(TEEC_Session sess,char *answer,uint32_t err_origin){
	TEEC_Operation op;
	TEEC_Result res;
	memset(&op, 0 , sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,TEEC_MEMREF_TEMP_INPUT,TEEC_VALUE_OUTPUT,TEEC_NONE);
	op.params[1].tmpref.buffer = answer;
	op.params[1].tmpref.size = strlen(answer);
	dbg("bk1");
	res = TEEC_InvokeCommand(&sess, TA_QUIZ_CMD_CHECK_ANSWER, &op,&err_origin);
	dbg("bk1.5");
	if (res != TEEC_SUCCESS){
		errx(1, "TEEC_Send_Answer failed with code 0x%x origin 0x%x",res, err_origin);
		return -2;
	}
	if(op.params[0].value.a == 1){
		dbg("bk2");
		printf("\n----------------------- \nScore = %d %% \n-----------------------  \n",op.params[2].value.a );
		return op.params[2].value.a;
	}	
	return -1;
}


int main(void){
 	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op,init_vals;
	TEEC_UUID uuid = TA_QUIZ_UUID;
	char question[DEF_QUES_SIZE] = {0};
	uint32_t err_origin;
	//strt file read
	FILE *infile;
	infile = fopen("/system/bin/test.txt","r");
	int num = 0;			
	struct question temp;
	int i;
	while(fread(&temp,sizeof(struct question),1,infile) != 0){
		// printf("%s \n %s \n",temp.q,temp.answer);
		num++;
	}
	fseek(infile,0,SEEK_SET);	
	struct question ques[num];
	for(i =0 ; i < num;++i){
		fread(&ques[i],sizeof(struct question),1,infile);
	}
	//end of read from file
	memset(&init_vals,0,sizeof(init_vals));
	init_vals.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_MEMREF_TEMP_INPUT,TEEC_NONE,TEEC_NONE);
	init_vals.params[0].value.a = num;
	init_vals.params[1].tmpref.buffer = ques;
	init_vals.params[1].tmpref.size = sizeof(ques[0])*num;

	res = TEEC_InitializeContext(NULL, &ctx); // This basically checks if a TEE is present in the system
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);   
	/*
	 * Open a session to the  TA
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,TEEC_LOGIN_PUBLIC, NULL, &init_vals, &err_origin);
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
	printf("Hello\n");
	
	while(1){
		// dbg("start");
		FILE *infile,*outfile,*debug;
		infile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appout","r");
		outfile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appin","r");
		if(infile && outfile){

			char mode;
			fread(&mode,sizeof(char),1,infile);
			fclose(infile);
			fclose(outfile);
			if(mode == '1'){
				//fetch question
				res = TEEC_InvokeCommand(&sess, TA_QUIZ_CMD_START_QUIZ, &op,&err_origin);
				if (res != TEEC_SUCCESS)
					errx(1, "TEEC_Get_Question failed with code 0x%x origin 0x%x",res, err_origin);	
				if(op.params[0].value.a == 0){
					break;
				}else{
					printf("\n *[Question]: %s \n",question );
					infile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appout","w");
					fputs("0",infile );
					outfile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appin","w");
					fputs(question,outfile );
					fflush(infile);
					fflush(outfile);
					fclose(infile);
					fclose(outfile);
					// printf("[Expecting answer] \n");
					// char answer[DEF_QUES_SIZE];
					// fgets(answer,DEF_QUES_SIZE,stdin);
					// if ((strlen(answer)>0) && (answer[strlen (answer) - 1] == '\n'))
		        		// answer[strlen (answer) - 1] = '\0';
		        	// send_answer(sess,answer,err_origin);

					}
				}else if(mode == '2'){
					printf("Expecting answer\n");
					char answer[40]="unix";
					infile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appout","r");
					outfile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appin","w");
					// fseek(infile,2,SEEK_SET);
					// fread(answer,sizeof(char),40,infile);
					printf("Entered answer is %s \n",answer );
					//start dbg
					/*
					*/
					dbg("hi begin");
					//end dbg
					int ans_res = send_answer(sess,answer,err_origin);
					dbg("hi end");

					if(ans_res == -1){
						dbg("still remaining");
						fputs("3",outfile);
					}else if(ans_res == -2){
						dbg("error");
						fputs("Error",outfile);
					}else{
						// char str[45];
						// sprintf(str,"4\n%d",ans_res);
						dbg("else seg");
						fputs("4\n6",outfile);
						fclose(infile);
						fflush(outfile);
						fflush(infile);
						fclose(infile);
						fclose(outfile);
						goto exit;
					}
					fclose(infile);
					infile = fopen("/data/data/com.example.rahulmahadev.myapplication/files/appout","w");
					fputs("0",infile);

					fflush(infile);
					fflush(outfile);
					fclose(infile);
					fclose(outfile);
				}else if(mode == '0'){
					printf("Waiting for device\n");
				}
				else{
					printf("Waiting !!!\n");
				}
								
		}else{
			printf("Access denied\n");
		}

		sleep(1);
		

	}
	
	/*
	printf("********************** QUIZ **************************\n");
	while(1){
		res = TEEC_InvokeCommand(&sess, TA_QUIZ_CMD_START_QUIZ, &op,&err_origin);
		if (res != TEEC_SUCCESS)
			errx(1, "TEEC_Get_Question failed with code 0x%x origin 0x%x",res, err_origin);	
		if(op.params[0].value.a == 0){
			break;
		}else{
			printf("\n----------------------- \n Question:\n %s \n",question );
			printf(" Enter your anwer:\n");
			char answer[DEF_QUES_SIZE];
			fgets(answer,DEF_QUES_SIZE,stdin);
			if ((strlen(answer)>0) && (answer[strlen (answer) - 1] == '\n'))
        		answer[strlen (answer) - 1] = '\0';
        	send_answer(sess,answer,err_origin);

		}
		
	}	

	*/

	exit:
	printf("Quiz ended\n");
	// remove("/data/data/com.example.rahulmahadev.myapplication/files/appout");
	// remove("/data/data/com.example.rahulmahadev.myapplication/files/appin")
	/* 
	 * Close the session 
	 */
	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
	dbg("end");

 	return 0;
}
