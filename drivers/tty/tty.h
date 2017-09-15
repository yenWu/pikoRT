

struct tty_buf {
    int size;
    
    /* Data ptr */
    unsigned long data[0];
}

struct tty_bufhead {

}

struct tty_port {
    struct tty_bufhead  buf;
}

struct tty_struct {
    struct tty_driver   *driver;
    const struct tty_operations *ops;

    struct tty_ldisc    *ldisc;

    struct list_head    write_waiters;
    struct list_head    read_waiters;

    struct tty_port *port;
}

//FIXME:
struct tty_file_private {
    struct tty_struct *tty;
    struct file *file;
    struct list_head list;
}
