#include "systemc.h"
#include "Slave.h"
#include "Master .h"
#define HCLK 20
int sc_main(int argc, char* argv[]) {

	sc_clock clk("clock", HCLK,0.5,1,true);

	sc_clock hwrite("HWRITE",HCLK*2,0.5,1,false);

	sc_signal< bool > hreset;

	sc_fifo<int>* fifo_inst;fifo_inst=new sc_fifo<int>(20);

	sc_fifo<int>* fifo_inst_0;fifo_inst_0=new sc_fifo<int>(20);

	Slave S1("Slave");

	Master U1 ("Master");

	hreset=1;
	

	U1.P_clk(clk);

	U1.sender(*fifo_inst);

	U1.receiver(*fifo_inst_0);

	U1.hwrite(hwrite);

	U1.hreset(hreset);
	

	S1.P_clk(clk);

	S1.receiver(*fifo_inst);

	S1.sender(*fifo_inst_0);

	S1.hwrite(hwrite);

	S1.hreset(hreset);
	
	sc_start();

	sc_initialize();

	sc_stop();

	return (0);
}