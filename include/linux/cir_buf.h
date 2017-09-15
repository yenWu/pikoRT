#ifndef _LINUX_CIR_BUF_H
#define _LINUX_CIR_BUF_H

struct cir_buf {
    unsigned char   *buf;
    int             head;
    int             tail;
};

#define CIR_CNT(hd, tl, size) 

#define 
