# DNS_proxy_server
A simple DNS proxy server written in C.  
This server receives parameters from the configuration file.  
Then creates two sockets for interaction with clients and the upstream server.  
Further, all client requests are filtered by domain.  
If the domain is on the blacklist, an error is returned with the code specified in the configuration file.  
Otherwise, the request is forwarded to the upstream server.
Also for handling data server will create a child process
Additionally, to process the data, the server will create a child process, which will terminate after responding to the request.


The configuration file contains the following parameters:
- Address of proxy DNS server  
- Address of upstream DNS server  
- List of domain names to filter resolving  
- Type of DNS proxy server's response for blacklisted domains


## Build

### Prerequisites
Make sure you have the following tools and libraries installed:
- [GCC](https://gcc.gnu.org) (GNU Compiler Collection) or any C compiler
- [Make](https://www.gnu.org/software/make/) (build automation tool)
- **json-c** library


### Instructions
1. **Clone the repository:**
   ```base
   git clone https://github.com/formodx/DNS_proxy_server.git
   cd DNS_proxy_server
   ```

2. **Install json-c:**  

    on Debian/Ubuntu-based systems:
    ```bash
    sudo apt-get install libjson-c-dev
    ```

    on Fedora/RHEL-based systems:
    ```bash
    sudo dnf install json-c-devel
    ```

    on macOS (using Homebrew):
    ```bash
    brew install json-c
    ```

3. **Build the project:**
   ```bash
   make
   ```


### Clean the build (optional)
If you want to clean up the compiled files, you can use:
```bash
make clean
```
