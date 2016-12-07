`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
//Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
//Date Created: Wed Dec  7 02:08:26 2016
//Assignment: 3
//File: matt_test2.v
//Description: An HLSM module which represents the C-like behavioral description 
//             into a scheduled high-level statement machine implemented in Verilog.
//
//////////////////////////////////////////////////////////////////////////////////

module HLSM(Clk, Rst, Start, Done, a, b, c, z, x);
	input Clk, Rst, Start;
	output reg Done;

	input signed [31:0] a, b, c;

	output signed [31:0] z, x;

	reg dLTe, dEQe;
	reg signed [31:0] d, e, f, g, h;

	reg[4:0] state;
	parameter sWait = 0, s2 = 1, s3 = 2, s4 = 3, s5 = 4, s6 = 5, s7 = 6, s8 = 7, s9 = 8, sFinal = 9;

	always@(posedge Clk) begin
		if(Rst == 1) begin
			state <= sWait;
			d <= 0;
			e <= 0;
			f <= 0;
			g <= 0;
			h <= 0;
			dLTe <= 0;
			dEQe <= 0;
			z <= 0;
			x <= 0;
			Done <= 0;
		end
		else begin
			case(state)
				sWait: begin
					Done <= 0;
					if (Start == 1)
						state <= s2;
					else
						state <= sWait;
				end
				s2: begin
					d <= a + b;
					state <= s3;
				s3: begin
					g <= dLTe ? d : e;
					state <= s4;
				s4: begin
					e <= a + c;
					state <= s5;
				s5: begin
					dLTe <= d < e;
					state <= s6;
				s6: begin
					x <= g << dLTe;
					state <= s7;
				s7: begin
					dEQe <= d == e;
					state <= s8;
				s8: begin
					f <= a - b;
					h <= dEQe ? g : f;
					state <= s9;
				s9: begin
					z <= h >> dEQe;
					state <= sFinal;
				sFinal: begin
					Done <= 1;
					state <= sWait;
				end
			endcase
		end
	end

endmodule
