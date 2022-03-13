`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:23:07 11/09/2021 
// Design Name: 
// Module Name:    display_and_drop 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module display_and_drop (
    output   reg [6 : 0]   seven_seg1,   //am facut out-urile de tip reg pentru a
    output   reg [6 : 0]   seven_seg2,   //le putea folosi in always
    output   reg [6 : 0]   seven_seg3,
    output   reg [6 : 0]   seven_seg4,
    output   reg [0 : 0]   drop_activated,
    input    [15: 0]   t_act,
    input    [15: 0]   t_lim,
    input              drop_en);
	 
	 reg [1:0] number;    //voi folosi acest parametru pentru a diferentia cazuriile
								 //din case
	 
	 always @(*) begin
	 
	 		seven_seg1 = 7'b000_0000;   //am initializat varibilele pentru a nu avea
			seven_seg2 = 7'b000_0000;   //latch-uri (am eliminat warningurile)
			seven_seg3 = 7'b000_0000;
			seven_seg4 = 7'b000_0000;
	 
	 number = 'd2;
	 drop_activated = 0;
	 
	//urmeaza seturi de if-uri care imi delimiteaza cazurile, atunci cand gaseste
	//unul din cazurile descrise in tema voi da valoare de 1 sau 0 lui drop_activated
	//si ii voi da o valoare de la 1 la 3 lui number el fiind folosit la case-ul de mai
	//jos
		if(drop_en == 0) begin
			drop_activated = 0;
			number = 'd0;
		end
	
	if(t_act <= t_lim) begin
		if(drop_en == 1) begin
			drop_activated = 1;
			number = 'd1;
		end
	end
	
	if(t_act > t_lim) begin
		if(drop_en == 1) begin
			drop_activated =0;
			number = 'd2;
		end
	end
	
	//case-ul in care in functie de parametrul number imi da valori lui seven_seg(1-4)
	case(number)
		'd0: begin						   //Cold
			seven_seg1 = 7'b011_1001;
			seven_seg2 = 7'b101_1100;
			seven_seg3 = 7'b011_1000;
			seven_seg4 = 7'b101_1110;
		end
		
		'd1: begin						   //Drop
			seven_seg1 = 7'b101_1110;
			seven_seg2 = 7'b101_0000;
			seven_seg3 = 7'b101_1100;
			seven_seg4 = 7'b111_0011;
		end
		
		'd2: begin						   //Hot
			seven_seg1 = 7'b000_0000;
			seven_seg2 = 7'b111_0110;
			seven_seg3 = 7'b101_1100;
			seven_seg4 = 7'b111_1000;
		end
		
	endcase
		
		
		
		end
	 

endmodule
