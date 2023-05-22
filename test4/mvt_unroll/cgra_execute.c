void cgra_execute(void** din_addr, void** dout_addr)
{
	unsigned short int cin[28][3] __attribute__((aligned(8))) = {
		{0x1000, 0x4000, 0x0008},
		{0x0401, 0x2800, 0x0009},
		{0x0000, 0x0000, 0x000a},
		{0x0810, 0x0101, 0x000b},
		{0x0000, 0x4000, 0x0010},
		{0x0401, 0x2800, 0x0011},
		{0x0000, 0x0000, 0x0012},
		{0x0010, 0x0000, 0x0013},
		{0x3000, 0x4000, 0x0018},
		{0xb401, 0x29ff, 0x0019},
		{0x0000, 0x0000, 0x001a},
		{0x0010, 0x0000, 0x001b},
		{0x2000, 0x4000, 0x0020},
		{0x0401, 0x2800, 0x0021},
		{0x0000, 0x0000, 0x0022},
		{0x0010, 0x0000, 0x0023},
		{0x0002, 0x0000, 0x0058},
		{0x0000, 0x0001, 0x0060},
		{0x0400, 0x0000, 0x0068},
		{0x0881, 0x0001, 0x00a9},
		{0x4803, 0x0000, 0x00b1},
		{0x000c, 0x0200, 0x00f8},
		{0x0000, 0x0620, 0x0188},
		{0x0300, 0x0000, 0x0218},
		{0x080f, 0x0000, 0x0261},
		{0x0000, 0x8002, 0x0262},
		{0x0a02, 0x0280, 0x0263},
		{0x0000, 0x0000, 0x0264},
	};

	load_cfg(cin, 0x40000, 168, 0, 0);
	load_data(din_addr[0], 0x8000, 1600, 0, 0);
	load_data(din_addr[1], 0xc000, 80, 0, 0);
	load_data(din_addr[2], 0x0, 80, 0, 0);
	config(0x0, 28, 0, 0);
	execute(0xf, 0, 0);
	store(dout_addr[0], 0x4000, 80, 0, 0);
}
