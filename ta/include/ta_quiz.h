#ifndef TEMPLATE_TA_H
#define TEMPLATE_TA_H

/*
 * This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define TA_QUIZ_UUID { 0x11111111, 0x1111, 0x1111, \
	{ 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11} }

/* The Trusted Application Function ID(s) implemented by this TA */

#define DEF_QUES_SIZE 120
#define DEF_ANS_SIZE 40
#define TA_QUIZ_CMD_START_QUIZ 0
#define TA_QUIZ_CMD_CHECK_ANSWER 1

#endif
