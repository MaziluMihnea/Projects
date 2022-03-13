`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:22:45 11/09/2021 
// Design Name: 
// Module Name:    square_root 
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
module square_root(
    output reg [15:0] out,
    input  [7:0] in );
	
	
	//declararea varibilelor necesare pentru algoritmul meu
    reg [31:0] num;   //num este numarul pe care vreau sa il pun in radical
    reg [31:0] res;	 //res este rezultatul
    reg [31:0] bit_;  //bit este o variabila de care am nevoie pentru implementare
    integer i;
   //am pus numerele pe 32 de biti pentru ca algoritmul are nevoie de 16 iteratii 
	//(rezultatul va fi pe 16 biti in final) iar algoritmul meu la fiecare iteratie
	//imi shifteaza res spre exemplu cu 1 bit
	
	
always@(*) begin
 
	 i = 0;
	 num[7:0]=in;      //num am mentionat ca va fi numarul meu initial, el trebuie
	 num = num << 24;  //sa fie pe 32 de biti pentru a putea realiza calculele
    res [31:0] = 0;   //res trebuie initializat cu 0
    bit_ = 1 << 30;   //bit este variabila auxiliara si el trebuie sa fie cea mai
							 //mare putere a lui 4 mai mica decat numarul meu (num)
     
    while(i < 16) begin    //in acest for scad din numarul bit
        if (bit_ > num) begin					 //astfel incat sa fie mai mic decat 
                bit_ = bit_ >> 2;          //num (trebuie sa fie putere a lui 4
        end                                //de aceea shiftare cu 2)
		  i = i+1;
    end
	 
	 i = 0;      //reinitializarea lui i cu 0 pentru urmatorul while
    
	 //implementarea programului digit by digit gasit in documentatia oferita in
	 //C si adaptat
    while(i < 16) begin
        if (bit_ != 0) begin
            if (num >= res + bit_) begin
                num = num - (res + bit_);
                res = (res >> 1) + bit_;
            end else begin
                res = res >> 1;
                end
            bit_ = bit_ >> 2;
        end
		  i = i + 1;
    end
     
     out[15:0] = res [19:4];  //atribuirea valorii lui res lui out 
end

endmodule
