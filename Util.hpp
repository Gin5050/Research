/*Utilクラス*/
#ifndef UTIL_HEAD
#define UTIL_HEAD

class Utile
{
public:
  Utile() {}

  static void shuffle(int *sh, int size){
    int i, j;
    int temp;
    int temp1;
    int temp2;
    uniform_int_distribution<> randuni(0, size - 1);

    for(i = 0; i < size; i++) sh[i] = i;
    for(i = 0; i < size; i++){
      temp = randuni(mt);
      temp1 = sh[i];
      sh[i] = sh[temp];
      sh[temp] = temp1;
    }
  } 
 ~Utile() {}
};

#endif
