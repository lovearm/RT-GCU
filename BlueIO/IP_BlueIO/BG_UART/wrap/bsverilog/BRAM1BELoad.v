// Copyright (c) 2000-2011 Bluespec, Inc.

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
// $Revision: 25100 $
// $Date: 2011-09-01 18:44:19 +0000 (Thu, 01 Sep 2011) $

`ifdef BSV_ASSIGNMENT_DELAY
`else
 `define BSV_ASSIGNMENT_DELAY
`endif

// Single-Ported BRAM with byte enables and ability to load from file
module BRAM1BELoad(CLK,
                   EN,
                   WE,
                   ADDR,
                   DI,
                   DO
                  );

   parameter                      FILENAME   = "";
   parameter                      PIPELINED  = 0;
   parameter                      ADDR_WIDTH = 1;
   parameter                      DATA_WIDTH = 1;
   parameter                      CHUNKSIZE  = 1;
   parameter                      WE_WIDTH   = 1;
   parameter                      MEMSIZE    = 1;
   parameter                      BINARY     = 0;

   input                          CLK;
   input                          EN;
   input [WE_WIDTH-1:0]           WE;
   input [ADDR_WIDTH-1:0]         ADDR;
   input [DATA_WIDTH-1:0]         DI;
   output [DATA_WIDTH-1:0]        DO;

   reg [DATA_WIDTH-1:0]           RAM[0:MEMSIZE-1];
   reg [DATA_WIDTH-1:0]           DO_R;
   reg [DATA_WIDTH-1:0]           DO_R2;

   reg [DATA_WIDTH-1:0]           DATA;
   wire [DATA_WIDTH-1:0]          DATAwr;

   assign DATAwr = RAM[ADDR] ;

   // synopsys translate_off
   initial
   begin : init_block
`ifdef BSV_NO_INITIAL_BLOCKS
`else
      integer i;
      for (i = 0; i < MEMSIZE; i = i + 1) begin
         RAM[i] = { ((DATA_WIDTH+1)/2) { 2'b10 } };
      end
      DO_R  = { ((DATA_WIDTH+1)/2) { 2'b10 } };
      DO_R2 = { ((DATA_WIDTH+1)/2) { 2'b10 } };
`endif // !`ifdef BSV_NO_INITIAL_BLOCKS
      if (BINARY)
        $readmemb(FILENAME, RAM, 0, MEMSIZE-1);
      else
        $readmemh(FILENAME, RAM, 0, MEMSIZE-1);
   end
   // synopsys translate_on

   // iverilog does not support the full verilog-2001 language.  This fixes that for simulation.
`ifdef __ICARUS__
   reg [DATA_WIDTH-1:0] MASK, IMASK;

   always @(WE or DI or DATAwr) begin : combo1
      integer j;
      MASK  = 0;
      IMASK = 0;

      for(j = WE_WIDTH-1; j >= 0; j = j - 1) begin
         if (WE[j]) MASK = (MASK << 8) | { { DATA_WIDTH-CHUNKSIZE { 1'b0 } }, { CHUNKSIZE { 1'b1 } } };
         else       MASK = (MASK << 8);
      end
      IMASK = ~MASK;

      DATA = (DATAwr & IMASK) | (DI & MASK);
   end

`else
   always @(WE or DI or DATAwr) begin : combo1
      integer j;
      // DATA = 0; // While this line is better coding sytle, it leads to incorrect synthsis for some tools
      for(j = 0; j < WE_WIDTH; j = j + 1) begin
         if (WE[j]) DATA[j*CHUNKSIZE +: CHUNKSIZE] = DI[j*CHUNKSIZE +: CHUNKSIZE];
         else       DATA[j*CHUNKSIZE +: CHUNKSIZE] = DATAwr[j*CHUNKSIZE +: CHUNKSIZE];
      end
   end
`endif // !`ifdef __ICARUS__

   always @(posedge CLK) begin
      if (EN) begin
         if (|WE) begin
            RAM[ADDR] <= `BSV_ASSIGNMENT_DELAY DATA;
            DO_R <= `BSV_ASSIGNMENT_DELAY DATA;
         end
         else begin
            DO_R <= `BSV_ASSIGNMENT_DELAY RAM[ADDR];
         end
      end
      DO_R2 <= `BSV_ASSIGNMENT_DELAY DO_R;
   end

   // Output driver
   assign DO = (PIPELINED) ? DO_R2 : DO_R;

endmodule // BRAM1BELoad
