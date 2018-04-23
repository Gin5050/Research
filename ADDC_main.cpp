#include "define.h"
#include "system.h"
//#include "degree_define.h"
//#include "awgn.h"

void Initialaize_nodes(NODE *n_data, int N);

int main(){
  int i, j;
  int nowround;
  int N;
  double p_sleep;
  double t_count;
  double PPP_CDF[TEMP_NUM] = {0};
  double temp;

  NODE *n_data;
  
  PPP(PPP_CDF);
  for(p_sleep = 0.5; p_sleep < 0.6; p_sleep += 0.1){

    for(nowround = 0; nowround < REPEATNUM; nowround++){
      temp = randuni(mt);
      for(i = 0; i < TEMP_NUM; i++)
	if(temp < PPP_CDF[i]) break;
      N = i;
      n_data = new NODE[N];
      cout << N << endl;

      Initialaize_nodes(n_data, N);
      t_count = 0;
      while(t_count < OBSERVE){
	
	t_count += TCOUNT;
      }
    }
  }
  
  return 0;
}

void Initialaize_nodes(NODE *n_data, int N){
  int i,j;
  uniform_real_distribution<> randX(-X_RANGE/2.0, X_RANGE/2.0);
  uniform_real_distribution<> randY(-Y_RANGE/2.0, Y_RANGE/2.0);
  uniform_real_distribution<> randSint(0, Sint);
  uniform_real_distribution<> PHASE(0, 2*M_PI);
  
  for(i = 0; i < N; i++){
    n_data[i].Initialaze(i);
  }
}

void Process_node(NODE *n_data, int N, double t_count, double p_sleep){
  int i;

  for(i = 0; i < N; i++){
    switch (n_data[shf[i]].mode){

      case SLEEP:
	if(abs(t_count - n_data[shf[i]].activetime) < TCOUNT){
	  if(n_data[shf[i]].state == SLEEP_TEMP) //SenderからBe_ACKを受信した場合
	    n_data[shf[i]].Intlz_Sleep_to_Rx(t_count);	  
	  else
	    Select_mode(n_data, t_count, shf[i], p_sleep, car_num, poison_array); //通常動作による起動orスリープ選択
	}
	n_data[shf[i]].sleep_time+=TCOUNT;
	break;

      case CSMA:
	CSMA_CA(n_data, shf[i], t_count);
	n_data[shf[i]].csma_time+=TCOUNT;
	break;
	
      case BEACON:
	n_data[shf[i]].Intlz_Be_to_Re_Be(t_count);
	n_data[shf[i]].tx_time+=TCOUNT;
	break;

      case Re_Be_ACK: //ACK待機
	if(abs(n_data[shf[i]].re_be_end - t_count) + SECURE < TCOUNT){
	  n_data[shf[i]].Intlz_Re_Be_to_CSMA(t_count);
	}
	else
	  Re_Be_func(n_data, t_count, shf[i], p_data);
	//n_data[shf[i]].rx_time+=TCOUNT;
	break;
	
      case TRANSMIT:
	n_data[shf[i]].tr_cnt++;
	if(abs(n_data[shf[i]].tr_end - t_count) + SECURE < TCOUNT){
	  if(n_data[shf[i]].state == SENDER){
	    //cout << t_count << "\tTRANSMIT = " << shf[i] << "\t" << n_data[shf[i]].rx_nodeNum << endl;
	  }	 
	  n_data[shf[i]].Intlz_Tx_to_Sleep(t_count);
	  n_data[shf[i]].trn_num++;	
	}
  	n_data[shf[i]].tx_time+=TCOUNT;
	//n_data[shf[i]].tr_cnt++;
	break;

      case SENDER:	
  	if(abs(n_data[shf[i]].s_end - t_count) + SECURE < TCOUNT){
	  if(n_data[shf[i]].rx_nodeNum >= RX_NODE){
	    n_data[shf[i]].Intlz_Sender_to_CSMA(t_count);
	    communNode.push_back(n_data[shf[i]].rx_nodeNum);
	    //cout << n_data[shf[i]].rx_nodeNum << endl;
	  }
	  else
	    n_data[shf[i]].Intlz_Sender_to_Sleep(t_count);
	  n_data[shf[i]].sender_time += TCOUNT;
	}
  	else Sender(n_data, shf[i], t_count, p_sleep, p_data);
	break;

      case Be_ACK:
	if(abs(n_data[shf[i]].s_end - t_count) + SECURE < TCOUNT){
	  n_data[shf[i]].s_end+=TCOUNT;
	}
	n_data[shf[i]].next_mode = SENDER;
	n_data[shf[i]].tx_time+=TCOUNT;
	break;

      case RECEIVE:
	n_data[shf[i]].rx_time+=TCOUNT;
  	if(abs(n_data[shf[i]].rx_end - t_count) + SECURE < TCOUNT){
	  n_data[shf[i]].Intlz_Rx_to_Sleep(t_count);
	  //n_data[shf[i]].check = FALSE;
	}
  	else Receive(n_data, shf[i], t_count, p_data);
	break;

      case COLLISION_MG:
	n_data[shf[i]].tx_time++;
	n_data[shf[i]].Intlz_to_Sleep(t_count);
	break;
    }
  }
}
