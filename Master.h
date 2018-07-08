#ifndef Master
#define Master
#include "systemc.h"
bool to_slave=false;
bool to_master=false;
SC_MODULE(Master)
{

	sc_in< bool > P_clk;

	sc_in< bool > hwrite;

	sc_in<bool> hreset;

	sc_port<sc_fifo_out_if< int > > sender;
  
  	sc_port<sc_fifo_in_if<int > >receiver; 

	void writer();

	void reader();

	SC_CTOR(Master)
	{

		SC_METHOD(writer);

		sensitive<< P_clk <<hwrite;

		//next_trigger(P_clk.posedge_event() & hwrite.negedge_event())

		SC_METHOD(reader);

		sensitive<<P_clk<<hwrite;


	}
};
#endif