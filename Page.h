#ifndef _PAGES_H_
#define _PAGES_H_

#define NO_PRACT 6
byte state = 0b11000000;

const char* PARAM_INPUT_1 = "practicum";
const char* PARAM_INPUT_2 = "state";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title> Long live the king </title>
    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }
        h2 {
            font-size: 3.0rem;
        }
        p{
            font-size: 3.0rem;
        }
        body{
            max-width: 600px;
            margin: 0px auto;
            padding-bottom: 25px;
        }
        .switch{
            position: relative;
            display: inline-block;
            width: 120px;
            height: 68px;
        }
        .switch input {
            display: none;
        }
        .slider {
            position: absolute; 
            top: 0; 
            left: 0; 
            right: 0; 
            bottom: 0; 
            background-color: #ccc; 
            border-radius: 34px
            }
        .slider:before {
            position: absolute; 
            content: ""; 
            height: 52px; 
            width: 52px; 
            left: 8px; 
            bottom: 8px; 
            background-color: #fff; 
            -webkit-transition: .4s; 
            transition: .4s; 
            border-radius: 68px
        }
        input:checked+.slider {
            background-color: #2196F3
        }
        input:checked+.slider:before {
            -webkit-transform: translateX(52px); 
            -ms-transform: translateX(52px); 
            transform: translateX(52px)
        }
    </style>
    
    </head>
    <body>
    <h1>Ayke heeft gelijk</h1>
        %PLACEHOLDER%
    </body>
    <script>
        function toggleCheckbox(element){
            var xhr = new XMLHttpRequest();
            if(element.checked){
                xhr.open("GET", "/on", true);
            } else {
                xhr.open("GET","/off", true);
            }
            xhr.send();
        }
        function toggleCheckbox2(element){
            var xhr = new XMLHttpRequest();
            if(element.checked){
                if(element.id!=0){document.getElementById("0").checked = false;}
                if(element.id!=1){document.getElementById("1").checked = false;}
                if(element.id!=2){document.getElementById("2").checked = false;}
                if(element.id!=3){document.getElementById("3").checked = false;}
                if(element.id!=4){document.getElementById("4").checked = false;}
                if(element.id!=5){document.getElementById("5").checked = false;}
                
                xhr.open("GET", "/update?practicum="+element.id+"&state=1", true);
            } else {
                xhr.open("GET","/update?practicum="+element.id+"&state=0", true);
            }
            xhr.send();
        }

    </script>
    </html>
)rawliteral";

String practState(int numPract){
    if((state & (1<<numPract) ) != 0){
        return "checked";
    } else return "";
    return ";";
}

String processor(const String& var){

  if(var == "PLACEHOLDER"){
    String buttons = "";

    buttons = "<h4>Power Switch</h4>\n<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"LED\"><span class=\"slider\"></span></label>\n";
    

    for(int i = 0; i < NO_PRACT; i++){
        String practStateVal = practState(i); 
        buttons += "<p>Practicum #" + String(i+1) + " <label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox2(this)\" id=\"" + String(i) + "\" "+ practStateVal +"><span class=\"slider\"></span></label>\n";
    }
    return buttons;
  }
}


#endif