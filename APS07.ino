
#include <MsTimer2.h>
//класс обработки сигналов кнопки сброс и датчиков______________________________
class Datchik{
   public:
     bool flagPress=0; //признак нажатия
     bool flagClick=0; //признак принятого решения о стабильном состоянии
     bool flagSb=0;//признак удержания кнопки сброс
             
     void scanState();//метод проверки состояния сигнала
     Datchik(byte pin, int timeButton); //конструктор, установка номера вывода
                                                 // и времени подтверждения
   private:
     int _count=1; //счетчик подтверждения стабильного состояния
     int _count_sb=1; //доп. счетчик кнопки сброса
     int _timeButton=0; //время подтверждения сотояния кнопки/датчика
     byte _pin=0; //номер вывода кнопки/датчика

};
//реализация методов класса_________________________

//метод проверки нажатия кнопки
void Datchik::scanState(){
      flagPress=digitalRead(_pin);
  if (flagPress==1) {_count=0; flagClick=0;_count_sb=0;} //если состояние сигнала не меняется, обнул. счетчики нажатия и удержания 
  else _count++;// иначе +1 к счетчику состояния сигнала
  if (_count>=_timeButton){  //состояние сигнала стало устойчивым
       _count=0; //обнуление состояния счетчика
       flagClick=1; //принято решение о стаб.состоянии срабатывания
       _count_sb++;//+1 доп. счетчик
       if (_count_sb>=90) {
              _count_sb=0;//обнуление доп счетчика
              flagSb=1; //признак удержания сброса
                } 
    }
}
//конструктор - установка номера пина и времени подтверждения состояния
Datchik::Datchik(byte pin, int timeButton){
  _pin=pin;
  _timeButton=timeButton;
  pinMode(_pin,INPUT); //пин определяем как вход
}
//Создание объектов - датчиков(кнопок)________________
Datchik datchik1(9,800); //датчик1 подключен к 9 пину, время ожидания 1 с.
Datchik datchik2(8,800); //датчик2 подключен к 8 пину, время ожидания 1 с.
Datchik datchik3(7,800);//датчик3 подключен к 7 пину, время ожидания 1 с.
Datchik datchik4(6,800);//датчик4 подключен к 6 пину, время ожидания 1 с.
Datchik datchik5(5,800);//датчик5 подключен к 5 пину, время ожидания 1 с.
Datchik datchik6(15,800);//датчик6 подключен к 15 пину, время ожидания 1 с.
Datchik datchik7(16,800);//датчик7 подключен к 16 пину, время ожидания 1 с.
Datchik sbros(4,15);    //сброс подключен к 4 пину, время ожидания 30 мс.

//Глобальные переменные
int timerCount=0;//Программный таймер - счетчик мигания/звука зуммера, 250/250
bool flagSbros1=0;//признак нажатия сброса на срабатывание датчика1
bool flagSbros2=0;//признак нажатия сброса на срабатывание датчика2
bool flagSbros3=0;//признак нажатия сброса на срабатывание датчика3
bool flagSbros4=0;//признак нажатия сброса на срабатывание датчика4
bool flagSbros5=0;//признак нажатия сброса на срабатывание датчика5
bool flagSbros6=0;//признак нажатия сброса на срабатывание датчика6
bool flagSbros7=0;//признак нажатия сброса на срабатывание датчика7
bool nachalo=0;//признак отработки контроля светодиодов при подаче питания
bool flagZumKan1=0;//признак необходимости включения зуммера для канала1
bool flagZumKan2=0;//признак необходимости включения зуммера для канала2
bool flagZumKan3=0;//признак необходимости включения зуммера для канала3
bool flagZumKan4=0;//признак необходимости включения зуммера для канала4
bool flagZumKan5=0;//признак необходимости включения зуммера для канала5
bool flagZumKan6=0;//признак необходимости включения зуммера для канала6
bool flagZumKan7=0;//признак необходимости включения зуммера для канала7
bool flagZum=0;//Признак включения зуммера
bool flagAvar1=0;//Признак аварии канала1
bool flagAvar2=0;//Признак аварии канала2
bool flagAvar3=0;//Признак аварии канала3
bool flagAvar4=0;//Признак аварии канала4
bool flagAvar5=0;//Признак аварии канала5
bool flagAvar6=0;//Признак аварии канала6
bool flagAvar7=0;//Признак аварии канала7
//Основная программа______________________________________________________________
void setup() {
  MsTimer2::set(2, interrupt); //период прерывания по таймеру 2мс
  MsTimer2::start(); //разрешаем прерывания
  pinMode(2,OUTPUT);        //зуммер
  pinMode(3,OUTPUT);        //светодиод датчика 1
  pinMode(10,OUTPUT);        //светодиод датчика 2
  pinMode(11,OUTPUT);        //светодиод датчика 3
  pinMode(12,OUTPUT);        //светодиод датчика 4
  pinMode(13,OUTPUT);        //светодиод датчика 5
  pinMode(14,OUTPUT);        //светодиод датчика 6
  pinMode(17,OUTPUT);        //светодиод датчика 7  
     
}

