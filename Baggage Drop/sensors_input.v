`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:22:25 11/09/2021 
// Design Name: 
// Module Name:    sensors_input 
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
module sensors_input (
   output   [7 : 0]   height,
   input    [7 : 0]   sensor1,
   input    [7 : 0]   sensor2,
   input    [7 : 0]   sensor3,
   input    [7 : 0]   sensor4);
	
	reg [7:0] o; //declararea unui reg in care sa fac calcule in
				    //always. acest reg este echivalentul lui height
	reg [7:0] s; //un reg de care am nevoie pentru a calcula suma senzorilor
	
	always @(*) begin
	
	//if-urile mele vad despre ce caz este vorba testand daca am valori nule in
	//senzorul 1 sau 3, mai apoi in senzorul 2 sau 4, daca nu atunci fac suma
	//tuturor
		
	if (!sensor1 || !sensor3) begin
		o = (sensor2 + sensor4)/2;  //calculez inaltimea pentru acest caz 
		s = sensor2 + sensor4;      //suma celor 2 senzori 
		if(s[0] == 1'b1) begin      //verific daca ultimul bit al sumei este 1
			o = o + 1'b1;				 //astfel verific daca numarul se imparte exact
			end							 //sau nu la 2, daca nu voi adauga 1
		end else
			if (!sensor2 || !sensor4) begin
				o = (sensor1 + sensor3)/2;
				s = sensor1 + sensor3;
				if(s[0] == 1'b1) begin
					o = o + 1'b1;
				end	
			end else begin
				o = (sensor1 + sensor2 + sensor3 + sensor4)/4;
				s = sensor1 + sensor2 + sensor3 + sensor4;
				if((s[0] == 1'b1 && s[1] == 1'b1) || (s[0] == 1'b0 && s[1] == 1'b1)) begin
					o = o + 1'b1;      //mai mult sau mai putin acelasi lucru ca la if-urile
					end					 //anterioare doar ca de data asta verific ultimii 2
				end                   //biti cu combinatiile 11 respectiv 10
	
	end 
	
	assign height = o;   //o era variabila auxiliara folosita doar pentru reg 
								//iar acum doar atribui valoarea lui height


endmodule
