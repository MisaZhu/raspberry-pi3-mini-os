#include "fork.h"
#include "printf.h"
#include "utils.h"
#include "sched.h"
#include "mm.h"
#include "mini_uart.h"
#include "led_blink.h"
#include "sys.h"
#include "threadtype.h"
#include "fs.h"
#include "thread.h"
#include "pm.h"
extern struct mailbox user_ipc_mail[mail_size];
int add_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg);
thread_t thread_id_self(void);
int _thread_join (thread_t thread, void **status);
void signal(thread_t thread);


extern unsigned long user_page_start;


void kservice_uart_write(char *fmt, ...){	
	printf(fmt);
}


int  kservice_fork(){
	return copy_process(FORK_THREAD, 0, 0, 0);
}

void  kservice_exit(void *status){
	_thread_exit (status);
}

void  kservice_led_blink(void){
#ifdef GPIO
	led_blink();
#endif
}

char  kservice_uart_read(void){
	return uart_recv();
}

int  kservice_create_thread(thread_t *thread, const struct thread_attr_t *attr,void * (*start_routine)(void *),void* arg){
	return _thread_create(thread, attr, start_routine, arg);
}

thread_t  kservice_thread_self(void){
	return thread_id_self();
}

int  kservice_thread_join (thread_t thread, void **value_ptr ){
	_thread_join(thread,value_ptr);
	return 0;
}

void  kservice_thread_exit (thread_t thread){
	
}

void  kservice_thread_signal(thread_t thread){
	_thread_cond_signal(thread);
}

void  kservice_list_file(void){
	list();
}

void  kservice_cd_folder(char* file_name){
	cd(file_name);
}

void  kservice_dump_file(char* file_name){
	dump(file_name);
}

void  kservice_root_file(char* file_name){
	cd_root();
}

void  kservice_run_file(char* file_name){
	run_file(file_name);
}

/*ipc_*/
int ipc_index_push =0;
void kservice_send_msg(unsigned int type, int pid, void* msg){
	user_ipc_mail[ipc_index_push].dst_task = pid;/*pid?*/
	user_ipc_mail[ipc_index_push].from = current;
	user_ipc_mail[ipc_index_push].msg = msg;
	user_ipc_mail[ipc_index_push++].letter_type = type;/*0:empty*/
	if(ipc_index_push == mail_size){ipc_index_push=0;}
	accept_reply();	
}

struct mailbox kservice_recieve_msg(unsigned int ipc_type){
	return recieve_msg(ipc_type);
}

void  kservice_mutex_trylock(struct thread_mutex *mutex){
	_thread_mutex_trylock(mutex);
}

void  kservice_mutex_lock(struct thread_mutex *mutex){
	_thread_mutex_lock(mutex);
}

void  kservice_mutex_unlock(struct thread_mutex *mutex){
	_thread_mutex_unlock(mutex);
}

void  kservice_com_file(char* file_name){
	compt_file(file_name);
}

unsigned long  kservice_allocate_upage(){
	return allocate_user_page((struct task_struct *)&(current->cpu_context->x19),0);
}

void kservice_free_page(unsigned long p){
	free_page(p);
}


void  kservice_rm_compt(char* compt_name){
	rm_compt_file(compt_name);
}

struct mm_info kservice_allocate_kpage(int count){
	return allocate_kernel_page(count);
}

void  kservice_schedule(){
	schedule();
}

int  kservice_reg_compt(char* compt_name){
	return reg_compt(compt_name);
}

int kservice_unreg_compt(char* compt_name){
	return unreg_compt(compt_name);
}

void kservice_ls_dev(void){
	ls_dev();
}

void kservice_ls_compt(void){
	ls_compt();
}

void  kservice_change_sched(char* file_name){
	send_msg(Change_Sched,thread_id_self(), file_name);
}


int kservice_dev_read(int dev,unsigned int lba, unsigned char *buffer, unsigned int num){	
		
	switch(dev){
		case 1:
			return sd_readblock(lba, buffer, num);
		default:
			return 0;
	}
	
}

void kservice_dir_interface(unsigned int* addr){
	fat_listdirectory(addr);
}

extern unsigned char _end;

unsigned char* kservice_end(void){
	return &_end;
}



void * const sys_call_table[] = {kservice_uart_write, /*0*/
kservice_fork, /*1*/
kservice_exit, /*2*/
kservice_led_blink, /*3*/
kservice_uart_read, /*4*/  
kservice_create_thread, /*5*/
kservice_thread_self, /*6*/
kservice_thread_join, /*7*/
kservice_thread_exit, /*8*/
kservice_thread_signal, /*9*/
kservice_list_file, /*10*/
kservice_cd_folder, /*11*/
kservice_dump_file, /*12*/
kservice_root_file, /*13*/
kservice_run_file, /*14*/
kservice_send_msg, /*15*/
kservice_recieve_msg, /*16*/
kservice_mutex_trylock, /*17*/
kservice_mutex_lock, /*18*/
kservice_mutex_unlock, /*19*/
kservice_com_file, /*20*/
kservice_allocate_upage, /*21*/
kservice_free_page, /*22*/
kservice_rm_compt, /*23*/
kservice_ls_dev, /*24*/
kservice_ls_compt, /*25*/
kservice_change_sched,/*26*/
/*below for symbol table*/
kservice_allocate_kpage, /*27*/
kservice_schedule, /*28*/
kservice_reg_compt, /*29*/
kservice_unreg_compt, /*30*/
kservice_dev_read, /*31*/
kservice_dir_interface, /*31*/
}; 
