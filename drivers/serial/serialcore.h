/*
 * This structure describes all the operations that can be done on the
 * physical hardware.  See Documentation/serial/driver for details.
 */
struct uart_ops {
	int		(*startup)(struct uart_port *);
	void		(*send_xchar)(struct uart_port *, char ch);
	unsigned int	(*tx_empty)(struct uart_port *);
	void		(*start_tx)(struct uart_port *);
	void		(*stop_tx)(struct uart_port *);
	void		(*stop_rx)(struct uart_port *);
	void		(*flush_buffer)(struct uart_port *);
	void		(*set_ldisc)(struct uart_port *, struct ktermios *);

#if 0
	void		(*set_mctrl)(struct uart_port *, unsigned int mctrl);
	unsigned int	(*get_mctrl)(struct uart_port *);
	void		(*throttle)(struct uart_port *);
	void		(*unthrottle)(struct uart_port *);
	void		(*enable_ms)(struct uart_port *);
	void		(*break_ctl)(struct uart_port *, int ctl);
	void		(*shutdown)(struct uart_port *);
	void		(*set_termios)(struct uart_port *, struct ktermios *new,
				       struct ktermios *old);
	void		(*pm)(struct uart_port *, unsigned int state,
			      unsigned int oldstate);
#endif

	/*
	 * Return a string describing the type of the port
	 */
	const char	*(*type)(struct uart_port *);

	/*
	 * Release IO and memory resources used by the port.
	 * This includes iounmap if necessary.
	 */
	void		(*release_port)(struct uart_port *);

	/*
	 * Request IO and memory resources used by the port.
	 * This includes iomapping the port if necessary.
	 */
	int		(*request_port)(struct uart_port *);
	void		(*config_port)(struct uart_port *, int);
#if 0
	int		(*verify_port)(struct uart_port *, struct serial_struct *);
	int		(*ioctl)(struct uart_port *, unsigned int, unsigned long);
#endif
};

struct uart_port {
	unsigned long			*mapbase;
	int 					irq;
	unsigned int			uartclk;
	unsigned int			fifosize;
	const struct uart_ops	*ops;
	struct uart_state		state;

}

struct uart_state {
	struct tty_port		port;
	struct cbuf_info	xmit;
//	struct list_head	remove_wait;
}

struct uart_driver {
	int major;
	int minor;

	struct uart_state state;
	struct tty_driver tty_driver;
}
