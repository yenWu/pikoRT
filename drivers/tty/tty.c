#include <kernel/fs.h>
#include <kernel/kernel.h>

static int tty_open(struct inode *inode, struct file *file)
{
    file->f_private = inode->i_private;
    
    //Set up relative infrastructure

    return 0;
}

static ssize_t tty_write()
{
    struct tty_ldisc *ld;

    if (ld->ops->write)
        ld->ops->write();

    return count;
}

static ssize_t tty_read(struct file *file, char *buf, size_t count,
            off_t *offset)
{
    struct tty_ldisc *ld;  

    if (ld->ops->read)
        ld->ops->read();

    return count;
}

static const struct file_operations tty_fops = {
    .open   = tty_open;
    .read   = tty_read;
    .write  = tty_write;
}

struct tty_file_private ttyS0_private = {
    .tty    = ttyS0,
    .file   = NULL,//FIXME:
//    .list   = LIST_INIT_HEAD(ttyS0_private.list),
}

static struct inode tty_inode = {
    .i_fop = &tty_fops,
    .i_private = NULL,  //persudo device
}

int console_init()
{
    n_tty_init();
}

int tty_init(void)
{
    struct dentry dentry = {.d_inode = &ttyS0_inode, .d_name = "ttyS0"};

    init_tmpfs_inode(&tty_inode);
    vfs_link(0, dev_inode(), &dentry);

    return 0;
}
