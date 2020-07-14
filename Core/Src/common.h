#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>

#define _TRUE	-1
#define _FALSE	0

#define _LOOPS_K12	1000U
#define BLOBSIZE	72
#define TARGETSIZE	8
#define JOBSIZE		(BLOBSIZE + TARGETSIZE)

#define RECEIVE_JOB(dptr, size)	\
	HAL_UART_Receive_IT(&huart2, dptr, size)

extern volatile uint32_t hash_ctr;
extern volatile uint32_t stat_hashrate;
extern volatile uint32_t nonce_ctr;
extern uint64_t nonce;
extern uint64_t target;
extern uint8_t blob[BLOBSIZE];
extern uint8_t job_buf[JOBSIZE];

struct flag_s
{
	uint8_t new_job;
	uint8_t print_stat;
};

extern struct flag_s flag;

void rtc_trig(void);
void send_nonce(uint64_t nonce);
void write_job(uint8_t *job_buf, uint8_t *blob, uint64_t *target);

extern void mine(uint64_t nonce, uint64_t target, unsigned char *blob);
extern void send_nonce(uint64_t nonce);
