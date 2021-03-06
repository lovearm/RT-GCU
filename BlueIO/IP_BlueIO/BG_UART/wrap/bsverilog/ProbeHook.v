
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
// $Revision: 24715 $
// $Date: 2011-07-28 13:39:27 +0000 (Thu, 28 Jul 2011) $

`ifdef BSV_ASSIGNMENT_DELAY
`else
`define BSV_ASSIGNMENT_DELAY
`endif

module ProbeHook (
                  .UCLK(uclk),
                  .URST_N(urst_n),
                  .ACK(ack),
                  .DATAUP(data),
                  .DATAVALID(dataValid),
                  .DELAY(delay),
                  .CMDEN(cmden),
                  .CMD(probenum_cmd),
                  .CTIMER(ctimer)
                  );
   input uclk;
   input urst_n;
   input ack;

   output [31:0] data;
   wire [31:0]   data;

   output        dataValid;
   wire          dataValid;

   output        delay;
   wire          delay = 1'b0;

   input         cmden;
   input [18:0]  probenum_cmd;
   input         ctimer;

   assign data = 32'b0;
   assign dataValid = 1'b0;

   // This module does not do anything.  It only leaves a crumb
   // for other tools to connect to these signals.
   // The output signals are passive
endmodule
