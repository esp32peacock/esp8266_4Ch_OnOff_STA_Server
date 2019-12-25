#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer webServer(80);
const int max_e = 10;
int LED_state1[max_e] = {0,0,0,0,0,0,0,0,0,0};
int LED_state2[max_e] = {0,0,0,0,0,0,0,0,0,0};
int LED_state3[max_e] = {0,0,0,0,0,0,0,0,0,0};
int LED_state4[max_e] = {0,0,0,0,0,0,0,0,0,0};
String clnt[max_e];
int last_clnt = 0;


void handleRoot() {

  String html = "<html><head>";
  html+="<style>";
  html+=".tab {position:absolute;left:150px; }";
  html+=".tab2 {position:absolute;left:50px; }";
  html+="}";
  html+="</style>";
  html+="<script>";
  html+=" var ch_state = ['0000','0000','0000','0000','0000','0000','0000','0000','0000','0000']\n";
  html+=" function callback(resp) {\n";
  html+="  console.log(resp);\n";
  html+=" }\n";
  html+=" function httpGetAsync(theUrl, callback)\n";
  html+=" {\n";
  html+="  var xmlHttp = new XMLHttpRequest();\n";
  html+="  xmlHttp.onreadystatechange = function() {\n"; 
  html+="    if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {\n";
  html+="      callback(xmlHttp.responseText);\n";
  html+="    } else {\n";
  html+="      console.log(xmlHttp.responseText);\n";
  html+="    }\n";
  html+="  }\n";
  html+="  xmlHttp.open('GET', theUrl, true);\n";
  html+="  xmlHttp.send(null);\n";
  html+=" }\n";
  html+=" function chx(ch,onoff,b_state) {\n";
  html+="  var rt='';\n";
  html+="  for(var i=0; i<4; i++) {\n";
  html+="   if(i==(ch-1)) {\n";
  html+="    rt+=onoff;\n";
  html+="   } else {\n";
  html+="    rt+=b_state[i];\n";
  html+="   }\n"; 
  html+="  }\n";
  html+="  return rt;\n";
  html+=" }\n";
  html+=" function ledon(e,ch) {\n";
  html+="  ch_state[e]=chx(ch,'1',ch_state[e]);\n";
  html+="  tch = ch.toString()\n";  
  html+="  httpGetAsync('http://'+window.location.host+'/post?e='+e.toString()+'&ch='+tch+'&led=1',callback);\n";
  html+="  document.getElementById('pin'+tch+e.toString()).innerHTML = 'D'+(ch+3).toString()+' STATE = 1';\n";    
  html+=" }\n";
  html+=" function ledoff(e,ch) {\n";
  html+="  ch_state[e]=chx(ch,'0',ch_state[e]);\n";
  html+="  tch = ch.toString()\n";  
  html+="  httpGetAsync('http://'+window.location.host+'/post?e='+e.toString()+'&ch='+tch+'&led=0',callback);\n";     
  html+="  document.getElementById('pin'+tch+e.toString()).innerHTML = 'D'+(ch+3).toString()+' STATE = 0';\n";    
  html+=" }\n";
  html+=" function cc() {\n";
  html+="  return String.fromCharCode(34);\n";
  html+=" }\n";
  html+=" function callbackEsp(resp) {\n";
  html+="  var html = ''\n";
  html+="  if (resp.length==0) {\n";
  html+="   html+='<span class='+cc()+'tab2'+cc()+'>No Client Found</span><br><br>';\n";
  html+="  } else {\n";
  html+="   var res = resp.split(',');\n";
  html+="   for (var i=0; i<res.length; i++) {\n";
  html+="    html ='<span class='+cc()+'tab2'+cc()+'>'+res[i]+'</span><br><br>';\n";
  html+="    html+='<span class='+cc()+'tab'+cc()+'><div id='+cc()+'pin1'+i.toString()+cc()+'>D4 STATE = '+ch_state[i].substring(0,1)+'&nbsp</div>';\n";
  html+="    html+='</span><span class='+cc()+'tab2'+cc()+'><button onclick='+cc()+'ledon('+i.toString()+',1)'+cc()+'>ON</button>&nbsp<button onclick='+cc()+'ledoff('+i.toString()+',1)'+cc()+'>OFF</button></span><br><br>';\n";
  html+="    html+='<span class='+cc()+'tab'+cc()+'><div id='+cc()+'pin2'+i.toString()+cc()+'>D5 STATE = '+ch_state[i].substring(1,2)+'&nbsp</div>';\n";
  html+="    html+='</span><span class='+cc()+'tab2'+cc()+'><button onclick='+cc()+'ledon('+i.toString()+',2)'+cc()+'>ON</button>&nbsp<button onclick='+cc()+'ledoff('+i.toString()+',2)'+cc()+'>OFF</button></div></span><br><br>';\n";
  html+="    html+='<span class='+cc()+'tab'+cc()+'><div id='+cc()+'pin3'+i.toString()+cc()+'>D6 STATE = '+ch_state[i].substring(2,3)+'&nbsp</div>';\n";
  html+="    html+='</span><span class='+cc()+'tab2'+cc()+'><button onclick='+cc()+'ledon('+i.toString()+',3)'+cc()+'>ON</button>&nbsp<button onclick='+cc()+'ledoff('+i.toString()+',3)'+cc()+'>OFF</button></div></span><br><br>';\n";
  html+="    html+='<span class='+cc()+'tab'+cc()+'><div id='+cc()+'pin4'+i.toString()+cc()+'>D7 STATE = '+ch_state[i].substring(3,4)+'&nbsp</div>';\n";
  html+="    html+='</span><span class='+cc()+'tab2'+cc()+'><button onclick='+cc()+'ledon('+i.toString()+',4)'+cc()+'>ON</button>&nbsp<button onclick='+cc()+'ledoff('+i.toString()+',4)'+cc()+'>OFF</button></div></span><br><br>';\n";
  html+="   }\n";
  html+="  }\n";
  html+="  document.getElementById('esp').innerHTML = html;\n";
  html+=" }\n";
  html+=" var refreshCode = setInterval( function()\n";
  html+=" {\n";
  html+="  httpGetAsync('http://'+window.location.host+'/esp',callbackEsp);\n";            
  html+=" }, 10000);\n";


  
  html+="</script></head><body><br><br><br>";
  html+="<div id='esp'>";
  
  html+="</div>";
  html+="</body></html>";
  Serial.println("Send HTML page");
  webServer.send(200, "text/html", html);
}

