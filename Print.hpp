/*Print専用クラス*/
#ifndef PRINT_HEAD
#define PRINT_HEAD

class Print{
 public:
  
  static void printBasicInfo(){
    cout << "---------------------------------------------------------" << endl;
    cout <<  "Obserbation time = " << OBSERVE << "\tSint = " << Sint << endl;
    cout << "PACKETSIZE = " << PACKETSIZE << "\tBITS_COUNT = " << BITS_COUNT
	 << "\tBits_per1u = " << BITS_PER_1us << endl;
    cout << "x range = " << X_RANGE << "\ty range = " << Y_RANGE << endl;
    cout << "DENSITY = " << DENSITY << "\tLambda = " << Lambda << endl;
    cout << "---------------------------------------------------------" << endl;
  }
  
  static void printNodesMode(NODE *n_data, int N, double t_count){
    for(int i = 0; i < N; i++){
      if((n_data[i].mode != SLEEP) && (n_data[i].mode != Re_Be_ACK)){
	cout <<  t_count << "\t" << i << "\t" << n_data[i].mode << endl;
      }
    }
  }

  static void printTransNodes(int N, double t_count, ModeMemory *modeMemo, NODE *n_data){

    for(int i = 0; i < N; i++){
      if(n_data[i].next_mode != n_data[i].mode){
	if(n_data[i].next_mode == TRANSMIT){
	  cout << "Start Tx\t"<< t_count << "\t" << i << "\t" <<modeMemo->Trans_node.size() << endl; 
	}	  	 	 
      }
    }
    // list<int>::iterator it = modeMemo->Trans_node.begin();
    // while(it != modeMemo->Trans_node.end()){
    //   cout << t_count << "\t" << *it << endl;
    //   ++it;
    // }  
  }
  
  static void printInspectDBPSK(){
    double avb = 0;
    
    for(int i = 0; i < avbSinr.size(); i++){
      avb += avbSinr[i];
    }
    cout << "adv Sinr = " << avb / (double)avbSinr.size() << "\tBER = " << ber / (double)avbSinr.size() << endl;
  }

  static void printNodesActiveTime(NODE *n_data, int N){
    for(int i = 0; i < N; i++){      
      cout  << i << "\t" << n_data[i].activetime << endl;
    }
  }

  static  void printRecAndTransPacket(NODE *n_data, MovingSink *car, int N){
    int cnt = 0;
    cout << "# of Node = " << N << endl;
    cout << "RecPacket\t" << car->recPackets << "\t";
    for(int i = 0; i < N; i++ ){
      cnt += n_data[i].transCount;
    }
    cout << "TransPacket\t" << cnt << endl;
  }

  static void printNodeProcess(NODE *n_data, int N, double t_count){

    //cout << t_count << endl;
    for(int i = 0; i < 1; i++){
      if((abs(n_data[i].activetime - t_count) < TCOUNT)){
	cout << "id =" << n_data[i].id << "\tActiveTime = " << n_data[i].activetime << endl;
      }
      if(n_data[i].ca_time < TCOUNT && n_data[i].prevState == CSMA){
	cout << "CSMA\t" << "id =" << n_data[i].id << "\tca_tome = " << n_data[i].ca_time << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if(abs(n_data[i].be_end - t_count) < TCOUNT){
	cout << "BEACON\t" << "id =" << n_data[i].id << "\tTime = " << t_count << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if(abs(n_data[i].re_be_end - t_count) < TCOUNT){
	cout << "Re_be_ACK\t" << "id =" << n_data[i].id << "\tTime = " << t_count << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << endl;
      }
      if(n_data[i].next_mode != n_data[i].mode){
	cout << "Mode Change\t" << "id =" << n_data[i].id << "\tmode = " << n_data[i].mode << "\tnext_mode = " << n_data[i].next_mode << "\tprevState = " << n_data[i].prevState << endl;
      }
    }
  }

  static void printListSize(ModeMemory *modeMemo){
    cout << "All_trans = " << modeMemo->All_trans.size() << endl;
    cout << "Trans = " << modeMemo->Trans_node.size() << endl;
    cout << "Beacon = " << modeMemo->Beacon_node.size() << endl;
    cout << "ACK = " << modeMemo->ACK_node.size() << endl;
  }
};

#endif
