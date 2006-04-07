/*
Linux Real Mode Interface - A library of DPMI-like functions for Linux.

Copyright (C) 1998 by Josh Vanderhoof

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL JOSH VANDERHOOF BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#if defined(__i386__) && (defined(__linux__) || defined(__NetBSD__) \
	|| defined(__FreeBSD__) || defined(__OpenBSD__))

#include <stdio.h>
#include <string.h>

#if defined(__linux__)

#include <asm/vm86.h>
#include <signal.h>

#ifdef USE_LIBC_VM86
#include <sys/vm86.h>
#endif

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)

#include <sys/param.h>
#include <signal.h>
#include <setjmp.h>
#include <machine/psl.h>
#include <machine/vm86.h>
#include <machine/sysarch.h>

#endif /* __NetBSD__ || __FreeBSD__ || __OpenBSD__ */

#if defined(__FreeBSD__)
#include <sys/ucontext.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "lrmi.h"

#define REAL_MEM_BASE 	((void *)0x10000)
#define REAL_MEM_SIZE 	0x40000
#define REAL_MEM_BLOCKS 	0x100

struct mem_block {
	unsigned int size : 20;
	unsigned int free : 1;
};

static struct {
	int ready;
	int count;
	struct mem_block blocks[REAL_MEM_BLOCKS];
} mem_info = { 0 };

static int
read_file(char *name, void *p, size_t n)
{
	int fd;

	fd = open(name, O_RDONLY);

	if (fd == -1) {
		perror("open");
		return 0;
	}

	if (read(fd, p, n) != n) {
		perror("read");
		close(fd);
		return 0;
	}

	close(fd);

	return 1;
}

