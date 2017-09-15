
struct n_tty_data {
    /* producer-published */
    
}

static struct tty_ldisc_ops n_tty_ops = {
    //.magic
    //.name
    .open       = n_tty_open,
    .close      = n_tty_close,
    .read       = n_tty_read,
    .write      = n_tty_write,
}

static int n_tty(struct tty_struct *tty)
{
    struct n_tty_data *ldata;

    ldata = malloc(sizeof(*ldata));
    if (!ldata)
        goto err;

    

    return 0;
err:
    return -ENOMEM;
}

static ssize_t n_tty_read(struct tty_struct *tty, struct file *file,
            unsigned char *buf, size_t nr)
{
    struct n_tty_data *ldata = tty->disc_data;
}


void n_tty_init(void)
{
    tty_register_ldisc(N_TTY, &n_tty_ops):
}
