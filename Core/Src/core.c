#include "common.h"

void rtc_trig(void)
{
	flag.print_stat = _TRUE;
	stat_hashrate = hash_ctr / 60;
	hash_ctr=0;
}

void send_nonce(uint64_t nonce)
{
	printf("SHARE: %016llx\r\n", nonce);
	nonce_ctr++;
}

void write_job(uint8_t *job_buf, uint8_t *blob, uint64_t *target)
{
	memcpy(blob, job_buf, BLOBSIZE);
	memcpy(target, job_buf, TARGETSIZE);
}
