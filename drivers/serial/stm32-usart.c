#include <sys/types.h>

#include <kernel/cbuf.h>
#include <kernel/fs.h>
#include <kernel/irq.h>
#include <kernel/serial.h>
#include <kernel/types.h>

#include <cmsis.h>
#include "platform.h"

static struct cbuf_info cbuf;
static char buf[16];

static int stm32_getc(struct serial_info *serial, char *c)
{
    serial->rx_count--;
    cbuf_getc(&cbuf, c);

    return 0;
}

static int stm32_putc(struct serial_info *serial, char c)
{
    USART_TypeDef *uart = serial->priv;

    while (!(uart->SR & USART_SR_TXE))
        ;
    uart->DR = c;

    return 0;
}

static int stm32_puts(struct serial_info *serial,
                      size_t len,
                      size_t *retlen,
                      const char *buf)
{
    *retlen = len;
    for (int i = 0; len > 0; len--, i++)
        stm32_putc(serial, buf[i]);

    return 0;
}

static struct serial_ops stm32_ops = {
    .serial_getc = stm32_getc,
    .serial_putc = stm32_putc,
    .serial_puts = stm32_puts,
};

struct serial_info stm32_info = {
    .priv = USARTx,
    .ops = &stm32_ops,
};

static void stm32_uartx_isr(void)
{
    if (USARTx->SR & USART_SR_RXNE) {
        char c = (char) USARTx->DR;
        cbuf_putc(&cbuf, c);
        stm32_info.rx_count++;
        serial_activity_callback(&stm32_info);
    }
}

extern const struct file_operations serialchar_fops;

static struct inode stm32_inode = {
    .i_fop = &serialchar_fops,
    .i_private = &stm32_info,
};

int serial_init(void)
{
    usart_init();

    struct dentry dentry = {.d_inode = &stm32_inode, .d_name = "ttyS0"};

    cbuf_init(&cbuf, buf, 16);

    init_tmpfs_inode(&stm32_inode);
    vfs_link(0, dev_inode(), &dentry);

    /* enable rx interrupt */
    request_irq(USARTx_IRQn, stm32_uartx_isr);
    NVIC_SetPriority(USARTx_IRQn, 0xE);
    NVIC_EnableIRQ(USARTx_IRQn);

    return 0;
}
