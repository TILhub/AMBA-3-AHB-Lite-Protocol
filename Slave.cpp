#include "Slave.h"

void Slave::writer(){

	if(to_master==true && hreset && (P_clk.posedge()==true) && (hwrite.negedge()==true)
		&& P_clk==true && hwrite == false){

		cout<<" ## Slave: " << sc_simulation_time()<< " P_CLK.negedge: " << P_clk.negedge()
		<<" Hreset: "<<hreset<<" Hwrite.posedge: "<<hwrite.posedge()<<endl;

		int t=(-1)*rand()%1024;

		cout<<" ## Value Sent By Slave: "<<t<<endl<<endl;

		//sender->write(t);	

		to_master=false;	

	}
}

void Slave::reader()
{

  if(P_clk.negedge()==true && hwrite.posedge()==true && hreset &&
  	&& P_clk==false && hwrite == true && 
   	to_slave==true){

	cout<<" # Slave: " << sc_simulation_time()<< " P_CLK.negedge: " << P_clk.negedge()
	<<" Hreset: "<<hreset<<" Hwrite.posedge: "<<hwrite.posedge()<<endl;

    cout<<" # Value Recieved(Slave): "<<receiver->read()<<endl<<endl;

    to_slave=false;

	}
}
