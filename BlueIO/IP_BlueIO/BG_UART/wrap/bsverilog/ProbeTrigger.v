
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

//
// REPLICATED CODE WARNING
// If these encodings change, the following files must be kept in sync:
//
// SceMiSerialProbe.bsv
//
// Verilog Probes (ProbeTrigger.v, ProbeCapture.v etc.)
//
// C++ Transactor code (src/lib/SceMi/bsvxactors/SerialProbeXactor.cxx)
//

`define PDISABLED   3'd1
`define PENABLED    3'd2
`define PTRIGGERED  3'd3
`define PADDING    13'h1FFF

module ProbeTrigger (
                     UCLK,
                     URST_N,
                     // Down link
                     CMDEN,
                     CMD,
                     // Up link
                     DELAY,
                     DATAUP,
                     DATAVALID,
                     ACK,
                     //
                     CLK,
                     // Trigger Data
                     TRIGGER
                 );

   parameter TriggerId = 0;
   parameter NumWords = 1;
   parameter CaptureIds = { NumWords { 16'hFFFF } };
   parameter CWidth  = 1 ;      // 1 + log2 (NumWords)

   input UCLK;
   input URST_N;
   input CLK;

   input TRIGGER;

   output [31:0] DATAUP;
   output        DELAY;
   output        DATAVALID;
   input         ACK;

   input         CMDEN;
   input [18:0]  CMD;

   // Output declarations
   reg [31:0]    DATAUP;        // on UCLK
   wire          DELAY;
   wire          DATAVALID;

   wire [15:0] 	probenum = CMD [18:3];
   wire [2:0]   cmd      = CMD  [2:0];

   // On local clock
   reg                  triggerToggle; // not reset

   // on UCLK/reset
   reg [7:0]            enableCount;
   reg                  enabled;
   reg                  sendDisable;
   reg [CWidth-1:0]     wordCount;
   reg [16*NumWords-1:0] reportIds;

   reg                  sending;
   reg                  sentToggle; // not reset

   wire triggered   = triggerToggle != sentToggle;
   wire sendTrigger = enabled && triggered ;
   wire myCommand   = CMDEN && (probenum == TriggerId[15:0]);
   wire wordCountZero = wordCount == {CWidth {1'b0}};

   assign DELAY   = enabled && triggered;
   assign DATAVALID = sending;

   always @(posedge UCLK) begin
      if (URST_N != 1'b1) begin
         enabled     <= `BSV_ASSIGNMENT_DELAY 1'b0;
         enableCount <= `BSV_ASSIGNMENT_DELAY 8'b0;
         sending     <= `BSV_ASSIGNMENT_DELAY 1'b0;
         sendDisable <= `BSV_ASSIGNMENT_DELAY 1'b0;
         wordCount   <= `BSV_ASSIGNMENT_DELAY { CWidth {1'b0}};
         reportIds   <= `BSV_ASSIGNMENT_DELAY CaptureIds;
      end
      else begin
         if (!sending && sendTrigger) begin
            DATAUP      <= `BSV_ASSIGNMENT_DELAY {TriggerId[15:0], `PADDING, `PTRIGGERED};
            sending     <= `BSV_ASSIGNMENT_DELAY 1'b1;
            wordCount   <= `BSV_ASSIGNMENT_DELAY NumWords [CWidth-1:0] ;
            reportIds   <= `BSV_ASSIGNMENT_DELAY CaptureIds;
         end
         else if (sending && ACK && !wordCountZero) begin
            DATAUP     <= `BSV_ASSIGNMENT_DELAY {reportIds[15:0], `PADDING, `PTRIGGERED};
            wordCount  <= `BSV_ASSIGNMENT_DELAY wordCount - 1'b1;
            reportIds  <= `BSV_ASSIGNMENT_DELAY reportIds >> 16;
         end
         else if (sending && ACK && sendDisable) begin
            DATAUP <= `BSV_ASSIGNMENT_DELAY {TriggerId[15:0], `PADDING, `PDISABLED};
         end
         else if (sending && ACK) begin
            sending <= `BSV_ASSIGNMENT_DELAY 1'b0;
         end

         // sendDisable  flop
         if (sendDisable && wordCountZero && ACK) begin
            sendDisable <= 1'b0;
         end
         else if(!wordCountZero && ACK) begin
            sendDisable <= `BSV_ASSIGNMENT_DELAY (enableCount == 8'b1);
         end

         // clear the toggle after sending all works
         if (triggered && (!enabled || (sending && wordCountZero) )) begin
            sentToggle <= `BSV_ASSIGNMENT_DELAY ! sentToggle;
         end

         // Process commands
         if (myCommand && cmd == `PDISABLED) begin
            enableCount <= `BSV_ASSIGNMENT_DELAY 8'b0;
            enabled     <= `BSV_ASSIGNMENT_DELAY 1'b0;
         end
         else if (myCommand && cmd == `PTRIGGERED) begin
            enableCount <= `BSV_ASSIGNMENT_DELAY enableCount - 8'd1;
            enabled     <= `BSV_ASSIGNMENT_DELAY enableCount != 8'd1;
         end
         else if (myCommand && cmd == `PENABLED) begin
            enableCount <= `BSV_ASSIGNMENT_DELAY (enableCount != 8'hff) ? enableCount + 8'd1 : 8'hff ;
            enabled     <= `BSV_ASSIGNMENT_DELAY 1'b1;
         end
      end
   end

   // Local clock == uclock and lclock are edge aligned.
   always @(posedge CLK) begin
      if (TRIGGER) begin
         triggerToggle <= `BSV_ASSIGNMENT_DELAY ! triggerToggle;
      end
      // synopsys translate_off
      if (enabled && triggered && TRIGGER) begin
         $display ("Error: %m ProbeTrigger has been double triggered!");
      end
      // synopsys translate_on
   end

`ifdef BSV_NO_INITIAL_BLOCKS
`else // not BSV_NO_INITIAL_BLOCKS
   // synopsys translate_off
   initial begin
      DATAUP         <= { 16 {2'b10}};
      triggerToggle  <= 1'b0;
      sentToggle     <= 1'b0;
      enableCount    <= {4 {2'b01}};
      sending        <= 1'b0;
      sendDisable    <= 1'b0;
      enabled        <= 1'b0;
      reportIds      <= {NumWords * 8 {2'b10}} ;
   end
   // synopsys translate_on
`endif

endmodule
