# internet
```
//IN PROGRES OF ACTIVE CREATION  
//YOU COULD SAY IT IS EARLY ACCESS  
   
Created to simplify and speed up creation of small iot projects with some central server.  
I just hate micropython and even more hate cpp internet stuff  
  
This template is using both cores one for data processing and gathering, other for network related stuff  
  
This text file is to explain how to use it, to some extent how it works, state of work, plans and ideas that will eventualy be implemented  
  
cores structure:  
    core0: DATA       - Processing and gathering data  
    core1: Internet   - requests in, out  
  
  
Internet  
    - bootack: Device on boot sends to server that it has booted and proceding with operation  
    - time:    Device aslo on boot (by deafult) request from same sever with normal http/text time and date to   initialize RTC
    - Ping:    Every X min of time (def: 1min)(configured in config.cpp - uint8_t REPORT_TIME) pings server to show   that it is indeed still working
    - Raport:  Every X min of time (def: 1min)(configured in config.cpp - uint8_t PING_TIME  ) sends to server some   data whathewer you want // in time of writing not operating to full extent  
    build in tasks: bootack, time, ping are by deafult executed  
  
    Network credentials:  
        Location: user/core1/secret/secret.cpp  
        You can programm multiple network ssid and passwords  
        That helps with mobility and networking  
          
        There is template of this file named secret_copy.cpp if you want to use it change name to secret.cpp  
  
        todo: at least one creadential written in flash by internet request or by pulling from server  
  
    Ip,port of server can be changed in config.cpp   
        variable: std::string SERVER_IP         // deafult "192.168.1.2"  
        variable: uint16_t    SERVER_PORT       // deafult 80  
  
    Headers  
        This data is transmited by Get (in url)  
        Every request to server must have:  
            pico_id:   unique id to know which device asks, reboots, etc   -> config.cpp - const uint8_t PICO_ID  
            pico_desc: short text saying what this device mainly do        -> config.cpp - const char*  PICO_DESC  
        These two could aslo be used as some sort of authetication of device  
            action:    Tells server what device want to tell/get  
              
        urls for build in actions: bootack, time, ping, raport is defined by deafult:  
            "/newSystem/traffic_control.php"  
            Location:   
                config.cpp: variable: std::string SERVER_MAIN_PATH
        * BootAck: has extra header "version" with value equal to -> config.cpp - std::string PICO_VERSION
  
    RESPONSE TEXT:  
  
        I assume that there is some sort main central unit with static ip that will prowide communication  
        It is my template and ip adress and specifics of data transportation may be odd  
  
        Basicly server side everything is php that return html/text file back to pico and pico interprets it.  
        Normally data is started by "<||>" (4 chars) and ended by "<||>", only two per file is allowed (ending and   starting)
        Single data like hour, minutes are devided by singular "|", and records are devided by two char sequence of "||"  
        Examples (whole file, without http headers):  
            time:       <||>10|20<||>           // 10:20  
            two times:  <||>10|20||10|30<||>    // 10:20 and 10:30 maybe start and end of action  
          
        function that get time from server get data in this format:  
            <||>22-50-39-2025-05-17-6<||>  
            hour-minute-second-year-month-day-day_of_the week  
            <||> signals start and end of data  
  
    LISTENING:
        pico can recive request from web on port 80
        path: user/core1/get_param_processing_user.cpp ->user_process_get_request (std::string head_name, std::string head_value)
            head_name -> name of parameter
            head_value-> value of parameter

            return string that will be forwarded back to requester

        //todo 
            Basic functionalities build in
        


    todo:  
        - Moving watchdog to core0 (explanation in Watchdog header)  
  
Processing side:  
    libaries are build depending on #define set on FALSE to skip and TRUE to build, directive located in config.cpp file  
    built in modules already have thiers basic inits and rutine programmed which is compiled if module is enabled  
    libary folder is: core0/libs/lib_name  
  
    After startup:  
        -> Initialization of module:       user/core0/modules/modules.cpp -> init_submodules();  
        -> Execute rutine of every module: user/core0/modules/modules.cpp -> rutine_submodules();  
      
    rutine_submodules() is invoked in core0/main0.cpp where is located main loop of core  
      
  
  
    built in libaries:  
        -one wire                         
        -display 2x16 with i2c converter 
        -bmp280                          
        -pzem004
        -dht20                           //in time of writing none are operating/tested  
  
Communication folder  
    This folder is used to declare any variables that will be seen by core0 and core1  
        -they should be 32bit long max, that ensures variable is not readed when writed by other core  
        -If it is variable longer then 32bit, struct, array, etc, you should program some sort of flag that will tell   other core to not touch this set of data to ensure peacufull reading/writing
  
Watchdog  
    watchdog is located in core0 deafult watchdog timeout is 8sec
    core1 have counter that increments every loop cycle.
    if core0 check core1 counter and discovers it is same for more then 5 times then one of following will be executed
        1: Whole board reset
        2: core1 reset by multicore function
    that can be changed in config editing #define CONFIG_ON_CORE_1_STUCK
  
Time   
    If server sucessfully dowloads time from server it should set flag   
    bool TIME_INITIETED, located in namespace CONFIG -> config.cpp  
        CONFIG::TIME_INITIETED  
    if triggered is fuction to dowload time again this flag will be turned to false  
    until time is properly dowloaded and interpreted  
  
UPDATES
    As project is rapidly improving i though how hard would be to update already written app
    To adress it there i added folders/files that will not be edited by updates and could be used to progress faster
        user/

        

There is massive room to improve and add multiple of features, but honestly i deeply doubt that anyone will read this  
let alone use this template so i really dont know how much will and work i should put to document, explain, and go   above what i really need to meet somthing that someone could use
  
//this file will be later redacted and rewritten by chatgpt as i suck at english after 13y of education   
```