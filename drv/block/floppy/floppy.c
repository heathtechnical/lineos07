#include<system.h>
#include<types.h>
#include<portio.h>
#include<page.h>
#include<heap.h>
#include<devmgr.h>
#include<floppy.h>

#define DEV_FLOPPY_MAJOR 0

u8 floppy_io(void);

char *floppydev_types[5] = {
        "no floppy drive",
        "360kb 5.25in floppy drive",
        "1.2mb 5.25in floppy drive",
        "720kb 3.5in",
        "1.44mb 3.5in",
        "2.88mb 3.5in"
};

void dump_floppies(dev_t *dev)
{
	di_floppydev *info;

	info=(di_floppydev *)dev->devinfo;

	kprintf("DEV0: %s %d %s\n",dev->name,info->type,info->typestr);
}

u32 init_floppy(void)
{
	u16 i,f0,f1;
	di_floppydev *di_f0, *di_f1;
	
	/* Ask CMOS for drive information */
	outportb(0x70,0x10);
	i=inportb(0x71);

	f0=i >> 4;
	f1=i & 0xf;

	if(f0){
		/* floppy0 present */

		/* Allocate device info structure */
		di_f0=kmalloc(sizeof(di_floppydev),NULL);
		if(!di_f0){
			panic("Unable to kmalloc di_floppydev structure!");
		}

		di_f0->type=f0;
		di_f0->typestr=floppydev_types[f0];

		register_device("floppy0",DEV_FLOPPY_MAJOR,0,di_f0,sizeof(di_floppydev));
	}

	if(f1){
		/* floppy1 present */

		/* Allocate device info structure */
		di_f1=kmalloc(sizeof(di_floppydev),NULL);
		if(!di_f1){
			panic("Unable to kmalloc di_floppydev structure!");
		}

		di_f1->type=f1;
		di_f1->typestr=floppydev_types[f1];

		register_device("floppy1",DEV_FLOPPY_MAJOR,1,di_f1,sizeof(di_floppydev));
	}

	foreachdevice(DEV_FLOPPY_MAJOR,&dump_floppies);

	floppy_io();
}

//u8 floppy_io(di_floppydev *floppy, void *buffer, int mode)
u8 floppy_io(void)
{
	u32 paddr;
	paddr=(u32)alloc_pages(1,ZONE_DMA);
	map_temp(paddr,1);
}
