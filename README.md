# HTTP-Server-Test
Basic HTTP server that supports HTTP/1.1 in C++

## Steps to run: 
1. Clone this repo to your local directory
2. Run workingserver.cpp with `g++ workingserver.cpp -o workingserver` in the directory
3. Run `./workingserver`
4. Terminal should show that it is waiting for connection
5. Go to google chrome and type in the following link: http://localhost:8080/info.html
6. This will request from the server through port 8080 the file: info.html
7. The server should return the specified web page that you requested
8. HTTP request header will be present in your terminal

## Additional Comments: 
- I tried to solve the issue of handling at least 10,000 concurrent connections and serving at least 100,000 requests per second
in the second file: `serverside1.cpp`

Thank you.
