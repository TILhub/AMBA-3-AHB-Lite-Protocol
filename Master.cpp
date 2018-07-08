
#include "Master.h"
void Master::reader(){
	if(hreset && (P_clk.posedge()==true) && 
		hwrite==true && P_clk==true &&
		(hwrite.posedge()==true)){

	cout<<" %%  Master: " << sc_simulation_time()<< " P_CLK.posedge: " 
	<< P_clk.posedge()<<" Hreset: "<<hreset<<" Hwrite.negedge: "
	<<hwrite.negedge()<<endl	;
	cout<<" %% Master Sending signal to recieve Data\n\n";
	to_master=true;

	}
}
void Master::writer()
{

  if((hwrite.negedge()==true) && (P_clk.posedge()==true) 
  	&&P_clk==true  && hwrite==false 
  	&& hreset)
  {

	cout<<" % Master: " << sc_simulation_time()<< " P_CLK.posedge: " 
	<< P_clk.posedge()<<" Hreset: "<<hreset<<" Hwrite.negedge: "
	<<hwrite.negedge()<<endl	;

	int val=rand()%1024+1024;

	cout<<" % Value Sending(Master End): "<<val<<endl<<endl;

	sender->write(val);

    to_slave=true;

	}

}