void loop() 
{
 //Контроль светодиодов при включении 
if (nachalo==0) {
       mig_nach();//вызов ф-ции мигания при включении
       nachalo=1;   

}

      //канал1, датчик не замкнут,  сброс был нажат, аварии нет
     if ((datchik1.flagClick==0) and (flagAvar1==0)) {
                              flagSbros1=0;
                              flagZumKan1=0;
                              digitalWrite(3,LOW);
                              }

     //канал1, датчик замкнут, есть авария 
     if ((datchik1.flagClick==1) and (flagSbros1==0)){
         flagAvar1=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan1=1; digitalWrite(3,HIGH);}
         if (timerCount>250){flagZumKan1=0; digitalWrite(3,LOW);}
          }
     
     //канал1, датчик не замкнут, сброса не было, есть авария  
     if ((datchik1.flagClick==0) and (flagAvar1==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan1=1; digitalWrite(3,HIGH);}
         if (timerCount>250){flagZumKan1=0; digitalWrite(3,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar1==1)) {
                              flagSbros1=1;
                              flagZumKan1=0;
                              flagAvar1=0;
                              digitalWrite(3,HIGH);
                              }


      //канал2, датчик не замкнут,  сброс был нажат,  аварии нет
     if ((datchik2.flagClick==0) and (flagAvar2==0)) {
                              flagSbros2=0;
                              flagZumKan2=0;
                              digitalWrite(10,LOW);
                              }

     //канал2, датчик замкнут, есть авария 
     if ((datchik2.flagClick==1) and (flagSbros2==0)){
         flagAvar2=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan2=1; digitalWrite(10,HIGH);}
         if (timerCount>250){flagZumKan2=0; digitalWrite(10,LOW);}
          }
     
     //канал2, датчик не замкнут, сброса не было, есть авария  
     if ((datchik2.flagClick==0) and (flagAvar2==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan2=1; digitalWrite(10,HIGH);}
         if (timerCount>250){flagZumKan2=0; digitalWrite(10,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar2==1)) {
                              flagSbros2=1;
                              flagZumKan2=0;
                              flagAvar2=0;
                              digitalWrite(10,HIGH);
                              }


      //канал3, датчик не замкнут,  сброс был нажат, аварии нет
     if ((datchik3.flagClick==0) and (flagAvar3==0)) {
                              flagSbros3=0;
                              flagZumKan3=0;
                              digitalWrite(11,LOW);
                              }

     //канал3, датчик замкнут, есть авария 
     if ((datchik3.flagClick==1) and (flagSbros3==0)){
         flagAvar3=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan3=1; digitalWrite(11,HIGH);}
         if (timerCount>250){flagZumKan3=0; digitalWrite(11,LOW);}
          }
     
     //канал3, датчик не замкнут, сброса не было, есть авария  
     if ((datchik3.flagClick==0) and (flagAvar3==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan3=1; digitalWrite(11,HIGH);}
         if (timerCount>250){flagZumKan3=0; digitalWrite(11,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar3==1)) {
                              flagSbros3=1;
                              flagZumKan3=0;
                              flagAvar3=0;
                              digitalWrite(11,HIGH);
                              }

  //канал4, датчик не замкнут,  сброс был нажат,  аварии нет
     if ((datchik4.flagClick==0) and (flagAvar4==0)) {
                              flagSbros4=0;
                              flagZumKan4=0;
                              digitalWrite(12,LOW);
                              }

     //канал4, датчик замкнут, есть авария 
     if ((datchik4.flagClick==1) and (flagSbros4==0)){
         flagAvar4=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan4=1; digitalWrite(12,HIGH);}
         if (timerCount>250){flagZumKan4=0; digitalWrite(12,LOW);}
          }
     
     //канал4, датчик не замкнут, сброса не было, есть авария  
     if ((datchik4.flagClick==0) and (flagAvar4==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan4=1; digitalWrite(12,HIGH);}
         if (timerCount>250){flagZumKan4=0; digitalWrite(12,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar4==1)) {
                              flagSbros4=1;
                              flagZumKan4=0;
                              flagAvar4=0;
                              digitalWrite(12,HIGH);
                              }


 //канал5, датчик не замкнут,  сброс был нажат,  аварии нет
     if ((datchik5.flagClick==0) and (flagAvar5==0)) {
                              flagSbros5=0;
                              flagZumKan5=0;
                              digitalWrite(13,LOW);
                              }

     //канал5, датчик замкнут, есть авария 
     if ((datchik5.flagClick==1) and (flagSbros5==0)){
         flagAvar5=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan5=1; digitalWrite(13,HIGH);}
         if (timerCount>250){flagZumKan5=0; digitalWrite(13,LOW);}
          }
     
     //канал5, датчик не замкнут, сброса не было, есть авария  
     if ((datchik5.flagClick==0) and (flagAvar5==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan5=1; digitalWrite(13,HIGH);}
         if (timerCount>250){flagZumKan5=0; digitalWrite(13,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar5==1)) {
                              flagSbros5=1;
                              flagZumKan5=0;
                              flagAvar5=0;
                              digitalWrite(13,HIGH);
                             }

 //канал6, датчик не замкнут,  сброс был нажат, аварии нет
     if ((datchik6.flagClick==0) and (flagAvar6==0)) {
                              flagSbros6=0;
                              flagZumKan6=0;
                              digitalWrite(14,LOW);
                              }

     //канал6, датчик замкнут, есть авария 
     if ((datchik6.flagClick==1) and (flagSbros6==0)){
         flagAvar6=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan6=1; digitalWrite(14,HIGH);}
         if (timerCount>250){flagZumKan6=0; digitalWrite(14,LOW);}
          }
     
     //канал6, датчик не замкнут, сброса не было, есть авария  
     if ((datchik6.flagClick==0) and (flagAvar6==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan6=1; digitalWrite(14,HIGH);}
         if (timerCount>250){flagZumKan6=0; digitalWrite(14,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar6==1)) {
                              flagSbros6=1;
                              flagZumKan6=0;
                              flagAvar6=0;
                              digitalWrite(14,HIGH);
                              }

 //канал7, датчик не замкнут,  сброс был нажат, аварии нет
     if ((datchik7.flagClick==0) and (flagAvar7==0)) {
                              flagSbros7=0;
                              flagZumKan7=0;
                              digitalWrite(17,LOW);
                              }

     //канал7, датчик замкнут, есть авария 
     if ((datchik7.flagClick==1) and (flagSbros7==0)){
         flagAvar7=1;
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan7=1; digitalWrite(17,HIGH);}
         if (timerCount>250){flagZumKan7=0; digitalWrite(17,LOW);}
          }
     
     //канал7, датчик не замкнут, сброса не было, есть авария  
     if ((datchik7.flagClick==0) and (flagAvar7==1)) {
         //мигание-звук по счетчику-таймеру
         if (timerCount<250){flagZumKan7=1; digitalWrite(17,HIGH);}
         if (timerCount>250){flagZumKan7=0; digitalWrite(17,LOW);}
      }
                                                              
     //нажатие сброса при замкнутом датчике, снимается авария
     if ((sbros.flagClick==1) and (flagAvar7==1)) {
                              flagSbros7=1;
                              flagZumKan7=0;
                              flagAvar7=0;
                              digitalWrite(17,HIGH);
                              }
                        

 //Работа зуммера
 flagZum=flagZumKan1 | flagZumKan2 | flagZumKan3 | flagZumKan4 | flagZumKan5 | flagZumKan6 | flagZumKan7 ;//логическое сложение признаков включения зуммера
