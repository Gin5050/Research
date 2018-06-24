/*Print専用クラス*/
#ifndef PRINT_HEAD
#define PRINT_HEAD

class Print{
 public:
  static void printNodesMode(NODE *n_data, int N, double t_count){
    for(int i = 0; i < N; i++){
      if((n_data[i].mode != SLEEP) && (n_data[i].mode != Re_Be_ACK)){
	cout << t_count << "\t" << i << "\t" << n_data[i].mode << endl;
      }
    }
  }

  static void printNodesActiveTime(NODE *n_data, int N){
    for(int i = 0; i < N; i++){      
      cout  << i << "\t" << n_data[i].activetime << endl;
    }
  }

  static  void printRecAndTransPacket(NODE *n_data, MovingSink *car, int N){
    int cnt = 0;
    cout << "RecPacket\t" << car->recPackets << "\t";
    for(int i = 0; i < N; i++ ){
      cnt += n_data[i].transCount;
    }
    cout << "TransPacket\t" << cnt << endl;
  }
};

#endif
