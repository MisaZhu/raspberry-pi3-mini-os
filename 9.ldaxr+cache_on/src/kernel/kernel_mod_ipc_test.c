#include <printf.h>
#include "fat.h"
#include "fs.h"
#include "pm.h"
#include "sched.h"

void ipc_test(void)
{
	
	printf("IPC_TEST Starts receiving messages....\n\r");
	/*Rendezvous Message-Passing or Mailbox Message-Passing*/
	while(1){	
		struct mailbox rev = recieve_msg(Rendezvous);
		printf("kernel_mod_ipc_test rev.msg:%d\n\r",rev.msg);	
	}
}

