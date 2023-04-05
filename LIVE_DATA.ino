 
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define PI 3.14159265359
float y[1000], x[1000],sum=0,xmean,ACF[1000],pulse;
int b ,sample_length=1000;
//
//
void setup() {
//  // put your setup code here, to run once:
Serial.begin(9600);

 
  
  
   }


void loop() {

  b=0;
while (b<1000){
 x[b]=analogRead(A0);

 delay(10);
 b=b+1;
  }
 
  sum=0;
  for ( b=0; b<1000; b++)
    sum=sum+x[b];
  xmean=sum/1000;
  for(b=0;b<1000;b++)
  {
  y[b]=x[b]-xmean;
  Serial.println(y[b]);

  }
  //Autocorelation Function

  for( int i=0;i<sample_length;i++)
  {
    float l=0;
    for( int j=0;j<sample_length-i;j++)
      l=l+y[j]*y[j+i];
    ACF[i]=l;
  }
  float acf=ACF[0];
  for(int i=0;i<sample_length;i++)
    ACF[i]=ACF[i]/acf;

  // To find the number of samples between first two maximas
    // First we find the position of first zero crossing point(FZCP) 
     int FZCP_pos;
      int win_len=0;
     float new_win[1000];
     for( int i=1;i<sample_length-1;i++)
     {
      if(ACF[i]*ACF[i-1]<=0)
      {
      FZCP_pos=i;
      break;
      }
     }

     // We create a separate window from the FZCP to the end and find the position of maximum value in this window
     for( int i=FZCP_pos;i<sample_length-1;i++)
     {
       new_win[i-FZCP_pos]=ACF[i];
       win_len++;
     }
     float max_val=0;
     for( int i=0;i<win_len;i++)
     {
      if(new_win[i]>=max_val)
      max_val=new_win[i];
     }
     //max_val=max(new_win[i],max_val);
     int max2;
     for(int i=0;i<win_len;i++)
     {
      if(new_win[i]==max_val)
      {
      max2=i;
      break;
      }
     }
     float T=0.01;
     float f=1/T;
     int FZCP=FZCP_pos+1;
     int L_max2=max2+1;
     int L_max=FZCP+L_max2-1;
     float period=L_max/f;
      pulse=60/period;
     //Serial.println(pulse);
  
 // put your main code here, to run repeatedly:
 lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse Rate:");
  lcd.print(pulse);
  delay(1000);

}
