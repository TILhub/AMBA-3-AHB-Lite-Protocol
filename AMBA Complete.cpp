#include "systemc.h"
#define HCLK 20
#define WORD_SIZE 8
#define ADDRESS 2*32767
bool to_slave=false,hburst_flag=false;
bool to_master=false;bool htrans_flag=false;

SC_MODULE(Master)
{

	sc_in< bool > P_clk;

	sc_in< bool > hwrite;

	sc_in<bool> hreset;

	sc_port<sc_fifo_out_if< int > > sender;

	sc_port<sc_fifo_out_if< int > > hburst;

	sc_port<sc_fifo_out_if<int > > htrans;

	sc_port<sc_fifo_out_if< int > > address;
  
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

		//SC_METHOD(Master_to_Slave_signal);

		//sensitive<<P_clk;
	}
};


SC_MODULE(Slave)
{

	sc_in< bool > P_clk;

	sc_in< bool > hwrite;

	sc_in<bool> hreset;

	sc_port<sc_fifo_in_if<int > >receiver;

	sc_port<sc_fifo_in_if< int > > address;

	sc_port<sc_fifo_in_if<int > >hburst; 

	sc_port<sc_fifo_in_if<int > >htrans; 

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

int sc_main(int argc, char* argv[]) {

	sc_clock clk("clock", HCLK,0.5,1,true);

	sc_clock hwrite("HWRITE",HCLK*2,0.5,1,false);

	sc_signal< bool > hreset;

	sc_fifo<int>* fifo_inst;
	fifo_inst=new sc_fifo<int>(20);

	sc_fifo<int>* fifo_inst_0;
	fifo_inst_0=new sc_fifo<int>(20);

	sc_fifo<int>* fifo_htrans;
	fifo_htrans=new sc_fifo<int>(20);
	
	sc_fifo<int>* fifo_hburst;
	fifo_hburst=new sc_fifo<int>(20);

	sc_fifo<int>* fifo_address;
	fifo_address=new sc_fifo<int>(20);

	Slave S1("Slave");

	Master U1 ("Master");

	hreset=1;
	

	U1.P_clk(clk);

	U1.sender(*fifo_inst);

	U1.receiver(*fifo_inst_0);

	U1.htrans(*fifo_htrans);

	U1.hburst(*fifo_hburst);

	U1.address(*fifo_address);

	U1.hwrite(hwrite);

	U1.hreset(hreset);
	

	S1.P_clk(clk);

	S1.receiver(*fifo_inst);

	S1.sender(*fifo_inst_0);

	S1.htrans(*fifo_htrans);

	S1.hburst(*fifo_hburst);

	S1.address(*fifo_address);

	S1.hwrite(hwrite);

	S1.hreset(hreset);
	
	sc_start();

	sc_initialize();

	sc_stop();

	return (0);
}

void Master::reader(){
	
	if(hreset && (P_clk.posedge()==true) && 
		hwrite==true && P_clk==true &&
		(hwrite.posedge()==true)){

	int adr=rand()%ADDRESS+ADDRESS;
	cout<<" %%  Master: " << sc_simulation_time()<< " P_CLK.posedge: " 
	<< P_clk.posedge()<<" Hreset: "<<hreset<<" Hwrite.negedge: "
	<<hwrite.negedge()<<" Address Requested: "<<adr<<endl;
	cout<<" %% Master Sending signal to recieve Data\n\n";
	to_master=true;
	address->write(adr);

	}
}

void Master::writer()
{	
	
  	if((hwrite.negedge()==true) && (P_clk.posedge()==true) 
  	&&P_clk==true  && hwrite==false 
  	&& hreset)
  	{
  		int v=rand()%4;
		htrans->write(v);
		if(v==0)	cout<<" % Master Not Ready: HTRANS=00--->IDLE\n\n";
		else if(v==1)	cout<<" % Master in busy State: Htrans=01--->BUSY\n\n";
		else if(v==2){
			cout<<" % Master: " << sc_simulation_time()<< " P_CLK.posedge: "<< P_clk.posedge()<<" Hreset: "
			<<hreset<<" Hwrite.negedge: "<<hwrite.negedge()<<" HTRANS: "<<v<<"HTRANS=2--->NONSEQ"<<endl;

			int val=rand()%1024+1024;
			int adr=rand()%ADDRESS+ADDRESS;
			cout<<" % Value Sending(Master End): "<<val<<" ADDRESS Requested: "<<adr<<endl<<endl;

			sender->write(val);

			address->write(adr);

			htrans->write(2);

		    to_slave=true;

		    htrans_flag=true;
		}
		else if(v==3){
			int burst=rand()%2;
			if(burst == 0){

				cout<<" % Master: " << sc_simulation_time()<< " P_CLK.posedge: "<< P_clk.posedge()<<" Hreset: "
				<<hreset<<" Hwrite.negedge: "<<hwrite.negedge()<<" HTRANS: 3"<<"--->SEQ" <<"HBURST: 0"<<endl;

				int val=rand()%1024+1024;

				int adr=rand()%ADDRESS+ADDRESS;

				cout<<" % Value Sending(Master End): "<<val<<" ADDRESS Requested: "<<adr<<endl<<endl;

				sender->write(val);

				address->write(adr);

			    htrans->write(3);

			    hburst->write(0);
			    
			    to_slave=true;

			    hburst_flag=true;

			    htrans_flag=true;
			}
			else if(burst==1){
				
				cout<<" % Master: " << sc_simulation_time()<< " P_CLK.posedge: "<< P_clk.posedge()<<" Hreset: "
				<<hreset<<" Hwrite.negedge: "<<hwrite.negedge()<<" HTRANS: 3"<<"--->SEQ" <<"HBURST: 1 INCR"<<endl;

				int val=rand()%1024;

				int adr=rand()%ADDRESS+ADDRESS;

				cout<<" % Value Sending(Master End): "<<val<<" ADDRESS Requested: "<<adr<<endl<<endl;

				sender->write(val);

				address->write(adr);

			    hburst->write(1);

			    htrans->write(3);

			    hburst_flag=true;

			    htrans_flag=true;
				
				to_slave=true;

				next_trigger(HCLK,SC_NS);
				
				cout<<" % Master: " << sc_simulation_time()<< " P_CLK.posedge: "<< P_clk.posedge()<<" Hreset: "
				<<hreset<<" Hwrite.negedge: "<<hwrite.negedge()<<" HTRANS: 3"<<"--->SEQ" <<"HBURST: INCR"<<endl;

				val=rand()%1024;

				cout<<" % Value Sending(Master End): "<<val<<endl<<endl;

				address->write(adr+WORD_SIZE);

			    hburst->write(1);

			    htrans->write(3);

				sender->write(val);

			    to_slave=true;
			    
			    hburst_flag=true;

			    htrans_flag=true;
			
		}
		

	}

	}
}


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

  if(P_clk.negedge()==true && hwrite.posedge()==true && hreset 
  	&& P_clk==false && hwrite == true && 
   	to_slave==true){

	cout<<" # Slave: " << sc_simulation_time()<< " P_CLK.negedge: " << P_clk.negedge()
	<<" Hreset: "<<hreset<<" Hwrite.posedge: "<<hwrite.posedge()<<endl;

    cout<<" # Value Recieved(Slave): "<<receiver->read()<<endl;
    cout<<" # Address: "<<address->read();
    if(hburst_flag==true){
    	cout<<" HBURST VALUE: "<<hburst->read();
    	hburst_flag=false;
    }
    if(htrans_flag==true){
    	cout<<" Htrans Value: "<<htrans->read();
    }
    cout<<endl<<endl;

    to_slave=false;

	}
}

