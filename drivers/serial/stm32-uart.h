#include <kernel/serial.h>

#include <cmsis.h>

#define NR_STM32_PORTS 6

struct stm32_port {
    unsigned int            irq;
    struct serial_info      serial;
    /* Not yet to implemente DMA */
}

static struct stm32_port stm32_ports[NR_STM32_PORTS];
