#include "print.h"
#include "nasmfunc.h"
#include "int.h"
#include "dsctbl.h"
#include "timer.h"
#include "memory.h"
#include "task.h"
#include "r8169.h"
#include "workqueue.h"
#include "ether.h"
#include "netdev.h"
#include "arp.h"
#include "icmp.h"

int *FONT_ADR;

void print_key(void *_ch) {
	uint8_t *ch = (uint8_t *)_ch;
	printstr_app("print_key: ");
	printhex_app(*ch);
	printstr_app("\n");
	mem_free1m((uint32_t) ch);
}

void int_keyboard(int *esp) {
	uint8_t *ch = (uint8_t *)mem_alloc(1);
	pic_sendeoi(KEYBOARD_IRQ);
	*ch = io_in8(0x0060);	// get input key code
	printstr_log("keyint\n");
	wq_push(print_key, ch);
	return;
}

int pitnum;
void int_pit(int *esp) {
	pic_sendeoi(PIT_IRQ);
	pitnum++;
	if(pitnum%200 == 0) {
		printstr_log("task_switch: ");
		task_show();
		task_switch();
	}
	return;
}

void task_b_main() {
	int i;
	while(1) {
		for(i = 0; i < 200000000; i++){
		}
		printstr_log("task_b_main\n");
	}
}

void task_c_main() {
	int i;
	while(1) {
		for(i = 0; i < 200000000; i++){
		}
		printstr_log("task_c_main\n");
	}
}

void kstart(void)
{
	initscreen();
	init_gdtidt();
	init_interrupt();
	mem_init();
	init_pit();
	wq_init();

	init_arptable();
	init_r8169();
	uint32_t ip_addr = (192 << 24) | (168 << 16) | (1 << 8) | 2;
	netdev_set_ip_addr(ip_addr);

	io_sti();

	struct TASK *task_a;
	struct TASK *task_b;
	struct TASK *task_c;
	task_a = task_init();
	task_b = task_alloc();
	task_b->tss.eip = (int) &task_b_main;
	task_run(task_b);

	task_c = task_alloc();
	task_c->tss.eip = (int) &task_c_main;
	task_run(task_c);

	wq_set_receiver(task_a);

	while(1){
		io_cli();
		if(wq_empty()) {
			printstr_log("task sleep\n");
			task_sleep();
			io_sti();
		} else {
			io_sti();
			wq_execute();
		}
	}
}
