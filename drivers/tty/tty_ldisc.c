

enum {
    N_TTY   0,
    NR_DISCI,
}

static struct tty_ldisc_ops *tty_ldiscs[NR_LDISCS] = {
    
    }

static struct tty_ldisc *tty_ldisc_alloc(struct tty_struct *tty, int disc)
{
    struct tty_ldisc *ld;

    ld = malloc(sizeof(struct tty_ldisc));

    ld->ops = tty_ldiscs[disc];
//    ld->tty = tty
}

static void tty_ldisc_free(struct tty_struct *tty)
{
    free(tty->ldisc);
}

//FIXME:why kernel directly assign n_tty?
void tty_ldisc_init(struct tty_struct *tty)
{
    struct tty_ldisc *ld = tty_ldisc_alloc(tty, N_TTY);
}

void tty_ldisc_deinit(struct tty_struct *tty)
{

}