void handleIP() {
  String message = WiFi.localIP().toString();
  Serial.println("Send "+message);
  webServer.send(200, "text/plain", message);
}


void handlePost() {
  String message = "Number of args received:";
  message += webServer.args();  
  message += "\n";                  
  for (int i = 0; i < webServer.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";
    message += webServer.argName(i) + ": ";
    message += webServer.arg(i) + "\n";
    if(webServer.argName(i)=="number") {
      Serial.println("Received "+String(webServer.arg(i)));
    }
  }
  if (webServer.args()>0) {
    int ch = webServer.arg(1).toInt();
    if (ch == 1) {
      LED_state1[webServer.arg(0).toInt()] = webServer.arg(2).toInt();
    } else if (ch == 2) {
      LED_state2[webServer.arg(0).toInt()] = webServer.arg(2).toInt();      
    } else if (ch == 3) {
      LED_state3[webServer.arg(0).toInt()] = webServer.arg(2).toInt();      
    } else {
      LED_state4[webServer.arg(0).toInt()] = webServer.arg(2).toInt();      
    }
  }
  webServer.send(200, "text/plain", message);
}

int search_client(String id) {
  int i = 0;
  for(i=0; i<max_e; i++) {
    if (clnt[i] == id) {
      break;
    }
  }
  if (i==10) {
    return (-1);
  } else {
    return i;
  } 
}

void add_client(String id) {
  if (search_client(id)<0) {
    clnt[last_clnt] = id;
    Serial.println("Add Client "+id);
    last_clnt+=1;
  }
}

void handleGet() {
  String message;
  if (webServer.args()>0) {
    String id = webServer.arg(0);
    add_client(id);
    int sc = search_client(id);
    if (sc>=0) {
      message = String(LED_state1[sc])+','+String(LED_state2[sc])+','+String(LED_state3[sc])+','+String(LED_state4[sc]);  
    }
  } 
  Serial.println("Get "+message);
  webServer.send(200, "text/plain", message);  
}

void handleEsp() {
  String message;
  if (last_clnt>0) {
    for (int i=0; i<last_clnt; i++) {
      message+=clnt[i]+',';
    }
    message=message.substring(0,message.length()-1);
  }
  Serial.println("Esp "+message);
  webServer.send(200, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("Bobzt3rz","12345678");
  while (WiFi.status() != WL_CONNECTED) {
	  Serial.print('.');
    delay(200);
  }
  Serial.println(' ');
  Serial.println(WiFi.localIP());  
  Serial.println("Web Server Start...");
  webServer.on("/", handleRoot);
  webServer.on("/ip", handleIP);
  webServer.on("/post", handlePost); 
  webServer.on("/get", handleGet);
  webServer.on("/esp", handleEsp);
  webServer.onNotFound([]() {
    Serial.println("Hello World!");
    String message = "Hello World!\n\n";
    message += "URI: ";
    message += webServer.uri();

    webServer.send(200, "text/plain", message);
  });
  webServer.begin();
}

void loop() {
  webServer.handleClient();
}