static int
map_file(void *start, size_t length, int prot, int flags, char *name, long offset)
{
	void *m;
	int fd;

	fd = open(name, (flags & MAP_SHARED) ? O_RDWR : O_RDONLY);

	if (fd == -1) {
		perror("open");
		return 0;
	}

	m = mmap(start, length, prot, flags, fd, offset);

	if (m == (void *)-1) {
		perror("mmap");
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}

static int
real_mem_init(void)
{
	if (mem_info.ready)
		return 1;

	if (!map_file((void *)REAL_MEM_BASE, REAL_MEM_SIZE,
	 PROT_READ | PROT_WRITE | PROT_EXEC,
	 MAP_FIXED | MAP_PRIVATE, "/dev/zero", 0))
		return 0;

	mem_info.ready = 1;
	mem_info.count = 1;
	mem_info.blocks[0].size = REAL_MEM_SIZE;
	mem_info.blocks[0].free = 1;

	return 1;
}

static void
real_mem_deinit(void)
{
	if (mem_info.ready) {
		munmap((void *)REAL_MEM_BASE, REAL_MEM_SIZE);
		mem_info.ready = 0;
	}
}


static void
insert_block(int i)
{
	memmove(
	 mem_info.blocks + i + 1,
	 mem_info.blocks + i,
	 (mem_info.count - i) * sizeof(struct mem_block));

	mem_info.count++;
}

static void
delete_block(int i)
{
	mem_info.count--;

	memmove(
	 mem_info.blocks + i,
	 mem_info.blocks + i + 1,
	 (mem_info.count - i) * sizeof(struct mem_block));
}

void *
LRMI_alloc_real(int size)
{
	int i;
	char *r = (char *)REAL_MEM_BASE;

	if (!mem_info.ready)
		return NULL;

	if (mem_info.count == REAL_MEM_BLOCKS)
		return NULL;

	size = (size + 15) & ~15;

	for (i = 0; i < mem_info.count; i++) {
		if (mem_info.blocks[i].free && size < mem_info.blocks[i].size) {
			insert_block(i);

			mem_info.blocks[i].size = size;
			mem_info.blocks[i].free = 0;
			mem_info.blocks[i + 1].size -= size;

			return (void *)r;
		}

		r += mem_info.blocks[i].size;
	}

	return NULL;
}


void
LRMI_free_real(void *m)
{
	int i;
	char *r = (char *)REAL_MEM_BASE;

	if (!mem_info.ready)
		return;

	i = 0;
	while (m != (void *)r) {
		r += mem_info.blocks[i].size;
		i++;
		if (i == mem_info.count)
			return;
	}

	mem_info.blocks[i].free = 1;

	if (i + 1 < mem_info.count && mem_info.blocks[i + 1].free) {
		mem_info.blocks[i].size += mem_info.blocks[i + 1].size;
		delete_block(i + 1);
	}

	if (i - 1 >= 0 && mem_info.blocks[i - 1].free) {
		mem_info.blocks[i - 1].size += mem_info.blocks[i].size;
		delete_block(i);
	}
}


#if defined(__linux__)
#define DEFAULT_VM86_FLAGS 	(IF_MASK | IOPL_MASK)
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#define DEFAULT_VM86_FLAGS  (PSL_I | PSL_IOPL)
#define TF_MASK         PSL_T
#define VIF_MASK        PSL_VIF
#endif
#define DEFAULT_STACK_SIZE 	0x1000
#define RETURN_TO_32_INT 	255

#if defined(__linux__)
#define CONTEXT_REGS	context.vm.regs
#define REG(x)			x
#elif defined(__NetBSD__) || defined(__OpenBSD__)
#define CONTEXT_REGS	context.vm.substr.regs
#define REG(x)			vmsc.sc_ ## x
#elif defined(__FreeBSD__)
#define CONTEXT_REGS	context.vm.uc
#define REG(x)			uc_mcontext.mc_ ## x
#endif

static struct {
	int ready;
	unsigned short ret_seg, ret_off;
	unsigned short stack_seg, stack_off;
#if defined(__linux__) || defined(__NetBSD__) || defined(__OpenBSD__)
	struct vm86_struct vm;
#elif defined(__FreeBSD__)
	struct {
		struct vm86_init_args init;
		ucontext_t uc;
	} vm;
#endif
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
	int success;
	jmp_buf env;
	void *old_sighandler;
	int vret;
#endif
} context = { 0 };


static inline void
set_bit(unsigned int bit, void *array)
{
	unsigned char *a = array;

	a[bit / 8] |= (1 << (bit % 8));
}


static inline unsigned int
get_int_seg(int i)
{
	return *(unsigned short *)(i * 4 + 2);
}


static inline unsigned int
get_int_off(int i)
{
	return *(unsigned short *)(i * 4);
}


static inline void
pushw(unsigned short i)
{
	CONTEXT_REGS.REG(esp) -= 2;
	*(unsigned short *)(((unsigned int)CONTEXT_REGS.REG(ss) << 4) +
		CONTEXT_REGS.REG(esp)) = i;
}


int
LRMI_init(void)
{
	void *m;

	if (context.ready)
		return 1;

	if (!real_mem_init())
		return 0;

	/*
	 Map the Interrupt Vectors (0x0 - 0x400) + BIOS data (0x400 - 0x502)
	 and the ROM (0xa0000 - 0x100000)
	*/
	if (!map_file((void *)0, 0x502,
	 PROT_READ | PROT_WRITE | PROT_EXEC,
	 MAP_FIXED | MAP_PRIVATE, "/dev/zero", 0)) {
		real_mem_deinit();
		return 0;
	}

	if (!read_file("/dev/mem", (void *)0, 0x502)) {
		munmap((void *)0, 0x502);
		real_mem_deinit();
		return 0;
	}

	if (!map_file((void *)0xa0000, 0x100000 - 0xa0000,
	 PROT_READ | PROT_WRITE,
	 MAP_FIXED | MAP_SHARED, "/dev/mem", 0xa0000)) {
		munmap((void *)0, 0x502);
		real_mem_deinit();
		return 0;
	}

	/*
	 Allocate a stack
	*/
	m = LRMI_alloc_real(DEFAULT_STACK_SIZE);

	context.stack_seg = (unsigned int)m >> 4;
	context.stack_off = DEFAULT_STACK_SIZE;

	/*
	 Allocate the return to 32 bit routine
	*/
	m = LRMI_alloc_real(2);

	context.ret_seg = (unsigned int)m >> 4;
	context.ret_off = (unsigned int)m & 0xf;

	((unsigned char *)m)[0] = 0xcd; 	/* int opcode */
	((unsigned char *)m)[1] = RETURN_TO_32_INT;

	memset(&context.vm, 0, sizeof(context.vm));

	/*
	 Enable kernel emulation of all ints except RETURN_TO_32_INT
	*/
#if defined(__linux__)
	memset(&context.vm.int_revectored, 0, sizeof(context.vm.int_revectored));
	set_bit(RETURN_TO_32_INT, &context.vm.int_revectored);
#elif defined(__NetBSD__) || defined(__OpenBSD__)
	set_bit(RETURN_TO_32_INT, &context.vm.int_byuser);
#elif defined(__FreeBSD__)
	set_bit(RETURN_TO_32_INT, &context.vm.init.int_map);
#endif

	context.ready = 1;

	return 1;
}


static void
set_regs(struct LRMI_regs *r)
{
	CONTEXT_REGS.REG(edi) = r->edi;
	CONTEXT_REGS.REG(esi) = r->esi;
	CONTEXT_REGS.REG(ebp) = r->ebp;
	CONTEXT_REGS.REG(ebx) = r->ebx;
	CONTEXT_REGS.REG(edx) = r->edx;
	CONTEXT_REGS.REG(ecx) = r->ecx;
	CONTEXT_REGS.REG(eax) = r->eax;
	CONTEXT_REGS.REG(eflags) = DEFAULT_VM86_FLAGS;
	CONTEXT_REGS.REG(es) = r->es;
	CONTEXT_REGS.REG(ds) = r->ds;
	CONTEXT_REGS.REG(fs) = r->fs;
	CONTEXT_REGS.REG(gs) = r->gs;
}


static void
get_regs(struct LRMI_regs *r)
{
	r->edi = CONTEXT_REGS.REG(edi);
	r->esi = CONTEXT_REGS.REG(esi);
	r->ebp = CONTEXT_REGS.REG(ebp);
	r->ebx = CONTEXT_REGS.REG(ebx);
	r->edx = CONTEXT_REGS.REG(edx);
	r->ecx = CONTEXT_REGS.REG(ecx);
	r->eax = CONTEXT_REGS.REG(eax);
	r->flags = CONTEXT_REGS.REG(eflags);
	r->es = CONTEXT_REGS.REG(es);
	r->ds = CONTEXT_REGS.REG(ds);
	r->fs = CONTEXT_REGS.REG(fs);
	r->gs = CONTEXT_REGS.REG(gs);
}

#define DIRECTION_FLAG 	(1 << 10)

enum {
	CSEG = 0x2e, SSEG = 0x36, DSEG = 0x3e,
	ESEG = 0x26, FSEG = 0x64, GSEG = 0x65,
};

static void
em_ins(int size)
{
	unsigned int edx, edi;

	edx = CONTEXT_REGS.REG(edx) & 0xffff;
	edi = CONTEXT_REGS.REG(edi) & 0xffff;
	edi += (unsigned int)CONTEXT_REGS.REG(es) << 4;

	if (CONTEXT_REGS.REG(eflags) & DIRECTION_FLAG) {
		if (size == 4)
			asm volatile ("std; insl; cld"
			 : "=D" (edi) : "d" (edx), "0" (edi));
		else if (size == 2)
			asm volatile ("std; insw; cld"
			 : "=D" (edi) : "d" (edx), "0" (edi));
		else
			asm volatile ("std; insb; cld"
			 : "=D" (edi) : "d" (edx), "0" (edi));
	} else {
		if (size == 4)
			asm volatile ("cld; insl"
			 : "=D" (edi) : "d" (edx), "0" (edi));
		else if (size == 2)
			asm volatile ("cld; insw"
			 : "=D" (edi) : "d" (edx), "0" (edi));
		else
			asm volatile ("cld; insb"
			 : "=D" (edi) : "d" (edx), "0" (edi));
	}

	edi -= (unsigned int)CONTEXT_REGS.REG(es) << 4;

	CONTEXT_REGS.REG(edi) &= 0xffff0000;
	CONTEXT_REGS.REG(edi) |= edi & 0xffff;
}

static void
em_rep_ins(int size)
{
	unsigned int cx;

	cx = CONTEXT_REGS.REG(ecx) & 0xffff;

	while (cx--)
		em_ins(size);

	CONTEXT_REGS.REG(ecx) &= 0xffff0000;
}

static void
em_outs(int size, int seg)
{
	unsigned int edx, esi, base;

	edx = CONTEXT_REGS.REG(edx) & 0xffff;
	esi = CONTEXT_REGS.REG(esi) & 0xffff;

	switch (seg) {
	case CSEG: base = CONTEXT_REGS.REG(cs); break;
	case SSEG: base = CONTEXT_REGS.REG(ss); break;
	case ESEG: base = CONTEXT_REGS.REG(es); break;
	case FSEG: base = CONTEXT_REGS.REG(fs); break;
	case GSEG: base = CONTEXT_REGS.REG(gs); break;
	default:
	case DSEG: base = CONTEXT_REGS.REG(ds); break;
	}

	esi += base << 4;

	if (CONTEXT_REGS.REG(eflags) & DIRECTION_FLAG) {
		if (size == 4)
			asm volatile ("std; outsl; cld"
			 : "=S" (esi) : "d" (edx), "0" (esi));
		else if (size == 2)
			asm volatile ("std; outsw; cld"
			 : "=S" (esi) : "d" (edx), "0" (esi));
		else
			asm volatile ("std; outsb; cld"
			 : "=S" (esi) : "d" (edx), "0" (esi));
	} else {
		if (size == 4)
			asm volatile ("cld; outsl"
			 : "=S" (esi) : "d" (edx), "0" (esi));
		else if (size == 2)
			asm volatile ("cld; outsw"
			 : "=S" (esi) : "d" (edx), "0" (esi));
		else
			asm volatile ("cld; outsb"
			 : "=S" (esi) : "d" (edx), "0" (esi));
	}

	esi -= base << 4;

	CONTEXT_REGS.REG(esi) &= 0xffff0000;
	CONTEXT_REGS.REG(esi) |= esi & 0xffff;
}

static void
em_rep_outs(int size, int seg)
{
	unsigned int cx;

	cx = CONTEXT_REGS.REG(ecx) & 0xffff;

	while (cx--)
		em_outs(size, seg);

	CONTEXT_REGS.REG(ecx) &= 0xffff0000;
}

static void
em_inbl(unsigned char literal)
{
	asm volatile ("inb %w1, %b0"
	 : "=a" (CONTEXT_REGS.REG(eax))
	 : "d" (literal), "0" (CONTEXT_REGS.REG(eax)));
}

static void
em_inb(void)
{
	asm volatile ("inb %w1, %b0"
	 : "=a" (CONTEXT_REGS.REG(eax))
	 : "d" (CONTEXT_REGS.REG(edx)), "0" (CONTEXT_REGS.REG(eax)));
}

static void
em_inw(void)
{
	asm volatile ("inw %w1, %w0"
	 : "=a" (CONTEXT_REGS.REG(eax))
	 : "d" (CONTEXT_REGS.REG(edx)), "0" (CONTEXT_REGS.REG(eax)));
}

static void
em_inl(void)
{
	asm volatile ("inl %w1, %0"
	 : "=a" (CONTEXT_REGS.REG(eax))
	 : "d" (CONTEXT_REGS.REG(edx)));
}

static void
em_outbl(unsigned char literal)
{
	asm volatile ("outb %b0, %w1"
	 : : "a" (CONTEXT_REGS.REG(eax)),
	 "d" (literal));
}

static void
em_outb(void)
{
	asm volatile ("outb %b0, %w1"
	 : : "a" (CONTEXT_REGS.REG(eax)),
	 "d" (CONTEXT_REGS.REG(edx)));
}

static void
em_outw(void)
{
	asm volatile ("outw %w0, %w1"
	 : : "a" (CONTEXT_REGS.REG(eax)),
	 "d" (CONTEXT_REGS.REG(edx)));
}

static void
em_outl(void)
{
	asm volatile ("outl %0, %w1"
	 : : "a" (CONTEXT_REGS.REG(eax)),
	 "d" (CONTEXT_REGS.REG(edx)));
}

static int
emulate(void)
{
	unsigned char *insn;
	struct {
		unsigned char seg;
		unsigned int size : 1;
		unsigned int rep : 1;
	} prefix = { DSEG, 0, 0 };
	int i = 0;

	insn = (unsigned char *)((unsigned int)CONTEXT_REGS.REG(cs) << 4);
	insn += CONTEXT_REGS.REG(eip);

	while (1) {
		if (insn[i] == 0x66) {
			prefix.size = 1 - prefix.size;
			i++;
		} else if (insn[i] == 0xf3) {
			prefix.rep = 1;
			i++;
		} else if (insn[i] == CSEG || insn[i] == SSEG
		 || insn[i] == DSEG || insn[i] == ESEG
		 || insn[i] == FSEG || insn[i] == GSEG) {
			prefix.seg = insn[i];
			i++;
		} else if (insn[i] == 0xf0 || insn[i] == 0xf2
		 || insn[i] == 0x67) {
			/* these prefixes are just ignored */
			i++;
		} else if (insn[i] == 0x6c) {
			if (prefix.rep)
				em_rep_ins(1);
			else
				em_ins(1);
			i++;
			break;
		} else if (insn[i] == 0x6d) {
			if (prefix.rep) {
				if (prefix.size)
					em_rep_ins(4);
				else
					em_rep_ins(2);
			} else {
				if (prefix.size)
					em_ins(4);
				else
					em_ins(2);
			}
			i++;
			break;
		} else if (insn[i] == 0x6e) {
			if (prefix.rep)
				em_rep_outs(1, prefix.seg);
			else
				em_outs(1, prefix.seg);
			i++;
			break;
		} else if (insn[i] == 0x6f) {
			if (prefix.rep) {
				if (prefix.size)
					em_rep_outs(4, prefix.seg);
				else
					em_rep_outs(2, prefix.seg);
			} else {
				if (prefix.size)
					em_outs(4, prefix.seg);
				else
					em_outs(2, prefix.seg);
			}
			i++;
			break;
		} else if (insn[i] == 0xe4) {
			em_inbl(insn[i + 1]);
			i += 2;
			break;
		} else if (insn[i] == 0xec) {
			em_inb();
			i++;
			break;
		} else if (insn[i] == 0xed) {
			if (prefix.size)
				em_inl();
			else
				em_inw();
			i++;
			break;
		} else if (insn[i] == 0xe6) {
			em_outbl(insn[i + 1]);
			i += 2;
			break;
		} else if (insn[i] == 0xee) {
			em_outb();
			i++;
			break;
		} else if (insn[i] == 0xef) {
			if (prefix.size)
				em_outl();
			else
				em_outw();

			i++;
			break;
		} else
			return 0;
	}

	CONTEXT_REGS.REG(eip) += i;
	return 1;
}


#if defined(__linux__)
/*
 I don't know how to make sure I get the right vm86() from libc.
 The one I want is syscall # 113 (vm86old() in libc 5, vm86() in glibc)
 which should be declared as "int vm86(struct vm86_struct *);" in
 <sys/vm86.h>.

 This just does syscall 113 with inline asm, which should work
 for both libc's (I hope).
*/
#if !defined(USE_LIBC_VM86)
static int
lrmi_vm86(struct vm86_struct *vm)
{
	int r;
#ifdef __PIC__
	asm volatile (
	 "pushl %%ebx\n\t"
	 "movl %2, %%ebx\n\t"
	 "int $0x80\n\t"
	 "popl %%ebx"
	 : "=a" (r)
	 : "0" (113), "r" (vm));
#else
	asm volatile (
	 "int $0x80"
	 : "=a" (r)
	 : "0" (113), "b" (vm));
#endif
	return r;
}
#else
#define lrmi_vm86 vm86
#endif
#endif /* __linux__ */


static void
debug_info(int vret)
{
#ifdef LRMI_DEBUG
	int i;
	unsigned char *p;

	fputs("vm86() failed\n", stderr);
	fprintf(stderr, "return = 0x%x\n", vret);
	fprintf(stderr, "eax = 0x%08x\n", CONTEXT_REGS.REG(eax));
	fprintf(stderr, "ebx = 0x%08x\n", CONTEXT_REGS.REG(ebx));
	fprintf(stderr, "ecx = 0x%08x\n", CONTEXT_REGS.REG(ecx));
	fprintf(stderr, "edx = 0x%08x\n", CONTEXT_REGS.REG(edx));
	fprintf(stderr, "esi = 0x%08x\n", CONTEXT_REGS.REG(esi));
	fprintf(stderr, "edi = 0x%08x\n", CONTEXT_REGS.REG(edi));
	fprintf(stderr, "ebp = 0x%08x\n", CONTEXT_REGS.REG(ebp));
	fprintf(stderr, "eip = 0x%08x\n", CONTEXT_REGS.REG(eip));
	fprintf(stderr, "cs  = 0x%04x\n", CONTEXT_REGS.REG(cs));
	fprintf(stderr, "esp = 0x%08x\n", CONTEXT_REGS.REG(esp));
	fprintf(stderr, "ss  = 0x%04x\n", CONTEXT_REGS.REG(ss));
	fprintf(stderr, "ds  = 0x%04x\n", CONTEXT_REGS.REG(ds));
	fprintf(stderr, "es  = 0x%04x\n", CONTEXT_REGS.REG(es));
	fprintf(stderr, "fs  = 0x%04x\n", CONTEXT_REGS.REG(fs));
	fprintf(stderr, "gs  = 0x%04x\n", CONTEXT_REGS.REG(gs));
	fprintf(stderr, "eflags  = 0x%08x\n", CONTEXT_REGS.REG(eflags));

	fputs("cs:ip = [ ", stderr);

	p = (unsigned char *)((CONTEXT_REGS.REG(cs) << 4) + (CONTEXT_REGS.REG(eip) & 0xffff));

	for (i = 0; i < 16; ++i)
		fprintf(stderr, "%02x ", (unsigned int)p[i]);

	fputs("]\n", stderr);
#endif
}


#if defined(__linux__)
static int
run_vm86(void)
{
	unsigned int vret;
	sigset_t all_sigs, old_sigs;
	unsigned long old_gs, old_fs;

	while (1) {
		// FIXME: may apply this to BSD equivalents?
		sigfillset(&all_sigs);
		sigprocmask(SIG_SETMASK, &all_sigs, &old_sigs);
		asm volatile ("mov %%gs, %0" : "=rm" (old_gs));
		asm volatile ("mov %%fs, %0" : "=rm" (old_fs));
		vret = lrmi_vm86(&context.vm);
		asm volatile ("mov %0, %%gs" :: "rm" (old_gs));
		asm volatile ("mov %0, %%fs" :: "rm" (old_fs));
		sigprocmask(SIG_SETMASK, &old_sigs, NULL);

		if (VM86_TYPE(vret) == VM86_INTx) {
			unsigned int v = VM86_ARG(vret);

			if (v == RETURN_TO_32_INT)
				return 1;

			pushw(CONTEXT_REGS.REG(eflags));
			pushw(CONTEXT_REGS.REG(cs));
			pushw(CONTEXT_REGS.REG(eip));

			CONTEXT_REGS.REG(cs) = get_int_seg(v);
			CONTEXT_REGS.REG(eip) = get_int_off(v);
			CONTEXT_REGS.REG(eflags) &= ~(VIF_MASK | TF_MASK);

			continue;
		}

		if (VM86_TYPE(vret) != VM86_UNKNOWN)
			break;

		if (!emulate())
			break;
	}

	debug_info(vret);

	return 0;
}
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#if defined(__NetBSD__) || defined(__OpenBSD__)
static void
vm86_callback(int sig, int code, struct sigcontext *sc)
{
	/* Sync our context with what the kernel develivered to us. */
	memcpy(&CONTEXT_REGS, sc, sizeof(*sc));

	switch (VM86_TYPE(code)) {
		case VM86_INTx:
		{
			unsigned int v = VM86_ARG(code);

			if (v == RETURN_TO_32_INT) {
				context.success = 1;
				longjmp(context.env, 1);
			}

			pushw(CONTEXT_REGS.REG(eflags));
			pushw(CONTEXT_REGS.REG(cs));
			pushw(CONTEXT_REGS.REG(eip));

			CONTEXT_REGS.REG(cs) = get_int_seg(v);
			CONTEXT_REGS.REG(eip) = get_int_off(v);
			CONTEXT_REGS.REG(eflags) &= ~(VIF_MASK | TF_MASK);

			break;
		}

		case VM86_UNKNOWN:
			if (emulate() == 0) {
				context.success = 0;
				context.vret = code;
				longjmp(context.env, 1);
			}
			break;

		default:
			context.success = 0;
			context.vret = code;
			longjmp(context.env, 1);
			return;
	}

	/* ...and sync our context back to the kernel. */
	memcpy(sc, &CONTEXT_REGS, sizeof(*sc));
}
#elif defined(__FreeBSD__)
static void
vm86_callback(int sig, int code, struct sigcontext *sc)
{
	unsigned char *addr;

	/* Sync our context with what the kernel develivered to us. */
	memcpy(&CONTEXT_REGS, sc, sizeof(*sc));

	if (code) {
		/* XXX probably need to call original signal handler here */
		context.success = 0;
		context.vret = code;
		longjmp(context.env, 1);
	}

	addr = (unsigned char *)((CONTEXT_REGS.REG(cs) << 4) +
		CONTEXT_REGS.REG(eip));

	if (addr[0] == 0xcd) { /* int opcode */
		if (addr[1] == RETURN_TO_32_INT) {
			context.success = 1;
			longjmp(context.env, 1);
		}

		pushw(CONTEXT_REGS.REG(eflags));
		pushw(CONTEXT_REGS.REG(cs));
		pushw(CONTEXT_REGS.REG(eip));

		CONTEXT_REGS.REG(cs) = get_int_seg(addr[1]);
		CONTEXT_REGS.REG(eip) = get_int_off(addr[1]);
		CONTEXT_REGS.REG(eflags) &= ~(VIF_MASK | TF_MASK);
	} else {
		if (emulate() == 0) {
			context.success = 0;
			longjmp(context.env, 1);
		}
	}

	/* ...and sync our context back to the kernel. */
	memcpy(sc, &CONTEXT_REGS, sizeof(*sc));
}
#endif /* __FreeBSD__ */

static int
run_vm86(void)
{
	if (context.old_sighandler) {
#ifdef LRMI_DEBUG
		fprintf(stderr, "run_vm86: callback already installed\n");
#endif
		return (0);
	}

#if defined(__NetBSD__) || defined(__OpenBSD__)
	context.old_sighandler = signal(SIGURG, (void (*)(int))vm86_callback);
#elif defined(__FreeBSD__)
	context.old_sighandler = signal(SIGBUS, (void (*)(int))vm86_callback);
#endif

	if (context.old_sighandler == (void *)-1) {
		context.old_sighandler = NULL;
#ifdef LRMI_DEBUG
		fprintf(stderr, "run_vm86: cannot install callback\n");
#endif
		return (0);
	}

	if (setjmp(context.env)) {
#if defined(__NetBSD__) || defined(__OpenBSD__)
		(void) signal(SIGURG, context.old_sighandler);
#elif defined(__FreeBSD__)
		(void) signal(SIGBUS, context.old_sighandler);
#endif
		context.old_sighandler = NULL;

		if (context.success)
			return (1);
		debug_info(context.vret);
		return (0);
	}

#if defined(__NetBSD__) || defined(__OpenBSD__)
	if (i386_vm86(&context.vm) == -1)
		return (0);
#elif defined(__FreeBSD__)
	if (i386_vm86(VM86_INIT, &context.vm.init))
		return 0;

	CONTEXT_REGS.REG(eflags) |= PSL_VM | PSL_VIF;
	sigreturn(&context.vm.uc);
#endif /* __FreeBSD__ */

	/* NOTREACHED */
	return (0);
}
#endif	/* __NetBSD__ || __FreeBSD__ || __OpenBSD__ */

int
LRMI_call(struct LRMI_regs *r)
{
	unsigned int vret;

	memset(&CONTEXT_REGS, 0, sizeof(CONTEXT_REGS));

	set_regs(r);

	CONTEXT_REGS.REG(cs) = r->cs;
	CONTEXT_REGS.REG(eip) = r->ip;

	if (r->ss == 0 && r->sp == 0) {
		CONTEXT_REGS.REG(ss) = context.stack_seg;
		CONTEXT_REGS.REG(esp) = context.stack_off;
	} else {
		CONTEXT_REGS.REG(ss) = r->ss;
		CONTEXT_REGS.REG(esp) = r->sp;
	}

	pushw(context.ret_seg);
	pushw(context.ret_off);

	vret = run_vm86();

	get_regs(r);

	return vret;
}


int
LRMI_int(int i, struct LRMI_regs *r)
{
	unsigned int vret;
	unsigned int seg, off;

	seg = get_int_seg(i);
	off = get_int_off(i);

	/*
	 If the interrupt is in regular memory, it's probably
	 still pointing at a dos TSR (which is now gone).
	*/
	if (seg < 0xa000 || (seg << 4) + off >= 0x100000) {
#ifdef LRMI_DEBUG
		fprintf(stderr, "Int 0x%x is not in rom (%04x:%04x)\n", i, seg, off);
#endif
		return 0;
	}

	memset(&CONTEXT_REGS, 0, sizeof(CONTEXT_REGS));

	set_regs(r);

	CONTEXT_REGS.REG(cs) = seg;
	CONTEXT_REGS.REG(eip) = off;

	if (r->ss == 0 && r->sp == 0) {
		CONTEXT_REGS.REG(ss) = context.stack_seg;
		CONTEXT_REGS.REG(esp) = context.stack_off;
	} else {
		CONTEXT_REGS.REG(ss) = r->ss;
		CONTEXT_REGS.REG(esp) = r->sp;
	}

	pushw(DEFAULT_VM86_FLAGS);
	pushw(context.ret_seg);
	pushw(context.ret_off);

	vret = run_vm86();

	get_regs(r);

	return vret;
}

#else /* (__linux__ || __NetBSD__ || __FreeBSD__ || __OpenBSD__) && __i386__ */
#warning "LRMI is not supported on your system!"
#endif
