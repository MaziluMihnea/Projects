////////////////////////////////////////////////////////////////////////////////
// Company: Student
// Engineer: Prunel Marian-Alexandru
// Create Date:   19:34  11/27/2017
// Description: Wall follower algorithm using right hand rule
////////////////////////////////////////////////////////////////////////////////

module maze(
input 		          			clk,
input [maze_width - 1:0]  		starting_col, starting_row, 	// indicii punctului de start
input  			  					maze_in, 			// ofera informa?ii despre punctul de coordonate [row, col]
output reg[maze_width - 1:0] 	row, col,	 		// selecteaza un rând si o coloana din labirint
output reg			  				maze_oe,			// output enable (activeaza citirea din labirint la rândul ?i coloana date) - semnal sincron	
output reg			  				maze_we, 			// write enable (activeaza scrierea în labirint la rândul ?i coloana date) - semnal sincron
output reg			  				done);		 	// ie?irea din labirint a fost gasita; semnalul ramane activ 

//starile principale ale automatului
`define start       		  0	//Starea de start
`define mrg_in_fata       1	//Stare in care fac efectiv miscarea		
`define dr_deschis        2	//Verific daca este in dreapta deschis
`define ver_zid_in_fata   3	//Verific daca in fata este zid
`define st_deschis        4	//Verific daca in stanga este deschis
`define gata              5	//Starea in care termin
`define mut_dreapta		  6	//Stari in care mut pozitia pe urmatoarea astfel incat sa verific 
`define mut_stanga		  7
`define mut_fata			  8

parameter maze_width = 6; 



reg [5:0] row_aux, col_aux;  //Coordonate auxiliare folosite in program 
reg [1:0] directie; //Valoare ce ma ajuta sa stiu in ce parte ma uit (privire de sus)
		//directie deplasare:
			//  0->jos
			//  1->stanga
			//  2->sus
			//  3->dreapta


reg [4:0] state, next_state ;

always @(posedge clk) begin
	if(done == 0)
		state <= next_state;
end

always @(*) begin
    next_state = `start;    //Initializarea unor variabile
	 maze_we = 0;
	 maze_oe = 0;
	 done = 0;
	 case(state)
	 
			`start: begin		//Doar initializez 

				directie = 0;	//Directia de plecare este in jos
				maze_we = 1;	//Pun 2 de unde plec
				row = starting_row;	//Initializez row si col cu unde sunt
				col = starting_col;
				row_aux = starting_row;
				col_aux = starting_col;

				next_state = `mut_dreapta;	//Urmatoarea stare
			end

			`mut_dreapta: begin
			
				case(directie)
			
					0: begin
						col_aux = col;
						col = col - 1;
						
						next_state = `dr_deschis; 
					end
			
					1: begin
						row_aux = row;
						row = row - 1;
						
						next_state = `dr_deschis;
					end
			
					2: begin
						col_aux = col;
						col = col + 1;
						
						next_state = `dr_deschis;		
					end
			
					3: begin
						row_aux = row;
						row = row + 1;
						
						next_state = `dr_deschis;
					end
			
				endcase
				maze_oe = 1;
			
			end

			`dr_deschis : begin
				
				case(directie)
				
					0: begin //in jos
					
						if(maze_in == 1) begin
							next_state = `mut_fata;
							col = col_aux;
						end
						else begin
							directie = 1;
							next_state = `mrg_in_fata;
							col = col_aux;
						end
					
					end
					
					1: begin //in dreapta
					
						if(maze_in == 1) begin
							next_state = `mut_fata;
							row = row_aux;
						end
						else begin
							directie = 2;
							next_state = `mrg_in_fata;
							row = row_aux;
						end
					
					end
					
					2: begin //in sus
					
						if(maze_in == 1) begin
							next_state = `mut_fata;
							col = col_aux;
						end
						else begin
							directie = 3;
							next_state = `mrg_in_fata;
							col = col_aux;
						end
					
					end
					
					3: begin //in stanga
					
						if(maze_in == 1) begin
							next_state = `mut_fata;
							row = row_aux;
						end
						else begin
							directie = 0;
							next_state = `mrg_in_fata;
							row = row_aux;
						end
					
					end
					
				endcase

			end
			
			`mut_fata: begin
			
				case(directie)
					
					0: begin
						row_aux = row;
						row = row + 1;
						
						next_state = `ver_zid_in_fata;
					end
					
					1: begin
						col_aux = col;
						col = col - 1;
						
						next_state = `ver_zid_in_fata;
					end
					
					2: begin
						row_aux = row;
						row = row - 1;
						
						next_state = `ver_zid_in_fata;
					end
					
					3: begin
						col_aux = col;
						col = col + 1;
						
						next_state = `ver_zid_in_fata;
					end
					
				endcase
				maze_oe = 1;
			end


			`ver_zid_in_fata: begin
				
			case(directie)
			
			0: begin
			
			if(maze_in == 1) begin
			next_state = `mut_stanga;
			row = row_aux;
			end
			else begin 
			next_state = `mrg_in_fata;
			row = row_aux;
			end
			
			end
			
			1: begin
			
			if(maze_in == 1) begin
			next_state = `mut_stanga;
			col = col_aux;
			end
			else begin 
			next_state = `mrg_in_fata;
			col = col_aux;
			end
			
			end
			
			2: begin
			
			if(maze_in == 1) begin
			next_state = `mut_stanga;
			row = row_aux;
			end
			else begin 
			next_state = `mrg_in_fata;
			row = row_aux;
			end
			
			end
			
			3: begin
			
			if(maze_in == 1) begin
			next_state = `mut_stanga;
			col = col_aux;
			end
			else begin 
			next_state = `mrg_in_fata;
			col = col_aux;
			end
			
			end
			
			endcase

			end
			
			`mut_stanga: begin
				
					case(directie)
						
						0: begin
							col_aux = col;
							col = col + 1;
							
							next_state = `st_deschis;
						end
						
						1: begin
							row_aux = row;
							row = row + 1;
							
							next_state = `st_deschis;				
						end
						
						2: begin
							col_aux = col;
							col = col - 1;
							
							next_state = `st_deschis;
						end
						
						3: begin
							row_aux = row;
							row = row - 1;
							
							next_state = `st_deschis;
						end
						
					endcase
					maze_oe = 1;
			end


			`st_deschis: begin 
				
				case(directie)
				
						0: begin 
					
							if(maze_in == 1)begin
								directie = 2;
								next_state = `mrg_in_fata;
								col = col_aux;
							end
							else begin
								directie = 3;
								next_state = `mrg_in_fata;
								col = col_aux;
							end
					
						end

						1: begin 
					
							if(maze_in == 1)begin
								directie = 3;
								next_state = `mrg_in_fata;
								row = row_aux;
							end
							else begin
								directie = 0;
								next_state = `mrg_in_fata;
								row = row_aux;
							end

						end

						2: begin 
					
							if(maze_in == 1)begin
								directie = 0;
								next_state = `mrg_in_fata;
								col = col_aux;
							end
							else begin
								directie = 1;
								next_state = `mrg_in_fata;
								col = col_aux;
							end

						end

						3: begin 
					
							if(maze_in == 1)begin
								directie = 1;
								next_state = `mrg_in_fata;
								row = row_aux;
							end
							else begin
								directie = 2;
								next_state = `mrg_in_fata;
								row = row_aux;
							end

						end

				endcase

			end

			`mrg_in_fata: begin 

				case(directie)
				
					0: begin
						row = row + 1;
						maze_we = 1;
					end
				
					1: begin
						col = col - 1;
						maze_we = 1;
					end
				
					2: begin
						row = row - 1;
						maze_we = 1;
					end
				
					3: begin
						col = col + 1;
						maze_we = 1;
					end
				
				endcase
				
				if(col == 0 || col == 63 || row == 0 || row == 63)
				next_state = `gata;
				else next_state = `mut_dreapta;
				
				

			end

			`gata: done = 1;
			
	endcase

end

endmodule