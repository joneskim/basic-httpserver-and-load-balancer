# Building a basic HTTP Load Balancer and Servers in C

This project demonstrates the fundamental principles of building a distributed system by implementing a simple HTTP load balancer, and multiple backend servers in C.

The load balancer basically distributes incoming client requests among the backend servers using a round-robin algorithm. I started with a combined server then split the code into separate modules for better organization and reusability. Used sockets for network communication and basic logging and error handling.



## Key concepts

### Distributed Systems

A collection of independent computers that appear to users as a single coherent system.

Goals: Scalability, fault tolerance, resource sharing, performance.

Challenges: Consistency, handling partial failures, network latency, security.



### Load Balancing

Distributing network traffic across multiple servers to prevent overload, improve performance, and increase availability.

Algorithms: Round-Robin(Distribute requests sequentially to each other)

Least Connections: Send connections to the server with the fewest active connections.

IP Hashing: Use a hash of the client's IP address to determine which server to use(ensures a client consistently connects to the same server)

Weighted Round-Robin/Least Connections: Assign weights to servers based on their capacity, and distribute requests proportionally.

### HTTP (Hypertext Transfer Protocol)

Request/Response model

Methods: GET, POST, PUT, DELETE

Status Codes: 200 OK, 404 Not Found, 500 Internal Server Error, 503 Service Unavailable, etc

Headers: Metadata about the request or response (eg. Content-Type, Content-Length)



### Sockets

An endpoint for communication between two processes over a network

We've got 2 main types: Stream sockets (TCP, connection-oriented) and datagram sockets (UDP, connectionless)

Address: IP address and port number



### Network Programming in C

- socket(): creates a socket
- bind(): associates a socket an address (IP and port)
- listen(): puts a socket in listening mode (for servers)
- accept(): accepts an incoming connection (for servers)
- connect(): connects to a remote socket (for clients)
- read() / write() (or recv() / send() ): receive and send data over a socket
- close(): closes a socket










