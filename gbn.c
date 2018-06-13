#include "../include/simulator.h"
#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */
int checksum( packet1)

struct pkt packet1;
{
        int sum = 0;
        int csum;


      for (int j = 0; j <20; j++)

    {

        sum =sum + (int )packet1.payload[j];

    }
    csum =packet1.acknum+packet1.seqnum+sum;

    return csum;
}
struct pkt pktqueue[20000];
struct pkt sendack;
int N,base,nextseqnum,n,seq_no,m,expectedseqnum;
void A_output(message)
  struct msg message;
{ 
  pktqueue[n].seqnum=seq_no;
  pktqueue[n].acknum=0;
  for (int i=0;i<20;i++) pktqueue[n].payload[i]=message.data[i];
  pktqueue[n].checksum=checksum(pktqueue[n]);  
  seq_no++;
  n++;
  if(nextseqnum<(base+N))
  {
    tolayer3(0,pktqueue[nextseqnum]);
    if(base==nextseqnum) starttimer(0,35.0);
    nextseqnum++;
  }  


}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
  int computedchecksum;
 int sum=0;

      for (int i = 0; i <20; i++)
 {

     sum=sum+(int )packet.payload[i];

    }
    computedchecksum=sum+packet.acknum+packet.seqnum;
    if(computedchecksum==packet.checksum)
    {
      base=packet.acknum+1;
      if(base==nextseqnum)
      {
        stoptimer(0);

      }
      else starttimer(0,35.0);
    }

}

/* called when A's timer goes off */
void A_timerinterrupt()
{ starttimer(0,35.0);
  for(m=base;m<nextseqnum;m++)
  {
    tolayer3(0,pktqueue[m]);
  }


}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{ n=0;
  seq_no=0;
  base=0;
  nextseqnum=0;
  N=getwinsize();
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
  int computedchecksum;
          int sum=0;

      for (int i = 0; i <20; i++)

    {

     sum=sum+(int )packet.payload[i];

    }
    computedchecksum=sum+packet.acknum+packet.seqnum;
    if((computedchecksum==packet.checksum)&&(packet.seqnum==expectedseqnum))
    {
      tolayer5(1,packet.payload);
      sendack.seqnum=0;
      sendack.acknum=expectedseqnum;
      sendack.checksum=checksum(sendack);
      tolayer3(1,sendack);
      expectedseqnum++;

    }
    if((computedchecksum==packet.checksum)&&(packet.seqnum!=expectedseqnum))
    {
       sendack.seqnum=0;
      sendack.acknum=expectedseqnum-1;
      sendack.checksum=checksum(sendack);
      tolayer3(1,sendack);
    }
  
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{ expectedseqnum=0;

}
                                                                                                                             59,1          Bot
