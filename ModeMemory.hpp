/*Nodeの各モードを記憶するクラス*/
#ifndef MODE_MEMORY_HEAD
#define MODE_MEMORY_HEAD

class ModeMemory{
 public:
  list<int> Beacon_node;
  list<int> Trans_node;
  list<int> ACK_node;
  list<int> All_trans;

  ModeMemory(){}

  void addTrans(int id){
    Trans_node.push_back(id);
    All_trans.push_back(id);
  }

  void addBeacon(int id){
    Beacon_node.push_back(id);
    All_trans.push_back(id);
  }

  void addAck(int id){
    ACK_node.push_back(id);
    All_trans.push_back(id);
  }

  void deleteBeacon(int id){
    list<int>::iterator it = Beacon_node.begin();  
    while(it != Beacon_node.end()){
      if(*it == id){
	Beacon_node.erase(it);
	break;
      }
      ++it;
    }
    deleteAllTrans(id);
  }

  void deleteTrans(int id){
    list<int>::iterator it = Trans_node.begin();
    while(it != Trans_node.end()){
      if(*it == id){
	Trans_node.erase(it);
	break;
      }
      ++it;
    }
    deleteAllTrans(id);
  }

  void deleteACKnodes(int id){
    list<int>::iterator it = ACK_node.begin();
    while(it != ACK_node.end()){
      if(*it == id){
	ACK_node.erase(it);
	break;
      }
      ++it;
    }
    deleteAllTrans(id);
  }

  void deleteAllTrans(int id){
    list<int>::iterator it = All_trans.begin();
    while(it != All_trans.end()){
      if(*it == id){
	All_trans.erase(it);
	break;
      }
      ++it;
    }    
  }
};
#endif
