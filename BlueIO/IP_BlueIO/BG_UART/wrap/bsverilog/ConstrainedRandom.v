
// Copyright (c) 2000-2009 Bluespec, Inc.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// $Revision: 17872 $
// $Date: 2009-09-18 14:32:56 +0000 (Fri, 18 Sep 2009) $

`ifdef BSV_ASSIGNMENT_DELAY
`else
`define BSV_ASSIGNMENT_DELAY
`endif

module ConstrainedRandom(CLK, RST_N, OUT, EN);

   parameter width = 1 ;
   parameter min = 0 ;
   parameter max = 0 ;

   input           CLK;
   input           RST_N;
   input           EN;
   output [width - 1: 0] OUT;

   reg [width - 1: 0]    OUT;
   reg [width - 1: 0]    OUT2;

   integer 	    i;
   always@(posedge CLK)
     begin
        if (!RST_N)
          OUT <= min;
        else if (EN)
	  begin
	     i = 0;
	     OUT2 = 0;
	     for (i = 0; i <= width; i = i + 32) begin
		OUT2 = {OUT2, $random};
	     end
	     if ((1 + (max - min)) == 0)
	       OUT <= `BSV_ASSIGNMENT_DELAY min + OUT2;
	     else
	       OUT <= `BSV_ASSIGNMENT_DELAY min + (OUT2 % (1 + (max - min)));
	  end
	
     end

   // synopsys translate_off
   initial begin
      OUT = {((128 + 1)/2){2'b10}} ;
   end
   // synopsys translate_on


endmodule // ConstrainedRandom


