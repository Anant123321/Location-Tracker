String in;
String lon,lat;
int key=0,index[3];
boolean b = false;
void setup() 
{
  Serial1.begin(9600);
  Serial.begin(9600); 
  Serial1.println("AT+CMGF=1");   //entering into sms reading mode
  delay(1000);
  Serial1.println("AT+CNMI=2,2,0,0,0");  //tells sim808 to send any message recieved to serial 
  delay(1000);
}

void loop() 
{
  if (!in.equals(""))
  {
    if (in.indexOf("latlng") > -1)
    {
      Serial1.println("AT+SAPBR=3,1,\"APN\",\"www\""); //turning on data connection
      delay(1000);
      Serial1.println("AT+SAPBR=1,1");
      delay(5000);
      Serial1.println("AT+CIPGSMLOC=1,1"); // getting location
      delay(1000);
      in = "";
      b = true;
    }
    else if(in.indexOf(":") > -1 && b)
    {
      Serial1.println("AT+CGATT=0");
      delay(1000);
      for(int i=0;i<in.length();i++)  // filtering out the latitude and longitude from string
      {
          if(key==3)break;
          if(in.charAt(i)==',')
          {
            index[key]=i;
            key++;
          }
      }
      lon=in.substring(index[0]+1,index[1]);
      lat=in.substring(index[1]+1,index[2]);
      Serial.println(lat);
      Serial.println(lon);
      Serial1.println("AT+CMGS=\"8837686685\"");  // entring the no. where location is to be sent
      delay(1000);
      Serial1.print("http://maps.google.com/maps?q=loc:" + lat + "," + lon); // writing the msg
      delay(1000);
      Serial1.write(26); // pressing send button
      delay(5000);
      Serial1.print("AT+CMGDA=\"DEL ALL\"");  // deleting all the previous messages
      delay(1000);   
      lat = "";
      lon = "";
      in = "";
      b = false;
    }
  }
}

void serialEvent1()
{
  while (Serial1.available() > 0)
    in = Serial1.readString();
}
