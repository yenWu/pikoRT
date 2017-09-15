struct tty_ldisc_ops {
    int     (*open)(struct tty_struct *);
    void    (*close)(struct tty_strucct *);
    (*read)(struct tty_struct *tty, struct file *file,
        unsigned char *buf, size_t nr);
    ssize_t (*write)(struct tty_struct *tty, struct file *file,
        const unsigned char *buf, size_t nr);
}

struct tty_ldisc {
    struct tty_ldisc_ops    *ops;
//    struct tty_struct       *tty;
}
