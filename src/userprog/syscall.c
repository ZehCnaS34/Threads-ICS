#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <user/syscall.h>
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int arg[MAX_ARGS];
  check_valid_ptr((const void*) f->esp);
  int syscall_test = * (int *) f->esp;
  if(syscall_test == SYS_HALT){
      	halt();
  }
  else if(syscall_test == SYS_EXIT){
	get_arg(f, &arg[0], 1);
	exit(arg[0]);
  }
  else if(syscall_test == SYS_EXEC){  
	get_arg(f, &arg[0], 1);
	arg[0] = user_to_kernel_ptr((const void *) arg[0]);
	f->eax = exec((const char *) arg[0]); 
  }
   else if(syscall_test == SYS_WAIT){ 
	get_arg(f, &arg[0], 1);
	f->eax = wait(arg[0]);
   }
   else if(syscall_test == SYS_CREATE){
	get_arg(f, &arg[0], 2);
	arg[0] = user_to_kernel_ptr((const void *) arg[0]);
	f->eax = create((const char *)arg[0], (unsigned) arg[1]);
   }
    else if(syscall_test == SYS_REMOVE){
	get_arg(f, &arg[0], 1);
	arg[0] = user_to_kernel_ptr((const void *) arg[0]);
	f->eax = remove((const char *) arg[0]);
    }
    else if(syscall_test == SYS_OPEN){
	get_arg(f, &arg[0], 1);
	arg[0] = user_to_kernel_ptr((const void *) arg[0]);
	f->eax = open((const char *) arg[0]);
    }
    else if(syscall_test == SYS_FILESIZE){
	get_arg(f, &arg[0], 1);
	f->eax = filesize(arg[0]);
    }
    else if(syscall_test == SYS_READ){
	get_arg(f, &arg[0], 3);
	check_valid_buffer((void *) arg[1], (unsigned) arg[2]);
	arg[1] = user_to_kernel_ptr((const void *) arg[1]);
	f->eax = read(arg[0], (void *) arg[1], (unsigned) arg[2]);
    }
    else if(syscall_test == SYS_WRITE){
	get_arg(f, &arg[0], 3);
	check_valid_buffer((void *) arg[1], (unsigned) arg[2]);
	arg[1] = user_to_kernel_ptr((const void *) arg[1]);
	f->eax = write(arg[0], (const void *) arg[1], (unsigned) arg[2]);
    }
    else if(syscall_test == SYS_SEEK){
	get_arg(f, &arg[0], 2);
	seek(arg[0], (unsigned) arg[1]);
    } 
    else if(syscall_test == SYS_TELL){
	get_arg(f, &arg[0], 1);
	f->eax = tell(arg[0]);
    }
    else if(syscall_test == SYS_CLOSE){
	get_arg(f, &arg[0], 1);
	close(arg[0]);
    }
    }
}
static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}
