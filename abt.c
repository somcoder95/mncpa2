#include <stdlib.h>
#include <stdio.h>
#include "../include/simulator.h"



/* ******************************************************************
 *  ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
 *
 *     This code should be used for PA2, unidirectional data transfer 
 *        protocols (from A to B). Network properties:
 *           - one way network delay averages five time units (longer if there
 *                are other messages in the channel for GBN), but can be larger
 *                   - packets can be corrupted (either the header or the data portion)
 *                        or lost, according to user-defined probabilities
 *                           - packets will be delivered in the order in which they were sent
 *                                (although some can be lost).
 *                                **********************************************************************/

/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/

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
int expectedseqnum1,pinpipe;
struct pkt lastspkt;
struct pkt sndpkt;
struct pkt lastpkt;
struct pkt validack;
struct pkt sendack;
struct pkt lastrcvdpkt;
int pktcount,n;
int m=1;
int expectedacknum,expectedseqnum;
struct msg queue[20000];
void A_output(message)
  struct msg message;


{
for(int i=0;i<20;i++)
 {
queue[n].data[i]=message.data[i];
 }
n++;


if((pktcount==0)&&(pinpipe==0))
  {
        for(int i=0;i<20;i++)
   { sndpkt.payload[i]=queue[0].data[i];

   }

  sndpkt.acknum=0;
  sndpkt.seqnum=0;
  sndpkt.checksum=checksum(sndpkt);
  printf("Checksumofsendpkt is %d\n",sndpkt.checksum);
  tolayer3(0,sndpkt);
  pinpipe=1;
  starttimer(0,20.0);
  lastpkt=sndpkt;

  printf("payload of lastpkt is %s\n",lastpkt.payload);
 pktcount++;
  printf("Packet count is %d\n ",pktcount);
                                          
}
 if((validack.acknum==lastpkt.seqnum)&&(pinpipe==0)&&(pktcount!=0))
  { lastspkt=sndpkt;
    if(validack.acknum==1) sndpkt.seqnum=0;

    if(validack.acknum==0) sndpkt.seqnum=1;
    sndpkt.acknum=0;
    for(int i=0;i<20;i++)
   {sndpkt.payload[i]=queue[m].data[i];
   }

    sndpkt.checksum=checksum(sndpkt);
  tolayer3(0,sndpkt);
  pinpipe=1;
  starttimer(0,20.0);
  lastpkt=sndpkt;
 expectedacknum= lastpkt.seqnum;
if(lastpkt.seqnum==1)
 {
 expectedseqnum=0;
}
if(lastpkt.seqnum==0)
{expectedseqnum=1;
}
  m++;


 }


}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
struct pkt packet;

{
 printf("checksum of received ack is %d\n",packet.checksum);
 int computedchecksum;
 int sum=0;

      for (int i = 0; i <20; i++)
 {

     sum=sum+(int )packet.payload[i];

    }
    computedchecksum=sum+packet.acknum+packet.seqnum;
   printf("Computed checksum of received ack is %d\n",computedchecksum);
    if((computedchecksum==packet.checksum))
     {
        validack=packet;
         printf("Valid ack is %d",validack.acknum);
        stoptimer(0);
        pinpipe=0;

        }

}

/* called when A's timer goes off */
void A_timerinterrupt()
{

tolayer3(0,lastpkt);
pinpipe=1;
starttimer(0,20.0);
printf("Resent packet is %s",lastpkt.payload);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
pktcount=0;
  n=0;
pinpipe=0;


}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)

  struct pkt packet;

 {
 printf("Received message :%s",packet.payload);
 int computedchecksum;
          int sum=0;

      for (int i = 0; i <20; i++)

    {

     sum=sum+(int )packet.payload[i];

    }
    computedchecksum=sum+packet.acknum+packet.seqnum;
    if(computedchecksum==packet.checksum)
     {

       lastrcvdpkt=packet;
        printf("last received packet is %s",lastrcvdpkt.payload);


             if(packet.seqnum==1) sendack.acknum=1;
         if(packet.seqnum==0) sendack.acknum=0;
         sendack.checksum=checksum(sendack);
         printf("checksum of ack is %d\n",sendack.checksum);
         printf("acknum of ack is %d\n",sendack.acknum);
       tolayer3(1,sendack);

      if(packet.seqnum==expectedseqnum1)
      {
       tolayer5(1,packet.payload);
       if(packet.seqnum==1) expectedseqnum1=0;
       if(packet.seqnum==0) expectedseqnum1=1;
       }
      }


  }

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{ expectedseqnum1=0;
}

                           