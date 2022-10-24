/************** lab5base.c file ******************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include "type.h"
//#include <util.c>

MINODE minode[NMINODE]; // in memory INODEs
MTABLE mtable[NMTABLE]; // mount tables
OFT oft[NOFT];          // Opened file instance
PROC proc[NPROC];       // PROC structures
PROC *running;          // current executing PROC

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;   

#define BLK 1024

int fd;             // opened vdisk for READ
int inodes_block;   // inodes start block number

char gpath[128];    // token strings
char *name[32];
int n;

// global variables
MINODE minode[NMINODE], *root;
MTABLE mtable[NMTABLE];
PROC proc[NPROC], *running;
int ninodes, nblocks, bmap, imap, iblock;
int dev;
char gline[25], *name[16]; // tokenized component string strings
int nname; // number of component strings
char *rootdev = "disk1"; // default root_device

/** Initialize file system
 * 
 * Initializing all the global data structures
 * points "running" at PROC[0] - process 0 of the superuser
*/
int fs_init()
{
  int i,j;
  for (i=0; i<NMINODE; i++) // initialize all minodes as FREE
    minode[i].refCount = FREE;
  for (i=0; i<NMTABLE; i++) // initialize mtable entries as FREE
    mtable[i].dev = FREE;
  for (i=0; i<NPROC; i++){ // initialize PROCs
    proc[i].status = FREE; // ready to run
    proc[i].pid = i; // pid = 0 to NPROC-1
    proc[i].uid = i; // P0 is a superuser process
  for (j=0; j<NFD; j++)
    proc[i].fd[j] = 0; // all file descriptors are NULL
    proc[i].next = &proc[i+1];
  }
  proc[NPROC-1].next = &proc[0]; // circular list
  running = &proc[0]; // P0 runs first
}

/** Mount Root file system
 * Reads the superblock of the root device to verify the device is a valid EXT2 file system
 * Then it loads the root INODE (ino = 2) of the root device into a minode and sets the root pointer to the root minode
 * It also sets the CWD of all PROCs to the root minode
 * A mount table entry is allocated to record the mounted root file system
*/
int mount_root(char *rootdev) // mount root file system
{
  int i;
  MTABLE *mp;
  SUPER *sp;
  GD *gp;
  char buf[BLKSIZE];
  dev = open(rootdev, O_RDWR);
  if (dev < 0){
    printf("panic : can’t open root device\n");
    exit(1);
  }
  /* get super block of rootdev */
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;
  /* check magic number */
  if (sp->s_magic != SUPER_MAGIC){
    printf("super magic=%x : %s is not an EXT2 filesys\n",
    sp->s_magic, rootdev);
    exit(0);
  }
  // fill mount table mtable[0] with rootdev information
  mp = &mtable[0]; // use mtable[0]
  mp->dev = dev;
  // copy super block info into mtable[0]
  ninodes = mp->ninodes = sp->s_inodes_count;
  nblocks = mp->nblocks = sp->s_blocks_count;
  strcpy(mp->devName, rootdev);
  strcpy(mp->mntName, "/");
  get_block(dev, 2, buf);
  gp = (GD *)buf;
  bmap = mp->bmap = gp->bg_block_bitmap;
  imap = mp->imap = gp->bg_inode_bitmap;
  iblock = mp->iblock = gp->bg_inode_table;
  printf("bmap=%d imap=%d iblock=%d\n", bmap, imap, iblock);
  // call iget(), which inc minode’s refCount
  root = iget(dev, 2); // get root inode
  mp->mntDirPtr = root; // double link
  root->mntPtr = mp;
  // set proc CWDs
  for (i=0; i<NPROC; i++) // set proc’s CWD
    proc[i].cwd = iget(dev, 2); // each inc refCount by 1
  printf("mount : %s mounted on / \n", rootdev);
  return 0;
}

int get_block(int fd, int blk, char *buf)
{
   lseek(fd, blk*BLK, 0);
   read(fd, buf, BLK);
}   

int search(INODE *ip, char *name)
{
  // Chapter 11.4.4  int i; 
  // Exercise 6
}

/*************************************************************************/
int tokenize(char *pathname)
{
  // you did this many times
} 

char *disk = "vdisk";

int main(int argc, char *argv[])   // a.out pathname
{
  char buf[BLK];  // use more bufs if you need them

//   1. open vdisk for READ: print fd value
//   2. read SUPER block #1 to verify EXT2 fs : print s_magic in HEX
//   3. read GD block #2 to get inodes_block=bg_inode_table: print inodes_block 

//   4. read inodes_block into buf[] to get root INODE #2 : set ino=2 
//           INODE *ip = (INODE *)buf + 1;
			    
//   5. tokenize pathame=argv[1]);
     
//   6. for (i=0; i<n; i++){
//         printf("===========================================\n");
//         printf("search name[%d]=%s in ino=%d\n", i, name[i], ino);
//         ino = search(ip, name[i]);

//         if (ino==0){
//            printf("name %s does not exist\n", name[i]);
//            exit(1);
//         }
//         // MAILman's algrithm:
//         blk    = (ino-1) / 8 + inodes_block;
//         offset = (ino-1) % 8;
  
//         get_block(fd, blk, buf);

//         ip = (INODE *)buf + offset;
//    } 


//7. HERE, ip->INODE of pathname
   printf("****************  DISK BLOCKS  *******************\n");
   for (int i=0; i<15; i++){
      if (ip->i_block[i]) 
         printf("block[%2d] = %d\n", i, ip->i_block[i]);
   }

   printf("================ Direct Blocks ===================\n");
   
   if (ip->i_block[12]){
      printf("===============  Indirect blocks   ===============\n"); 
   }

   if (ip->i_block[13]){
     printf("===========  Double Indirect blocks   ============\n"); 
   }
}