if (flagZum==1) digitalWrite(2,HIGH);// включаем зуммер
else digitalWrite(2,LOW); // выключаем зуммер
                              
 //функция контроль исправности светодиодов
 
 if (sbros.flagSb==1) {//признак удержания кнопки сброс  
       sbros.flagSb=0;  
       mig_nach();     
       flagSbros1=0;
       flagSbros2=0;
       flagSbros3=0;
       flagSbros4=0;
       flagSbros5=0;
       flagSbros6=0;
       flagSbros7=0;
  }  
     
} 
//мигание светодиодов (контроль при включении)
void mig_nach() {
       digitalWrite(3,HIGH);
       digitalWrite(10,HIGH);
       digitalWrite(11,HIGH);
       digitalWrite(12,HIGH);
       digitalWrite(13,HIGH);
       digitalWrite(14,HIGH);
       digitalWrite(17,HIGH);
       digitalWrite(2,HIGH);
       delay(2000);
       digitalWrite(3,LOW);
       digitalWrite(10,LOW);
       digitalWrite(11,LOW);
       digitalWrite(12,LOW);
       digitalWrite(13,LOW);
       digitalWrite(14,LOW);
       digitalWrite(17,LOW);
       digitalWrite(2,LOW);    
  }
    
//обработчик прерывания:

void interrupt() {
  datchik1.scanState();// вызов метода стабильного ожидания стаб. состояния датчика 1
  datchik2.scanState();//вызов метода стаб. состояния датчика 2
  datchik3.scanState();//вызов метода стаб. состояния датчика 3
  datchik4.scanState();//вызов метода стаб. состояния датчика 4
  datchik5.scanState();//вызов метода стаб. состояния датчика 5
  datchik6.scanState();//вызов метода стаб. состояния датчика 6
  datchik7.scanState();//вызов метода стаб. состояния датчика 7
  sbros.scanState();//вызов метода стаб. состояния сброса
  //Обработка программного счетчика-таймера
  timerCount++;
  if (timerCount>=500) timerCount=0;
      
}


