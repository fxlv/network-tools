# network-tools
Things that do stuff via network

Currently two tools are provided:

* udpsender - send UDP packet(-s) to arbitrary destination
* udpraw    - send UDP packet(-s) to arbitrary destination, use RAW sockets, spoof UDP source IP and PORT

# Using
Provide target IP and PORT. The tool will pick random source IP and random source PORT for you.

```
sudo ./bin/udpraw -d 10.111.103.1 -p 2213`
```

Or you can specify what source ip and port to use:
```
sudo ./bin/udpraw -d 10.111.103.1 -p 2213 -s 10.111.103.2 -l 89
```

# Dependencies
To build on linux, you'll need `libbsd-dev`

# Building
As simple as running 
```
make
```
If you want to install it to /opt/network-tools:
```
make install
``` 

### Debug
A debug version will print out more info to stdout.
To build a debug version, run:
`make DEBUG=1`
