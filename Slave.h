#ifndef YOUR_NAME_INCLUDE
#define YOUR_NAME_INCLUDE
#include "systemc.h"
extern bool to_slave;
extern bool to_master;
SC_MODULE(Slave)
{

	sc_in< bool > P_clk;

	sc_in< bool > hwrite;

	sc_in<bool> hreset;

	sc_port<sc_fifo_in_if<int > >receiver; 

	sc_port<sc_fifo_out_if< int > > sender;

	void reader();

	void writer();

	SC_CTOR(Slave)
	{

		SC_METHOD(reader);

		sensitive << P_clk << hwrite ;


		SC_METHOD(writer);

		sensitive_pos<<hwrite<<P_clk;

	}

};

#endif