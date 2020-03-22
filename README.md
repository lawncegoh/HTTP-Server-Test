# HTTP-Server-Test
TCP server implemented

##Steps to run: +
1. Run workingserver.cpp with `g++ workingserver.cpp -o workingserver`
2. Run `./workingserver`
3. Terminal should show that it is waiting for connection
4. Go to google chrome and type in the following link: http://localhost:8080/info.html
5. This will request from the server through port 8080 the file: info.html
6. The server should return the specified web page that you requested. 
7. HTTP request header will be present in your terminal.

##Additional Comments: +
I tried to solve the issue of handling at least 10,000 concurrent connections and serving at least 100,000 requests per second
in the second file: `serverside1.cpp`
- What I tried to do in this new file was to implement multi-threading to help speed the process up. However, I have used my 
knowledge from both school and online research but this method seems to work only on windows. 
- Disclaimer: I am using a MacBook Pro but I tested this code on a school computer of mine and it works to a certain extent.
- Therefore, I would like to say that I would love to learn more about speeding up requests as such and to solve problems like 
c10k and even more optimisation problems if I am given the chance. 

Thank you.
